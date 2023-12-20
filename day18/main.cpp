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

struct line{
    long long sx;
    long long ex;
    long long sy;
    long long ey;
};

bool xcmp(line a, line b){
    return a.sx == b.sx? a.sy < b.sy : a.sx < b.sx;
}
bool ycmp(line a, line b){
    return a.sy == b.sy? a.sx < b.sx : a.sy < b.sy;
}

template <bool T>
class lineGroups{
public:
    vector<line>lines;
    void push_back(const line& l){
        if(_push_back(l)) _sort();
    }
    
    void debug_print_all_lines(){
        std::cout << (T? "X-lines" : "Y-lines") << "\n";
        for(auto l: lines){
            std::cout << (T? l.sx : l.sy) << "~" << (T? l.ex : l.ey) << (T? " y= " : " x= ") << (T? l.sy : l.sx) << "\n";
        }
    }
private:
    bool _push_back(const line& l){
            //find if there are mergable lines.
            // a line is mergable iff:
            //x-line: ex1 >= sx2 || ex2 >= sx1. 
            //y-line: ey1 >= sy2 || ey2 >= sy1
            for(int i=0;i<lines.size();i++){
                if(T) {
                    if(lines[i].sy != l.sy)continue; //needs to be on same place!
                    else if(lines[i].ex >= l.sx && lines[i].sx <= l.sx){
                        std::cout << "at y= " << lines[i].sy << "...\n";
                        //update lines[i]'s endpoint to l.ex
                        std::cout << "Merging " << lines[i].sx << "~" << lines[i].ex << " with " << l.sx << "~" << l.ex << " to ";
                        lines[i].ex = l.ex; //extended
                        std::cout << lines[i].sx << "~" << lines[i].ex << "\n";
                        return false;
                    }
                    else if(l.ex >= lines[i].sx && l.sx <= lines[i].sx){
                        std::cout << "at y= " << lines[i].sy << "...\n";
                        //update lines[i]'s start point to l.sx
                        std::cout << "Merging " << lines[i].sx<<  "~" << lines[i].ex << " with " << l.sx << "~" << l.ex << " to ";
                        lines[i].sx = l.sx;
                        std::cout << lines[i].sx << "~" << lines[i].ex << "\n";
                        return false;
                    }
                }
                else {
                    if(lines[i].sx != l.sx)continue;
                    else if(lines[i].ey >= l.sy && lines[i].sy <= l.sy){
                        std::cout << "at x= " << lines[i].sx << "...\n";
                        //update lines[i]'s endpoint to l.ey
                        std::cout << "Merging " << lines[i].sy << "~" << lines[i].ey << " with " << l.sy << "~" << l.ey << " to ";
                        lines[i].ey = l.ey; //extended
                        std::cout << lines[i].sy << "~" << lines[i].ey << "\n";
                        return false;
                    }
                    else if(l.ey >= lines[i].sy && l.sy <= lines[i].sy){
                        std::cout << "at x= " << lines[i].sx << "...\n";
                        std::cout << "Merging " << lines[i].sy  << "~" << lines[i].ey << " with " << l.sy << "~" << l.ey << " to ";
                        //update lines[i]'s start point to l.sy
                        lines[i].sy = l.sy;
                        std::cout << lines[i].sy << "~" << lines[i].ey << "\n";
                        return false;
                    }
                }
            }
            lines.push_back(l);
            return true;
        }
    void _sort(){
        std::sort(lines.begin(), lines.end(), T?xcmp:ycmp);
    }
};

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

        //because numbers are so big, we have to think line-wise!

        //construct line objects, with starting x value and ending y value.

        //Each lines get a certain amount of sharps

        //IF the lines meet each other, they must be merged! (or else it will be wonky and double count stuff)
        //Notice that all lines are either in x direction or y direction.

        //For each lines, if there exists four pairs of line l1, l2, l3, l4 such that they surround an area, we can add them.

        //we pick two from x-dir, two from y-dir.
        //So time complexity is actually O((N/2*N/2) * (N/2*N/2)) = (N^4/16). Which isn't too bad, right...?
        //right...?

        lineGroups<true>x_lines;
        lineGroups<false>y_lines;
        vector<std::pair<char, long long>>v;

        while(getline(f,s)){
            v.push_back(parse(s));
        }

        long long x = 0;
        long long y = 0;
        for(auto p: v){

            line l = {x,0,y,0};

            x += (p.first=='D'? p.second : p.first=='U'? -1 * p.second : 0);
            y += (p.first == 'R'? p.second : p.first =='L'? -1*p.second : 0);

            l.ex = x;
            l.ey = y;

            if(l.ex==l.sx){
                if(l.sy > l.ey) std::swap(l.sy, l.ey);
                y_lines.push_back(l);
            }
            else{
                if(l.sx > l.ex) std::swap(l.sx, l.ex);
                x_lines.push_back(l);
            } 
        }

        //line processing check (debug)

        x_lines.debug_print_all_lines();
        y_lines.debug_print_all_lines();

        //recursively call and see if we can "enclose" an area!

        std::vector<bool>xcheck(x_lines.lines.size(), false);
        std::vector<bool>ycheck(y_lines.lines.size(), false);

        for(int i=0;i<x_lines.lines.size();i++){
            if(!xcheck[i]) ans += solve(x_lines, y_lines, xcheck, ycheck);
        }
    }


    
    std::cout << "answer is " << ans << std::endl;
    
    
}