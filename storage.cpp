#include <iostream>
#include <fstream>
#include "storage.h"
#include <iomanip>
#include <sstream>
#include <filesystem>
using namespace std;

bool saveDatabase(const Database& db, const string& filepath) {
    string tempPath = filepath + ".tmp";
    ofstream out(tempPath);
    if (!out) return false;
    for (const auto& entry : db.hashes) {
        for (int i = 0; i < entry.second.capacity; ++i) {
            const Node* current = entry.second.buckets[i];
            while (current != nullptr) {
                out << "HSET " << quoted(entry.first) << ' ' << quoted(current->key) << ' ' << quoted(current->value) << '\n';
                current = current->next;
            }
        }
    }

    for (const auto& entry : db.sets) {
        for (int i = 0; i < entry.second.set.capacity; ++i) {
            Node* current = entry.second.set.buckets[i];
            while (current != nullptr) {
                out << "SADD " << quoted(entry.first) << " " << quoted(current->key) << '\n';
                current = current->next;
            }
        }
    }

    for (const auto& entry : db.stacks) {
        for (int i = 0; i <= entry.second.topIndex; ++i) {
            out << "SPUSH " << quoted(entry.first) << ' ' << quoted(entry.second.data[i]) << '\n';
        }
    }

    for (const auto& entry : db.queues) {
        for (int i = 0; i < entry.second.count; ++i) {
            int index = (entry.second.head + i) % entry.second.capacity;
            out << "QPUSH " << quoted(entry.first) << ' ' << quoted(entry.second.data[index]) << '\n';
        }
    }
    out.close();
    if (out.fail()) {
        error_code ec;
        filesystem::remove(tempPath, ec);
        return false;
    }

    error_code ec;
    filesystem::rename(tempPath, filepath, ec);
    if (ec) {
        error_code cleanupError;
        filesystem::remove(tempPath, cleanupError);
        return false;
    }
    return true;
}

bool loadDatabase(Database& db, const string& filepath) {
    ifstream in(filepath);
    if (!in) {
        error_code ec;
        if (filesystem::exists(filepath, ec) || ec) return false;
        ofstream emptyFile(filepath);
        if (!emptyFile) return false;
        emptyFile.close();
        return !emptyFile.fail();
    }
    string line;
    while (getline(in, line)) {
        istringstream row(line);
        string operation;
        if (!(row >> operation)) continue;
        if (operation == "HSET") {
            string name, key, value;
            if (!(row >> quoted(name) >> quoted(key) >> quoted(value))) return false;
            row >> ws;
            if (!row.eof()) return false;
            HSET(db, name, key, value);
        } else if (operation == "SADD") {
            string name, element;
            if (!(row >> quoted(name) >> quoted(element))) return false;
            row >> ws;
            if (!row.eof()) return false;
            SADD(db, name, element);
        } else if (operation == "SPUSH") {
            string name, value;
            if (!(row >> quoted(name) >> quoted(value))) return false;
            row >> ws;
            if (!row.eof()) return false;
            SPUSH(db, name, value);
        } else if (operation == "QPUSH") {
            string name, value;
            if (!(row >> quoted(name) >> quoted(value))) return false;
            row >> ws;
            if (!row.eof()) return false;
            QPUSH(db, name, value);
        } else {
            return false;
        }
    }
    return in.eof() && !in.bad();
}
