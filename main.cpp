#include <iostream>
#include "hash_table.h"
#include "set.h"
#include "stack.h"
#include "queue.h"
#include "database.h"
#include <sstream>
#include <iomanip>
#include "storage.h"
using namespace std;

bool hasExtraArguments(istringstream& input) {
    input >> ws;
    return !input.eof();
}

void testRehash() {
    HashTable ht;
    init(ht);

    // Добавляем 30 уникальных ключей
    for (int i = 0; i < 30; i++) {
        string key = "key" + to_string(i);
        HSET(ht, key, "value");
    }

    cout << "Size: " << ht.size << endl;
    cout << "Capacity: " << ht.capacity << endl;

    // Проверяем сохранность всех 30 записей
    int errors = 0;

    for (int i = 0; i < 30; i++) {
        string key = "key" + to_string(i);
        string result;

        if (!HGET(ht, key, result) || result != "value") {
            cout << "Ошибка при поиске: " << key << endl;
            errors++;
        }
    }

    if (errors == 0) {
        cout << "Все 30 ключей найдены" << endl;
    } else {
        cout << "Количество ошибок: " << errors << endl;
    }

    freeMemory(ht);
}

int main(int argc, char* argv[]) {
    if (argc != 5) {
        cerr << "--file имя_файла --query операция_над_структурой" << endl;
        return 1;
    }

    string file_flag = argv[1];
    string query_flag = argv[3];

    if (file_flag != "--file" || query_flag != "--query") {
        cerr << "Cмотрите первую подсказку по вводу команд" << endl;
        return 1;
    }
    string filepath = argv[2];
    string query = argv[4];

    istringstream input(query);
    string command;
    string name;
    if (!(input >> command >> quoted(name))) {
        cerr << "В запросе нужно указать команду и имя контейнера" << endl;
        return 1;
    }

    Database db;

    if (!loadDatabase(db, filepath)) {
        cerr << "Не удалось загрузить базу из файла " << filepath << endl;
        freeMemory(db);
        return 1;
    }

    if (command == "HGET") {
        string key;
        if (!(input >> quoted(key)) || hasExtraArguments(input)) {
            cerr << "Формат: HGET имя_таблицы ключ" << endl;
            freeMemory(db);
            return 1;
        }
        string result;
        if (HGET(db, name, key, result)) {
            cout << result << endl;
        } else cout << "Ключ не найден" << endl;

    } else if (command == "HSET") {
        string key;
        string value;
        if (!(input >> quoted(key) >> quoted(value)) || hasExtraArguments(input)) {
            cerr << "Формат: HSET имя_таблицы ключ значение\n";
            freeMemory(db);
            return 1;
        }
        // Построчный формат хранения не допускает переводов строки внутри строк.
        if (name.find_first_of("\r\n") != string::npos ||
            key.find_first_of("\r\n") != string::npos ||
            value.find_first_of("\r\n") != string::npos) {
            cerr << "Перевод строки внутри имени, ключа или значения недопустим" << endl;
            freeMemory(db);
            return 1;
        }
        HSET(db, name, key, value);
        if (!saveDatabase(db, filepath)) {
            cerr << "Не удалось сохранить базу данных" << endl;
            freeMemory(db);
            return 1;
        }
    } else if (command == "SADD") {
        string element;
        if (!(input >> quoted(element)) || hasExtraArguments(input)) {
            cerr << "Формат: SADD имя_множества элемент\n" << endl;
            freeMemory(db);
            return 1;
        }
        if (name.find_first_of("\r\n") != string::npos ||
            element.find_first_of("\r\n") != string::npos) {
            cerr << "Перевод строки внутри имени или элемента недопустим" << endl;
            freeMemory(db);
            return 1;
        }
        SADD(db, name, element);
        if (!saveDatabase(db, filepath)) {
            cerr << "Не удалось сохранить базу данных" << endl;
            freeMemory(db);
            return 1;
        }
    } else if (command == "SISMEMBER") {
        string element;
        if (!(input >> quoted(element)) || hasExtraArguments(input)) {
            cerr << "Формат: SISMEMBER имя_множества элемент\n" << endl;
            freeMemory(db);
            return 1;
        }
        if (SISMEMBER(db, name, element)) {
            cout << "TRUE" << endl;
        } else {
            cout << "FALSE" << endl;
        }
    } else if (command == "SREM") {
        string element;
        if (!(input >> quoted(element)) || hasExtraArguments(input)) {
            cerr << "Формат: SREM имя_множества элемент\n" << endl;
            freeMemory(db);
            return 1;
        }
        if (SREM(db, name, element)) {
            cout << element << endl;
        }
        if (!saveDatabase(db, filepath)) {
            cerr << "Не удалось сохранить базу данных" << endl;
            freeMemory(db);
            return 1;
        }
    } else if (command == "SPUSH") {
        string value;
        if (!(input >> quoted(value)) || hasExtraArguments(input)) {
            cerr << "Формат: SPUSH имя_стека элемент\n" << endl;
            freeMemory(db);
            return 1;
        }
        if (name.find_first_of("\r\n") != string::npos ||
            value.find_first_of("\r\n") != string::npos) {
            cerr << "Перевод строки внутри имени или элемента недопустим" << endl;
            freeMemory(db);
            return 1;
        }
        SPUSH(db, name, value);
        if (!saveDatabase(db, filepath)) {
            cerr << "Не удалось сохранить базу данных" << endl;
            freeMemory(db);
            return 1;
        }
    } else if (command == "SPOP") {
        if (hasExtraArguments(input)) {
            cerr << "Формат: SPOP имя_стека" << endl;
            freeMemory(db);
            return 1;
        }
        string result;
        if (SPOP(db, name, result)) {
            cout << result << endl;
        } else cerr << "Стек пуст" << endl;
        if (!saveDatabase(db, filepath)) {
            cerr << "Не удалось сохранить базу данных" << endl;
            freeMemory(db);
            return 1;
        }
    } else if (command == "QPUSH") {
        string value;
        if (!(input >> quoted(value)) || hasExtraArguments(input)) {
            cerr << "Формат: QPUSH имя_очереди элемент\n" << endl;
            freeMemory(db);
            return 1;
        }
        if (name.find_first_of("\r\n") != string::npos ||
            value.find_first_of("\r\n") != string::npos) {
            cerr << "Перевод строки внутри имени или элемента недопустим" << endl;
            freeMemory(db);
            return 1;
        }
        QPUSH(db, name, value);
        if (!saveDatabase(db, filepath)) {
            cerr << "Не удалось сохранить базу данных" << endl;
            freeMemory(db);
            return 1;
        }
    } else if (command == "QPOP") {
        if (hasExtraArguments(input)) {
            cerr << "Формат: QPOP имя_очереди" << endl;
            freeMemory(db);
            return 1;
        }
        string result;
        if (QPOP(db, name, result)) {
            cout << result << endl;
        } else {
            cerr << "Очередь пуста" << endl;
        }
        if (!saveDatabase(db, filepath)) {
            cerr << "Не удалось сохранить базу данных" << endl;
            freeMemory(db);
            return 1;
        }
    } else if (command == "HDEL") {
        string key;
        if (!(input >> quoted(key)) || hasExtraArguments(input)) {
            cerr << "Формат: HDEL имя_таблицы ключ\n" << endl;
            freeMemory(db);
            return 1;
        }
        if (HDEL(db, name, key)) {
            cout << key << endl;
        } else cout << "Ключ не найден" << endl;
        if (!saveDatabase(db, filepath)) {
            cerr << "Не удалось сохранить базу данных" << endl;
            freeMemory(db);
            return 1;
        }
    }
    else {
        cerr << "Неизвестная операция" << endl;
        freeMemory(db);
        return 1;
    }

    freeMemory(db);
    return 0;
}
