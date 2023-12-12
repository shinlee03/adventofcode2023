#include <iostream>
#include <fstream>

int main(int argc, char** argv){

    auto f1 = std::ifstream(argv[1]);
    auto f2 = std::ifstream(argv[2]);

    std::string l1, l2;
    int idx = 1;

    while(getline(f1, l1)){
        if(!f2.is_open()){
            std::cerr << "length does not match";
            exit(1);
        }
        getline(f2, l2);

        if(l1 != l2){
            std::cerr << "at line:" << idx << "\n" << l1 << "\n" << l2 << "\n do not match.\n";
            exit(1); 
        }
        idx++;
    }

    std::cout << "Looks Good To Me!" << std::endl;
    return 0;

}