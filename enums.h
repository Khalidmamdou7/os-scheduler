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

enum LogState {
    STARTED = 1,
    RESUMED = 2,
    STOPPED = 3,
    FINISHED = 4
};

enum LogMemState {
    ALLOCATED = 1,
    DEALLOCATED = 2,
    FAILED = 3
};