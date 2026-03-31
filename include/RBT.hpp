#pragma once
#include "Node.hpp"
#include <functional>
#include <memory>
#include <utility>

namespace details {

enum class BoundType { Inclusive, Exclusive };

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

    template <typename Iter>
    Red_Black_Tree(Iter begin, Iter end, const Comparator& comp = Comparator())
        : Red_Black_Tree(comp) {
        while (begin != end) {
            insert(*begin);
            ++begin;
        }
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

        auto y_node = std::exchange(x_ptr->right_, std::move(y_ptr->left_));
        if (x_ptr->right_)
            x_ptr->right_->parent_ = x_ptr;

        y_ptr->parent_ = x_ptr->parent_;
        if (x_ptr->parent_->left_.get() == x_ptr) {
            y_ptr->left_ = std::exchange(x_ptr->parent_->left_, std::move(y_node));
        } else {
            y_ptr->left_ = std::exchange(x_ptr->parent_->right_, std::move(y_node));
        }
        x_ptr->parent_ = y_ptr;

        rotate_weight_update(y_ptr, x_ptr);
    }

    void right_rotate(pointer y_ptr) noexcept {
        pointer x_ptr = y_ptr->left_.get();
        if (!x_ptr)
            return;

        auto x_node = std::exchange(y_ptr->left_, std::move(x_ptr->right_));
        if (y_ptr->left_)
            y_ptr->left_->parent_ = y_ptr;

        x_ptr->parent_ = y_ptr->parent_;
        if (y_ptr->parent_->left_.get() == y_ptr) {
            x_ptr->right_ = std::exchange(y_ptr->parent_->left_, std::move(x_node));
        } else {
            x_ptr->right_ = std::exchange(y_ptr->parent_->right_, std::move(x_node));
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

    template <BoundType Inclusivity> std::size_t rank(const key_type& bound) const noexcept {
        pointer current = sentinel_->left_.get();
        std::size_t amount = 0;

        while (current) {
            bool is_suitable;

            if constexpr (Inclusivity == BoundType::Inclusive) {
                is_suitable = !comp_(bound, current->key_);
            } else {
                is_suitable = comp_(current->key_, bound);
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

  public:
    std::size_t count_inclusive_range(const key_type& left_bnd,
                                      const key_type& right_bnd) const noexcept {
        if (!comp_(left_bnd, right_bnd))
            return 0;
        return (rank<BoundType::Inclusive>(right_bnd) - rank<BoundType::Exclusive>(left_bnd));
    }

    std::size_t count_exclusive_range(const key_type& left_bnd,
                                      const key_type& right_bnd) const noexcept {
        if (!comp_(left_bnd, right_bnd))
            return 0;
        return (rank<BoundType::Exclusive>(right_bnd) - rank<BoundType::Inclusive>(left_bnd));
    }

    std::size_t count_open_closed_range(const key_type& left_bnd,
                                        const key_type& right_bnd) const noexcept {
        if (!comp_(left_bnd, right_bnd))
            return 0;
        return (rank<BoundType::Inclusive>(right_bnd) - rank<BoundType::Inclusive>(left_bnd));
    }

    std::size_t count_closed_open_range(const key_type& left_bnd,
                                        const key_type& right_bnd) const noexcept {
        if (!comp_(left_bnd, right_bnd))
            return 0;
        return (rank<BoundType::Exclusive>(right_bnd) - rank<BoundType::Exclusive>(left_bnd));
    }

}; // <-- class Red_Black_Tree

template <typename Iter, typename Comp>
Red_Black_Tree(Iter begin, Iter end, const Comp& comp)
    -> Red_Black_Tree<typename std::iterator_traits<Iter>::value_type, Comp>;

template <typename Iter>
Red_Black_Tree(Iter begin, Iter end)
    -> Red_Black_Tree<typename std::iterator_traits<Iter>::value_type>;

} // namespace details
