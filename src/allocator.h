#pragma once

#include <memory>
#include <bitset>
#include <iostream>

namespace custom {

template <typename T, std::size_t TotalSize>
class allocator : public std::allocator<T> {
public:
    using value_type = T;
    using pointer = T*;
    using const_pointer = const T*;
    using reference = T&;
    using const_reference = const T&;

private:
    pointer area = nullptr;
    std::size_t allocated_size = 0;
    std::bitset<TotalSize> memory_map;

    void init() {
        area = static_cast<pointer>(std::malloc(TotalSize * sizeof(value_type)));
        if (area == nullptr) {
            throw std::bad_alloc();
        }
        memory_map.reset();
    }

public:
    allocator() {
        init();
    }

    ~allocator() {
        if (area != nullptr) {
            std::free(area);
        }
    }

    template <typename U>
    struct rebind {
        using other = allocator<U, TotalSize>;
    };

    pointer allocate(std::size_t n) {
        if (TotalSize < n + allocated_size) {
            throw std::bad_alloc();
        }

        std::size_t lo = 0, hi = 0;
        for (std::size_t i = 0; i < TotalSize; ++i) {
            if (memory_map.test(i)) {
                continue;
            }

            std::size_t j = i + 1;
            for (; j < TotalSize && !memory_map.test(j) && j - i + 1 <= n; ++j) {}
            if (j - i == n) {
                lo = i;
                hi = j;
                break;
            }
        }
        if (lo == 0 && hi == 0) {
            throw std::bad_alloc();
        }

        for (std::size_t i = lo; i < hi; ++i) {
            memory_map.flip(i);
        }
        allocated_size += n;
        return area + lo;
    }

    void deallocate(pointer p, std::size_t n) {
        std::size_t from = p - area;

        for (std::size_t i = 0; i < n; ++i, ++p) {
            memory_map.set(from + i, false);
        }

        allocated_size -= n;
    }
};

}