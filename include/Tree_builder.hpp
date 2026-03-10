#include "Node.hpp"
#include "RBT.hpp"
#include <filesystem>
#include <fstream>
#include <string>

namespace details {

namespace fs = std::filesystem;

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

    void dump_tree() const {
        std::ofstream
    }

    void dump_node(std::ofstream& file, const_pointer node) const {
        std::string bg_color = (node->color_ == Color::BLACK) ? "\"#333333\"" : "\"#ff4d4d\"";
        std::string text_col = (node->color_ == Color::BLACK) ? "Black" : "Red";

        file << "\"" << node->key_
             << "\" [label=<\n<TABLE BORDER=\"0\" CELLBORDER=\"1\" CELLSPACING=\"0\" "
                "CELLPADDING=\"4\">\n<TR><TD>Key: "
             << node->key_ << "</TD></TR>\n<TR><TD>Weight: " << node->weight_
             << "</TD></TR>\n<TR><TD BGCOLOR=" << bg_color << "><FONT COLOR=\"white\">" << text_col
             << "</FONT></TD></TR>\n</TABLE>\n>];\n\n";
    }

}; // <-- class Tree_builder

} // namespace details