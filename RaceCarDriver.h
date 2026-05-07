/**
 Author: Hannah Ross, Aubrey Bedford, TJ Ojo,
            Emeka Umeezeoke, Madison Quinter
 Assignment Title:  Group One Function
 Assignment Description: This program inputs the 'a, b, and c'
            values of the quadratic formula
            from the user and uses them to calculate the formula's roots.
 Due Date: 5/6/2026
 Date Created: 4/19/2026
 Date Last Modified: 5/6/2026

 We began the process on 4/19 meeting as a group over zoom discussing different
 ideas for a general algorithm. We discussed BFS, DFS and a graph scenario, and we
 ultimately ended up deciding to write pseudocode for the BFS and DFS to begin.

 We split the group-work as follows: TJ and Meka would work on the pseudocode and
 implementation of BFS and DFS, and Hannah, Aubrey and Madison would work with SDL
 Plotter and creating test cases and files. Each respective subgroup met throughout
 the week to work on their sections.

 We met again the following week and tested the BFS and DFS implementations on SDL.
 After discussing during the meeting, we decided to try implementing one more
 algorithm, the A* algorithm, in order to decrease our time.

 On 5/3-5/6, we discussed again and tested the BFS and A* algorithm.
*/

#ifndef RACECARDRIVER_H_
#define RACECARDRIVER_H_

#include "Racer.h"
#include <vector>
#include <queue>
#include <algorithm>


using namespace std;

DIRECTION nextMovePlaceInside_TeamOne(set<pair<int,int>>& walls);
DIRECTION invertDirection(DIRECTION dir);
void updateCurrentLocation(pair<int, int>& currentLocation, DIRECTION move);
stack<DIRECTION> reconstructPath_TeamOne(pair<int, int> start,
                                         pair<int, int> end,
                                         map<pair<int,int>,
                                         pair<pair<int, int>,
                                         DIRECTION>>& parentMap);
stack<DIRECTION> reconstructPath_Backtrack_TeamOne(pair<int, int> start,
                                                   pair<int, int> end,
                                                   map<pair<int,int>,
                                                   pair<pair<int, int>,
                                                   DIRECTION>>& parentMap);
void exploreNeighbors_TeamOne(pair<int, int> currentLocation,
                              map<pair<int,int>, pair<pair<int, int>,
                              DIRECTION>>& parentMap,
                              queue<pair<int, int>>& pointQueue,
                              set<pair<int,int>>& walls,
                              Racer* car);
void initializeBFS_TeamOne(pair<int, int>& startPos,
                           queue<pair<int, int>>& pointQueue,
                           map<pair<int,int>, pair<pair<int, int>,
                           DIRECTION>>& parentMap,
                           bool& isQueueInitialized);
DIRECTION BFSNextMove_TeamOne(set<pair<int,int>>& walls,
                              pair<int, int>& startLocation,
                              pair<int, int>& currentLocation,
                              queue<pair<int, int>>& pointQueue,
                              Racer* car);
vector<DIRECTION> nextMove_A_TeamOne(pair<int,int> start, pair<int,int> end,
                                     set<pair<int,int>>& wallsM);


class RaceCarDriver{
private:
    Racer* car;

public:
    RaceCarDriver(Racer* p = nullptr): car{p}{}

    DIRECTION nextMoveTeamOne(int run = 0) {
        // FOR BSF
        static set<pair<int,int>> walls;
        static set<pair<int,int>> freeSpaces;
        static set<pair<int,int>> visited;
        static queue<pair<int, int>> pointQueue;

        // from TJ
        static pair<int, int> startLocation = {0, 0};
        static pair<int, int> endLocation = {0, 0};
        static pair<int, int> currentLocation = {0, 0};

        if(run == 0) {
            // RUN BSF 1 HERE
            return BFSNextMove_TeamOne(walls,
                                       startLocation,currentLocation,
                                       pointQueue, car);
        } else if (run == 1) {
            // RUN HEURISTIC x1
            return nextMovePlaceInside_TeamOne(walls);
        } else {
            // RUN HEURISTIC x2
            return nextMovePlaceInside_TeamOne(walls);
        }
    }
};

DIRECTION nextMovePlaceInside_TeamOne(set<pair<int,int>>& walls) {
    static vector<DIRECTION> path;
    static int pathIndex = 0;

    cout << "inside helper " << endl;
    // Only compute path once
    if (path.empty()) {
        path = nextMove_A_TeamOne({0, 0},
                                  {2, 0}, walls);
        for (auto& d : path){
            if(d == EAST)  cout << "EAST" << endl;
            if(d == WEST)  cout << "WEST" << endl;
            if(d == NORTH) cout << "NORTH" << endl;
            if(d == SOUTH) cout << "SOUTH" << endl;
        }
    }

    if (pathIndex < path.size()) {
        return path[pathIndex++];
    }

    path.clear();
    pathIndex = 0;

    return EAST; // fallback
}

struct A_Details_TeamOne{

    int g;
    double f, h;
    int parent1, parent2;
    int x, y; // where node is in maze

};

bool isValid_TeamOne(int x, int y){
    return true;
}

bool isWall_TeamOne(set<pair<int,int>>& wallsM, int r, int c){
    for (auto& w : wallsM){
        if (w.first == r && w.second == c) return true;
    }
    return false;
}

vector<DIRECTION> nextMove_A_TeamOne(pair<int, int> start,
                                     pair<int, int> end,
                                     set<pair<int,int>>& wallsM){
    A_Details_TeamOne begin{};
    begin.f = 0;
    vector<A_Details_TeamOne> open; // open to exploration
    vector<A_Details_TeamOne> closed; // closed for eval

    // FINAL PATHS FOR FINAL
    vector<DIRECTION> finalDirection;

    begin.x = start.first;
    begin.y = start.second; // x and y of current
    begin.parent1 = -1; // needs init before starting
    begin.parent2 = -1;
    begin.g = 0;
    begin.h = 0;
    begin.f = 0;

    open.push_back(begin);

    while(!open.empty()){
        cout << "OPEN SIZE: " << open.size() << endl;

        int bestIndex = 0;
        for (int i = 1; i < open.size(); i++) {
            if (open[i].f < open[bestIndex].f) {
                bestIndex = i;
            }
        }

        A_Details_TeamOne q = open[bestIndex];
        open.erase(open.begin() + bestIndex);

        closed.push_back(q);

        //  Finding Successors:
        A_Details_TeamOne succ1{}; A_Details_TeamOne succ2{};
        A_Details_TeamOne succ3{}; A_Details_TeamOne succ4{};

        succ1.x = q.x + 1; succ1.y = q.y; // EAST
        succ2.x = q.x-1; succ2.y = q.y; //WEST
        succ3.x = q.x; succ3.y = q.y-1; // NORTH
        succ4.x = q.x; succ4.y = q.y+1; // SOUTH

        vector<A_Details_TeamOne> successors = {succ1, succ2,
                                                succ3, succ4};

        for (auto& succ : successors){

            if (!isValid_TeamOne(succ.x, succ.y)) continue;
            if (isWall_TeamOne(wallsM, succ.x, succ.y)) continue;

            // 1. GOAL CHECK FIRST
            if (succ.x == end.first && succ.y == end.second){
                succ.parent1 = q.x;
                succ.parent2 = q.y;
                closed.push_back(succ);

                A_Details_TeamOne current = succ;
                while (!(current.x == start.first
                && current.y == start.second)){
                    for (auto& node : closed){
                        if (node.x == current.parent1
                        && node.y == current.parent2){
                            int dx = current.x - node.x;
                            int dy = current.y - node.y;
                            if      (dx == 1)  finalDirection.push_back(EAST);
                            else if (dx == -1) finalDirection.push_back(WEST);
                            else if (dy == 1)  finalDirection.push_back(SOUTH);
                            else if (dy == -1) finalDirection.push_back(NORTH);
                            current = node;
                            break;
                        }
                    }
                }
                reverse(finalDirection.begin(),
                        finalDirection.end());
                return finalDirection; // return for car!
            }

            // 2. SET COSTS
            succ.parent1 = q.x;
            succ.parent2 = q.y;
            succ.g = q.g + 1;
            succ.h = abs(succ.x - end.first) +
                    abs(succ.y - end.second);
            succ.f = succ.g + succ.h;

            // 3. CHECKS FOR SKIPPING
            bool skip = false;
            for (auto& open_val : open){
                if (open_val.x == succ.x && open_val.y == succ.y){
                    if (succ.g >= open_val.g){
                        skip = true;
                    } else {
                        open_val.g = succ.g;
                        open_val.f = succ.f;
                        open_val.parent1 = q.x;
                        open_val.parent2 = q.y;
                        skip = true;
                    }
                    break;
                }
            }
            for (auto& closed_val : closed){
                if (closed_val.x == succ.x && closed_val.y == succ.y){
                    if (succ.g >= closed_val.g) skip = true;
                    break;
                }
            }

            // 4. ADD TO OPEN LAST
            if (!skip) open.push_back(succ);
        }
    }
    return {}; // no path found?
}

// BFS FUNCTIONS AND HELPER FUNCTIONS FOR TEAM ONE ---------------------------------------------

// function for inverting a direction (used for backtracking)
DIRECTION invertDirection(DIRECTION dir) {
    switch (dir) {
        case NORTH: return SOUTH;
        case SOUTH: return NORTH;
        case EAST:  return WEST;
        case WEST:  return EAST;
        //default: return NORTH; // default case, should never reach here
    }
}

// function to update currentLocation with each movement
void updateCurrentLocation(pair<int, int>& currentLocation, DIRECTION move) {
    switch (move) {
        case NORTH: currentLocation.second--; cout << "N" << endl; break;
        case SOUTH: currentLocation.second++; cout << "S" << endl; break;
        case EAST:  currentLocation.first++; cout << "E" << endl; break;
        case WEST:  currentLocation.first--; cout << "W" << endl; break;
    }
}

// function for reconstructPath_TeamOne()
stack<DIRECTION> reconstructPath_TeamOne(pair<int, int> start,
                                         pair<int, int> end,
                                         map<pair<int,int>,
                                         pair<pair<int, int>,
                                         DIRECTION>>& parentMap) {
    stack<DIRECTION> path;
    pair<int, int> current = end;

    while (!(current.first == start.first && current.second == start.second)) {
        pair<pair<int, int>, DIRECTION> parent = parentMap[{current.first,
                                                           current.second}];
        path.push(parent.second);
        current = parent.first;
        cout << "DIREC " << parent.second << endl;
    }

    return path;
}

// function for reconstructPath_Backtrack_TeamOne()
stack<DIRECTION> reconstructPath_Backtrack_TeamOne( pair<int, int>&start,
                                                    pair<int, int>& current,
                                                    pair<int, int>& target,
                                                    map<pair<int,int>,
                                                    pair<pair<int, int>,
                                                    DIRECTION>>& parentMap) {

    cout << "Start in backtrack " << start.first << " " << start.second << endl;

    vector<DIRECTION> pathUp;    // To go current -> start
    vector<DIRECTION> pathDown;  // To go start -> target
    stack<DIRECTION> finalPath;

    pair<int,int> node = current;
    while (!(node == start)) {
        pathUp.push_back(invertDirection(parentMap[node].second));
        node = parentMap[node].first;
    }

    node = target;
    while (!(node == start)) {
        pathDown.push_back(parentMap[node].second);
        node = parentMap[node].first;
    }

    reverse(pathDown.begin(), pathDown.end()); // Reversing pathDown to get correct order from start to target

    for (int i = pathDown.size()-1; i >= 0; i--) { // Reversed iteration
        finalPath.push(pathDown[i - 1]);
    }

    for (int i = pathUp.size()-1; i >= 0; i--) { // Reversed iteration
        finalPath.push(pathUp[i]);
    }

    return finalPath;

}

void exploreNeighbors_TeamOne(pair<int, int> currentLocation,
                              map<pair<int,int>, pair<pair<int, int>,
                              DIRECTION>>& parentMap,
                              queue<pair<int, int>>& pointQueue,
                              set<pair<int,int>>& walls,
                              Racer* car) {

    const int NUM_DIRECTIONS = 4;
    DIRECTION directions[NUM_DIRECTIONS] = {NORTH, SOUTH,
                                            EAST, WEST};

    // order of exploration: right, down, left, up
    for (int i = 0; i < NUM_DIRECTIONS; i++) {
        bool isWall = car->look(directions[i]);
        pair<int, int> neighbor = currentLocation;
        updateCurrentLocation(neighbor, directions[i]);

        if (isWall) {
            // wall — add to walls set, never touch parentMap (for A* later)
            walls.insert({neighbor.first, neighbor.second});
        }
        else if (parentMap.find(neighbor) == parentMap.end()) {
            // open path we haven't visited
            parentMap[neighbor] = {currentLocation, directions[i]};
            pointQueue.push(neighbor);
        }
    }
}

// function for initializing the BFS PointQueue
void initializeBFS_TeamOne(pair<int, int>& startPos,
                           queue<pair<int, int>>& pointQueue,
                           map<pair<int,int>, pair<pair<int, int>,
                           DIRECTION>>& parentMap,
                           bool& isQueueInitialized) {

    pair<int, int> start = startPos;
    pointQueue.push(start);
    parentMap[{start.first, start.second}] = {start, NORTH};
    // dummy value to represent the start point
    isQueueInitialized = true;
}

bool canPathDirectly(pair<int,int> current, pair<int,int> target,
                     map<pair<int,int>, pair<pair<int,int>,
                             DIRECTION>>& parentMap) {

    if (parentMap.find(target) == parentMap.end()) return false;

    pair<int,int> node = target;
    while (true) { // Replaced node != parentMap.at(node).first with true
        if (node == current) return true;
        if (node == parentMap.at(node).first) break; // Reached the start node
        node = parentMap.at(node).first;
    }

    return node == current; //
}

// function for BFS' next move (meant to be called in nextMoveTeamOne() when run == 0)
DIRECTION BFSNextMove_TeamOne(set<pair<int,int>>& walls,
                              pair<int, int>& startLocation,
                              pair<int, int>& currentLocation,
                              queue<pair<int, int>>& pointQueue,
                              Racer* car) {
    // static variables declarations inside function

    static stack<DIRECTION> pathToTarget;
    static pair<int, int> targetLocation = {0, 0};

    // start and end location were moved to be local to the TeamOneNextMove()
    static bool isPathing = false;
    static bool isQueueInitialized = false;
    static map<pair<int,int>, pair<pair<int, int>, DIRECTION>> parentMap;
    // note: walls will also be present in this function because the
    // TeamOneNextMove() has it statically declared

    // BFS Logic here!

    // if there is nothing in the Queue, give it the starting position
    if (!isQueueInitialized) {
        initializeBFS_TeamOne(currentLocation, pointQueue,
                              parentMap, isQueueInitialized);
    }

    // if the pathing buffer has stuff in it, pop and return the next move
    if (!pathToTarget.empty()) {
        cout << "Not researgin?? " << endl;
        DIRECTION nextMove = pathToTarget.top();
        pathToTarget.pop();
        updateCurrentLocation(currentLocation, nextMove);
        cout << "NExt move " << nextMove << endl;
        return nextMove;
    }

    // set boolean to false so we can start the pathfinding
    // and traversal process again
    isPathing = false;

    /**loop while the pointQueue is not empty and we are not
     * currently pathing to a target if we aren't at the targetLocation
     * yet, take this branch we want to build the path to be able to
     * explore targetLocation
     * if we are at the targetLocation, we now want to explore the
     * area around us
     * dequeue the targetLocation since we are now exploring it
     * explore the neighbors of the targetLocation and add them to
     * the queue if they are valid
     * also add the walls to the walls map that will be globally
     * relative to this function and
     * local relative to the TeamOneNextMove() function
     * */

    while (!pointQueue.empty()) {
        cout << "In queue? " << endl;
        targetLocation = pointQueue.front();

        if (currentLocation != targetLocation) {
            if (pathToTarget.empty()) {
                if (canPathDirectly(currentLocation,
                                    targetLocation, parentMap)) {
                    pathToTarget = reconstructPath_TeamOne(currentLocation,
                                                           targetLocation, parentMap);
                } else {
                    cout << "Start in func " << startLocation.first << " " << startLocation.second << endl;
                    pathToTarget = reconstructPath_Backtrack_TeamOne(startLocation,
                                                                     currentLocation,
                                                                     targetLocation, parentMap);
                }
            }

            DIRECTION nextMove = pathToTarget.top();
            pathToTarget.pop();
            updateCurrentLocation(currentLocation, nextMove);
            cout << "NExt move " << nextMove << endl;
            return nextMove;

        } else {

            pointQueue.pop();
            exploreNeighbors_TeamOne(currentLocation, parentMap,
                                     pointQueue, walls, car);
            while (!pathToTarget.empty()) pathToTarget.pop();

        }
    }

    /** if you are getting here, it means the queue is empty and you
     * haven't found the target which should never happen since the
     * mazes are guaranteed to have a solution, but if it does
     * just return something */

    assert(false); // should never reach here
}

#endif /* RACECARDRIVER_H_ */
