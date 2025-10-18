#include <memory>
#include "Node.hpp"

namespace details {

template <typename KeyT>
class Red_Black_Tree {

    using key_type = KeyT;
    using node_type = Node<key_type>;
    using pointer = node_type*;
    using const_pointer = const pointer;
    using unique_ptr = std::unique_ptr<node_type>;
    using const_unique_ptr = const unique_ptr;

    pointer root_ptr = nullptr;
    unique_ptr end_unique_ptr;
    pointer begin_ptr_;

    Red_Black_Tree() : end_unique_ptr{std::make_unique<node_type>(0)} {}

}; // <-- class Red_Black_Tree 

} // namespace details