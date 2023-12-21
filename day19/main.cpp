#include <iostream>
#include <fstream>

#include <string>
#include <vector>

using std::string;
using std::vector;

#include <unordered_map>
#include "workflow.hpp"

int main(int argc, char** argv){

    auto f = std::ifstream(argv[1]);
    string s;
    
    int ans = 0;

    workflows wfs;

    //parse workflows
    while(getline(f, s)){
        if(s.empty())break;
        wfs.create_workflow(s);
    }

    //Tester t;
    //t.runTests(wfs);
    //return 0;

    //get ratings
    if(!strcmp(argv[2], "1")){
        while(getline(f, s)){
            ratings r = ratings(s);

            //evaluate 
            if(wfs.evaluate(r)){
                ans += r.getAll();
            }
        }
    }
    else {
        f.close();

        //obv can't brute force.

        //reverse from 'A'. Construct Range Variables in workflow.hpp.


    }
    

    std::cout << "answer is " << ans << std::endl;

}