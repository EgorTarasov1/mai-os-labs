#include "NodeManager.h"
#include <iostream>
#include <sstream>

NodeManager::NodeManager() {
    context = zmq_ctx_new();

    sendSocket = zmq_socket(context, ZMQ_PAIR);
    zmq_bind(sendSocket, "tcp://*:5555"); 
    std::cout << "[NodeManager] Send socket bound to tcp://*:5555" << std::endl;

    recvSocket = zmq_socket(context, ZMQ_PAIR);
    zmq_connect(recvSocket, "tcp://localhost:5555");
    std::cout << "[NodeManager] Receive socket connected to tcp://localhost:5555" << std::endl;
}

NodeManager::~NodeManager() {
    for (auto& pair : nodes) {
        delete pair.second;
    }

    zmq_close(sendSocket);
    zmq_close(recvSocket);

    zmq_ctx_destroy(context);
    std::cout << "[NodeManager] Destroyed ZeroMQ context and sockets" << std::endl;
}

std::string NodeManager::createNode(int id, int parent) {
    if (nodes.find(id) != nodes.end()) {
        return "Error: Already exists";
    }

    if (parent != -1) { 
        if (nodes.find(parent) == nodes.end()) {
            return "Error: Parent not found";
        }
        children[parent].insert(id);
    }

    nodes[id] = new ComputeNode(id);
    std::cout << "[NodeManager] Created node " << id << std::endl;
    return "Ok:" + std::to_string(id);
}

std::string NodeManager::execCommand(int id, const std::string& command) {
    if (nodes.find(id) == nodes.end()) {
        return "Error: Not found";
    }

    std::string result = nodes[id]->execute(command);
    return result;
}

void NodeManager::killNodeRecursive(int id) {
    if (nodes.find(id) != nodes.end()) {
        nodes[id]->killNode();
    }

    if (children.find(id) != children.end()) {
        for (int childId : children[id]) {
            killNodeRecursive(childId);
        }
        children.erase(id);
    }

    nodes.erase(id);
}

std::string NodeManager::killNode(int id) {
    if (nodes.find(id) == nodes.end()) {
        return "Error: Not found";
    }

    killNodeRecursive(id);
    return "Ok: Node " + std::to_string(id) + " killed";
}

std::string NodeManager::pingNode(int id) {
    if (nodes.find(id) == nodes.end()) {
        return "Ok: 0";
    }

    std::string result = nodes[id]->execute("ping");
    return result;
}

void NodeManager::sendMessage(const std::string& message) {
    std::cout << "[sendMessage] Sending message: " << message << std::endl;
    zmq_send(sendSocket, message.c_str(), message.size(), 0);
}

std::string NodeManager::receiveMessage() {
    char buffer[512] = {0};

    std::cout << "[receiveMessage] Waiting for response..." << std::endl;
    zmq_recv(recvSocket, buffer, sizeof(buffer), 0);
    std::cout << "[receiveMessage] Received: " << buffer << std::endl;

    return std::string(buffer);
}