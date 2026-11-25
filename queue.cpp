#include "queue.h"
#include <iostream>
#include <string>
#include <limits>
#include <stdexcept>

using namespace std;

void init(Queue& q, int initcapacity) {
    if (q.data != nullptr) return;
    q.capacity = initcapacity > 0 ? initcapacity : 1;
    string* newData = new string[q.capacity];
    q.data = newData;
    q.head = 0;
    q.tail = 0;
    q.count = 0;
}

void QPUSH(Queue& q, const string& value) {
    if (q.data == nullptr) init(q, 5);
    if (q.count == q.capacity) {
        if (q.capacity > numeric_limits<int>::max() / 2) {
            throw length_error("Слишком большая очередь");
        }
        int newCapacity = q.capacity * 2;
        string* newData = new string[newCapacity];
        try {
            for (int i = 0; i < q.count; i++) {
                newData[i] = q.data[(q.head + i) % q.capacity];
            }
        } catch (...) {
            delete[] newData;
            throw;
        }
        delete[] q.data;
        q.data = newData;
        q.capacity = newCapacity;
        q.head = 0;
        q.tail = q.count;
    }
    q.data[q.tail] = value;
    q.tail = (q.tail + 1) % q.capacity;
    q.count++;
}

bool QPOP(Queue& q, string& result) {
    if (q.count == 0) return false;
    result = q.data[q.head];
    q.head = (q.head + 1) % q.capacity;
    q.count--;
    return true;
}

void freeMemory(Queue& q) {
    delete[] q.data;
    q.data = nullptr;
    q.capacity = 0;
    q.head = 0;
    q.tail = 0;
    q.count = 0;
}
