#include <iostream>
#include <fstream>
#include <string>
#include <cassert>
#include <vector>

using std::cin;
using std::cout;
using std::string;

int hash(string s){
    int ret = 0;

    for(char c: s){
        ret += c;
        ret *= 17;
        ret %= 256;
    }

    return ret;
}

void hash_test(){
    assert(hash("HASH") == 52);
    std::cout << "SUCCESS!\n";
}

std::vector<string> getAll(std::ifstream& i){
    std::vector<string> ret;

    string temp;
    string inp;
    while(getline(i, inp)){
        temp += inp;
    }

    string parse = "";
    for(char c: temp){
        if(c == ','){
            ret.push_back(parse);
            parse.clear();
        }
        else if(c == '\n'){
            continue;
        }
        else {
            parse += c;
        }
    }
    ret.push_back(parse);

    return ret;
}

int main(int argc, char** argv){
    auto f = std::ifstream(argv[1]);

    long long sum = 0;
    
    auto res = getAll(f);
    
    for(string s: res){
        sum += hash(s);
    }

    std::cout << "ANSWER IS " << sum << std::endl;
        
}