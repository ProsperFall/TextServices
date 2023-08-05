#include "TextServices.h"
#include <cassert>

STDAPI TextService::QueryInterface(REFIID riid, _Outptr_ void** ppvObj)
{
    if (ppvObj == nullptr)
    {
        return E_INVALIDARG;
    }

    *ppvObj = nullptr;

    if (IsEqualIID(riid, IID_IUnknown) ||
        IsEqualIID(riid, IID_ITfTextInputProcessor))
    {
        *ppvObj = (ITfTextInputProcessor*)this;
    }
    else if (IsEqualIID(riid, IID_ITfTextInputProcessorEx))
    {
        *ppvObj = (ITfTextInputProcessorEx*)this;
    }

    if (*ppvObj)
    {
        AddRef();
        return S_OK;
    }

    return E_NOINTERFACE;
}

STDAPI_(ULONG) TextService::AddRef() 
{
    return ++lrefCount;
}

STDAPI_(ULONG) TextService::Release()
{
    LONG cr = --lrefCount;

    assert(lrefCount >= 0);

    if (lrefCount == 0)
    {
        delete this;
    }

    return cr;
}