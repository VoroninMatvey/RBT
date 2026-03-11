#pragma once
#include "Node.hpp"
#include "RBT.hpp"
#include <filesystem>
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
    const_pointer root_;
    const std::string RBT_dir_;

  public:
    Tree_builder(const Red_Black_Tree<key_type>& RBT, const std::string& tree_dir)
        : root_{RBT.sentinel_->left_.get()}, RBT_dir_{tree_dir} {}

    void create_directory() const {
        fs::path dir = fs::path(RBT_dir_) / "data";
        fs::create_directories(dir);
    }

    void dump_tree(const std::string& node_shape = "none", const std::string& fontname = "Arial",
                   const std::string& edge_color = "#555555", const double penwidth = 1.5) const {

        create_directory();
        std::ofstream file = fs::path(RBT_dir_) / "data/Tree_builder.dot";

        file << "digraph RBT {\n\n\tnode [shape=" << node_shape << ", fontname=\"" << fontname
             << "\"];\n\tedge [color=\"" << edge_color << "\", penwidth=" << penwidth << "];\n\n";

        if (!root_) {
            file << "\t\"ROOT_NIL\" [shape = box, width = 1.0, height = 0.5, style = filled, "
                 << "fillcolor = black, fontcolor = white, label = \"EMPTY TREE\\n(NIL)\", "
                    "fontsize = 14];\n\n}";
            return;
        }

        recursive_traversal_dump(file, root_, root_->key_, NodeSide::left);
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
        std::string left_edge_end = (node->left_ ? to_str_generate(node->left_->key_) + "\";\n"
                                                 : to_str_generate(node->key_) + "_nilL\";\n");

        std::string right_edge_end = (node->right_ ? to_str_generate(node->right_->key_) + "\";\n"
                                                   : to_str_generate(node->key_) + "_nilR\";\n");

        file << "\"" << node->key_ << "\" -> \"" << left_edge_end;
        file << "\"" << node->key_ << "\" -> \"" << right_edge_end;
        file << "\n";
    }

    void dump_real_node(std::ofstream& file, const_pointer node) const {
        std::string bg_color = (node->color_ == Color::BLACK) ? "\"#333333\"" : "\"#ff4d4d\"";
        std::string text_col = (node->color_ == Color::BLACK) ? "Black" : "Red";

        file << "\"" << node->key_
             << "\" [label=<\n\t<TABLE BORDER=\"0\" CELLBORDER=\"1\" CELLSPACING=\"0\" "
                "CELLPADDING=\"4\">\n\t<TR><TD>Key: "
             << node->key_ << "</TD></TR>\n\t<TR><TD>Weight: " << node->weight_
             << "</TD></TR>\n\t<TR><TD BGCOLOR=" << bg_color << "><FONT COLOR=\"white\">"
             << text_col << "</FONT></TD></TR>\n\t</TABLE>\n>];\n\n";
    }

    void dump_nill_node(std::ofstream& file, const key_type& par_key, NodeSide side) const {
        std::string beg = "\"" + to_str_generate(par_key) + "_nil";
        beg += (side == NodeSide::left ? "L" : "R");
        file << beg;

        file << "\" [shape = box, width = 0.3, height = 0.2, style = filled, fillcolor = black, "
                "fontcolor = white, label = \"NIL\", fontsize = 8];\n\n";
    }

    std::string to_str_generate(const key_type& key) const {
        std::ostringstream oss;
        oss << key;
        return oss.str();
    }

}; // <-- class Tree_builder

} // namespace details