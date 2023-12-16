#include <iostream>
#include <fstream>
#include <string>
#include <vector>

using std::string;
using std::vector;

#include <unordered_map>
#include <utility>

bool DEBUG = false;

enum direction{
    up,
    down,
    right,
    left
};

enum grid {
    empty = '.',
    horizontal_splitters = '-',
    vertical_splitters = '|',
    mirror_leftup = '/',
    mirror_leftdown = '\\'
};

struct light{
    int x;
    int y;
    direction dir;  

    light(int a, int b, direction d): x{a}, y{b}, dir{d} {}
    bool operator==(const light &l) const {
        return x == l.x && y==l.y && dir ==l.dir;
    }

    bool in_range(int bx, int by){
        return x>=0 && y>=0 && x<bx && y<by;
    }

    
};

//for debug only
std::ostream& operator<<(std::ostream &os, light const &l){
        return os << "(" <<l.x << ", " << l.y << ", " << (l.dir==direction::up? "u" :  l.dir==direction::down? "d" : l.dir==direction::left? "l" : "r") << ")\n";
}

// hash for unordered map
template<>
struct std::hash<light>{
    std::size_t operator()(const light& l) const {
        //use cppref's idea: hash combine :)
        std::size_t h1 = std::hash<int>{}(l.x);
        std::size_t h2 = std::hash<int>{}(l.y);
        std::size_t h3 = std::hash<int>{}(l.dir);

        return ((h1 ^ (h2 << 1)) >> 1) ^ (h3 << 1);
    }
};

std::unordered_map<light, bool>check;

std::pair<bool, light> move_up(light light, vector<vector<grid>>&m){
    light.x--;  
    return {light.in_range(m.size(), m[0].size()), light};
}

std::pair<bool, light> move_down(light light, vector<vector<grid>>&m){
    light.x++;  
    return {light.in_range(m.size(), m[0].size()), light};
}

std::pair<bool, light> move_right(light light, vector<vector<grid>>&m){
    light.y++;  
    return {light.in_range(m.size(), m[0].size()), light};
}

std::pair<bool, light> move_left(light light, vector<vector<grid>>&m){
    light.y--;  
    return {light.in_range(m.size(), m[0].size()), light};
}

std::pair<bool, light> move_dir(light light, direction dir, vector<vector<grid>>&m){
    light.dir = dir;
    return dir==direction::up? move_up(light, m) : dir==direction::down? move_down(light, m) : dir==direction::right? move_right(light, m) : move_left(light,m);
}

string direction_string(direction d){
    return d==direction::up? "u" : d==direction::down? "d" : d==direction::right? "r" : "l";
}

void move(light light, vector<vector<grid>>&m){
    if(check[light]) return;
    if(DEBUG) std::cout << light;
    check[light] = true;

    switch(m[light.x][light.y]){
        case grid::empty: {
            auto next = move_dir(light, light.dir, m);
            if(!next.first)return;
            move(next.second, m);
            return;
        }
        case grid::horizontal_splitters: {
            //if light.dir is left or right, same dir
            //if light.dir if up or down, then split to left & right

            if(light.dir == direction::left || light.dir == direction::right){
                auto next = move_dir(light, light.dir, m);
                if(next.first)move(next.second, m);
                return;
            }
            else {
                auto n1 = move_dir(light, direction::right, m);
                auto n2 = move_dir(light, direction::left, m);

                if(n1.first)move(n1.second, m);
                if(n2.first)move(n2.second, m);
                return;
            }
        }
        case grid::vertical_splitters: {
            //if light.dir is up or down, same dir
            //if light.dir if left or right, then split to up & down

            if(light.dir == direction::up || light.dir == direction::down){
                auto next = move_dir(light, light.dir, m);
                if(next.first)move(next.second, m);
                return;
            }
            else {
                auto n1 = move_dir(light, direction::up, m);
                auto n2 = move_dir(light, direction::down, m);

                if(n1.first)move(n1.second, m);
                if(n2.first)move(n2.second, m);
                return;
            }
        }
        case grid::mirror_leftdown: {
            auto next = light.dir==direction::left? direction::up : light.dir==direction::right? direction::down : light.dir==direction::up? direction::left : direction::right;
            auto n = move_dir(light, next, m);
            if(n.first)move(n.second, m);
            return;
        }
        case grid::mirror_leftup: {
            auto next = light.dir==direction::left? direction::down : light.dir==direction::right? direction::up : light.dir==direction::up? direction::right : direction::left;
            auto n = move_dir(light, next, m);
            if(n.first)move(n.second, m);
            return;
        }
        default: {
            std::cerr << "ERROR: Supplied grid " << char(m[light.x][light.y]) <<  " is NOT valid.\n";
            exit(1); 
        }
    }
}

long long count_energized(int szx, int szy){
    long long ret = 0;
    for(int i=0;i<szx;i++){
        for(int j=0;j<szy;j++){
            if(check[light(i,j,direction::up)] || check[light(i,j,direction::down)] || check[light(i,j,direction::right)] || check[light(i,j,direction::left)]){
                if(DEBUG)std::cout << "#";
                ret++;
            }else if(DEBUG)std::cout << ".";
        }
        if(DEBUG)std::cout << "\n";
    }
    return ret;
}

int main(int argc, char** argv) {
    if(argc < 2){
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
    

    while(getline(f,s)){
        m.push_back({});
        for(char c: s){
            m.rbegin()->push_back(grid(c));
            if(DEBUG) std::cout << "inserting " << c << "\n";
        }
    }
    
    //DEBUG = true;
    long long ans = 0;

    if(!strcmp(argv[2], "1")){
        move(light(0,0,direction::right), m);

        ans = count_energized(m.size(), m[0].size());
    }else if(!strcmp(argv[2], "2")) {
        
        for(int i=0;i<m.size();i++){
            //up edge --> to down
            check.clear();
            move(light(0,i,direction::down), m);
            ans = std::max(ans, count_energized(m.size(), m[0].size()));
            
            //down edge --> to up
            check.clear();
            move(light(m.size()-1, i, direction::up), m);
            ans = std::max(ans, count_energized(m.size(), m[0].size()));
            
            //left edge --> to right
            check.clear();
            move(light(i, 0, direction::right), m);
            ans = std::max(ans, count_energized(m.size(), m[0].size()));

            //right edge --> to left
            check.clear();
            move(light(i, m[0].size()-1, direction::left), m);
            ans = std::max(ans, count_energized(m.size(), m[0].size()));
            
        }

    }else {
        std::cerr << "ERROR: Invalid task option\n";
        exit(1);
    }
    

    std::cout << "SUCCESS!" << std::endl;

    std::cout << "ANSWER: " << ans << "\n";
}