#ifndef HELPER

#include <string>
#include <vector>

using std::string;
using std::vector;

#include <cassert>
#include <fstream>
#include <iostream>

// helper function
// param s: string to verify
// param rule: number of consequent hot springs
// returns whether the string follows the rule supplied

bool verify(string s, vector<int>rule, int idx, int sum, bool was_springs){
    if(s.empty()){
        if(was_springs){
            //last count check!
            if(idx != rule.size()-1)return false; //didn't iterate through all rules
            if(sum != rule[idx])return false; //incorrect spring count
            return true;
        }else return idx == rule.size(); //only check we did all rules
    };

    if(s[0] == '.'){
        if(was_springs){
            if(sum == rule[idx]){
                return verify(s.substr(1), rule, idx+1, 0, false);
            }
            else return false;
        }
        else {
            return verify(s.substr(1), rule, idx, 0, false);
        }
    }
    if(s[0] == '#'){
        return verify(s.substr(1), rule, idx, sum+1, true);
    }

    return true;
}

void test_validity(){
    std::ifstream f = std::ifstream("test_true.txt");

    string line;
    while(getline(f, line)){
        if(!verify(line, {3,2,1}, 0, 0, false)){
            std::cerr << "ERROR ON INPUT " << line << " AS FALSE (SHOULD BE TRUE)\n";
        }
    }

    auto ff = std::ifstream("test_false.txt");
    while(getline(ff, line)){
        if(verify(line, {3,2,1}, 0, 0, false)){
            std::cerr << "ERROR ON INPUT " << line << " AS TRUE (SHOULD BE FALSE)\n";
        }
    }

    std::cout << "Test successful!" << std::endl;
}

struct parsedData{
    vector<int>rules;
    string phrase;
};

parsedData parse(string line){
    vector<string>res;

    std::string temp = "";
    for(char c: line){
        if(c == ' ' || c == ','){
            res.push_back(temp);
            temp.clear();
        }
        else temp+=c;
    }
    res.push_back(temp);

    std::string n = res[0];
    vector<int>rules;
    for(int i=1;i<res.size();i++){
        rules.push_back(std::stoi(res[i]));
    }
    return {rules, n};
}

#endif