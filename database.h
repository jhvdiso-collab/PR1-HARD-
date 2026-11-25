#pragma once
#include <unordered_map>
#include "queue.h"
#include "stack.h"
#include "set.h"
#include "hash_table.h"
struct Database {
    std::unordered_map<std::string, Queue> queues;
    std::unordered_map<std::string, Stack> stacks;
    std::unordered_map<std::string, Set> sets;
    std::unordered_map<std::string, HashTable> hashes;

    Database() = default;
    Database(const Database&) = delete;
    Database& operator=(const Database&) = delete;
};
//Queue
void QPUSH(Database& db, const std::string& name, const std::string& value);
bool QPOP(Database& db, const std::string& name, std::string& result);

void freeMemory(Database& db);

//Stack
void SPUSH(Database& db, const std::string& name, const std::string& value);
bool SPOP(Database& db, const std::string& name, std::string& result);

//SET
void SADD(Database& db, const std::string& name, const std::string& value);
bool SISMEMBER(const Database& db, const std::string& name, const std::string& value);
bool SREM(Database& db, const std::string& name, const std::string& value);

//Hash-Table
void HSET(Database& db, const std::string& name, const std::string& key, const std::string& value);
bool HGET(const Database& db, const std::string& name, const std::string& key, std::string& result);
bool HDEL(Database& db, const std::string& name, const std::string& key);
