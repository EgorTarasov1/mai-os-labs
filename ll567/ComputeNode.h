#ifndef COMPUTE_NODE_H
#define COMPUTE_NODE_H

#include <chrono>
#include <string>

class ComputeNode {
private:
    int id;
    bool timerRunning;   
    bool isAvailable;    
    std::chrono::steady_clock::time_point startTime;
    long long elapsedTime;

public:
    ComputeNode(int nodeId);

    std::string execute(const std::string& command);
    void killNode();            
    bool checkAvailability();   
};

#endif