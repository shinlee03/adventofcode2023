#ifndef WORKFLOW
#include <iostream>
#include <fstream>
#include <cassert>
#include <string>
#include <vector>

using std::string;
using std::vector;

#include <unordered_map>

class Range{
public:
    Range(){
        for(int i=1;i<=4000;i++){
            x[0] = 1;
            m[0] = 1;
            a[0] = 1;
            s[0]= 1;
        }
    }
    Range(bool b){
        if(b){
            for(int i=1;i<=4000;i++){
                x[0] = 0;
                m[0] = 0;
                a[0] = 0;
                s[0]= 10;
            }
        }
    }
    int x[4001];
    int m[4001];
    int a[4001];
    int s[4001];
};


class workflow{
public:
    enum req{x, m, a, s, none};
    class requirement{
        public: 
            requirement(req r_in, bool g_in, int v_in, int id_in):r{r_in}, g{g_in}, v{v_in}, id{id_in} {} 
            req r;
            bool g;
            int v;
            int id;
    };

    workflow(vector<requirement>req_in, string name_in) : reqs{req_in}, name{name_in}{}
    workflow(){reqs = {}; name = "";}
    vector<requirement>& getReqs(){
        return reqs;
    }
private:
    vector<requirement>reqs;
    string name;
};

class ratings{
public:
    ratings(int x, int m, int a, int s){
        _values.resize(4,0);
        _values = {x,m,a,s};
    }
    ratings(string s){
        string temp;
        _values.clear();
        for(char c: s){
            if(c == ','){
                _values.push_back(std::stoi(temp.substr(temp.find('=')+1)));
                temp.clear();
            }else temp+=c;
        }
        temp = temp.substr(temp.find('=')+1);
        temp.pop_back();
        _values.push_back(std::stoi(temp));
    }

    int getAll(){
        int ret = 0;
        for(int i=0;i<4;i++){
            ret += get_value(workflow::req(i));
        }
        return ret;
    }
    
    void change_value(workflow::req r, int val){
        _values[r] = val;
    }

    int get_value(workflow::req r){
        return _values[r];
    }
    
private:
    vector<int>_values;
};

class workflows{
public:
    workflows(){
        _internal_id["A"] = 1;
        _internal_id["R"] = 2;
        _internal_counter = 3;
    }
    void create_workflow(string s){
        auto res = _parse(s);
        auto w = workflow(res.second, res.first);
        _wfs[getId(res.first)] = w;
    }

    std::size_t getId(string name){
        if(_internal_id[name] == 0){
            _internal_id[name] = _internal_counter++;
        }
        return _internal_id[name];
    }

    bool evaluate(ratings r){
        //starts from the "in" workflow.
        int current_workflow = getId("in");

        while(current_workflow != 1 && current_workflow != 2){
            for(auto w : _wfs[current_workflow].getReqs()){
                //std::cout << "at workflow " << current_workflow << "\n";
                if((w.g && r.get_value(w.r)>w.v) || (!w.g && r.get_value(w.r) < w.v)){
                    //goto next workflow.
                    //std::cout << "going to workflow " << w.id << "\n";
                    current_workflow = w.id;
                    break;
                }
            }            
        }
        return current_workflow == 1;
    }

    const Range ALL = Range;
    Range reverse(int current_workflow){
        if(_reverse_wfs[current_workflow].empty()) return ALL;



    }
private:
    std::unordered_map<string, std::size_t>_internal_id;
    std::size_t _internal_counter;
    std::unordered_map<std::size_t, workflow>_wfs;
    std::unordered_map<std::size_t, std::vector<workflow>>_reverse_wfs;
    //reverse_wfs[i]: the number of workflows that reaches to id i.
    //** ID is now source, not dest.

    std::pair<string, vector<workflow::requirement>> _parse(string s){
        string temp = "";
        std::size_t idx = 0;
        for(; idx<s.size();idx++){
            if(s[idx] == '{')break;
            temp+=s[idx];
        }
        string name = temp;
        getId(name); //create entry for name if doesnt exist
        temp.clear(); idx++;
        vector<workflow::requirement> req;
        for(; idx<s.size();idx++){
            if(s[idx] == ','){
                //stop and parse!
                workflow::req r = workflow::req(temp[0]=='x'?0:temp[0]=='m'?1:temp[0]=='a'?2:3);
                bool g = temp[1] == '>';
                int val = std::stoi(temp.substr(2, temp.find(':')-2));
                string dest = temp.substr(temp.find(':')+1);
                req.push_back(workflow::requirement(r, g, val, getId(dest)));
                //for part 2: reverse!
                _reverse_wfs[getId(dest)].push_back(workflow::requirement(r, g, val, getId(name)));
                
                temp.clear();
            }
            else temp+=s[idx];
        }
        //should have last one (which is else clause)
        req.push_back(workflow::requirement(workflow::req::none, true, 0, getId(temp.substr(0,temp.size()-1))));

        return std::make_pair(name, req);
    }
    friend class Tester;
};

class Tester{
public:
    void runTests(workflows& wfs) {
        //based on example.txt

        for(std::pair<string, std::size_t> p: wfs._internal_id){
            std::cout << p.first << ": " << p.second << "\n";
        }

        assert(wfs._internal_id["A"]==1);
        assert(wfs._internal_id["R"]==2);
        assert(wfs._internal_id["px"]==3);
        assert(wfs._internal_id["qkq"]==4);
        assert(wfs._internal_id["rfg"]==5);
        assert(wfs._internal_id["pv"]==6);
        assert(wfs._internal_id["lnx"]==7);
        assert(wfs._internal_id["gd"]==8);
        assert(wfs._internal_id["qs"]==9);
        assert(wfs._internal_id["crn"]==10);
        assert(wfs._internal_id["in"]==11);
        assert(wfs._internal_id["qqz"]==12);
        assert(wfs._internal_id["hdj"]==13);

        

        std::cout << "Tests passed!\n";
    }

};
#endif