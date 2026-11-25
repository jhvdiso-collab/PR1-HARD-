#pragma once
#include <string>

struct Node {
    std::string key;
    std::string value;
    Node* next = nullptr;
};

struct HashTable {
    int capacity = 0;
    Node** buckets = nullptr;
    int size = 0;

    HashTable() = default;
    HashTable(const HashTable&) = delete;
    HashTable& operator=(const HashTable&) = delete;
};

void init(HashTable& ht);
int bucketIndex(const HashTable& ht, const std::string& key);
Node* findNode(const HashTable& ht, const std::string& key);
Node* HSET(HashTable& ht, const std::string& key, const std::string& value);
bool HGET(const HashTable& ht, const std::string& key, std::string& result);
bool HDEL(HashTable& ht, const std::string& key);
void rehash(HashTable& ht);
void freeMemory(HashTable& ht);
