#include <Windows.h>
#include <winternl.h>
#include <stdio.h>


// Avoiding debuggers by Sleepy https://github.com/SleepyG8

struct mystructs {

    PPEB pebaddr;
    PRTL_USER_PROCESS_PARAMETERS params;

}mystructs;

typedef NTSTATUS(NTAPI* pNtQueryInformationProcess)(
    HANDLE, PROCESSINFOCLASS, PVOID, ULONG, PULONG);

BOOL checkDebug(HANDLE hProcess) {
    HMODULE hNtDll = GetModuleHandle(L"ntdll.dll");
    if (!hNtDll) {
        printf("Failed to load ntdll.dll\n");
        return FALSE;
    }

    pNtQueryInformationProcess NtQueryInformationProcess =
        (pNtQueryInformationProcess)GetProcAddress(hNtDll, "NtQueryInformationProcess");
    if (!NtQueryInformationProcess) {
        printf("Failed to get NtQueryInformationProcess\n");
        return FALSE;
    }

    PEB pbi;
    PROCESS_BASIC_INFORMATION proc = { 0 };
    ULONG returnlen;
    NTSTATUS status = NtQueryInformationProcess(hProcess, ProcessBasicInformation, &proc, sizeof(PROCESS_BASIC_INFORMATION), &returnlen);
    if (status != 0) {
        printf("NtQueryInformationProcess failed (Status 0x%08X)\n", status);
        return FALSE;
    }

    struct mystructs peb;
    peb.pebaddr = proc.PebBaseAddress;
    //printf("Peb struct address: %p", peb.pebaddr);

    PEB_LDR_DATA ldrData;
    if (!ReadProcessMemory(hProcess, proc.PebBaseAddress, &pbi, sizeof(pbi), NULL)) {
        printf("Failed to read PEB from the target process (Error %lu)\n", GetLastError());
        return FALSE;
    }

    if (pbi.BeingDebugged == 0) {
        printf("fake injecting of code\n"); // 0 means not being debugged
        return TRUE;
    }
    else {
        printf("Nice try your reading the decoy\n"); // 1 means a debugger is found in the peb
        return FALSE;
    }

}


int main() {

    HANDLE hProcess = GetCurrentProcess();

    if (!checkDebug(hProcess)) {

        printf("running decoy function and do random stuff\n");

    }

    return 0;
}
