#include "CampusCompass.h"

#include <string>
#include <iostream>
#include <map>

using namespace std;

CampusCompass::CampusCompass() {
    this->graph = {}; // do need to construct the graph properly dont leave it like this
    this->student_info = {};
    this->loc_name = {};
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

void CampusCompass::insert(const string& name, const int& id, const int& home_id, const vector<string>& classes_taking)  {
    if (!student_info.count(id)) { // making sure the student is unique
        student_info[id] = make_pair(name, home_id);
        for (string class_code : classes_taking) {
            if (classes.count(class_code)) {
                classes[class_code].first.insert(id); // add student to that class
                student_schedule[id].insert(class_code); // add class to that student lol
            } else {
                cout << "unsuccessful" << endl;
                //return; maybe?
            }
        }
    } else {
        cout << "unsuccessful" << endl;
    }
}

void CampusCompass::remove(const int& id) {
    if (student_info.count(id)) {
        unordered_set<string> schedule = student_schedule[id];
        for (string class_code : schedule) { // removing student from all of their classes. if empty, just skips
            dropClass(id, class_code);
        }
        cout << "succesful" << endl;
    } else {
        cout << "unsuccessful" << endl;
    }
}

void CampusCompass::dropClass(const int& id, const string& class_code) {
    if (student_schedule.count(id) && student_schedule[id].count(class_code) && classes.count(class_code) && classes[class_code].first.count(id)) {
        classes[class_code].first.erase(id);
        student_schedule[id].erase(class_code);
        if (student_schedule[id].empty()) {
            student_schedule.erase(id);
            student_info.erase(id); // erasing the student's entry itself
        }
        cout << "succesful" << endl; // def going to be issues with too many couts
    }
    cout << "unsuccessful" << endl;

}
void CampusCompass::addClass(const int& id, const string& class_code) {
    if (student_schedule.count(id) && !student_schedule[id].count(class_code) && classes.count(class_code) && !classes[class_code].first.count(id)) {
        classes[class_code].first.insert(id); // add student to that class
        student_schedule[id].insert(class_code); // add class to that student
        cout << "successful" << endl;
    } else {
        cout << "unsuccessful" << endl;
    }
}

void CampusCompass::replaceClass(const int& id, const string& class_to_remove, const string& class_to_add) {
    if (student_info.count(id)) {
        this->dropClass(id, class_to_remove);
        this->addClass(id, class_to_add);
    } else {
        cout << "unsuccessful" << endl;
    }
}

void CampusCompass::removeClass(const string& class_code) {
    if (classes.count(class_code) && !classes[class_code].first.empty()) {
        cout << classes[class_code].first.size() << endl;
        for (int id : classes[class_code].first) {
            student_schedule[id].erase(class_code);
        }
        classes[class_code].first.clear();
    } else {
        cout << "unsuccessful" << endl;
    }
}

void CampusCompass::toggleEdges(const vector<pair<int, int>>& edges) { // always given valid edges, and i hope not duplicates
    for (auto [fst, snd] : edges) {
        for (auto i : graph[fst]) { // sets don't really work :C
            if (i.first == snd) {
                i.second.second = !i.second.second;
                break;
            }
        }
        for (auto i : graph[snd]) {
            if (i.first == fst) {
                i.second.second = !i.second.second;
                break;
            }
        }
    }
    cout << "successful" << endl;
}

int CampusCompass::checkEdgeBad(const int& id_a, const int& id_b) { // -1 dne, 0 open, 1 closed
    return 1;
}

bool CampusCompass::isConnected(const int& id_a, const int& id_b) {
    return false;
}

vector<pair<int,int>> CampusCompass::getShortestEdges(const int& student_id) {
    vector<pair<int,int>> edges = {};
    return edges;
}

void CampusCompass::printShortestEdges(const int& student_id) {

}

int CampusCompass::printStudentzone(const int& student_id) {
    return 0;
}
