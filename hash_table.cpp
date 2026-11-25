#include "hash_table.h"
#include <iostream>
#include <functional>
#include <limits>
#include <stdexcept>
using namespace std;

void init(HashTable& ht) {
    if (ht.buckets != nullptr) return;
    ht.capacity = 32;
    ht.size = 0;
    ht.buckets = new Node*[ht.capacity]();
}

int bucketIndex(const HashTable& ht, const string& key) {
    if (ht.capacity <= 0 || ht.buckets == nullptr) {
        throw logic_error("HashTable не инициализирована");
    }
    hash<string> hasher;
    auto hashValue = hasher(key);
    return static_cast<int>(hashValue % static_cast<size_t>(ht.capacity));
}

Node* findNode(const HashTable& ht, const string& key) {
    if (ht.buckets == nullptr || ht.capacity <= 0) return nullptr;
    Node* current = ht.buckets[bucketIndex(ht, key)];
    while (current != nullptr) {
        if (current->key == key) return current;
        current = current->next;
    }
    return nullptr;
}

Node* HSET(HashTable& ht, const string& key, const string& value) {
    if (ht.buckets == nullptr) init(ht);
    Node* current = findNode(ht, key);
    if (current != nullptr) {
        current->value = value;
        return current;
    }
    else {
        if ((static_cast<long long>(ht.size) + 1) * 4 >
            static_cast<long long>(ht.capacity) * 3) {
            rehash(ht);
        }
        int index = bucketIndex(ht, key);
        current = ht.buckets[index];
        Node* newNode = new Node();
        newNode->key = key;
        newNode->value = value;
        newNode->next = current;
        ht.buckets[index] = newNode;
        ht.size++;
        return newNode;
    }
}

bool HGET(const HashTable& ht, const string& key, string& result) {
    Node* current = findNode(ht, key);
    if (current == nullptr) return false;
    result = current->value;
    return true;
}

bool HDEL(HashTable& ht, const string& key) {
    if (ht.buckets == nullptr || ht.capacity <= 0) return false;
    int index = bucketIndex(ht, key);
    Node* current = ht.buckets[index];
    Node* previous = nullptr;
    while (current != nullptr && current->key != key) {
        previous = current;
        current = current->next;
    }
    if (current == nullptr) return false;
    if (previous == nullptr) {
        ht.buckets[index] = current->next;
    }
    else {
        previous->next = current->next;
    }
    delete current;
    ht.size--;
    return true;
}

void rehash(HashTable& ht) {
    if (ht.buckets == nullptr || ht.capacity <= 0) {
        init(ht);
        return;
    }
    if (ht.capacity > numeric_limits<int>::max() / 2) {
        throw length_error("Слишком большая хеш-таблица");
    }
    int newCapacity = ht.capacity * 2;
    Node** newBuckets = new Node*[newCapacity]();
    for (int i = 0; i < ht.capacity; i++) {
        Node* current = ht.buckets[i];
        while (current != nullptr) {
            Node* next = current->next;
            hash<string> hasher;
            auto tmp = hasher(current->key);
            int newIndex = static_cast<int>(tmp % static_cast<size_t>(newCapacity));
            current->next = newBuckets[newIndex];
            newBuckets[newIndex] = current;
            current = next;
        }
    }
    delete[] ht.buckets;
    ht.buckets = newBuckets;
    ht.capacity = newCapacity;
}

void freeMemory(HashTable& ht) {
    for (int i = 0; i < ht.capacity; i++) {
        Node* current = ht.buckets[i];
        while (current != nullptr) {
            Node* tmp = current->next;
            delete current;
            current = tmp;
        }
    }
    delete[] ht.buckets;
    ht.buckets = nullptr;
    ht.size = 0;
    ht.capacity = 0;
}
