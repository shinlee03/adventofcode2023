#include <iostream>
#include <fstream>
#include <string>
#include <unordered_map>

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

    long long ans = 0; //store sum of values
    std::string line = ""; //placeholder var for processing input
    
    if(!strcmp(argv[2], "2")){

        std::unordered_map<std::string, int>dict = {{"one" , 1},{"two" , 2},{"three" , 3},{"four" , 4},{"five" , 5},{"six" , 6},{"seven" , 7},{"eight" , 8},{"nine" , 9}};
        while(getline(f, line)){

            //std::cout << line << ": ";
            //parse word to number
            if(line.size() > 3){
                for(int i=0;i<=line.size()-3; i++){
                    if(dict[line.substr(i,3)]){
                        int n = dict[line.substr(i,3)];
                        line.insert(i, std::to_string(n));
                        i++;

                    }
                    else if(i<=line.size()-4 && dict[line.substr(i,4)]){
                        int n = dict[line.substr(i,4)];
                        line.insert(i, std::to_string(n));
                        i++;
                    }
                    else if(i<=line.size()-5 && dict[line.substr(i,5)]){
                        int n = dict[line.substr(i,5)];
                        line.insert(i, std::to_string(n));
                        i++;
                    }
                    
                }
            }

            //std::cout << " " << line << ": "; 

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

            //std::cout << "a: " << a << ", b: " << b << ", ";
            
            ans += 10*a + b;
            //std::cout << "ans = " << ans << "\n";

        }
    }
    else if(!strcmp(argv[2], "1")){
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
    }else {
        std::cerr << "WRONG OPTION\n";
        exit(1);
    }
    

    std::cout << "the computed answer is " << ans << std::endl;
    return 0;
}
