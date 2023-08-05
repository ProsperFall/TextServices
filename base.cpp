#include <wtypes.h>
#include "define.h"

const BYTE GuidSymbols[] = {
    3, 2, 1, 0, '-', 5, 4, '-', 7, 6, '-', 8, 9, '-', 10, 11, 12, 13, 14, 15
};

const WCHAR HexDigits[] = L"0123456789ABCDEF";

BOOL CLSIDToString(REFGUID refGUID, _Out_writes_(39) WCHAR* pCLSIDString)
{
    WCHAR* pTemp = pCLSIDString;
    const BYTE* pBytes = (const BYTE*)&refGUID;

    DWORD j = 0;
    pTemp[j++] = L'{';
    for (int i = 0; i < sizeof(GuidSymbols) && j < (CLSID_STRLEN - 2); i++)
    {
        if (GuidSymbols[i] == '-')
        {
            pTemp[j++] = L'-';
        }
        else
        {
            pTemp[j++] = HexDigits[(pBytes[GuidSymbols[i]] & 0xF0) >> 4];
            pTemp[j++] = HexDigits[(pBytes[GuidSymbols[i]] & 0x0F)];
        }
    }

    pTemp[j++] = L'}';
    pTemp[j] = L'\0';

    return TRUE;
}