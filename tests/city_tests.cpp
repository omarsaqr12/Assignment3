#include "BinaryTree.h"
#include "Coordinates.h"
#include "binaryTree.cpp"

#include <cmath>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <string>

namespace {
void require(bool condition, const std::string& label) {
    if (!condition) throw std::runtime_error(label);
}
void near(double actual, double expected, double tolerance, const std::string& label) {
    require(std::abs(actual - expected) <= tolerance, label);
}
bool badCoordinate(const std::string& input, bool lat) {
    try { geo::parseCoordinate(input, lat); }
    catch (const std::invalid_argument&) { return true; }
    return false;
}
}

int main() {
    try {
        near(geo::parseCoordinate("31d37lN", true), 31.0 + 37.0 / 60.0, 1e-12,
             "north latitude retains minutes");
        near(geo::parseCoordinate("12d46lS", true), -12.0 - 46.0 / 60.0, 1e-12,
             "south latitude sign");
        near(geo::parseCoordinate("82d54lW", false), -82.9, 1e-12,
             "west longitude sign");
        near(geo::parseCoordinate("1d2l30sE", false), 1.0 + 2.0 / 60.0 + 30.0 / 3600.0,
             1e-12, "seconds precision");
        require(badCoordinate("1d70lN", true), "minutes bound");
        require(badCoordinate("91d00lN", true), "latitude bound");
        require(badCoordinate("1d02lN", false), "longitude hemisphere");
        near(geo::distanceKm(0, 0, 0, 0), 0, 1e-12, "identical coordinates");
        near(geo::distanceKm(0, 0, 0, 1), 111.197, 0.01, "one longitude degree");
        require(std::isfinite(geo::distanceKm(0, 0, 0, 180)), "antipodal stability");

        binaryTree<int, std::string> tree;
        std::ostringstream sink;
        auto* previous = std::cout.rdbuf(sink.rdbuf());
        tree.preorder();
        tree.levelorder();
        std::cout.rdbuf(previous);
        require(tree.empty(), "empty traversals remain safe");
        require(tree.insert(4, "root"), "insert root");
        require(tree.insert(2, "left") && tree.insert(6, "right"), "insert children");
        require(tree.insert(1, "leaf") && tree.insert(3, "left-right"), "insert leaves");
        require(!tree.insert(2, "duplicate"), "reject duplicate key");
        std::string value;
        require(tree.retrieve(2, value) && value == "left", "retrieve preserves data");
        tree.remove(4); // Two children: successor replaces root.
        require(!tree.retrieve(4, value) && tree.retrieve(6, value), "two-child deletion");
        tree.remove(1);
        tree.remove(2);
        require(!tree.retrieve(1, value) && !tree.retrieve(2, value), "leaf/one-child deletion");
        tree.remove(3);
        tree.remove(6);
        require(tree.empty(), "delete all");

        binaryTree<int, int> longTree;
        for (int i = 0; i < 200; ++i) require(longTree.insert(i, i), "large insert");
        sink.str("");
        sink.clear();
        previous = std::cout.rdbuf(sink.rdbuf());
        longTree.preorder();
        std::cout.rdbuf(previous);
        int lines = 0;
        for (char c : sink.str()) if (c == '\n') ++lines;
        require(lines == 200, "preorder must not truncate at legacy stack capacity");
        std::cout << "PASS: city coordinate and BST regression checks\n";
    } catch (const std::exception& error) {
        std::cerr << "FAIL: " << error.what() << '\n';
        return 1;
    }
}
