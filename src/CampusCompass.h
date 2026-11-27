#pragma once
#include <string>
#include <unordered_map>
#include <vector>
#include <utility>
#include <unordered_set>

using namespace std;

class CampusCompass {
private:
    // Think about what member variables you need to initialize
    // pretty sparse since paths tend to not overlap, so an adjacency list. contains a vector for the "to" vertex, along with the time and a "unwalkable" boolean
    unordered_map<int, vector<pair<int,pair<int, bool>>>> graph; // make graph from prev data, we dont add or remove nodes/edges. Also remember that these are undirected, and the boolean should be false bc it represents "CLOSED"
    unordered_map<int, pair<string, int>> student_info; // student id to their residence id and name
    unordered_map<int, unordered_set<string>> student_schedule; // student id to set of the classes they are in
    unordered_map<int, string> loc_name; // i freaking love sets and maps
    unordered_map<string, pair<unordered_set<int>,int>> classes; // class id with a pair containing the set of student ids and location id

    bool addClass(const int& id, const string& class_code);
    vector<pair<int,int>> getShortestEdges(const int& ufid); // returns the edges, dijkstras
    bool validateUFID(const string& ufid);
    bool validateRegistered(const string& ufid);
    bool validateName(const string& name); // helper regex functions, for the simple ones on canvas
    bool validateClass(const string& class_code);

public:
    bool insert(const string& name, const int& ufid, const int& home_id, const vector<string>& classes_taking);
    bool remove(const int& ufid);
    bool dropClass(const int& ufid, const string& class_code);
    bool replaceClass(const int& ufid, const string& class_to_remove, const string& class_to_add);
    int removeClass(const string& class_code);
    bool toggleEdges(const vector<pair<int, int>>& edges);
    int checkEdgeBad(const int& id_a, const int& id_b); // -1 dne, 0 open, 1 closed. if doing boolean, -1 does mean true, so beware
    bool isConnected(const int& id_a, const int& id_b);
    void printShortestEdges(const int& ufid);
    int printStudentzone(const int& ufid); // prints MST cost, probably kruskals?

    // maybe we use actual error throwing thats caught in main for stuff like this, hmmmmm so i dont need to like pass along "all good" values. maybe a good idea to pass along anyways
    // Think about what helper functions you will need in the algorithm, remember all of these need to print successful at some point
    CampusCompass(); // constructor
    bool ParseCSV(const string &edges_filepath, const string &classes_filepath);
    string ParseCommand(const string &command);


};