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
            node_alloc.destroy(tmp);
            node_alloc.deallocate(tmp, 1);
        }
    };

    iterator begin() const {
        return {head};
    }

    iterator end() const {
        return {nullptr};
    }

    void add(const T& val) {
        node* n = node_alloc.allocate(1);
        node_alloc.construct(n, std::move(val), head);
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
    using node_allocator_traits = typename std::allocator_traits<allocator>::template rebind_traits<node>;
    using node_allocator_type = typename node_allocator_traits::allocator_type;
    using node_pointer = typename node_allocator_traits::pointer;

    node*       head  = nullptr;
    std::size_t size_ = 0;

    node_allocator_type node_alloc;
};

}
