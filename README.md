# PR1-HARD-

Учебная консольная NoSQL-СУБД на C++17 с собственной реализацией хеш-таблицы, множества, стека и очереди.

## Сборка

```bash
g++ -std=c++17 -Wall -Wextra -g main.cpp hash_table.cpp set.cpp stack.cpp queue.cpp database.cpp storage.cpp -o dbms
```

## Использование

```bash
./dbms --file data.db --query 'HSET users name "Ivan Petrov"'
./dbms --file data.db --query 'HGET users name'
```

Поддерживаемые операции: HSET, HGET, HDEL, SADD, SISMEMBER, SREM, SPUSH, SPOP, QPUSH, QPOP.
