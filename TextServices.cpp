#include "msctf.h"
#include "ctffunc.h"

#include "TextServices.h"

namespace TextServices
{
	HINSTANCE dllInstanceHandle;

	LONG dllRefCount = -1;

    CRITICAL_SECTION TextServiceCriticalSection;
	HFONT defaultlFontHandle;

	extern const CLSID	TextServiceCLSID = { 0x54146429, 0xdbd0, 0xb64e,{ 0x54, 0x1d, 0x1a, 0x37, 0x67, 0x3c, 0x4b, 0x02 } };			//54146429-DBD0-B64E-541D-1A37673C4B02
	extern const GUID	TextServiceGuidProfile = { 0xa1b9d221,0x2b23 , 0x2b16,{ 0x3a, 0xee, 0xea, 0x32, 0xcb, 0x08, 0xbe, 0x12 } };
}
TextService::TextService()
{
    DllAddRef();

    lpThreadMgr = nullptr;

    ldwActivateFlags = 0;
    ltfClientId = 0;

    //_threadMgrEventSinkCookie = TF_INVALID_COOKIE;

    //_pTextEditSinkContext = nullptr;
    //_textEditSinkCookie = TF_INVALID_COOKIE;

    //_activeLanguageProfileNotifySinkCookie = TF_INVALID_COOKIE;

    //_dwThreadFocusSinkCookie = TF_INVALID_COOKIE;

    //_pComposition = nullptr;

    //_pCompositionProcessorEngine = nullptr;

    //_candidateMode = CANDIDATE_NONE;
    //_pCandidateListUIPresenter = nullptr;
    //_isCandidateWithWildcard = FALSE;

    //_pDocMgrLastFocused = nullptr;

    //_pSIPIMEOnOffCompartment = nullptr;
    //_dwSIPIMEOnOffCompartmentSinkCookie = 0;
    //_msgWndHandle = nullptr;

    //_pContext = nullptr;

    lrefCount = 1;
}
TextService::~TextService()
{
    DllRelease();
}
HRESULT TextService::CreateInstance(_In_ IUnknown* pUnkOuter, REFIID riid, _Outptr_ void** ppvObj)
{
    TextService* pSampleIME = nullptr;
    HRESULT hr = S_OK;

    if (ppvObj == nullptr)
    {
        return E_INVALIDARG;
    }

    *ppvObj = nullptr;

    if (nullptr != pUnkOuter)
    {
        return CLASS_E_NOAGGREGATION;
    }

    pSampleIME = new (std::nothrow)TextService();
    if (pSampleIME == nullptr)
    {
        return E_OUTOFMEMORY;
    }

    hr = pSampleIME->QueryInterface(riid, ppvObj);

    pSampleIME->Release();

    return hr;
}