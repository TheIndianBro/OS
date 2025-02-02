#include <iostream>
#include <vector>
#include <queue>
#include <unordered_map>
#include <thread>
#include <chrono>
#include <algorithm>

using namespace std;

// Process Control Block (PCB) Structure
struct Process {
    int pid;
    int burst_time;
    int priority;
    int arrival_time;
};

// CPU Scheduler
class Scheduler {
private:
    queue<Process> ready_queue;
    vector<Process> processes;
    int time_quantum;
public:
    Scheduler(int tq) : time_quantum(tq) {}

    void addProcess(Process p) {
        ready_queue.push(p);
        processes.push_back(p);
    }

    void roundRobin() {
        queue<Process> temp_queue = ready_queue;
        while (!temp_queue.empty()) {
            Process current = temp_queue.front();
            temp_queue.pop();
            cout << "Executing Process: " << current.pid << endl;
            this_thread::sleep_for(chrono::milliseconds(time_quantum * 100));
            cout << "Process " << current.pid << " execution completed" << endl;
        }
    }

    void shortestJobFirst() {
        sort(processes.begin(), processes.end(), [](Process a, Process b) {
            return a.burst_time < b.burst_time;
        });
        for (auto &p : processes) {
            cout << "Executing Process: " << p.pid << " with burst time: " << p.burst_time << endl;
            this_thread::sleep_for(chrono::milliseconds(p.burst_time * 100));
            cout << "Process " << p.pid << " execution completed" << endl;
        }
    }
};

// Simple File System
class FileSystem {
private:
    unordered_map<string, string> files;
public:
    void createFile(string name, string content) {
        files[name] = content;
    }

    void readFile(string name) {
        if (files.find(name) != files.end()) {
            cout << "File Content: " << files[name] << endl;
        } else {
            cout << "File not found" << endl;
        }
    }
};

// Memory Management (Simple Paging)
class MemoryManager {
private:
    vector<int> memory;
    int size;
public:
    MemoryManager(int s) : size(s) {
        memory.resize(size, -1);
    }

    void allocate(int pid, int address) {
        if (address >= 0 && address < size) {
            memory[address] = pid;
            cout << "Memory allocated for process " << pid << " at address " << address << endl;
        } else {
            cout << "Memory allocation failed" << endl;
        }
    }
};

// Kernel Simulation
class Kernel {
private:
    Scheduler scheduler;
    FileSystem fs;
    MemoryManager mm;
public:
    Kernel(int tq, int mem_size) : scheduler(tq), mm(mem_size) {}

    void createProcess(int pid, int burst_time, int priority, int arrival_time) {
        Process p = {pid, burst_time, priority, arrival_time};
        scheduler.addProcess(p);
    }

    void createFile(string name, string content) {
        fs.createFile(name, content);
    }

    void readFile(string name) {
        fs.readFile(name);
    }

    void allocateMemory(int pid, int address) {
        mm.allocate(pid, address);
    }

    void runRoundRobin() {
        scheduler.roundRobin();
    }

    void runShortestJobFirst() {
        scheduler.shortestJobFirst();
    }
};

int main() {
    Kernel kernel(2, 100);
    
    // Creating processes
    kernel.createProcess(1, 5, 1, 0);
    kernel.createProcess(2, 3, 2, 1);
    
    // Running the scheduler
    cout << "Running Round Robin Scheduling:" << endl;
    kernel.runRoundRobin();
    
    cout << "\nRunning Shortest Job First Scheduling:" << endl;
    kernel.runShortestJobFirst();
    
    // File System Operations
    kernel.createFile("test.txt", "Hello, OS!");
    kernel.readFile("test.txt");
    
    // Memory Allocation
    kernel.allocateMemory(1, 10);
    kernel.allocateMemory(2, 20);
    
    return 0;
}
