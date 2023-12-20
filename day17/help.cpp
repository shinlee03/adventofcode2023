#include <iostream>
#include <fstream>
#include <string>

int main(int argc, char** argv){
    auto f = std::ifstream(argv[1]);
    auto ff = std::ifstream(argv[2]);

    int ans = 0;
    std::string s;
    std::string ss;
    while(getline(f,s)){
        getline(ff, ss);

        for(int i=0;i<s.size();i++){
            if(s[i] != ss[i]){
                ans += s[i] - '0';
            }
        }
    }

    std::cout << ans;
}