#include "msctf.h"
#include "guiddef.h"

#include "TextServices.h"

BOOL RegisterServer();
BOOL RegisterProfiles();
BOOL RegisterCategories();

void UnregisterProfiles();
void UnregisterCategories();
void UnregisterServer();

class CClassFactory : public IClassFactory
{
public:
    // IUnknown methods
    STDMETHODIMP QueryInterface(REFIID riid, _Outptr_ void** ppvObj);
    STDMETHODIMP_(ULONG) AddRef(void);
    STDMETHODIMP_(ULONG) Release(void);

    // IClassFactory methods
    STDMETHODIMP CreateInstance(_In_opt_ IUnknown* pUnkOuter, _In_ REFIID riid, _COM_Outptr_ void** ppvObj);
    STDMETHODIMP LockServer(BOOL fLock);

    // Constructor
    CClassFactory(REFCLSID rclsid, HRESULT(*pfnCreateInstance)(IUnknown* pUnkOuter, REFIID riid, void** ppvObj))
        : _rclsid(rclsid)
    {
        _pfnCreateInstance = pfnCreateInstance;
    }

public:
    REFCLSID _rclsid;
    HRESULT(*_pfnCreateInstance)(IUnknown* pUnkOuter, REFIID riid, _COM_Outptr_ void** ppvObj);
private:
    CClassFactory& operator=(const CClassFactory& rhn) { rhn; };
};

STDAPI CClassFactory::QueryInterface(REFIID riid, _Outptr_ void** ppvObj)
{
    if (IsEqualIID(riid, IID_IClassFactory) || IsEqualIID(riid, IID_IUnknown))
    {
        *ppvObj = this;
        DllAddRef();
        return NOERROR;
    }
    *ppvObj = nullptr;

    return E_NOINTERFACE;
}

//+---------------------------------------------------------------------------
//
//  CClassFactory::AddRef
//
//----------------------------------------------------------------------------

STDAPI_(ULONG) CClassFactory::AddRef()
{
    DllAddRef();
    return (TextServices::dllRefCount + 1);
}

//+---------------------------------------------------------------------------
//
//  CClassFactory::Release
//
//----------------------------------------------------------------------------

STDAPI_(ULONG) CClassFactory::Release()
{
    DllRelease();
    return (TextServices::dllRefCount + 1);
}

//+---------------------------------------------------------------------------
//
//  CClassFactory::CreateInstance
//
//----------------------------------------------------------------------------

STDAPI CClassFactory::CreateInstance(_In_opt_ IUnknown* pUnkOuter, _In_ REFIID riid, _COM_Outptr_ void** ppvObj)
{
    return _pfnCreateInstance(pUnkOuter, riid, ppvObj);
}

//+---------------------------------------------------------------------------
//
//  CClassFactory::LockServer
//
//----------------------------------------------------------------------------

STDAPI CClassFactory::LockServer(BOOL fLock)
{
    if (fLock)
    {
        DllAddRef();
    }
    else
    {
        DllRelease();
    }

    return S_OK;
}

static CClassFactory* classFactoryObjects[1] = { nullptr };

void BuildGlobalObjects(void)
{
    classFactoryObjects[0] = new (std::nothrow)CClassFactory(TextServices::TextServiceCLSID, TextService::CreateInstance);;
}
void FreeGlobalObjects(void)
{
    for (int i = 0; i < ARRAYSIZE(classFactoryObjects); i++)
    {
        if (nullptr != classFactoryObjects[i])
        {
            delete classFactoryObjects[i];
            classFactoryObjects[i] = nullptr;
        }
    }

    DeleteObject(TextServices::defaultlFontHandle);
}

void DllAddRef(void)
{
    InterlockedIncrement(&TextServices::dllRefCount);
}
void DllRelease(void)
{
    if (InterlockedDecrement(&TextServices::dllRefCount) < 0)
    {
        EnterCriticalSection(&TextServices::TextServiceCriticalSection);

        if (nullptr != classFactoryObjects[0])
        {
            FreeGlobalObjects();
            //HeapDestroy(hHeap);
        }
        LeaveCriticalSection(&TextServices::TextServiceCriticalSection);
    }
}

_Check_return_
STDAPI DllGetClassObject(_In_ REFCLSID rclsid, _In_ REFIID riid, _Outptr_ LPVOID FAR* ppv)
{

    //HeapCreate(HEAP_NO_SERIALIZE, 0,0);
    if (classFactoryObjects[0] == nullptr)
    {
        EnterCriticalSection(&TextServices::TextServiceCriticalSection);

        // need to check ref again after grabbing mutex
        if (classFactoryObjects[0] == nullptr)
        {
            BuildGlobalObjects();
        }

        LeaveCriticalSection(&TextServices::TextServiceCriticalSection);
    }
    if (IsEqualIID(riid, IID_IClassFactory) ||
        IsEqualIID(riid, IID_IUnknown))
    {
        for (int i = 0; i < ARRAYSIZE(classFactoryObjects); i++)
        {
            if (nullptr != classFactoryObjects[i] &&
                IsEqualGUID(rclsid, classFactoryObjects[i]->_rclsid))
            {
                *ppv = (void*)classFactoryObjects[i];
                DllAddRef();    // class factory holds DLL ref count
                return NOERROR;
            }
        }
    }

    *ppv = nullptr;

    return CLASS_E_CLASSNOTAVAILABLE;
}
__control_entrypoint(DllExport)
STDAPI DllCanUnloadNow(void)
{
    if (TextServices::dllRefCount >= 0)
    {
        return S_FALSE;
    }
    return S_OK;
}

STDAPI DllUnregisterServer(void)
{
    UnregisterProfiles();
    UnregisterCategories();
    UnregisterServer();
    return S_OK;
}

STDAPI DllRegisterServer(void)
{
    if ((!RegisterServer()) || (!RegisterProfiles()) || (!RegisterCategories()))
    {
        DllUnregisterServer();
        return E_FAIL;
    }
    return S_OK;
}