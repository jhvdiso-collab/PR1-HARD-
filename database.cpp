#include "database.h"
#include <iostream>
#include <string>

using namespace std;

void QPUSH(Database& db, const string& name, const string& value) {
    auto entry = db.queues.try_emplace(name);
    if (entry.second == true) {
        init(entry.first->second, 5);
    }
    QPUSH(entry.first->second, value);
}

bool QPOP(Database& db, const string& name, string& result) {
    auto it = db.queues.find(name);
    if (it == db.queues.end()) return false;
    return QPOP(it->second, result);
}

void SPUSH(Database& db, const string& name, const string& value) {
    auto entry = db.stacks.try_emplace(name);
    if (entry.second) {
        init(entry.first->second, 5);
    }
    SPUSH(entry.first->second, value);
}

bool SPOP(Database& db, const string& name, string& result) {
    auto it = db.stacks.find(name);
    if (it == db.stacks.end()) return false;
    return SPOP(it->second, result);
}

void SADD(Database& db, const string& name, const string& value) {
    auto entry = db.sets.try_emplace(name);
    if (entry.second) {
        init(entry.first->second);
    }
    SADD(entry.first->second, value);
}

bool SISMEMBER(const Database& db, const string& name, const string& value) {
    auto it = db.sets.find(name);
    if (it == db.sets.end()) return false;
    return SISMEMBER(it->second, value);
}

bool SREM(Database& db, const string& name, const string& value) {
    auto it = db.sets.find(name);
    if (it == db.sets.end()) return false;
    return SREM(it->second, value);
}

void HSET(Database& db, const string& name, const string& key, const string& value) {
    auto entry = db.hashes.try_emplace(name);
    if (entry.second) {
        init(entry.first->second);
    }
    HSET(entry.first->second, key, value);
}

bool HGET(const Database& db, const string& name, const string& key, string& result) {
    auto it = db.hashes.find(name);
    if (it == db.hashes.end()) return false;
    return HGET(it->second, key, result);
}

bool HDEL(Database& db, const string& name, const string& key) {
    auto it = db.hashes.find(name);
    if (it == db.hashes.end()) return false;
    return HDEL(it->second, key);
}

void freeMemory(Database& db) {
    for (auto& entry : db.queues) {
        freeMemory(entry.second);
    }
    for (auto& entry : db.stacks) {
        freeMemory(entry.second);
    }
    for (auto& entry : db.sets) {
        freeMemory(entry.second);
    }
    for (auto& entry : db.hashes) {
        freeMemory(entry.second);
    }
    db.queues.clear();
    db.stacks.clear();
    db.sets.clear();
    db.hashes.clear();
}
