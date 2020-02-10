#include <iostream>
#include "State.h"
#include <queue>
#include <vector>
#include <stack>
#include <math.h>
#include <chrono>


struct Compare_total_cost{
    bool operator()(State const& s1, State const& s2)
    {
        return s1.total_cost > s2.total_cost;
    }
};

int Total_cost (State s, int heuristic_no) {
    s.total_cost = 0;
    for (int i = 0; i < 9; i++) {
        int j = 0;
        while (j != s.tiles[i]) {
            j++;
        }
        if (heuristic_no == 1) {
            s.total_cost += (abs((j / 3) - (i / 3))) + (abs((j % 3) - (i % 3)));
        }else {
                s.total_cost += sqrt((pow((j / 3 - i / 3), 2)) + (pow((j % 3 - i % 3), 2)));
            }
    }
        s.total_cost += s.cost;
        return s.total_cost;
}

bool is_here_with_queue (State n, std::queue<State> frontier, std::vector<std::vector<int> > explored){
    while(!frontier.empty()){
        if(n.tiles == frontier.front().tiles){
            return true;
        }
        frontier.pop();
    }
    while(!explored.empty()){
        if(n.tiles == explored.front()){
            return true;
        }
        explored.erase(explored.begin());
    }
    return false;
}
bool is_here_with_stack (State n, std::stack<State> frontier, std::vector<std::vector<int> > explored){
    while(!frontier.empty()){
        if(n.tiles == frontier.top().tiles){
            return true;
        }
        frontier.pop();
    }
    while(!explored.empty()){
        if(n.tiles == explored.front()){
            return true;
        }
        explored.erase(explored.begin());
    }
    return false;
}

bool is_here_with_priorityQueue (State n, std::priority_queue<State, std::vector<State>, Compare_total_cost> frontier, std::vector<std::vector<int> > explored){
    int i = 0;
    while(!frontier.empty()){
        if(n.tiles == frontier.top().tiles){
            return true;
        }
        frontier.pop();
    }
    while(!explored.empty()){
        if(n.tiles == explored.front()){
            return true;
        }
        explored.erase(explored.begin());
    }
    return false;
}

std::vector<State> allocate_neighbors (State s){
    std::vector<int> t = s.tiles;
    int cost = s.cost;
    std::vector<State> ns;
    State n;
    int i = 0;
    while(s.tiles[i] != 0){
        i++;
    }
    //Right
    if((i != 2)&&(i != 5)&&(i != 8)){
        for (int j = 0; j < 9; j++) {
            if ((j != i)&&(j != i + 1)) {
                n.tiles[j] = s.tiles[j];
            } else if(j == i){
                n.tiles[i] = s.tiles[j+1];
                n.tiles[j+1] = 0;
            }
        }
        n.cost = s.cost + 1;
        n.parents = s.parents;
        n.parents.push_back(s);
        ns.push_back(n);
    }
    //Down
    if(i < 6) {
        for (int j = 0; j < 9; j++) {
            if ((j != i)&&(j != i + 3)) {
                n.tiles[j] = s.tiles[j];
            } else if(j == i){
                n.tiles[i] = s.tiles[j+3];
                n.tiles[j+3] = 0;
            }
        }
        n.cost = s.cost + 1;
        n.parents = s.parents;
        n.parents.push_back(s);
        ns.push_back(n);
    }
    //Left
    if((i != 0)&&(i != 3)&&(i != 6)){
        for (int j = 0; j < 9; j++) {
            if ((j != i - 1)&&(j != i)) {
                n.tiles[j] = s.tiles[j];
            } else if(j == i - 1){
                n.tiles[i] = s.tiles[j];
                n.tiles[j] = 0;
            }
        }
        n.cost = s.cost + 1;
        n.parents = s.parents;
        n.parents.push_back(s);
        ns.push_back(n);
    }
    //Up
    if(i > 2) {
        for (int j = 0; j < 9; j++) {
            if ((j != i - 3) && (j != i)) {
                n.tiles[j] = s.tiles[j];
            } else if (j == i - 3) {
                n.tiles[i] = s.tiles[j];
                n.tiles[j] = 0;
            }
        }
        n.cost = s.cost + 1;
        n.parents = s.parents;
        n.parents.push_back(s);
        ns.push_back(n);
    }

    return ns;
}


State BFS(State initial, State goal){
    std::queue<State> frontier;
    frontier.push(initial);
    std::vector<std::vector<int> > explored;
    State s;
    State n;
    while(! frontier.empty()){
        s = frontier.front();
        frontier.pop();
        explored.push_back(s.tiles);
        if(s.tiles == goal.tiles){
            std::cout<< "(Expanded nodes = " << explored.size()<<")";
            return s;
        }
        while(!s.neighbors.empty()){
            n = s.neighbors.front();
            s.neighbors.erase(s.neighbors.begin());
            if(!is_here_with_queue(n, frontier, explored)){
               n.neighbors = allocate_neighbors(n);
               frontier.push(n);
            }
        }
    }
    std::cout<< "(Expanded nodes = " << explored.size()<<")";
    s.tiles.clear();
    return s;
}


State DFS(State initial, State goal) {
    std::stack<State> frontier;
    frontier.push(initial);
    std::vector<std::vector<int> > explored;
    State s;
    State n;
    while(! frontier.empty()){
        s = frontier.top();
        frontier.pop();
        explored.push_back(s.tiles);
        if(s.tiles == goal.tiles){
            std::cout<< "(Expanded nodes = " << explored.size()<<")";
            return s;
        }
        while(!s.neighbors.empty()){
            n = s.neighbors.back();
            s.neighbors.pop_back();
            if(!is_here_with_stack(n, frontier, explored)){
                n.neighbors = allocate_neighbors(n);
                frontier.push(n);
            }
        }
    }
    std::cout<< "(Expanded nodes = " << explored.size()<<")";
    s.tiles.clear();
    return s;
}


State A(State initial, State goal, int heuristic_no) {
    std::priority_queue<State, std::vector<State>, Compare_total_cost> frontier;
    frontier.push(initial);
    std::vector<std::vector<int> > explored;
    State s;
    State n;
    while(! frontier.empty()){
        s = frontier.top();
        frontier.pop();
        explored.push_back(s.tiles);
        if(s.tiles == goal.tiles){
            std::cout<< "(Expanded nodes = " << explored.size()<<")";
            return s;
        }
        while(!s.neighbors.empty()){
            n = s.neighbors.front();
            s.neighbors.erase(s.neighbors.begin());
            if(!is_here_with_priorityQueue(n, frontier, explored)){
                n.total_cost = Total_cost(n, heuristic_no);
                n.neighbors = allocate_neighbors(n);
                frontier.push(n);
            }
        }
    }
    std::cout<< "(Expanded nodes = " << explored.size()<<")";
    s.tiles.clear();
    return s;
}


int main() {
    State goal;
    goal.tiles = {0, 1, 2, 3, 4, 5, 6, 7, 8};
    State initial;
    for(int i = 0; i < 9; i++) {
        std::cin>>initial.tiles[i];
    }
    initial.neighbors = allocate_neighbors(initial);
    State s;
    auto end = std::chrono::high_resolution_clock::now();;
    auto start = std::chrono::high_resolution_clock::now();;
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
    start = std::chrono::high_resolution_clock::now();
    std::cout << "BFS : ";
    s = BFS(initial, goal);
    end = std::chrono::high_resolution_clock::now();
    duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
    std::cout << "(Time = "<< duration.count()<<"microsecond)";
    bool success_bfs = (!s.tiles.empty());
    if(success_bfs){
        std::cout << "\nSuccess\n";
        std::cout << "Total_Cost : " << s.cost <<"\n";
        int steps = 0;
        while(!s.parents.empty()){
            std::cout << "Step " << steps << " :  (with Cost = " << s.parents.front().cost << ")," << "(In depth = " << s.parents.front().cost << ") \n";
            steps ++;
            for(int j = 0; j < 3; j++){
                for(int i = 0; i < 3; i++){
                    std::cout << s.parents.front().tiles[i+j*3] << " ";
                }
                std::cout <<"\n";
            }
            s.parents.erase(s.parents.begin());
            std::cout <<"\n";
        }
        std::cout << "Step " << steps << " :  (with Cost = " << s.cost << ")," << "(In depth = " << s.cost << ") \n";
        steps = 0;
        for(int j = 0; j < 3; j++){
            for(int i = 0; i < 3; i++){
                std::cout << s.tiles[i+j*3] << " ";
            }
            std::cout <<"\n";
        }
        std::cout <<"\n";
    } else{
        std::cout << "\nFailure\n";
    }
    start = std::chrono::high_resolution_clock::now();
    std::cout << "A 1 : ";
    initial.total_cost = Total_cost(initial, 1);
    initial.neighbors = allocate_neighbors(initial);
    s = A(initial, goal, 1);
    end = std::chrono::high_resolution_clock::now();
    duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
    std::cout << "(Time = "<< duration.count()<<"microsecond)";
    bool success_a1 = (!s.tiles.empty());
    if(success_a1){
        std::cout << "\nSuccess\n";
        std::cout << "Total_Cost : " << s.total_cost <<"\n";int steps = 0;
        while(!s.parents.empty()){
            std::cout << "Step " << steps << " :  (with Cost(g)= " << s.parents.front().cost << "), (with h = " << (s.parents.front().total_cost - s.parents.front().cost)<< "),(with Total_cost(f) = " <<s.parents.front().total_cost<< "),(In depth = " << s.parents.front().cost << ") \n";
            steps ++;
            for(int j = 0; j < 3; j++){
                for(int i = 0; i < 3; i++){
                    std::cout << s.parents.front().tiles[i+j*3] << " ";
                }
                std::cout <<"\n";
            }
            s.parents.erase(s.parents.begin());
            std::cout <<"\n";
        }
        std::cout << "Step " << steps << " :  (with Cost(g)= " << s.cost << "), (with h = " << (s.total_cost - s.cost)<< "),(with Total_cost(f) = " <<s.total_cost<< "),(In depth = " << s.cost << ") \n";
        steps = 0;
        for(int j = 0; j < 3; j++){
            for(int i = 0; i < 3; i++){
                std::cout << s.tiles[i+j*3] << " ";
            }
            std::cout <<"\n";
        }
        std::cout <<"\n";
    }  else{
        std::cout << "\nFailure";
    }
    start = std::chrono::high_resolution_clock::now();
    std::cout << "A 2 : ";
    initial.total_cost = Total_cost(initial, 2);
    initial.neighbors = allocate_neighbors(initial);
    s = A(initial, goal, 2);
    end = std::chrono::high_resolution_clock::now();
    duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
    std::cout << "(Time = "<< duration.count()<<"microsecond)";
    bool success_a2 = (!s.tiles.empty());
    if(success_a2){
        std::cout << "\nSuccess\n";
        std::cout << "Total_Cost : " << s.total_cost <<"\n";int steps = 0;
        while(!s.parents.empty()){
            std::cout << "Step " << steps << " : (with Cost(g)= " << s.parents.front().cost << "), (with h = " << (s.parents.front().total_cost - s.parents.front().cost)<< "),(with Total_cost(f) = " <<s.parents.front().total_cost<< "),(In depth = " << s.parents.front().cost << ") \n";
            steps ++;
            for(int j = 0; j < 3; j++){
                for(int i = 0; i < 3; i++){
                    std::cout << s.parents.front().tiles[i+j*3] << " ";
                }
                std::cout <<"\n";
            }
            s.parents.erase(s.parents.begin());
            std::cout <<"\n";
        }
        std::cout << "Step " << steps << " :  (with Cost(g)= " << s.cost << "), (with h = " << (s.total_cost - s.cost)<< "),(with Total_cost(f) = " <<s.total_cost<< "),(In depth = " << s.cost << ") \n";
        steps = 0;
        for(int j = 0; j < 3; j++){
            for(int i = 0; i < 3; i++){
                std::cout << s.tiles[i+j*3] << " ";
            }
            std::cout <<"\n";
        }
        std::cout <<"\n";
    }  else{
        std::cout << "\nFailure\n";
    }
    initial.neighbors = allocate_neighbors(initial);
    start = std::chrono::high_resolution_clock::now();
    std::cout << "DFS : ";
    s = DFS(initial, goal);
    end = std::chrono::high_resolution_clock::now();
    duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
    std::cout << "(Time = "<< duration.count()<<"microsecond)";
    bool success_dfs = (!s.tiles.empty());
    if(success_dfs){
        std::cout << "\nSuccess\n";
        std::cout << "Total_Cost : " << s.cost <<"\n";
        int steps = 0;
        while(!s.parents.empty()){
          std::cout << "Step " << steps << " :  (with Cost = " << s.parents.front().cost << ")," << "(In depth = " << s.parents.front().cost << ") \n";
          steps ++;
            for(int j = 0; j < 3; j++){
                for(int i = 0; i < 3; i++){
                    std::cout << s.parents.front().tiles[i+j*3] << " ";
                }
            std::cout <<"\n";
            }
            s.parents.erase(s.parents.begin());
            std::cout <<"\n";
        }
        std::cout << "Step " << steps << " :  (with Cost = " << s.cost << ")," << "(In depth = " << s.cost << ") \n";
        steps = 0;
        for(int j = 0; j < 3; j++){
            for(int i = 0; i < 3; i++){
                std::cout << s.tiles[i+j*3] << " ";
            }
            std::cout <<"\n";
        }
        std::cout <<"\n";
}  else{
    std::cout << "\nFailure\n";
}

    return 0;
}
