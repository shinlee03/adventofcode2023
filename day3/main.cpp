#include <iostream>
#include <fstream>
#include <string>
#include <vector>

using std::string;
using std::vector;

struct c{
    int x;
    int y;

    c(int a, int b): x{a}, y{b}{}
};

bool isdigit(char c){
    return c>='0' && c<='9';
}

void mark(c c, vector<vector<bool>>&check, vector<vector<char>>&m){
    //can even move diagonally!
    int dx[8] = {1,0,-1,0,1,1,-1,-1};
    int dy[8] = {0,1,0,-1,1,-1,1,-1};

    for(int i=0;i<8;i++){
        int x = c.x + dx[i]; 
        int y = c.y + dy[i];

        if(x<0 || y<0 || x>=m.size() || y>=m[0].size())continue;
        check[x][y] = true;
    }
}

long long getGear(c a, vector<vector<char>>&m){
    int dx[8] = {1,0,-1,0,1,1,-1,-1};
    int dy[8] = {0,1,0,-1,1,-1,1,-1};

    vector<c> nums;

    for(int i=0;i<8;i++){
        int x = a.x + dx[i]; 
        int y = a.y + dy[i];

        if(x<0 || y<0 || x>=m.size() || y>=m[0].size())continue;

        if(isdigit(m[x][y])){
            nums.push_back({x,y});
        }
    }

    vector<vector<bool>>check(m.size(), vector<bool>(m[0].size(), false));
    vector<string>v;
    for(c n: nums){
        if(check[n.x][n.y])continue;
        check[n.x][n.y] = true;

        //check both ways! :)
        int x = n.x;
        int y = n.y;

        while(y>=0 && isdigit(m[x][y])){
            check[x][y] = true;
            y--;
        }
        y++;

        string temp = "";
        while(y<m[0].size() && isdigit(m[x][y])){
            check[x][y] = true;
            temp += m[x][y];
            y++;
        }

        v.push_back(temp);
    }

    if(v.size() == 2){
        return std::stoi(v[0]) * std::stoi(v[1]);
        
    }else return 0;
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

    vector<vector<char>>m;
    vector<c>symbols;
    string s;
    while(getline(f, s)){
        m.push_back({});
        
        for(char c: s){
            if(!isdigit(c) && c!='.'){
                symbols.push_back({int(m.size()-1), int(m.rbegin()->size())});
            }
            m.rbegin()->push_back(c);
        }

    }
    vector<vector<bool>>check(m.size(), vector<bool>(m[0].size(), false));
    long long ans = 0;

    if(!strcmp(argv[2], "1")){
        for(c c: symbols){
            mark(c, check, m);
        }

        for(int i=0;i<m.size();i++){

            bool checked = false;
            string temp = "";
            for(int j=0;j<m[i].size();j++){
                if(isdigit(m[i][j])){
                    checked = checked || check[i][j];
                    temp += m[i][j];
                }
                else {
                    if(checked && !temp.empty()){
                        ans += std::stoi(temp);
                    }
                    temp.clear();
                    checked = false;
                }
            }
            if(checked && !temp.empty()){
                ans += std::stoi(temp);
            }
        }
    }else {
        for(c c: symbols){
            if(m[c.x][c.y] == '*'){
                ans += getGear(c, m);
            }
        }
    }

    std::cout << "answer is " << ans << std::endl;

}

