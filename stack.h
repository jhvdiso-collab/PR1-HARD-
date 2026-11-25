#pragma once
#include <string>
struct Stack {
    int topIndex = -1;
    int capacity = 0;
    std::string* data = nullptr;

    Stack() = default;
    Stack(const Stack&) = delete;
    Stack& operator=(const Stack&) = delete;
};

void init(Stack& st, int initcapacity);
bool isEmpty(const Stack& st);
void SPUSH(Stack& st, const std::string& value);
bool SPOP(Stack& st, std::string& value);
void freeMemory(Stack& st);
