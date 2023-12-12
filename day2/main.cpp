#include <iostream>
#include <fstream>
#include <string>
#include <cassert>
#include <vector>

using std::ifstream;
using std::string;


class Game {
public:
    Game(int id): _id{id}, _red{12}, _green{13}, _blue{14}, _feasible{true}, _min_red{0}, _min_green{0}, _min_blue{0} {

    }
    
    class instance{
        public: int r=0, g=0, b=0;
    };

    void push_game(instance i){
        if(i.r > _red || i.g > _green || i.b > _blue){
            _feasible = false;
        }

        _min_red = std::max(_min_red, i.r);
        _min_green = std::max(_min_green, i.g);
        _min_blue = std::max(_min_blue, i.b);
    }

    bool is_feasible() {
        return _feasible;
    }

    int get_id() {
        return _id;
    }

    long long get_power(){
        return _min_red * _min_green * _min_blue;
    }

    
private:
    int _id;
    int _red;
    int _green;
    int _blue;
    bool _feasible;
    int _min_red;
    int _min_green;
    int _min_blue;

};

//helper function: split string input with colon or semicolon
std::vector<string> split_by_colon_or_semi(string s){
    std::vector<string> ret;
    std::string temp = "";
    for(char c: s){
        if(c == ':' || c== ';'){
            ret.push_back(temp);
            temp.clear();
        }else if(c!=' ')temp+=c;
    }
    ret.push_back(temp);

    return ret;
}

Game::instance parseGame(string s){
    Game::instance i;
    //split by comma first
    std::string temp = "";

    for(char c: s){
        if(c == ','){
            //parse, find space.
            std::string num = "";
            std::string color = "";
            for(char cc: temp){
                if(cc >= '0' && cc <= '9'){
                    num += cc;
                }
                else color+= cc;
            }

            if(color == "red"){
                i.r = std::stoi(num);
            }else if(color == "blue"){
                i.b = std::stoi(num);
            }else if(color == "green"){
                i.g = std::stoi(num);
            }else {
                std::cerr << "INVALID COLOR\n";
                exit(1);
            }

            temp.clear();
        }
        else temp+=c;
    }

    std::string num = "";
    std::string color = "";
    for(char cc: temp){
        if(cc >= '0' && cc <= '9'){
            num += cc;
        }
        else color+= cc;
    }

    if(color == "red"){
        i.r = std::stoi(num);
    }else if(color == "blue"){
        i.b = std::stoi(num);
    }else if(color == "green"){
        i.g = std::stoi(num);
    }else {
        std::cerr << "INVALID COLOR\n";
        exit(1);
    }

    return i;
}

int main(int argc, char** argv) {

    auto f = ifstream(argv[1]);
    int ans = 0;
    long long ans2 = 0;
    string line;
    while(getline(f, line)){
        auto res = split_by_colon_or_semi(line);
        string gameid = res[0].substr(4);
        Game g(std::stoi(gameid));

        for(int i=1;i<res.size();i++){
            g.push_game(parseGame(res[i]));
        }
        if(g.is_feasible()){
            ans += g.get_id();
        }
        ans2 += g.get_power();
    }
    
    std::cout << "answer is " << ans << std::endl;
    std::cout << "answer to part 2 is " << ans2 << std::endl;

}