#include "TextServices.h"

STDAPI TextService::ActivateEx(ITfThreadMgr* pThreadMgr, TfClientId tfClientId, DWORD dwFlags)
{
	lpThreadMgr = pThreadMgr;
	lpThreadMgr->AddRef();

	ltfClientId = tfClientId;
	ldwActivateFlags = dwFlags;

	return S_OK;
ExitError:
	Deactivate();
	return E_FAIL;
}