#pragma once
#include <string>
#include <map>
#include <vector>
#include <utility>


using namespace std;

class CampusCompass {
private:
    // Think about what member variables you need to initialize
    // pretty sparse since paths tend to not overlap, so an adjacency list
    map<int, vector<pair<int,pair<int, bool>>>> locations; // make graph from prev data, we dont add or remove nodes/edges. Also remember that these are undirected, and the boolean should be false bc it represents "CLOSED"
    int num_edges;
    int num_nodes;
    map<int, string> student_id_to_name; // mappers. maybe make students their own class? probably not classes
    map<int, string> loc_id_to_name;
    map<string, pair<int,vector<int>>> classes; // class id with a pair containing location id and vector of student ids

    void insert(const string& name, const string& id, const string& home_id, const vector<string>& classes_taking);
    void remove(const string &student_id);
    void dropClass(const string& id, const string& class_code);
    void replaceClass(const string& id, const string& class_to_remove, const string& class_to_add);
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
