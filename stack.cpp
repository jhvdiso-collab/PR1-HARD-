#include "stack.h"
#include <iostream>
#include <string>
#include <limits>
#include <stdexcept>

using namespace std;

void init(Stack& st, int initcapacity) {
    if (st.data != nullptr) return;
    st.capacity = initcapacity > 0 ? initcapacity : 1;
    string* newData = new string[st.capacity];
    st.topIndex = -1;
    st.data = newData;
}

bool isEmpty(const Stack& st) {
    return st.topIndex == -1;
}

void SPUSH(Stack& st, const string& value) {
    if (st.data == nullptr) init(st, 5);
    if (st.topIndex == st.capacity - 1) {
        if (st.capacity > numeric_limits<int>::max() / 2) {
            throw length_error("Слишком большой стек");
        }
        int newCapacity = st.capacity * 2;
        string* newData = new string[newCapacity];
        try {
            for (int i = 0; i <= st.topIndex; i++) {
                newData[i] = st.data[i];
            }
        } catch (...) {
            delete[] newData;
            throw;
        }
        delete[] st.data;
        st.data = newData;
        st.capacity = newCapacity;
    }
    st.data[st.topIndex + 1] = value;
    st.topIndex++;
}

bool SPOP(Stack& st, string& value) {
    if (isEmpty(st)) return false;
    value = st.data[st.topIndex];
    st.topIndex--;
    return true;
}

void freeMemory(Stack& st) {
    delete[] st.data;
    st.data = nullptr;
    st.capacity = 0;
    st.topIndex = -1;
}
