#ifndef VECTOR_H
#define VECTOR_H

#include <iostream>


template <typename T>
class VectorList {
private:
    T* data;        // Pointer to dynamically allocated array
    int size;      // Current number of elements
    int capacity;  // Allocated memory size

    void resize(int newCapacity); // Helper function to resize the array

public:
    // Constructor & Destructor
    VectorList(int capacity = 10);
    ~VectorList();

    // Basic Operations
    void push_back(const T& value);
    void pop_back();
    void insert(int index, const T& value);
    void erase(int index);
    void clear();

    // Access Functions
    T at(int index) const;
    T operator[](int index) const;
    T front() const;
    T back() const;

    // Size & Capacity Management
    int getSize() const;
    int getCapacity() const;
    bool empty() const;
    void reserve(int newCapacity);
    void shrink_to_fit();

    // Utility Functions
    void print() const;
};
// Constructor
template <typename T>
VectorList<T>::VectorList(int capacity) : size(0), capacity(capacity) {
    data = new T[capacity];
}

// Destructor
template <typename T>
VectorList<T>::~VectorList() {
    delete[] data;
}

// Resize the internal array
template <typename T>
void VectorList<T>::resize(int newCapacity) {
    if (newCapacity <= capacity) return;
    T* newData = new T[newCapacity];
    for (int i = 0; i < size; i++) {
        newData[i] = data[i];
    }
    delete[] data;
    data = newData;
    capacity = newCapacity;
}

// Add element to the end
template <typename T>
void VectorList<T>::push_back(const T& value) {
    if (size == capacity) {
        resize(capacity * 2);
    }
    data[size++] = value;
}

// Remove last element
template <typename T>
void VectorList<T>::pop_back() {
    if (size > 0) {
        size--;
    }
}

// Insert element at index
template <typename T>
void VectorList<T>::insert(int index, const T& value) {
    if (index < 0 || index > size) {
        std::cout << "Invalid index for insert operation" << std::endl;
        return;
    }
    if (size == capacity) {
        resize(capacity * 2);
    }
    for (int i = size; i > index; i--) {
        data[i] = data[i - 1];
    }
    data[index] = value;
    size++;
}

// Remove element at index
template <typename T>
void VectorList<T>::erase(int index) {
    if (index < 0 || index >= size) {
        std::cout << "Invalid index for erase operation." << std::endl;
        return;
    }
    for (int i = index; i < size - 1; i++) {
        data[i] = data[i + 1];
    }
    size--;
}

// Clear all elements
template <typename T>
void VectorList<T>::clear() {
    size = 0;
}

// Get element at index
template <typename T>
T VectorList<T>::at(int index) const {
    if (index < 0 || index >= size) {
        std::cout << "Invalid index for at() operation." << std::endl;
        return T();
    }
    return data[index];
}

// Overloaded [] operator to can access elements by it 
template <typename T>
T VectorList<T>::operator[](int index) const {
    if (index < 0 || index >= size) {
        std::cout << "Invalid index for [] operator." << std::endl;
        return T();
    }
    return data[index];
}

// Get first element
template <typename T>
T VectorList<T>::front() const {
    if (size == 0) {
        std::cout << "VectorList is empty." << std::endl;
        return T();
    }
    return data[0];
}

// Get last element
template <typename T>
T VectorList<T>::back() const {
    if (size == 0) {
        std::cout << "VectorList is empty." << std::endl;
        return T();
    }
    return data[size - 1];
}

// Get size of list
template <typename T>
int VectorList<T>::getSize() const {
    return size;
}

// Get capacity of list
template <typename T>
int VectorList<T>::getCapacity() const {
    return capacity;
}

// Check if empty
template <typename T>
bool VectorList<T>::empty() const {
    return size == 0;
}

// Reserve memory
template <typename T>
void VectorList<T>::reserve(int newCapacity) {
    if (newCapacity > capacity) {
        resize(newCapacity);
    }
}

// Shrink capacity to match size
template <typename T>
void VectorList<T>::shrink_to_fit() {
    if (size < capacity) {
        resize(size);
    }
}

// Print elements
template <typename T>
void VectorList<T>::print() const {
    for (int i = 0; i < size; i++) {
        std::cout << data[i] << " ";
    }
    std::cout << std::endl;
}

#endif // VECTORLIST_H

