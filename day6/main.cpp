#include <iostream>
#include <fstream>
#include <string>

#include "helper.hpp"

using std::string;

int main(int argc, char** argv){
    auto f = std::ifstream(argv[1]);
    const bool ispartone = !strcmp(argv[2], "1");
    string s;
    getline(f, s);
    auto times = ispartone? parse<true>(s) : parse<false>(s);
    getline(f, s);
    std::cout << "\n";
    auto distances = ispartone? parse<true>(s) : parse<false>(s);

    if(!times.has_value() || !distances.has_value()){
        std::cerr << "Incorrect input format.\n";
        exit(-1);
    }
    long long ans = ways(times.value()[0], distances.value()[0]);
    for(int i=1;i<times.value().size();i++){
        ans *= ways(times.value()[i], distances.value()[i]);
    }

    std::cout << "ANSWER IS " << ans << std::endl;

    return 0;
}