#include "ComputeNode.h"
#include <iostream> 
ComputeNode::ComputeNode(int nodeId) 
    : id(nodeId), timerRunning(false), isAvailable(true), elapsedTime(0) {}

std::string ComputeNode::execute(const std::string& command) {
    std::cout << "[ComputeNode::execute] Received command: " << command << std::endl;

    if (!isAvailable) {
        std::cout << "[ComputeNode] Node " << id << " is unavailable" << std::endl;
        return "Error:" + std::to_string(id) + ": Node is unavailable";
    }

    if (command == "start") {
        if (!timerRunning) {
            timerRunning = true;
            startTime = std::chrono::steady_clock::now();
            std::cout << "[ComputeNode] Timer started for node " << id << std::endl;
        } else {
            std::cout << "[ComputeNode] Timer already running for node " << id << std::endl;
        }
        return "Ok:" + std::to_string(id);
    } else if (command == "stop") {
        if (timerRunning) {
            auto now = std::chrono::steady_clock::now();
            elapsedTime += std::chrono::duration_cast<std::chrono::milliseconds>(now - startTime).count();
            timerRunning = false;
            std::cout << "[ComputeNode] Timer stopped for node " << id 
                      << ", total elapsed time: " << elapsedTime << " milliseconds" << std::endl;
        } else {
            std::cout << "[ComputeNode] Timer already stopped for node " << id << std::endl;
        }
        return "Ok:" + std::to_string(id);
    } else if (command == "time") {
        long long currentTime = elapsedTime;
        if (timerRunning) {
            auto now = std::chrono::steady_clock::now();
            currentTime += std::chrono::duration_cast<std::chrono::milliseconds>(now - startTime).count();
        }
        std::cout << "[ComputeNode] Current time for node " << id << ": " 
                  << currentTime << " milliseconds" << std::endl;
        return "Ok:" + std::to_string(id) + ": " + std::to_string(currentTime);
} else if (command == "ping") {
        return isAvailable ? "Ok: 1" : "Ok: 0";
    }

    std::cout << "[ComputeNode] Unknown command: " << command << " for node " << id << std::endl;
    return "Error:" + std::to_string(id) + ": Unknown command";
}

void ComputeNode::killNode() {
    isAvailable = false;
    std::cout << "[ComputeNode] Node " << id << " killed" << std::endl;
}

bool ComputeNode::checkAvailability() {
    return isAvailable;
}