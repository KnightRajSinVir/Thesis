#include <iostream>
#include <fstream>
#include "graph.hpp"

simple_graph g;

int main() {
    g.init();
    g.run(8);
    g.end();
    return 0;
}
