#include "CampusCompass.h"

#include <string>
#include <iostream>
#include <iomanip>
#include <fstream>
#include <sstream>
#include <regex>

using namespace std;

CampusCompass::CampusCompass() {
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
        graph[loc1].insert(EdgeInfo(loc2, time));
        graph[loc2].insert(EdgeInfo(loc1, time));
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
            try {
                string name;
                int ufid;
                int home_id;
                int n;
                args >> quoted(name) >> ufid >> home_id >> n;
                if (args && validateName(name) && validateUFID(ufid) && !validateRegistered(ufid)) {
                    vector<string> class_codes;
                    string value;
                    int count = 0;
                    while (getline(args, value, ' ')) {
                        if (validateClass(value)) {
                            count++;
                            class_codes.push_back(value);
                        }
                    }
                    if (count == n && this->insert(name, ufid, home_id, class_codes)) {
                        result = "successful";
                    }
                }
            } catch (...) {}
        } else if (main == "remove") {
            int ufid;
            args >> ufid;
            if (args && !(args >> std::ws) && this->remove(ufid)) {
                result = "successful";
            }
        } else if (main == "dropClass") {
            int ufid;
            string class_code;
            args >> ufid >> class_code;
            if (args && !(args >> ws) && this->dropClass(ufid, class_code)) {
                result = "successful";
            }
        } else if (main == "replaceClass") {
            string ufidS;
            string class_to_remove;
            string class_to_add;
            args >> ufidS >> class_to_remove >> class_to_add;
            if (args && !(args >> ws) && this->replaceClass(stoi(ufidS), class_to_remove, class_to_add)) {
                result = "successful";
            }
        } else if (main == "removeClass") {
            string class_code;
            args >> class_code;
            if (args && !(args >> ws)) {
                if (int temp = this->removeClass(class_code) >= 0) { // in-line-if instantiation still scares me
                    result = to_string(temp);
                }
            }

        } else if (main == "toggleEdgesClosure") { // always given valid edges that's nice
            int n;
            args >> n;
            int from;
            int to;
            int count = 0;
            vector<pair<int,int>> edges;
            while (args >> from >> to) {
                count++;
                edges.emplace_back(from, to);
            }
            if (count == n && this->toggleEdges(edges)) {
                result = "success";
            }

        } else if (main == "checkEdgeStatus") {
            int locA;
            int locB;
            args >> locA >> locB;
            if (args && !(args >> ws)) {
                int code = this->checkEdgeBad(locA, locB);
                if (code == 1) {
                    result = "closed";
                } else if (code == 0) {
                    result = "open";
                } else {
                    result = "DNE";
                }
            }
        } else if (main == "isConnected") {
            int locA;
            int locB;
            args >> locA >> locB;
            if (args && !(args >> ws) && this->isConnected(locA, locB)) {
                result = "success";
            }
        } else if (main == "printShortestEdges") {

        } else if (main == "printStudentZone") {

        }
    } catch (const exception& e) {
        cerr << "remember to remove this, but error: " << endl << e.what() << endl;;

    }
    return result;
}

bool CampusCompass::insert(const string& name, const int& ufid, const int& home_id, const vector<string>& classes_taking)  {
    if (validateRegistered(ufid)) {// making sure the student is unique
        return false;
    } // THIS ONE HAS TO BE VALIDATED OUTSIDE THE FUNCTION, OR ELSE IT WILL DO HALF OF IT AND THEN HALT WHICH IS BAD
    student_info[ufid] = make_pair(name, home_id);
    for (string class_code : classes_taking) {
        if (validateClass(class_code)) {
            classes[class_code].first.insert(ufid); // add student to that class
            student_schedule[ufid].insert(class_code); // add class to that student lol
        } else {
            return false;
        }
    }
    return true;

}

bool CampusCompass::remove(const int& ufid) {
    if (!validateUFID(ufid) || !validateRegistered(ufid)) {
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
    if (validateUFID(ufid) && validateRegistered(ufid) && student_schedule[ufid].count(class_code) && validateClass(class_code) && classes[class_code].first.count(ufid)) {
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
    if (validateUFID(id) && validateRegistered(id) && !student_schedule[id].count(class_code) && validateClass(class_code) && !classes[class_code].first.count(id) && student_schedule[id].size() < 6) {
        classes[class_code].first.insert(id); // add student to that class
        student_schedule[id].insert(class_code); // add class to that student
        return true;
    } else {
        return false;
    }
}

bool CampusCompass::replaceClass(const int& ufid, const string& class_to_remove, const string& class_to_add) {
    if (!validateUFID(ufid) || !validateClass(class_to_add) || !validateClass(class_to_remove) || !validateRegistered(ufid)) {
        return false;
    }
    return this->dropClass(ufid, class_to_remove) && this->addClass(ufid, class_to_add);
}

int CampusCompass::removeClass(const string& class_code) {
    if (!validateClass(class_code) || classes[class_code].first.empty()) {
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
    int count = 0;
    try {
        for (auto [fst, snd] : edges) {
            // for (auto& i : graph[fst]) { // sets don't really work :C
            //     if (i.to == snd) {
            //         i.closed = !i.closed;
            //         check++;
            //         break;
            //     }
            // }
            // for (auto& i : graph[snd]) {
            //     if (i.to == fst) {
            //         i.closed = !i.closed;
            //         check++;
            //         break;
            //     }
            // }
            graph[fst].find(snd)->toggle();
            graph[snd].find(fst)->toggle();
            if (graph[fst].find(snd)->closed == graph[snd].find(fst)->closed) {
                count++;
            }
        }
        return count == static_cast<int>(edges.size());
    } catch (...) {
        return false;
    }
}

int CampusCompass::checkEdgeBad(const int& id_a, const int& id_b) { // -1 dne, 0 open, 1 closed
    if (graph.count(id_a)) {
        for (auto edge : graph[id_a]) {
            if (edge.to == id_b) {
                return edge.closed;
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

bool CampusCompass::validateUFID(const int& ufid) { // only for use when inserting a new student, because it contains uniqueness
    try {
        string ufidS = to_string(ufid);
        regex reg = regex("[0-9]{8}");
        return regex_match(ufidS, reg);
    } catch (...) {
        return false;
    }
}

bool CampusCompass::validateRegistered(const int& ufid) {
    try {
        return this->student_info.count(ufid);
    } catch (...) {
        return false;
    }
}

bool CampusCompass::validateName(const string& name) {
    try {
        regex reg = regex("[A-Za-z ]*");
        return regex_match(name, reg);
    } catch (...) {
        return false;
    }
}

bool CampusCompass::validateClass(const string& class_code) {
    try {
        regex reg = regex("[A-Z]{3}[0-9]{4}");
        return regex_match(class_code, reg) && classes.count(class_code);
    } catch (...) {
        return false;
    }
}

bool CampusCompass::validateLocation(const int& loc_id) {
    try {
        return this->loc_name.count(loc_id);
    } catch (...) {
        return false;
    }
}

void CampusCompass::printShortestEdges(const int& ufid) {

}

int CampusCompass::printStudentzone(const int& ufid) {
    return 0;
}