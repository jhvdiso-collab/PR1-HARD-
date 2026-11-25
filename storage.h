#pragma once
#include "database.h"

bool saveDatabase(const Database& db, const std::string& filepath);
bool loadDatabase(Database& db, const std::string& filepath);
