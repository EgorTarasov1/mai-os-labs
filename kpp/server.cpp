#include <iostream>
#include <windows.h>
#include <string>
#include <thread>
#include <vector>
#include "GameManager.h"
#include "Player.h"

int main() {
    GameManager gameManager;

    while (true) {
        std::cout << "Waiting for clients..." << std::endl;

        HANDLE hPipe = CreateNamedPipeW(
            L"\\\\.\\pipe\\MyPipe",
            PIPE_ACCESS_DUPLEX,
            PIPE_TYPE_MESSAGE | PIPE_READMODE_MESSAGE | PIPE_WAIT,
            PIPE_UNLIMITED_INSTANCES,
            512, 512, 0, NULL);

        if (hPipe == INVALID_HANDLE_VALUE) {
            std::cerr << "Failed to create pipe. Error: " << GetLastError() << std::endl;
            continue;
        }

        if (ConnectNamedPipe(hPipe, NULL)) {
            std::thread([hPipe, &gameManager]() {
                char buffer[512];
                DWORD bytesRead;

                if (ReadFile(hPipe, buffer, sizeof(buffer), &bytesRead, NULL)) {
                    std::string message(buffer, bytesRead);
                    std::string response = gameManager.processRequest(message);

                    DWORD bytesWritten;
                    WriteFile(hPipe, response.c_str(), response.size(), &bytesWritten, NULL);
                }

                CloseHandle(hPipe);
            }).detach();
        }
    }

    return 0;
}