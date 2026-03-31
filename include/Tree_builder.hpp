#pragma once
#include "Node.hpp"
#include "RBT.hpp"
#include <filesystem>
#include <format>
#include <fstream>
#include <sstream>
#include <string>

namespace details {

namespace fs = std::filesystem;

enum class NodeSide { left, right };

template <typename KeyT> class Tree_builder final {

  public:
    using key_type = KeyT;
    using node_type = Node<key_type>;
    using pointer = node_type*;
    using const_pointer = const node_type*;

  private:
    const Red_Black_Tree<KeyT>& RBT_;
    const std::string RBT_dir_;

  public:
    Tree_builder(const Red_Black_Tree<key_type>& RBT, const std::string& tree_dir)
        : RBT_{RBT}, RBT_dir_{tree_dir} {}

    void create_directory() const {
        fs::path dir = fs::path(RBT_dir_) / "data";
        fs::create_directories(dir);
    }

    void dump_tree(std::size_t num, const std::string& node_shape = "none",
                   const std::string& fontname = "Arial", const std::string& edge_color = "#555555",
                   const double penwidth = 1.5) const {

        create_directory();
        std::ofstream file =
            fs::path(RBT_dir_) / ("data/Tree_builder" + std::to_string(num) + ".dot");

        // clang-format off
        file << std::format(R"DOT(
        digraph RBT {{

            node [shape={}, fontname="{}"];
            edge [color="{}", penwidth={}];


            )DOT", node_shape, fontname, edge_color, penwidth);
        // clang-format on

        const_pointer root = RBT_.sentinel_->left_.get();
        // clang-format off
        if (!root) {
            file << R"DOT(
                "ROOT_NIL" [
                    shape = box, 
                    width = 1.0, 
                    height = 0.5, 
                    style = filled, 
                    fillcolor = black, 
                    fontcolor = white, 
                    label = "EMPTY TREE\n(NIL)", 
                    fontsize = 14
                ];
            }
            )DOT";
            return;
        }
        // clang-format on

        recursive_traversal_dump(file, root, root->key_, NodeSide::left);
        file << "}";
    }

    void recursive_traversal_dump(std::ofstream& file, const_pointer node, const key_type& par_key,
                                  NodeSide side) const {
        if (!node) {
            dump_nill_node(file, par_key, side);
            return;
        }

        const key_type& key = node->key_;
        dump_real_node(file, node);
        dump_edge(file, node);
        recursive_traversal_dump(file, node->left_.get(), key, NodeSide::left);
        recursive_traversal_dump(file, node->right_.get(), key, NodeSide::right);
    }

    void dump_edge(std::ofstream& file, const_pointer node) const {
        std::string left_edge_end = (node->left_ ? to_str_generate(node->left_->key_)
                                                 : to_str_generate(node->key_) + "_nilL");

        std::string right_edge_end = (node->right_ ? to_str_generate(node->right_->key_)
                                                   : to_str_generate(node->key_) + "_nilR");

        // clang-format off
        file << std::format(R"(
            "{0}" -> "{1}";
            "{0}" -> "{2}";   
        )", to_str_generate(node->key_), left_edge_end, right_edge_end);
        // clang-format on
        file << std::endl;
    }

    void dump_real_node(std::ofstream& file, const_pointer node) const {
        std::string bg_color = (node->color_ == Color::BLACK) ? "#333333" : "#ff4d4d";
        std::string text_color = (node->color_ == Color::BLACK) ? "Black" : "Red";

        // clang-format off
        file << std::format(R"DOT(
            "{0}" [label=<
                <TABLE BORDER="0" CELLBORDER="1" CELLSPACING="0" CELLPADDING="4">
                    <TR><TD>Key: {0}</TD></TR>
                    <TR><TD>Weight: {1}</TD></TR>
                    <TR><TD BGCOLOR="{2}"><FONT COLOR="white">{3}</FONT></TD></TR>
                </TABLE>
            >];
        )DOT", to_str_generate(node->key_), node->weight_, bg_color, text_color);
        // clang-format on
        file << std::endl;
    }

    void dump_nill_node(std::ofstream& file, const key_type& par_key, NodeSide side) const {
        std::string suffix = (side == NodeSide::left ? "L" : "R");
        std::string nil_id = to_str_generate(par_key) + "_nil" + suffix;

        // clang-format off
        file << std::format(R"DOT(
            "{}" [ 
                shape = box,
                width = 0.3,
                height = 0.2,
                style = filled,
                fillcolor = black,
                fontcolor = white, 
                label = "NIL",
                fontsize = 8
            ];
        )DOT", nil_id);
        // clang-format on
    }

    // operator << must be overloaded for type KeyT
    std::string to_str_generate(const key_type& key) const {
        std::ostringstream oss;
        oss << key;
        return oss.str();
    }

}; // <-- class Tree_builder

} // namespace details