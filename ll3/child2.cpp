#include <iostream>
#include <fstream>
#include <string>
#include <windows.h>

#define MMAP_FILE_NAME "Local\\SharedMemory"
#define EVENT_NAME "Local\\DataReadyEvent"

std::string removeVowels(const std::string &input) {
    const std::string vowels = "aeiouAEIOU";
    std::string result;
    for (char ch : input) {
        if (vowels.find(ch) == std::string::npos) {
            result += ch;
        }
    }
    return result;
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        std::cerr << "File name argument is missing." << std::endl;
        return 1;
    }

    std::ofstream outFile(argv[1]);
    if (!outFile.is_open()) {
        std::cerr << "Failed to open file for writing." << std::endl;
        return 1;
    }

    HANDLE hFileMapping = OpenFileMapping(FILE_MAP_READ | FILE_MAP_WRITE, FALSE, MMAP_FILE_NAME);
    if (hFileMapping == NULL) {
        std::cerr << "Failed to open file mapping: " << GetLastError() << std::endl;
        return 1;
    }

    HANDLE hEvent = OpenEvent(SYNCHRONIZE, FALSE, EVENT_NAME);
    if (hEvent == NULL) {
        std::cerr << "Failed to open event: " << GetLastError() << std::endl;
        CloseHandle(hFileMapping);
        return 1;
    }

    LPVOID pBuffer = MapViewOfFile(hFileMapping, FILE_MAP_READ, 0, 0, 1024);
    if (pBuffer == NULL) {
        std::cerr << "Failed to map view of file.\n";
        CloseHandle(hFileMapping);
        CloseHandle(hEvent);
        return 1;
    }

    while (true) {
        WaitForSingleObject(hEvent, INFINITE);

        std::string inputLine(static_cast<char*>(pBuffer));

        if (inputLine == "exit") {
            break;
        }

        std::cout << "Child received data: " << inputLine << std::endl;

        std::string output = removeVowels(inputLine);

        outFile << output << std::endl;
    }

    outFile.close();
    CloseHandle(hFileMapping);
    CloseHandle(hEvent);

    return 0;
}