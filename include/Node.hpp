#pragma once
#include <memory>

namespace details {

enum class Color { RED, BLACK };

template <typename KeyT> struct Node final {

    using pointer = Node*;
    using unique_ptr = std::unique_ptr<Node>;

    const KeyT key_;
    pointer parent_ = nullptr;
    unique_ptr left_ = nullptr, right_ = nullptr;
    std::size_t weight_ = 0;
    Color color_ = Color::RED;

    explicit Node(const KeyT& key) : key_{key} {}

    pointer minimum(pointer val) const noexcept {
        if (!val)
            return val;

        while (val->left_) {
            val = val->left_.get();
        }
        return val;
    }

    pointer maximum(pointer val) const noexcept {
        if (!val)
            return val;

        while (val->right_) {
            val = val->right_.get();
        }
        return val;
    }

    // next node in tree traversal
    // return end_node, if this->key == max
    pointer successor() const noexcept {
        if (!parent_)
            return this; // if *this == end_node
        pointer ptr1 = this;
        if (ptr1->right_) {
            return minimum(ptr1->right_.get());
        }

        pointer ptr2 = ptr1->parent_;
        while (ptr2->right_.get() == ptr1) {
            ptr1 = ptr2;
            ptr2 = ptr2->parent_;
        }
        return ptr2;
    }

    // previous node in tree traversal
    // return end_node, if this->key == min
    pointer predecessor() const noexcept {
        pointer ptr1 = this;
        if (ptr1->left_) {
            return maximum(ptr1->left_.get());
        }

        if (!parent_)
            return this; // if tree is empty
        pointer ptr2 = ptr1->parent_;
        while (ptr2->parent_ && ptr2->left_.get() == ptr1) {
            ptr1 = ptr2;
            ptr2 = ptr2->parent_;
        }
        return ptr2;
    }
}; // <-- struct Node

} // namespace details