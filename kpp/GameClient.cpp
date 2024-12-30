#include "GameClient.h"
#include <windows.h>
#include <iostream>

std::string GameClient::sendRequest(const std::string& request) {
    HANDLE hPipe = connectToServer();
    if (hPipe == INVALID_HANDLE_VALUE) {
        return "Failed to connect to server.";
    }

    DWORD bytesWritten;
    if (!WriteFile(hPipe, request.c_str(), request.size(), &bytesWritten, NULL)) {
        return "Failed to send request.";
    }

    char buffer[512];
    DWORD bytesRead;
    if (!ReadFile(hPipe, buffer, sizeof(buffer), &bytesRead, NULL)) {
        return "Failed to read response.";
    }

    CloseHandle(hPipe);
    return std::string(buffer, bytesRead);
}

HANDLE GameClient::connectToServer() {
    return CreateFileW(
        L"\\\\.\\pipe\\MyPipe",
        GENERIC_READ | GENERIC_WRITE,
        0, NULL, OPEN_EXISTING, 0, NULL);
}