#include <iostream>
#include <Windows.h>
#include <vector>
#include <TlHelp32.h>

DWORD GetPid(const wchar_t* procName)
{
    DWORD pid = 0;
    HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    if (hSnapshot == INVALID_HANDLE_VALUE) return 0;

    PROCESSENTRY32 pe;
    pe.dwSize = sizeof(PROCESSENTRY32);

    if (Process32First(hSnapshot, &pe))
    {
        do {
            if (_wcsicmp(pe.szExeFile, procName) == 0) {
                pid = pe.th32ProcessID;
                break;
            }
        } while (Process32Next(hSnapshot, &pe));
    }

    CloseHandle(hSnapshot);
    return pid;
}


/*DWORD GetPid(const char* processName)
{
    std::wstring wchar = PCharToWString(processName);
    const wchar_t* wProcessName = wchar.data();

    DWORD pid = 0;

    HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    if (!hSnapshot) return 0;

    PROCESSENTRY32 pe;
    pe.dwSize = sizeof(PROCESSENTRY32);

    if (!Process32First(hSnapshot, &pe)) {
        CloseHandle(hSnapshot);
        return 0;
    }

    do {
        if (_wcsicmp(pe.szExeFile, wProcessName) != 0) continue;
        pid = pe.th32ProcessID;
        break;
    } while (Process32Next(hSnapshot, &pe));

    CloseHandle(hSnapshot);
    return pid;
}*/

bool Unhooked(HANDLE hProcess, const char* modeuleName, const char* APIname, const DWORD numBytes)
{
	FARPROC hLoadA = GetProcAddress(GetModuleHandleA(modeuleName), APIname);
	if (!hLoadA) return false;

	std::vector<BYTE> arr(numBytes);
	LPVOID pArr = arr.data();

	MoveMemory(pArr, hLoadA, numBytes);

	SIZE_T numBytesWriten;
	return WriteProcessMemory(hProcess, hLoadA, pArr, numBytes, &numBytesWriten);
}

int main()
{
	

}

