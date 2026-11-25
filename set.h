#pragma once
#include "hash_table.h"

struct Set {
    HashTable set;
};

void init(Set& s);
bool SISMEMBER(const Set& set, const std::string& str);
void SADD(Set& s, const std::string& element);
bool SREM(Set& s, const std::string& element);
void freeMemory(Set& s);
