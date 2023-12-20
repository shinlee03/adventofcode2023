#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <queue>
#include <unordered_map>
#include <utility>
#include <cassert>

using std::ifstream;
using std::string;
using std::vector;

std::unordered_map<int, bool>invalidgroup;
std::unordered_map<int, int>groupcount;


std::pair<char, int> parse(string s){
    string temp = "";
    
    for(char c: s.substr(2)){
        if(c == ' '){
            return {s[0], std::stoi(temp)};
        }
        else temp+=c;
    }

    std::cerr << "INVALID STRING";
    exit(1);
    return {s[0], -1e9}; //make compiler happy :)
}

std::pair<char, long long> parsehex(string s){
    auto c = *s.rbegin();
    return {c=='0'?'R':c=='1'?'D':c=='2'?'L':'U', std::stoll(s.substr(1,s.size()-2), nullptr, 16)};
}

std::pair<char, long long> parse_parttwo(string s){
    string temp = "";
    bool pass_num = false;

    for(char c: s.substr(2)){
        if(c== ' '){
            if(!pass_num){
                pass_num = true;
                temp.clear();
            }else {
                std::cerr << "INCORRECT INPUT\n";
            }
        }else temp+=c;
    }
    return parsehex(temp.substr(1, temp.size()-1));
    return {'E', -1e9};
}

int dx[4] = {1,0,-1,0};
int dy[4] = {0,1,0,-1};

void dfs(int x, int y, int group, vector<vector<char>>& b, vector<vector<int>>& groups){
    if(groups[x][y] != -1){
        if(group!= groups[x][y]){
            invalidgroup[group] = true;
            invalidgroup[groups[x][y]] = true;
        }
        return;
    }

    groups[x][y] = group;
    groupcount[group]++;

    if(x==0 || y==0 || x==b.size()-1 || y==b[0].size()-1){
        invalidgroup[group] = true;
    }
    
    for(int i=0;i<4;i++){
        int nx = x+dx[i];
        int ny = y+dy[i];

        if(nx<0 || ny<0 || nx >= b.size() || ny>=b[0].size() || b[nx][ny]=='#')continue;
        dfs(nx,ny, group, b, groups);
    }
}


int main(int argc, char** argv){

    auto f= ifstream(argv[1]);

    string s;
    bool flag = !strcmp(argv[2], "1");
    long long ans = 0;

    if(flag){
        vector<std::pair<char, long long>>v;
        while(getline(f,s)){
            v.push_back(parse(s));
        }

        //first round, find out max x and max y so we can construct fix-sized board

        int x = 0;
        int y = 0;

        int mx = 0;
        int my = 0;

        int mmx = 1e9;
        int mmy = 1e9;

        for(auto p: v){


            x += (p.first=='D'? p.second : p.first=='U'? -1 * p.second : 0);
            y += (p.first == 'R'? p.second : p.first =='L'? -1*p.second : 0);

            mx = std::max(x, mx);
            my = std::max(y,my);
            mmx = std::min(x,mmx);
            mmy = std::min(y,mmy);
        }

        const int paddingx = -1*mmx;
        const int paddingy = -1*mmy;

        //construct board
        std::vector<std::vector<char>>b(mx+paddingx+3, std::vector<char>(my+paddingy+3, '.'));

        //'fill' in accordingly

        x=0;
        y=0;
        
        for(auto p: v){

            int nx = x + (p.first=='D'? p.second : p.first=='U'? -1 * p.second : 0);
            int ny = y +  (p.first == 'R'? p.second : p.first =='L'? -1*p.second : 0);

            if(x==nx){
                for(int i=std::min(y,ny);i<=std::max(y,ny);i++){
                    b[x+paddingx][i+paddingy] = '#';
                }
            }
            else for(int i=std::min(x,nx);i<=std::max(x,nx);i++){
                b[i+paddingx][y+paddingy] = '#';
            }

            x = nx; y = ny;
        }

        //debug: check filled lava

        for(int i=0;i<b.size();i++){
            for(int j=0;j<b[0].size();j++){
                //std::cout << b[i+paddingx][j+paddingy];
                if(b[i][j] == '#')ans++;
            }
            //std::cout << "\n";
        }

        //then fill in whatever is "surrounded" :)
        //we can use dfs to do this

        std::vector<std::vector<int>>group(b.size(), std::vector<int>(b[0].size(), -1));
        

        int groups = 1;
        for(int i=0;i<b.size();i++){
            for(int j=0;j<b[0].size();j++){
                if(b[i][j] != '#' && group[i][j] == -1){
                    dfs(i,j,groups++,b, group);
                }
            }
        }

        for(int i=0;i<groups;i++){
            if(!invalidgroup[i]){
                //std::cout << "Group " << i << " has " << groupcount[i] << " more blocks :)\n";
                ans += groupcount[i];
            }
        }

    }else {

        //because numbers are so big, we have to think line-wise
        
        


        while(getline(f, s)){

        }
    }
    
    std::cout << "answer is " << ans << std::endl;
    
    
}