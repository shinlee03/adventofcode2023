#ifndef HELPER

#include <string>
#include <vector>
#include <iostream>
using std::string;

//SUPPLY BOOL T => TRUE IF SEAPARATE NUMBERS FOR SPACES, FALSE IF MERGE NUMBERS.
template <bool T>
std::optional<std::vector<long long>>parse(string s){
    string temp = "";
    try{
        s = s.substr(s.find(':')+1);
        std::vector<long long>ret;
        bool flag = true; 
        for(char c: s){
            if(c==' ' && !flag){
                if(T){
                    ret.push_back(std::stoll(temp));
                    temp.clear();
                    flag = true;
                }
            }
            else if(c==' ' && flag){
                continue;
            }
            else {
                temp += c;
                flag = false;
            }
            
        }
        ret.push_back(std::stoll(temp));
        return std::optional<std::vector<long long>>{ret};
    }catch(std::exception &e){
        std::cerr << "Error in Parse with temp: " << (temp.empty()? "EMPTY" : temp) << "\n";
        std::cerr << "\n" << e.what() << "\n";
        exit(-1);
        return std::nullopt;
    }   
    return std::nullopt;
}

//O(timelimt) time.
long long ways(long long timelimit, long long winthreshold){
    long long w = 0;
    for(long long t=0;t<=timelimit;t++){
        w += (timelimit-t)*t > winthreshold ? 1 : 0;
    }
    return w;
}

#endif