#pragma once
#include <string>

struct Queue {
    int capacity = 0;
    int head = 0;
    int tail = 0;
    int count = 0;
    std::string* data = nullptr;

    Queue() = default;
    Queue(const Queue&) = delete;
    Queue& operator=(const Queue&) = delete;
};

void init(Queue& q, int initcapacity);
void QPUSH(Queue& q, const std::string& value);
bool QPOP(Queue& q, std::string& result);
void freeMemory(Queue& q);
