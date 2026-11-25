#include "set.h"
#include <iostream>
#include <string>

using namespace std;

void init(Set& s) {
    init(s.set);
}

bool SISMEMBER(const Set& s, const string& element) {
    Node* find = findNode(s.set, element);
    return find != nullptr;
}

void SADD(Set& s, const string& element) {
    HSET(s.set, element, "");
}

bool SREM(Set& s, const string& element) {
    return HDEL(s.set, element);
}

void freeMemory(Set& s) {
    freeMemory(s.set);
}
