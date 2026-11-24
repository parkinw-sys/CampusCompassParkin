#include "CampusCompass.h"

#include <string>

using namespace std;

CampusCompass::CampusCompass() {
    this->locations = {}; // do need to construct the graph properly dont leave it like this
    this->num_edges = 0;
    this->num_nodes = 0;
    this->student_id_to_name = {};
    this->loc_id_to_name = {};
    this->classes = {};
}

bool CampusCompass::ParseCSV(const string &edges_filepath, const string &classes_filepath) {
    // return boolean based on whether parsing was successful or not
    return true;
}

bool CampusCompass::ParseCommand(const string &command) {
    // do whatever regex you need to parse validity
    // hint: return a boolean for validation when testing. For example:
    bool is_valid = true; // replace with your actual validity checking

    return is_valid;
}

void CampusCompass::insert(const string& name, const string& id, const string& home_id, const vector<string>& classes_taking)  {
    if
}

void CampusCompass::remove(const string& id) {
}

void CampusCompass::dropClass(const string& id, const string& class_code) {
}

void CampusCompass::replaceClass(const string& id, const string& class_to_remove, const string& class_to_add) {
}

void CampusCompass::removeClass(const string& class_code) {
}

void CampusCompass::toggleEdges(const vector<pair<string, string>>& edges) {
}

int CampusCompass::checkEdgeBad(const string& id_a, const string& id_b) {
}

bool CampusCompass::isConnected(const string& id_a, const string& id_b) {
}

vector<pair<int,int>> CampusCompass::printShortestEdges(const string& student_id) {
}

int CampusCompass::printStudentzone(const string& student_id) {
}
