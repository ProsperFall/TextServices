#include "TextServices.h"

STDAPI TextService::Activate(ITfThreadMgr* pThreadMgr, TfClientId tfClientId)
{
	lpThreadMgr = pThreadMgr;
	lpThreadMgr->AddRef();

	ltfClientId =tfClientId;
	ldwActivateFlags = 0;

	return S_OK;
ExitError:
	Deactivate();
	return E_FAIL;
}
STDAPI TextService::Deactivate()
{
	return S_OK;
}