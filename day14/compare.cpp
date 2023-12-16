#include <iostream>
#include <fstream>
#include <string>

int main(int argc, char** argv){
    auto f = std::ifstream(argv[1]);
    auto ff = std::ifstream(argv[2]);

    std::string s, ss;
    while(getline(f, s)){
        if(!ff.is_open()){
            std::cout << "INCORRECT\n";
            exit(1);
        }
        getline(ff, ss);
        if(s != ss){
            std::cout << "INCORRECT\n";
            exit(1);
        }
    }

    if(ff.is_open()){
        std::cout << "INCORRECT\n";
        exit(1);
    }

    std::cout << "CORRECT\n";
    return 0;
}