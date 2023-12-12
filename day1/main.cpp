#include <iostream>
#include <fstream>
#include <string>

int main(int argc, char** argv) {

    if(argc < 2){
        std::cout << "USAGE: ./main.exe <FILE_NAME> <OPTIONS>\n";
        exit(0);
    }


    auto f = std::ifstream(argv[1]);

    if(!f.is_open()){
        std::cerr << "can't open file :(\n";
        exit(1);
    }

    int ans = 0; //store sum of values
    std::string line = ""; //placeholder var for processing input

    while(getline(f, line)){
        int a = -1e9;
        int b = -1e9;

        for(char c: line){
            if(c>='0' && c<='9'){
                if(a == -1e9) a = c-'0'; //only if it's the first digit! :)
                b = c-'0';
            }
        }

        if(a == -1e9 || b == -1e9) {
            std::cerr << "no digits on this line...\n";
            exit(1);
        }

        ans += 10*a + b;
    }

    std::cout << "the computed answer is " << ans << std::endl;
    return 0;
}
