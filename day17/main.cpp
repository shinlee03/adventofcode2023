#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <queue>
#include <unordered_map>
#include <boost/functional/hash.hpp>
#include <utility>
#include <cassert>

using std::ifstream;
using std::string;
using std::vector;

enum dir{left, down, right, up};
const char glossary[4] = {'<', 'v', '>', '^'};

struct state{
    int x;
    int y;
    dir d;
    int count; //can only take up to 3 consecutive direction steps
    int loss; 

    bool operator==(const state &s) const {
        return x == s.x && y == s.y && s.d == d;
    }

    state(int a, int b, int c, int d, int e): x{a}, y{b}, d{dir(c)}, count{d}, loss{e}{}
};

template<>
struct std::hash<state>{
    std::size_t operator()(const state& s) const {
        std::size_t seed = 0;
        boost::hash_combine(seed, s.x);
        boost::hash_combine(seed, s.y);
        boost::hash_combine(seed, int(s.d));
        //how about cnt?
        boost::hash_combine(seed, s.count);
        return seed;
    }
};

class Comparator{
    public: bool operator()(state a, state b){
        return a.loss > b.loss;
    }
};

inline bool in_bound(std::pair<int,int>c, int sx, int sy){
    return c.first<sx && c.first>=0 && c.second<sy && c.second>=0;
}

std::pair<int,int> progress(int x, int y, dir d){
    return {x+(d==dir::down? 1 : d==dir::up? -1 : 0), y+(d==dir::left? -1 : d==dir::right? 1 : 0)};
}

dir turn_right(dir d){
    //left -> up, right -> down, up-> right, down->left
    return dir((d-1+4)%4);
}

dir turn_left(dir d){
    //left -> down, right->up, up->left, down->right
    return dir((d+1)%4);
}

int main(int argc, char** argv){

    ifstream f = ifstream(argv[1]);
    string s;

    bool debug = false;
    debug = !strcmp(argv[2], "3");

    vector<vector<int>>b;

    while(getline(f,s)){
        b.push_back({});
        for(char c: s){
            b.rbegin()->push_back(c-'0');
        }
    }

    std::priority_queue<state, vector<state>, Comparator>pq;

    pq.push({0,0,dir::right, 1,0});
    pq.push({0,0,dir::down, 1,0});

    int ans = 1e9;

    std::unordered_map<state, bool>m;

    if(debug){
        std::cout << "Progress Test Started\n";


        assert(progress(3,3,dir::left) == std::make_pair(3,2));
        assert(progress(3,3,dir::right) == std::make_pair(3,4));
        assert(progress(3,3,dir::up) == std::make_pair(2,3));
        assert(progress(3,3,dir::down) == std::make_pair(4,3));

        std::cout << "Progress test Passed\n\n";

        std::cout << "Turn Right Test Started\n";

        assert(turn_right(dir::left) == dir::up);
        assert(turn_right(dir::right) == dir::down);
        assert(turn_right(dir::down) == dir::left);
        assert(turn_right(dir::up) == dir::right);

        std::cout << "Turn Right Test Passed\n\n";

        std::cout << "Turn Left Test started\n";

        assert(turn_left(dir::left) == dir::down);
        assert(turn_left(dir::right) == dir::up);
        assert(turn_left(dir::down) == dir::right);
        assert(turn_left(dir::up) == dir::left);

        std::cout << "Turn Right Test Passed\n\n";

        std::priority_queue<state, vector<state>, Comparator>pq;

        std::cout << "Starting PQ test...\n";

        pq.push({0,0,dir::right, 0,0});
        pq.push({0,0,dir::left, 2, 3});

        assert(pq.top().loss == 0);

        pq.push({2,3,dir::left, 2, 0});
        pq.push({7,2,dir::left, 2, 8});
        pq.push({1,3,dir::left, 2, 7});
        pq.push({2,6,dir::left, 2, 2});

        assert(pq.top().loss == 0);
        pq.pop();
        assert(pq.top().loss == 0);
        pq.pop();
        assert(pq.top().loss == 2);

        std::cout << "Passed PQ test\n\n";

        std::unordered_map<state, bool>test;

        std::cout << "Running TEST input...\n";
    }
    
    if(!strcmp(argv[2], "1")){
        while(!pq.empty()){
            state now = pq.top();
            pq.pop();


            if(m[now]) continue;
            m[now] = true;


            if(now.x == b.size()-1 && now.y == b[0].size()-1){
                //reached end!
                ans = now.loss;
                break;
            }

            //check if it can keep moving forward!
            if(now.count < 3 && in_bound(progress(now.x,now.y,now.d), b.size(), b[0].size())){
                auto p = progress(now.x, now.y, now.d);
                pq.push({p.first, p.second, now.d, now.count+1, now.loss + b[p.first][p.second]});
                if(debug) std::cout << now.x << " " << now.y << " " << now.loss << " " << now.count  << " " << glossary[now.d] << p.first << " " << p.second << " " << now.loss+b[p.first][p.second] << " " << now.loss+1  << " "  << "\n";
            }

            //turn right
            if(in_bound(progress(now.x, now.y, turn_right(now.d)), b.size(), b[0].size())){
                auto p = progress(now.x, now.y, turn_right(now.d));
                pq.push({p.first, p.second, turn_right(now.d), 1, now.loss+b[p.first][p.second]});
                if(debug) std::cout << now.x << " " << now.y << " " << now.loss << " " << now.count  << " " << glossary[now.d] << p.first << " " << p.second << " " << now.loss+b[p.first][p.second] << " " << 1  << " "  << "\n";
            }

            //turn left
            if(in_bound(progress(now.x, now.y, turn_left(now.d)), b.size(), b[0].size())){
                auto p = progress(now.x, now.y, turn_left(now.d));
                pq.push({p.first, p.second, turn_left(now.d), 1, now.loss+b[p.first][p.second]});
                if(debug) std::cout << now.x << " " << now.y << " " << now.loss << " " << now.count  << " " << glossary[now.d] << p.first << " " << p.second << " " << now.loss+b[p.first][p.second] << " " << 1  << " "  << "\n";
            }
            
        }
    }else {
        while(!pq.empty()){
            state now = pq.top();
            pq.pop();

            if(m[now])continue;
            m[now] = true;

            if(now.x == b.size()-1 && now.y == b[0].size()-1){
                ans = now.loss;
                break;
            }

            //we need at least 4 consecutive steps in order to steer
            if(now.count < 4){
                auto p = progress(now.x, now.y, now.d);
                if(!in_bound(p, b.size(), b[0].size()))continue;
                pq.push({p.first, p.second, now.d, now.count+1, now.loss + b[p.first][p.second]});
                if(debug) std::cout << now.x << " " << now.y << " " << now.loss << " " << now.count  << " " << glossary[now.d] << p.first << " " << p.second << " " << now.loss+b[p.first][p.second] << " " << now.loss+1  << " "  << "\n";
            }else {
                //can steer, or go straight (as long as count<10)
                //check if it can keep moving forward!
                if(now.count < 10 && in_bound(progress(now.x,now.y,now.d), b.size(), b[0].size())){
                    auto p = progress(now.x, now.y, now.d);
                    pq.push({p.first, p.second, now.d, now.count+1, now.loss + b[p.first][p.second]});
                    if(debug) std::cout << now.x << " " << now.y << " " << now.loss << " " << now.count  << " " << glossary[now.d] << p.first << " " << p.second << " " << now.loss+b[p.first][p.second] << " " << now.loss+1  << " "  << "\n";
                }

                //turn right
                if(in_bound(progress(now.x, now.y, turn_right(now.d)), b.size(), b[0].size())){
                    auto p = progress(now.x, now.y, turn_right(now.d));
                    pq.push({p.first, p.second, turn_right(now.d), 1, now.loss+b[p.first][p.second]});
                    if(debug) std::cout << now.x << " " << now.y << " " << now.loss << " " << now.count  << " " << glossary[now.d] << p.first << " " << p.second << " " << now.loss+b[p.first][p.second] << " " << 1  << " "  << "\n";
                }

                //turn left
                if(in_bound(progress(now.x, now.y, turn_left(now.d)), b.size(), b[0].size())){
                    auto p = progress(now.x, now.y, turn_left(now.d));
                    pq.push({p.first, p.second, turn_left(now.d), 1, now.loss+b[p.first][p.second]});
                    if(debug) std::cout << now.x << " " << now.y << " " << now.loss << " " << now.count  << " " << glossary[now.d] << p.first << " " << p.second << " " << now.loss+b[p.first][p.second] << " " << 1  << " "  << "\n";
                }
            }
        }
    }
    


    if(debug){
        std::cout << "Got: " << ans << "\n"; 

        assert(ans == 55); //PROBLEM WRONG?
        std::cout << "Passed TEST Input\n\n";

        std::cout << "Passed all tests!\n";

        return 0;
    }
    std::cout << "answer is " << ans << std::endl;
}