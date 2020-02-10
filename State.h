#include <vector>

struct State {
    std::vector<int> tiles = {0, 0, 0, 0, 0, 0, 0, 0, 0};
    int cost = 0;
    int total_cost = 0;
    std::vector<State> neighbors;
    std::vector<State> parents;
};

