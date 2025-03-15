#include "vector.hpp"

int main() {
    VectorList<int> vec;

    vec.push_back(10);
    vec.push_back(20);
    vec.push_back(30);
    vec.print();

    vec.insert(1, 15);
    vec.print();

    vec.erase(2);
    vec.print();

    std::cout << "Element at index 1: " << vec.at(1) << std::endl;
    std::cout << "Front: " << vec.front() << std::endl;
    std::cout << "Back: " << vec.back() << std::endl;

    vec.pop_back();
    vec.print();

    vec.clear();
    std::cout << "After clearing: ";
    vec.print();

    return 0;
}
