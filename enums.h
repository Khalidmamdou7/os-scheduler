#pragma once

enum SchedulingAlgorithm {
    SJF = 1, // Shortest Job First
    HPF = 2, // Preemptive Highest Priority First
    RR = 3, // Round Robin
    MLF = 4 // Multiple level Feedback Loop
};

enum State {
    READY = 1,
    RUNNING = 2,
    BLOCKED = 4,
    TERMINATED = 5
};