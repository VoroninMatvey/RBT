#pragma once
#include "Node.hpp"
#include <functional>
#include <iostream> //
#include <memory>

namespace details {

template <typename KeyT> class Tree_builder;

template <typename KeyT, typename Comparator = std::less<KeyT>> class Red_Black_Tree final {
    friend class Tree_builder<KeyT>;

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
        root_ptr->weight_ = 1;
        begin_ptr_ = root_ptr.get();
        root_ptr->parent_ = sentinel_ptr_;
        root_ptr->color_ = Color::BLACK;
        sentinel_->left_ = std::move(root_ptr);
        return sentinel_->left_.get();
    }

  public:
    pointer insert(const key_type& key) {
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

        ins->weight_ = 1;
        while (par != sentinel_ptr_) {
            ++par->weight_;
            par = par->parent_;
        }

        RBT_insert_fixup(ins);
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

  private:
    void left_rotate(pointer x_ptr) noexcept {
        pointer y_ptr = x_ptr->right_.get();
        if (!y_ptr)
            return;

        auto temp = std::move(x_ptr->right_);
        x_ptr->right_ = std::move(y_ptr->left_);
        if (x_ptr->right_)
            x_ptr->right_->parent_ = x_ptr;

        y_ptr->parent_ = x_ptr->parent_;
        if (x_ptr->parent_->left_.get() == x_ptr) {
            y_ptr->left_ = std::move(x_ptr->parent_->left_);
            y_ptr->parent_->left_ = std::move(temp);
        } else {
            y_ptr->left_ = std::move(x_ptr->parent_->right_);
            y_ptr->parent_->right_ = std::move(temp);
        }
        x_ptr->parent_ = y_ptr;

        rotate_weight_update(y_ptr, x_ptr);
    }

    void right_rotate(pointer y_ptr) noexcept {
        pointer x_ptr = y_ptr->left_.get();
        if (!x_ptr)
            return;

        auto temp = std::move(y_ptr->left_);
        y_ptr->left_ = std::move(x_ptr->right_);
        if (y_ptr->left_)
            y_ptr->left_->parent_ = y_ptr;

        x_ptr->parent_ = y_ptr->parent_;
        if (y_ptr->parent_->left_.get() == y_ptr) {
            x_ptr->right_ = std::move(y_ptr->parent_->left_);
            x_ptr->parent_->left_ = std::move(temp);
        } else {
            x_ptr->right_ = std::move(y_ptr->parent_->right_);
            x_ptr->parent_->right_ = std::move(temp);
        }
        y_ptr->parent_ = x_ptr;

        rotate_weight_update(x_ptr, y_ptr);
    }

    void rotate_weight_update(pointer node_up, pointer node_down) noexcept {
        node_up->weight_ = node_down->weight_;

        node_down->weight_ = (node_down->left_ ? node_down->left_->weight_ : 0) +
                             (node_down->right_ ? node_down->right_->weight_ : 0) + 1;
    }

    void RBT_insert_fixup(pointer z) noexcept {
        while (z->parent_->color_ == Color::RED) {
            pointer parent = z->parent_;
            pointer grand = parent->parent_;

            bool is_parent_left = (parent == grand->left_.get());
            pointer uncle = (is_parent_left ? grand->right_.get() : grand->left_.get());

            if (uncle && uncle->color_ == Color::RED) {
                uncle->color_ = Color::BLACK;
                grand->color_ = Color::RED;
                parent->color_ = Color::BLACK;
                z = grand;
                continue;
            }

            if (is_parent_left) {
                if (z == parent->right_.get()) {
                    LR(z);
                } else {
                    LL(z);
                }
            } else {
                if (z == parent->left_.get()) {
                    RL(z);
                } else {
                    RR(z);
                }
            }
            break;
        }

        sentinel_ptr_->left_->color_ = Color::BLACK;
    }

    void LL(pointer z) noexcept {
        right_rotate(z->parent_->parent_);
        z->parent_->right_->color_ = Color::RED;
        z->parent_->color_ = Color::BLACK;
    }

    void LR(pointer z) noexcept {
        left_rotate(z->parent_);
        LL(z->left_.get());
    }

    void RR(pointer z) noexcept {
        left_rotate(z->parent_->parent_);
        z->parent_->left_->color_ = Color::RED;
        z->parent_->color_ = Color::BLACK;
    }

    void RL(pointer z) noexcept {
        right_rotate(z->parent_);
        RR(z->right_.get());
    }

  public:
    template <bool Inclusive> std::size_t rank(const key_type& border) const noexcept {
        pointer current = sentinel_->left_.get();
        std::size_t amount = 0;

        while (current) {
            bool is_suitable;

            if constexpr (Inclusive) {
                is_suitable = !comp_(border, current->key_);
            } else {
                is_suitable = comp_(current->key_, border);
            }

            if (is_suitable) {
                amount += (current->left_ ? current->left_->weight_ : 0) + 1;
                current = current->right_.get();
            } else {
                current = current->left_.get();
            }
        }
        return amount;
    }

}; // <-- class Red_Black_Tree

} // namespace details
