#include <iostream>
#include <map>
#include <array>
#include "allocator.h"
#include "list.h"

const int N = 10;

constexpr std::array<long long, N> factorial() {
    std::array<long long, N> arr = {};
    arr[0] = 1;
    for (size_t i = 1; i < N; ++i) {
        arr[i] = arr[i - 1] * i;
    }

    return arr;
}

template <typename K, typename V>
std::ostream& operator<<(std::ostream& os, const std::pair<K, V>& p) {
    os << p.first << ' ' << p.second;
    return os;
}

template <typename C>
void print(const C& container) {
    for (const auto& p : container) {
        std::cout << p << '\n';
    }
}

void MakeStdMapWithStdAllocator(const std::array<long long, N>& arr) {
    std::map<int, int> m;
    
    for (int i = 0; i < N; ++i) {
        m.emplace(i, arr[i]);
    }

    print(m);
}

void MakeStdMapWithCustomAllocator(const std::array<long long, N>& arr) {
    std::map<int, int, std::less<int>, custom::allocator<std::pair<const int, int>, 11>> m;

    for (int i = 0; i < N; ++i) {
        m.emplace(i, arr[i]);
    }

    print(m);
}

void MakeCustomListWithStdAllocator(const std::array<long long, N>& arr) {
    custom::list<int> l;

    for (int i = N - 1; i >= 0; --i) {
        l.add(arr[i]);
    }

    print(l);
}

void MakeCustomListWithCustomAllocator(const std::array<long long, N>& arr) {
    custom::list<int, custom::allocator<int, 11>> l;

    for (int i = N - 1; i >= 0; --i) {
        l.add(arr[i]);
    }

    print(l);
}


int main() {
    try {
        const std::array<long long, N> factorials = factorial();
        MakeStdMapWithStdAllocator(factorials);
        MakeStdMapWithCustomAllocator(factorials);
        MakeCustomListWithStdAllocator(factorials);
        MakeCustomListWithCustomAllocator(factorials);
    } catch (std::exception& exc) {
        std::cerr << exc.what() << std::endl;
    }

    return 0;
}