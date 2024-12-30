#include <windows.h>
#include <iostream>
#include <string>
#include <cstdlib>
#include <ctime>
#include <thread>  
#include <chrono> 

#define MMAP_FILE_NAME "Local\\SharedMemory"
#define EVENT_NAME "Local\\DataReadyEvent"

int main() {
    HANDLE hFileMapping, hEvent;
    SECURITY_ATTRIBUTES sa = {sizeof(SECURITY_ATTRIBUTES), NULL, TRUE};

    hFileMapping = CreateFileMapping(INVALID_HANDLE_VALUE, &sa, PAGE_READWRITE, 0, 1024, MMAP_FILE_NAME);
    if (hFileMapping == NULL) {
        std::cerr << "Unable to create file mapping: " << GetLastError() << std::endl;
        return 1;
    }

    hEvent = CreateEvent(&sa, FALSE, FALSE, EVENT_NAME);
    if (hEvent == NULL) {
        std::cerr << "Unable to create event: " << GetLastError() << std::endl;
        CloseHandle(hFileMapping);
        return 1;
    }

    std::string fileName1, fileName2;
    std::cout << "Enter the name of the file for child process 1: ";
    std::getline(std::cin, fileName1);
    std::cout << "Enter the name of the file for child process 2: ";
    std::getline(std::cin, fileName2);

    STARTUPINFO si1 = {sizeof(STARTUPINFO)};
    STARTUPINFO si2 = {sizeof(STARTUPINFO)};
    PROCESS_INFORMATION pi1, pi2;
    ZeroMemory(&pi1, sizeof(PROCESS_INFORMATION));
    ZeroMemory(&pi2, sizeof(PROCESS_INFORMATION));

    char cmdLine1[256], cmdLine2[256];
    sprintf_s(cmdLine1, sizeof(cmdLine1), "child1.exe \"%s\"", fileName1.c_str());
    sprintf_s(cmdLine2, sizeof(cmdLine2), "child2.exe \"%s\"", fileName2.c_str());

    if (!CreateProcess(NULL, cmdLine1, NULL, NULL, TRUE, 0, NULL, NULL, &si1, &pi1)) {
        std::cerr << "Failed to create child process 1.\n";
        CloseHandle(hFileMapping);
        CloseHandle(hEvent);
        return 1;
    }

    if (!CreateProcess(NULL, cmdLine2, NULL, NULL, TRUE, 0, NULL, NULL, &si2, &pi2)) {
        std::cerr << "Failed to create child process 2.\n";
        CloseHandle(hFileMapping);
        CloseHandle(hEvent);
        return 1;
    }

    CloseHandle(pi1.hThread);
    CloseHandle(pi2.hThread);

    LPVOID pBuffer = MapViewOfFile(hFileMapping, FILE_MAP_WRITE, 0, 0, 1024);
    if (pBuffer == NULL) {
        std::cerr << "Failed to map view of file.\n";
        CloseHandle(hFileMapping);
        CloseHandle(hEvent);
        return 1;
    }

    std::string inputLine;
    bool done = false;

    while (!done && std::getline(std::cin, inputLine)) {
        if (inputLine == "exit") {
            done = true;  
            memcpy(pBuffer, "exit", 5); 
            SetEvent(hEvent); 
            break;
        }

        if (inputLine.size() >= 1024) {
            std::cerr << "Input line is too long, skipping.\n";
            continue;
        }

        memcpy(pBuffer, inputLine.c_str(), inputLine.size() + 1);
        std::cout << "Sending data: " << inputLine << std::endl;

        SetEvent(hEvent);

        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }

    WaitForSingleObject(pi1.hProcess, INFINITE);
    WaitForSingleObject(pi2.hProcess, INFINITE);

    CloseHandle(pi1.hProcess);
    CloseHandle(pi2.hProcess);
    CloseHandle(hFileMapping);
    CloseHandle(hEvent);

    return 0;
}