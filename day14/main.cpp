#include <iostream>
#include <fstream>
#include <string>
#include <vector>

using std::string;
using std::vector;

#include <unordered_map>

const int CNT = 1e9;

const bool DEBUG = false;
enum grid{
    freerock = 'O',
    fixedrock = '#',
    empty = '.'
};

vector<int>north_bound;


void debug_print(vector<vector<grid>>&m){
    for(auto a: m){
        for(grid b: a){
            std::cout << char(b);
        }
        std::cout << "\n";
    }
}

long long calculate_weight(vector<vector<grid>>m){
    long long ret = 0;
    long long weight = 0;
    for(int i=m.size()-1; i>=0;i--){
        weight++;
        for(int j=0; j<m[i].size();j++){
            
            if(m[i][j] == grid('O')){
                ret += weight;
            }
        }
        
    }
    return ret;
}

void roll_north(vector<vector<grid>>&m){
    for(int i=0;i<m.size();i++){
        for(int j=0;j<m[0].size();j++){
            if(m[i][j] == '.') continue;
            else if(m[i][j] == '#'){
                //may have to recalculate northbound.
                if(north_bound[j] < i){
                    north_bound[j] = i+1;
                    while(north_bound[j]<m.size() && m[north_bound[j]][j] != '.'){
                        north_bound[j]++;
                    }   
                }
            }
            else if(m[i][j] == 'O' && north_bound[j] < m.size() && north_bound[j] < i){
                //roll over to bound!
                m[north_bound[j]][j] = grid('O');
                m[i][j] = grid('.');

                //recalculate northbound.
                while(north_bound[j]<m.size() && m[north_bound[j]][j] != '.'){
                    north_bound[j]++;
                }
            }   
            else {
                //can't move, but still update northbound!
                while(north_bound[j]<m.size() && m[north_bound[j]][j] != '.'){
                    north_bound[j]++;
                }
            }
        }     
    }
}

void manually_roll(vector<vector<grid>>&m){
    vector<int>north_bound;
    vector<int>south_bound;
    vector<int>west_bound;
    vector<int>east_bound;
     for(int i=0;i<m[0].size();i++){
            int idx = 0;
            while(idx < m.size() && m[idx][i] != '.'){
                idx++;
            }
            north_bound.push_back(idx);
        }

    for(int i=0;i<m.size();i++){
        for(int j=0;j<m[0].size();j++){
            if(m[i][j] == '.') continue;
            else if(m[i][j] == '#'){
                //may have to recalculate northbound.
                if(north_bound[j] < i){
                    north_bound[j] = i+1;
                    while(north_bound[j]<m.size() && m[north_bound[j]][j] != '.'){
                        north_bound[j]++;
                    }   
                }
            }
            else if(m[i][j] == 'O' && north_bound[j] < m.size() && north_bound[j] < i){
                //roll over to bound!
                m[north_bound[j]][j] = grid('O');
                m[i][j] = grid('.');

                //recalculate northbound.
                while(north_bound[j]<m.size() && m[north_bound[j]][j] != '.'){
                    north_bound[j]++;
                }
            }   
            else {
                //can't move, but still update northbound!
                while(north_bound[j]<m.size() && m[north_bound[j]][j] != '.'){
                    north_bound[j]++;
                }
            }
        }     
    }

    if(DEBUG){
        std::cout << "AFTER NORTH: \n";
        debug_print(m);
    }

    for(int i=0;i<m.size();i++){
            int idx = 0;
            while(idx < m[0].size() && m[i][idx] != '.'){
                idx++;
            }
            west_bound.push_back(idx);
        }

    for(int i=0;i<m[0].size();i++){
        for(int j=0;j<m.size();j++){
            if(m[j][i] == '.') continue;
            else if(m[j][i] == '#'){
                //may have to recalculate westbound.
                if(west_bound[j] < i){
                    west_bound[j] = i+1;
                    while(west_bound[j]<m[0].size() && m[j][west_bound[j]] != '.'){
                        west_bound[j]++;
                    }   
                }
            }
            else if(m[j][i] == 'O' && west_bound[j] < m.size() && west_bound[j] < i){
                //roll over to bound!
                m[j][west_bound[j]] = grid('O');
                m[j][i] = grid('.');

                //recalculate westbound.
                 while(west_bound[j]<m[0].size() && m[j][west_bound[j]] != '.'){
                        west_bound[j]++;
                }   
            }   
            else {
                //can't move, but still update westbound!
                 while(west_bound[j]<m[0].size() && m[j][west_bound[j]] != '.'){
                    west_bound[j]++;
                }   
            }
        }     
    }

    if(DEBUG) {
        std::cout << "AFTER WEST: \n";
        debug_print(m);
    }

    for(int i=0;i<m[0].size();i++){
            int idx = m.size()-1;
            while(idx >=0 && m[idx][i] != '.'){
                idx--;
            }
            south_bound.push_back(idx);
        }

    for(int i=m.size()-1;i>=0;i--){
        for(int j=0;j<m[0].size();j++){
            if(m[i][j] == '.') continue;
            else if(m[i][j] == '#'){
                //may have to recalculate northbound.
                if(south_bound[j] > i){
                    south_bound[j] = i-1;
                    while(south_bound[j]>=0 && m[south_bound[j]][j] != '.'){
                        south_bound[j]--;
                    }   
                }
            }
            else if(m[i][j] == 'O' && south_bound[j] >= 0 && south_bound[j] > i){
                //roll over to bound!
                m[south_bound[j]][j] = grid('O');
                m[i][j] = grid('.');

                //recalculate northbound.
                while(south_bound[j]>=0 && m[south_bound[j]][j] != '.'){
                    south_bound[j]--;
                }   
            }   
            else {
                //can't move, but still update northbound!
                while(south_bound[j]>=0 && m[south_bound[j]][j] != '.'){
                    south_bound[j]--;
                }   
            }
        }     
    }

    if(DEBUG){
        std::cout << "AFTER SOUTH: \n";
        debug_print(m);
    }

    for(int i=0;i<m.size();i++){
            int idx = m[0].size()-1;
            while(idx >=0 && m[i][idx] != '.'){
                idx--;
            }
            east_bound.push_back(idx);
        }

    for(int i=m[0].size()-1;i>=0;i--){
        for(int j=0;j<m.size();j++){
            if(m[j][i] == '.') continue;
            else if(m[j][i] == '#'){
                //may have to recalculate eastbound.
                if(east_bound[j] > i){
                    east_bound[j] = i-1;
                    while(east_bound[j]>=0 && m[j][east_bound[j]] != '.'){
                        east_bound[j]--;
                    }   
                }
            }
            else if(m[j][i] == 'O' && east_bound[j] >= 0 && east_bound[j] > i){
                //roll over to bound!
                m[j][east_bound[j]] = grid('O');
                m[j][i] = grid('.');

                //recalculate westbound.
                  while(east_bound[j]>=0 && m[j][east_bound[j]] != '.'){
                        east_bound[j]--;
                    }   
            }   
            else {
                //can't move, but still update westbound!
                 while(east_bound[j]>=0 && m[j][east_bound[j]] != '.'){
                        east_bound[j]--;
                    }   
            }
        }     
    }
    if(DEBUG) {
        std::cout << "AFTER EAST: \n";
        debug_print(m);
    }
}

int main(int argc, char** argv) {
    if(argc < 3){
        std::cerr << "USAGE: ./main <INPUT_FILE> <TASK_MODE>" << std::endl;
        exit(1);
    }
    auto f = std::ifstream(argv[1]);

    if(!f.is_open()){
        std::cerr << "Cannot open file or file named " << argv[1] << "does not exist.\n";
        exit(1);
    }

    vector<vector<grid>>m;
    string s;
    while(getline(f, s)){
        m.push_back({});

        for(char c: s){
            m.rbegin()->push_back(grid(c));
        }
    }

    long long ans = 0;

    if(!strcmp(argv[2], "1")){

        for(int i=0;i<m[0].size();i++){
            int idx = 0;
            while(idx < m.size() && m[idx][i] != '.'){
                idx++;
            }
            north_bound.push_back(idx);
        }

        roll_north(m);

        //debug_print(m);

        ans = calculate_weight(m);

    }else if(!strcmp(argv[2], "2")){
        //cannot manulally simulate, as each 'roll' takes 0.38s --> 380000000*4 s (not possible)
        
        //Should be "iterations" or "cycles" of states --> detect that.

        vector<vector<vector<grid>>>cached;
        cached.push_back({}); //0 -> none

        for(int i=1;i<=CNT; i++){
            //I'm so sorry cpu :(

            manually_roll(m);
            std::cout << "CYCLE " << i << " / " << CNT << "\n";
            //debug_print(m);
            //std::cout << "-----------\n";
            auto res = std::find(cached.begin(), cached.end(), m);
            
            if(res != cached.end()){
                std::cout << "FOUND CACHED CYCLE AT " << i << " = CACHED[" << res-cached.begin() << "]!\n";
                int found = res - cached.begin();
                int frequency = i - found; //repeats from res-cached.begin() ~ i .
                std::cout << "frequency is " << frequency << "\n";

                // 10^9th is [(10^9-found) % frequency]
                ans = calculate_weight(cached[(CNT-found)%frequency + found]);
                break;
            }
            else cached.push_back(m);  
        } 
        if(ans == 0){
            ans = calculate_weight(m);
        }

    }
    else {
        std::cerr << "WRONG OPTION ARG\n";
        exit(1);
    }

    std::cout << "SUCCESS!\n";
    std::cout << "Answer is: " << ans << "\n";

}