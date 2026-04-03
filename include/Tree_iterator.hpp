#pragma once
#include "Node.hpp"

namespace details {

template <typename KeyT, typename Comparator> class Red_Black_Tree;

template <typename KeyT> struct Tree_iterator final {

    template <typename Key, typename Comparator> friend class Red_Black_Tree;

    using iterator_category = std::bidirectional_iterator_tag;
    using difference_type = std::ptrdiff_t;
    using value_type = KeyT;
    using pointer = const value_type *;
    using const_pointer = pointer;
    using reference = const value_type &;
    using const_reference = reference;
    using node_type = Node<value_type>;
    using node_pointer = node_type *;

    explicit Tree_iterator(node_pointer ptr) noexcept : ptr_(ptr) {}
    Tree_iterator() noexcept : ptr_(nullptr) {}

    const_reference operator*() const noexcept { return ptr_->key_; }
    const_pointer operator->() const noexcept { return std::addressof(ptr_->key_); }

    Tree_iterator &operator++() noexcept {
        ptr_ = ptr_->successor();
        return *this;
    }

    Tree_iterator operator++(int) noexcept {
        Tree_iterator old = *this;
        ++(*this);
        return old;
    }

    Tree_iterator &operator--() noexcept {
        ptr_ = ptr_->predecessor();
        return *this;
    }

    Tree_iterator operator--(int) noexcept {
        Tree_iterator old = *this;
        --(*this);
        return old;
    }

    friend bool operator==(const Tree_iterator &lhs, const Tree_iterator &rhs) noexcept {
        return lhs.ptr_ == rhs.ptr_;
    }

    friend bool operator!=(const Tree_iterator &lhs, const Tree_iterator &rhs) noexcept {
        return !(lhs == rhs);
    }

  private:
    node_pointer ptr_;

}; // <-- struct Tree_iterator

} // namespace details