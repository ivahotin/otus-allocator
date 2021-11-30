#pragma once

#include <memory>

namespace custom {

template <typename T, typename allocator = std::allocator<T>>
class list {
public:
    struct node {
        node* next = nullptr;
        T value;

        node() {}
        explicit node(const T& val, node* next) : next(next), value(std::move(val)) {}
    };

    class iterator {
        friend class list<T, allocator>;

        iterator(node* n_) : node_(n_) {}

        public:
        iterator& operator++()
        {
            node_ = node_ -> next;
            return *this;
        }

        const T& operator*() const
        {
            return node_->value;
        }

        bool operator!=(const iterator& other)
        {
            return node_ != other.node_;
        }

    private:
        node* node_;
    };

    using node_allocator = typename allocator::template rebind<node>::other;

    list() = default;
    ~list() {
        while (head != nullptr) {
            auto tmp = head;
            head = head -> next;
            node_traits::destroy(node_alloc, tmp);
            node_traits::deallocate(node_alloc, tmp, 1);
        }
    };

    iterator begin() const {
        return {head};
    }

    iterator end() const {
        return {nullptr};
    }

    void add(const T& val) {
        node_pointer n = node_traits::allocate(node_alloc, 1);
        node_traits::construct(node_alloc, n, std::move(val), head);
        head = n;
        size_++;
    }

    std::size_t size() const {
        return size_;
    }

    bool empty() const {
        return size() == 0;
    }

private:
    using traits = std::allocator_traits<allocator>;
    using pointer = typename traits::pointer;
    using node_traits = typename std::allocator_traits<allocator>::template rebind_traits<node>;
    using node_allocator_type = typename node_traits::allocator_type;
    using node_pointer = typename node_traits::pointer;

    node*       head  = nullptr;
    std::size_t size_ = 0;

    allocator alloc;
    node_allocator_type node_alloc;
};

}
