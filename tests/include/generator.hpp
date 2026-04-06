#include <concepts>
#include <filesystem>
#include <fstream>
#include <iterator>
#include <random>
#include <set>
#include <stdexcept>
#include <string>

namespace Tests {

namespace fs = std::filesystem;
using size_t = std::size_t;

template <std::integral T> class generator final {

    using generator_type = std::mt19937;
    using value_type = T;

    generator_type gen_{std::random_device{}()};
    const std::string RBT_dir_;
    const size_t tests_num_;
    const size_t key_num_;
    const value_type min_;
    const value_type max_;

  public:
    generator(const std::string &str, const size_t tests_num, const size_t key_num,
              value_type min = -50000, value_type max = 50000)
        : RBT_dir_(str), tests_num_(tests_num), key_num_(key_num), min_(min), max_(max) {
        if (min > max) {
            throw std::invalid_argument("min value is greater than max value");
        }

        if (key_num_ > max_ - min_ + 1)
            throw std::invalid_argument(
                "There are not enough values ​​in the range for this number of keys");
    }

    void generate() {
        create_directories();
        fs::path data_tests_dir = fs::path(RBT_dir_) / "data" / "tests";

        for (size_t i = 1; i <= tests_num_; ++i) {
            std::ofstream test_i{data_tests_dir / "tests" / ("test" + std::to_string(i) + ".txt")};
            std::ofstream answ_i{data_tests_dir / "answers" /
                                 ("answer" + std::to_string(i) + ".txt")};

            single_test_generation(test_i, answ_i);
        }
    }

  private:
    void single_test_generation(std::ofstream &test_file, std::ofstream &answ_file) {
        std::set<value_type> set;

        while (set.size() < key_num_) {
            char request = ((random(0, 2) == 0) ? 'q' : 'k');

            if (request == 'q') {
                value_type left_bnd = random(min_, max_);
                value_type right_bnd = random(left_bnd, max_);
                answ_file << range_query(set, left_bnd, right_bnd) << " ";
                test_file << "q " << left_bnd << " " << right_bnd << " ";
            } else {
                value_type key = unique_key(set);
                set.insert(key);
                test_file << "k " << key << " ";
            }
        }
    }

    void create_directories() {
        fs::path data_tests_dir = fs::path(RBT_dir_) / "data" / "tests";

        fs::remove_all(fs::path(data_tests_dir));
        fs::create_directories(data_tests_dir / "answers");
        fs::create_directories(data_tests_dir / "tests");
    }

    value_type random(const value_type lower_lim, const value_type upper_lim) {
        std::uniform_int_distribution<value_type> dist(lower_lim, upper_lim);
        return dist(gen_);
    }

    value_type unique_key(const std::set<value_type> &set) {
        if (set.empty())
            return random(min_, max_);

        value_type key;
        do {
            key = random(min_, max_);
        } while (set.contains(key));

        return key;
    }

    size_t range_query(const std::set<value_type> &tree, value_type fst, value_type snd) {
        auto start = tree.lower_bound(fst);
        auto fin = tree.upper_bound(snd);
        return std::distance(start, fin);
    }

}; // <-- class generator

} // namespace Tests