#ifndef NODE_MANAGER_H
#define NODE_MANAGER_H

#include "ComputeNode.h"
#include <map>
#include <set>
#include <string>
#include <zmq.h>

class NodeManager {
private:
    std::map<int, ComputeNode*> nodes; 
    std::map<int, std::set<int>> children;

    void* context;
    void* sendSocket; // Сокет для отправки сообщений
    void* recvSocket; // Сокет для получения сообщений

    void processCommand(int id, const std::string& command);
    void killNodeRecursive(int id);

public:
    NodeManager();
    ~NodeManager();

    std::string createNode(int id, int parent = -1);
    std::string execCommand(int id, const std::string& command);
    std::string killNode(int id);
    std::string pingNode(int id);

    void sendMessage(const std::string& message);
    std::string receiveMessage();
};

#endif