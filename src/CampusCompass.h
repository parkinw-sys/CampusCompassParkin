#pragma once
#include <string>
#include <map>
#include <vector>
#include <utility>


using namespace std;

class CampusCompass {
private:
    // Think about what member variables you need to initialize
    // pretty sparse since paths tend to not overlap, so an adjacency list. contains a vector for the "to" vertex, along with the time and a "unwalkable" boolean
    map<int, vector<pair<int,pair<int, bool>>>> graph; // make graph from prev data, we dont add or remove nodes/edges. Also remember that these are undirected, and the boolean should be false bc it represents "CLOSED"
    map<int, pair<string, int>> student_info; // student id to their residence id and name
    map<int, vector<string>> student_schedule;
    map<int, string> loc_name;
    map<string, pair<vector<int>,int>> classes; // class id with a pair containing vector of student ids and location id

    void insert(const string& name, const int& id, const int& home_id, const vector<string>& classes_taking);
    void remove(const int& id);
    void dropClass(const int& id, const string& class_code);
    void addClass(const int& id, const string& class_code);
    void replaceClass(const int& id, const string& class_to_remove, const string& class_to_add);
    void removeClass(const string& class_code);
    void toggleEdges(const vector<pair<string, string>>& edges);
    int checkEdgeBad(const string& id_a, const string& id_b); // -1 dne, 0 open, 1 closed. if doing boolean, -1 does mean true, so beware
    bool isConnected(const string& id_a, const string& id_b);
    vector<pair<int,int>> printShortestEdges(const string& student_id); // returns the edges, dijkstras
    int printStudentzone(const string& student_id); // prints MST cost, probably kruskals?
public:
    // maybe we use actual error throwing thats caught in main for stuff like this, hmmmmm so i dont need to like pass along "all good" values. maybe a good idea to pass along anyways
    // Think about what helper functions you will need in the algorithm, remember all of these need to print successful at some point
    CampusCompass(); // constructor
    bool ParseCSV(const string &edges_filepath, const string &classes_filepath);
    bool ParseCommand(const string &command);


};