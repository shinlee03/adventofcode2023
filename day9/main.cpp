//
//  main.cpp
//  adventofcode9
//
//  Created by Shin Lee on 12/9/23.
//

#include <iostream>
#include <fstream>
#include <string>
#include <vector>

bool reverse = false;

bool all_zero(std::vector<int>& v){
    for(int i: v)if(i!=0)return false;
    return true;
}

int val(std::vector<int>v){

    if(all_zero(v)){
        return 0;
    }
    std::vector<int>next;
    for(int i=0;i<v.size()-1;i++){
        next.push_back(v[i+1]-v[i]);
    }
    
    
    return reverse? v[0] - val(next) : *v.rbegin() + val(next);
}

int main(int argc, char** argv) {

    if(argc < 2){
        std::cout << "USAGE: ./main.exe <FILE_NAME> <OPTIONS>\n";
        exit(0);
    }

    if(argc>2 && !strcmp(argv[2], "-r")){
        reverse = true;
    }

    auto f = std::ifstream(argv[1]);
    
    if(!f.is_open()){   
        std::cerr << "file not open\n";
        exit(1);
    }
    
    std::string line; //temporary input processor
    int ans = 0; //sum of things here
    
    while(getline(f, line)){
        std::vector<int>v;
        std::string num = "";
        for(char c: line){
            if(c!=' ')num+=c;
            else {
                v.push_back(std::stoi(num));
                num.clear();
            }
        }
        v.push_back(std::stoi(num));

        ans += val(v);
    }

    std::cout << "computed total values are: " << ans << std::endl;

    f.close();
}
