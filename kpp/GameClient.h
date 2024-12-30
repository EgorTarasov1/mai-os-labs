#ifndef GAMECLIENT_H
#define GAMECLIENT_H

#include <string>
#include <windows.h>

class GameClient {
public:
    std::string sendRequest(const std::string& request);

private:
    HANDLE connectToServer();
};

#endif