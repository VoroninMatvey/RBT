#include "Node.hpp"

namespace details {

template <typename KeyT> struct iterator final {

    using iterator_category = std::bidirectional_iterator_tag;
    using difference_type = std::ptrdiff_t;
    using value_type = KeyT;
    using pointer = const value_type*;
    using const_pointer = pointer;
    using reference = const value_type&;
    using const_reference = reference;
    using node_type = Node<value_type>;
    using node_pointer = node_type*;

    iterator(node_pointer ptr) noexcept : ptr_(ptr) {}

    const value_type& operator*() const noexcept {
        return ptr_->key_;
    }
    const value_type* operator->() const noexcept {
        return std::addressof(ptr_->key_);
    }

    iterator& operator++() noexcept {
        ptr_ = ptr_->successor();
        return *this;
    }

    iterator operator++(int) noexcept {
        iterator old = *this;
        ++(*this);
        return old;
    }

    iterator& operator--() noexcept {
        ptr_ = ptr_->predecessor();
        return *this;
    }

    iterator operator--(int) noexcept {
        iterator old = *this;
        --(*this);
        return old;
    }

    bool operator==(const iterator rhs) const noexcept {
        return ptr_ == rhs.ptr_;
    }
    bool operator!=(const iterator rhs) const noexcept {
        return !(*this == rhs);
    }

  private:
    node_pointer ptr_;

}; // <-- struct iterator

} // namespace details