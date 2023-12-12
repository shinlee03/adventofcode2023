#include <iostream>
#include <fstream>
#include <cassert>
#include <queue>
std::vector<std::vector<int>>distance;
std::vector<std::vector<char>>board;
std::vector<std::vector<bool>>visited;

struct state{
    int x;
    int y;
    int dist;
};

class comparator {
public:
    bool operator()(state a, state b) {
        return a.dist > b.dist;
    }
};

int main(int argc, char** argv) {


    auto f = std::ifstream(argv[1]);
    std::string line;

    
    int idx = 0;

    int x, y;

    while(getline(f, line)){
        board.push_back(std::vector<char>());

        for(char c: line){
            
            board[idx].push_back(c);
            if(c == 'S'){
                x = idx;
                y = board[idx].size()-1;
            }
        }

        

        idx++;
    }

    //DEBUG



    visited.resize(idx, std::vector<bool>(board[0].size(), false));
    distance.resize(idx,std::vector<int>(board[0].size(), -1e9));

    //std::cout << "DEBUG: x: " << x << ", y: " << y << "\n";

    assert(x != -1e9);
    assert(y != -1e9);

    std::priority_queue<state, std::vector<state>, comparator>pq;

    int dx[4] = {1,0,-1,0};
    int dy[4] = {0,1,0,-1};

    for(int i=0;i<4;i++)pq.push({x+dx[i],y+dy[i],1});

    distance[x][y] = 0;
    visited[x][y] = true;

    while(!pq.empty()){
        state s = pq.top();
        pq.pop();
        if(visited[s.x][s.y])continue;
        visited[s.x][s.y] = true;
        distance[s.x][s.y] = s.dist;
        if(board[s.x][s.y] == ',')continue;

        if(board[s.x][s.y] == '|'){
            pq.push({s.x+1, s.y, s.dist+1});
            pq.push({s.x-1, s.y, s.dist+1});
        }
        else if(board[s.x][s.y] == '-'){
            pq.push({s.x, s.y+1, s.dist+1});
            pq.push({s.x, s.y-1, s.dist+1});
        }
        else if(board[s.x][s.y] == 'F'){
            pq.push({s.x,s.y+1,s.dist+1});
            pq.push({s.x+1,s.y,s.dist+1});
        }
        else if(board[s.x][s.y] == '7'){
            pq.push({s.x,s.y-1,s.dist+1});
            pq.push({s.x+1,s.y,s.dist+1});
        }
        else if(board[s.x][s.y] == 'J'){
            pq.push({s.x-1,s.y,s.dist+1});
            pq.push({s.x,s.y-1,s.dist+1});
        }   
        else if(board[s.x][s.y] == 'L'){
            pq.push({s.x,s.y+1,s.dist+1});
            pq.push({s.x-1,s.y,s.dist+1});
        }

    }

    
    int ans = -1e9;

    for(auto v: distance){
        for(int j: v){
            ans = std::max(ans, j);
        }
    }

    std::cout << ans << std::endl;
}

