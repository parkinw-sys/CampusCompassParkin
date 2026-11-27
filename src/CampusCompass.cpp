#include "CampusCompass.h"

#include <string>
#include <iostream>
#include <iomanip>
#include <fstream>
#include <sstream>
#include <regex>

using namespace std;

CampusCompass::CampusCompass() {
    this->graph = {}; // do need to construct the graph properly dont leave it like this
    this->student_info = {};
    this->loc_name = {};
    this->classes = {};

}

bool CampusCompass::ParseCSV(const string &edges_filepath, const string &classes_filepath) {
    ifstream edges(edges_filepath);
    if (!edges.is_open()) {
        return false;
    }
    string line;
    while (getline(edges, line)) {
        if (line.at(0) == 'L') {
            continue; // skip the header
        }
        vector<string> row;
        stringstream ss(line);
        string value;
        while (getline(ss, value, ',')) {
            row.push_back(value);
        }
        int loc1 = stoi(row[0]);
        int loc2 = stoi(row[1]);
        int time = stoi(row[4]);
        loc_name[loc1] = row[2];
        loc_name[loc2] = row[3];
        graph[loc1].emplace_back(make_pair(loc2, make_pair(time, false)));
        graph[loc2].emplace_back(make_pair(loc1, make_pair(time, false)));
    }
    ifstream classy(classes_filepath);
    if (!classy.is_open()) {
        return false;
    }
    while (getline(classy, line)) {
        if (line.at(1) == 'l') {
            continue; // skip the header
        }
        vector<string> row;
        stringstream ss(line);
        string value;
        while (getline(ss, value, ',')) {
            row.push_back(value);
        }
        int location = stoi(row[1]);
        classes[row[0]].second = location;
    }
    return true;
}

string CampusCompass::ParseCommand(const string &command) {
    string result = "unsuccessful";
    // do whatever regex you need to parse validity
    try {
        stringstream args(command);
        string main;
        args >> main;
        if (main == "insert") { // no i will not use regex for this its a waste of time
            string name;
            string ufidS;
            string home_idS;
            string n;
            args >> quoted(name) >> ufidS >> home_idS >> n;
            if (validateName(name) && validateUFID(ufidS) && !validateRegistered(ufidS)) {
                vector<string> class_codes;
                string value;
                int count = 0;
                while (getline(args, value, ' ')) {
                    if (validateClass(value)) {
                        count++;
                        class_codes.push_back(value);
                    }
                }
                if (count == stoi(n) && this->insert(name, stoi(ufidS), stoi(home_idS), class_codes)) {
                    result = "successful";
                }
            }
        } else if (main == "remove") {
            string ufidS;
            args >> ufidS;
            if (validateUFID(ufidS) && validateRegistered(ufidS) && this->remove(stoi(ufidS))) {
                result = "successful";
            }
        } else if (main == "dropClass") {

        } else if (main == "replaceClass") {

        } else if (main == "removeClass") {

        } else if (main == "toggleEdgesClosure") {

        } else if (main == "checkEdgeStatus") {

        } else if (main == "isConnected") {

        } else if (main == "printShortestEdges") {

        } else if (main == "printStudentZone") {

        }
    } catch (const exception& e) {
        cerr << "remember to remove this, but error: " << endl << e.what() << endl;;

    }
    return result;
}

bool CampusCompass::insert(const string& name, const int& ufid, const int& home_id, const vector<string>& classes_taking)  {
    if (student_info.count(ufid)) {// making sure the student is unique
        return false;
    }
    student_info[ufid] = make_pair(name, home_id);
    for (string class_code : classes_taking) {
        if (classes.count(class_code)) {
            classes[class_code].first.insert(ufid); // add student to that class
            student_schedule[ufid].insert(class_code); // add class to that student lol
        } else {
            return false;
        }
    }
    return true;

}

bool CampusCompass::remove(const int& ufid) {
    if (!student_info.count(ufid)) {
        return false;
    }
    unordered_set<string> schedule = student_schedule[ufid];
    for (string class_code : schedule) { // removing student from all of their classes. if empty, just skips
        if (!dropClass(ufid, class_code)) {
            return false;
        }
    }
    return true;
}

bool CampusCompass::dropClass(const int& ufid, const string& class_code) {
    if (student_schedule.count(ufid) && student_schedule[ufid].count(class_code) && classes.count(class_code) && classes[class_code].first.count(ufid)) {
        classes[class_code].first.erase(ufid);
        student_schedule[ufid].erase(class_code);
        if (student_schedule[ufid].empty()) {
            student_schedule.erase(ufid);
            student_info.erase(ufid); // erasing the student's entry itself
        }
        return true;
    }
    return false;

}
bool CampusCompass::addClass(const int& id, const string& class_code) { // add a class to a student's schedule (and that student to the classes roster)
    if (student_schedule.count(id) && !student_schedule[id].count(class_code) && classes.count(class_code) && !classes[class_code].first.count(id) && student_schedule[id].size() < 6) {
        classes[class_code].first.insert(id); // add student to that class
        student_schedule[id].insert(class_code); // add class to that student
        return true;
    } else {
        return false;
    }
}

bool CampusCompass::replaceClass(const int& ufid, const string& class_to_remove, const string& class_to_add) {
    if (!student_info.count(ufid)) {
        return false;
    }
    return this->dropClass(ufid, class_to_remove) && this->addClass(ufid, class_to_add);
}

int CampusCompass::removeClass(const string& class_code) {
    if (!classes.count(class_code) || classes[class_code].first.empty()) {
        return -1;
    }
    int num = classes[class_code].first.size();
    for (int id : classes[class_code].first) {
        student_schedule[id].erase(class_code);
    }
    classes[class_code].first.clear();
    return num;
}

bool CampusCompass::toggleEdges(const vector<pair<int, int>>& edges) { // always given valid edges, and i hope not duplicates
    for (auto [fst, snd] : edges) {
        for (auto& i : graph[fst]) { // sets don't really work :C
            if (i.first == snd) {
                i.second.second = !i.second.second;
                break;
            }
        }
        for (auto& i : graph[snd]) {
            if (i.first == fst) {
                i.second.second = !i.second.second;
                break;
            }
        }
    }
    return true;
}

int CampusCompass::checkEdgeBad(const int& id_a, const int& id_b) { // -1 dne, 0 open, 1 closed
    if (graph.count(id_a)) {
        for (auto edge : graph[id_a]) {
            if (edge.first == id_b) {
                return edge.second.second;
            }
        }
    }
    return -1;
}

bool CampusCompass::isConnected(const int& id_a, const int& id_b) {
    return false;
}

vector<pair<int,int>> CampusCompass::getShortestEdges(const int& ufid) {
    vector<pair<int,int>> edges = {};
    return edges;
}

bool CampusCompass::validateUFID(const string& ufid) { // only for use when inserting a new student, because it contains uniqueness
    try {
        regex reg = regex("[0-9]{8}");
        return regex_match(ufid, reg);
    } catch (const exception& e) {
        return false;
    }
}

bool CampusCompass::validateRegistered(const string& ufid) {
    try {
        return this->student_info.count(stoi(ufid));
    } catch (const exception& e) {
        return false;
    }
}

bool CampusCompass::validateName(const string& name) {
    try {
        regex reg = regex("[A-Za-z ]*");
        return regex_match(name, reg);
    } catch (const exception& e) {
        return false;
    }
}

bool CampusCompass::validateClass(const string& class_code) {
    try {
        regex reg = regex("[A-Z]{3}[0-9]{4}");
        return regex_match(class_code, reg);
    } catch (const exception& e) {
        return false;
    }
}

void CampusCompass::printShortestEdges(const int& ufid) {

}

int CampusCompass::printStudentzone(const int& ufid) {
    return 0;
}
