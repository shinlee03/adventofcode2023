#include <iostream>
#include <fstream>

#include <vector>
#include <string>

#include <queue>

using std::vector;
using std::cout;
using std::string;

struct state{
    int x;
    int y;
    long long dist;
};

class cmp {
    public: bool operator() (state a, state b){
        return a.dist > b.dist;
    }
};

const long long expansion_constant = 1000000;

void expand_universe(vector<string>&v){

    //row expansion first.

    string to_insert = "";
    for(int i=0;i<v[0].size();i++)to_insert += 'e';

    for(int i=0;i<v.size();i++){
        if(v[i].find("#") == string::npos){
            v[i] = to_insert;
        }
    }

    //expand columns
    for(int i=0;i<v[0].size();i++){
        
        bool no_universe = true;

        for(int j=0;j<v.size();j++){
            if(v[j][i] == '#'){
                no_universe = false;
            }
        }

        if(no_universe){
            //insert column.
            for(int j=0;j<v.size();j++){
                v[j][i] = 'e';
            }
        }

    }


}

vector<vector<long long>>global_dist;
void bfs(vector<string>&board, int x, int y){


    std::priority_queue<state, vector<state>, cmp>pq;

    pq.push({x, y, 0});

    vector<vector<bool>>visit(board.size(), vector<bool>(board[0].size(), false));

    int dx[4] = {0,1,0,-1};
    int dy[4] = {1,0,-1,0};

    while(!pq.empty()){
        state n = pq.top();
        pq.pop();
        if(visit[n.x][n.y]) continue;
        visit[n.x][n.y] = true;
        global_dist[n.x][n.y] = n.dist;

        for(int i=0;i<4;i++){
            if(n.x+dx[i]<0 || n.y+dy[i]<0 || n.x+dx[i] >= board.size() || n.y+dy[i] >= board[0].size()){
                continue;
            }
            pq.push({n.x+dx[i], n.y+dy[i], n.dist+(board[n.x+dx[i]][n.y+dy[i]]=='e' ? expansion_constant : 1)});
        }
        
    }
}

int main(int argc, char** argv){

    auto f = std::ifstream(argv[1]);
    vector<string>v;

    string line;
    while(getline(f, line)){
        v.push_back(line);
    }  

    expand_universe(v);

    global_dist.resize(v.size(), vector<long long>(v[0].size(), 0));

    long long ans = 0;

    

    vector<state>galaxies;

    for(int i=0;i<v.size();i++){
        for(int j=0;j<v[0].size();j++){
            if(v[i][j] == '#'){
                galaxies.push_back({i,j,0});
                //std::cout << galaxies.size();
            }//else std::cout << v[i][j];
        }
        //std::cout << "\n";
    }


    

    for(int i=0;i<galaxies.size();i++){
        global_dist.resize(v.size(), vector<long long>(v[0].size(), 0));
        bfs(v, galaxies[i].x, galaxies[i].y);
        printf("BATCH: %d/%ld\n", i, galaxies.size());
        for(int j=i+1;j<galaxies.size();j++){
            ans += abs(global_dist[galaxies[i].x][galaxies[i].y] - global_dist[galaxies[j].x][galaxies[j].y]);
            //printf("(%d, %d) = %d\n",i+1,j+1,abs(global_dist[galaxies[i].x][galaxies[i].y] - global_dist[galaxies[j].x][galaxies[j].y]));
        }
    }
    
    std::cout << "The answer is: " << ans << "\n";
    return 0;

}