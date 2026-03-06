#include "Node.hpp"
#include <functional>
#include <memory>

namespace details {

template <typename KeyT, typename Comparator = std::less<KeyT>> class Red_Black_Tree {

  public:
    using key_type = KeyT;
    using node_type = Node<key_type>;
    using pointer = node_type*;
    using const_pointer = const node_type*;
    using unique_ptr = std::unique_ptr<node_type>;
    using const_unique_ptr = const std::unique_ptr<node_type>;

  private:
    unique_ptr sentinel_ = std::make_unique<node_type>(key_type{});
    pointer sentinel_ptr_ = sentinel_.get();
    pointer begin_ptr_ = sentinel_.get();
    [[no_unique_address]] Comparator comp_;

  public:
    explicit Red_Black_Tree(const Comparator& comp = Comparator()) : comp_{comp} {
        sentinel_->color_ = Color::BLACK;
    }

  private:
    pointer create_root_node(const key_type& key) {
        auto root_ptr = std::make_unique<node_type>(key);
        root_ptr->weight = 1;
        begin_ptr_ = root_ptr.get();
        root_ptr->parent_ = sentinel_ptr_;
        sentinel_->left_ = std::move(root_ptr);
        return sentinel_->left_.get();
    }

    pointer Tree_Insert(const key_type& key) {
        pointer par = nullptr;
        pointer ins = sentinel_ptr_->left_.get();

        if (!ins) {
            ins = create_root_node(key);
            return ins;
        }

        while (ins) {
            par = ins;
            if (comp_(key, ins->key_)) {
                ins = ins->left_.get();
            } else if (comp_(ins->key_, key)) {
                ins = ins->right_.get();
            } else {
                return par;
            }
        }

        if (comp_(key, par->key_)) {
            par->left_ = std::make_unique<node_type>(key);
            ins = par->left_.get();
            ins->parent_ = par;

            if (par == begin_ptr_)
                begin_ptr_ = ins;
        } else {
            par->right_ = std::make_unique<node_type>(key);
            ins = par->right_.get();
            ins->parent_ = par;
        }

        ins->weight = 1;
        while (par != sentinel_ptr_) {
            ++par->weight;
            par = par->parent_;
        }

        return ins;
    }

    pointer find(const key_type& key) const {
        pointer current = sentinel_ptr_->left_.get();

        while (current) {
            if (comp_(key, current->key_)) {
                current = current->left_.get();
            } else if (comp_(current->key_, key)) {
                current = current->right_.get();
            } else {
                return current;
            }
        }

        return sentinel_ptr_;
    }

}; // <-- class Red_Black_Tree

} // namespace details