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
    int idx = 0;
    const std::vector<std::string> verbose = {"one", "two", "three", "four", "five", "six", "seven", "eight", "nine"};
    while(getline(f, line)){
        std::cout << "BATCH " << idx++ << "\n";
        int pos = 0;
        while(pos < line.size()){
            std::cout << "pos = " << pos << "\n";
            bool ex = false;
            for(int i=1;i<line.size()-5;i++){
                std::cout << "looking at " << line.substr(pos, i) << "\n";
                for(int j=0;j<verbose.size();j++){
                    if(line.substr(pos, i) == verbose[j]){
                        std::cout << "Found " << j+1 << " with " << line.substr(pos, i) << "\n";
                        line.replace(line.begin()+pos, line.begin()+pos+verbose[j].size(), std::to_string(j+1));
                        std::cout << "line is now " << line << "\n";
                        pos++;
                        ex = true;
                        break;
                    }
                }
                if(ex)break;
            }
            if(!ex)pos++;
        }

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
