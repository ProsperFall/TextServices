#pragma once
#include "new.h"
#include "msctf.h"
#include "ctffunc.h"
#include "server.h"

namespace TextServices
{
    extern HINSTANCE dllInstanceHandle;

    extern LONG dllRefCount;

    extern CRITICAL_SECTION TextServiceCriticalSection;
    extern HFONT defaultlFontHandle;

	extern const CLSID	TextServiceCLSID;
	extern const GUID	TextServiceGuidProfile;
}

class TextService : public ITfTextInputProcessorEx  // :public ITfTextInputProcessor,
    //public ITfThreadMgrEventSink,
    //public ITfTextEditSink,
    //public ITfKeyEventSink,
    //public ITfCompositionSink,
    //public ITfDisplayAttributeProvider,
    //public ITfActiveLanguageProfileNotifySink,
    //public ITfThreadFocusSink,
    //public ITfFunctionProvider,
    //public ITfFnGetPreferredTouchKeyboardLayout
{
    TextService();
    ~TextService();
public:
    //IUnknown
    STDMETHODIMP QueryInterface(REFIID riid, _Outptr_ void** ppvObj);
    STDMETHODIMP_(ULONG) AddRef(void);
    STDMETHODIMP_(ULONG) Release(void);

    //ITfInputProcessor
    STDMETHODIMP Activate(ITfThreadMgr* pThreadMgr, TfClientId tfClientId);
    STDMETHODIMP Deactivate(void);

    //ITfInputProcessorEx
    STDMETHODIMP ActivateEx(ITfThreadMgr* pThreadMgr, TfClientId tfClientId, DWORD dwFlags);

    //ex
    static HRESULT CreateInstance(_In_ IUnknown* pUnkOuter, REFIID riid, _Outptr_ void** ppvObj);

private:
    //ITfInputProcessor
    ITfThreadMgr* lpThreadMgr;
    TfClientId ltfClientId;
    //ITfInputProcessorEx
    DWORD ldwActivateFlags;

    LONG lrefCount;
};