#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <unordered_map>

using std::string;
using std::vector;

bool isdigit(char c){
    return c>='0' && c<='9';
}
vector<vector<int>> parse(string s){
    string temp = "";
    vector<int>ansnum;
    vector<int>mynum;

    s = s.substr(s.find(':')+1);

    bool transition = false;
    for(char c: s){
        if(isdigit(c)){
            temp += c;
        }
        else if(c != '|' && !temp.empty()){
            if(!transition){
                ansnum.push_back(std::stoi(temp));
            }
            else {
                mynum.push_back(std::stoi(temp));
            }
            temp.clear();
        }
        else if(c == '|'){
            transition = true;
        }
    }
    mynum.push_back(std::stoi(temp));

    return {ansnum, mynum};
}

long long getScore(vector<vector<int>> res){
    int score = 0;
    for(int i: res[1]){
        if(std::find(res[0].begin(), res[0].end(), i) != res[0].end()){
            score = score==0? 1 : score*2;
        }
    }
    return score;
}

long long getCount(vector<vector<int>> res){
    int score = 0;
    for(int i: res[1]){
        if(std::find(res[0].begin(), res[0].end(), i) != res[0].end()){
            score++;
        }
    }
    return score;
}

long long dp(int n, std::unordered_map<int, bool>& check, std::unordered_map<int, long long>& memo, vector<string>&cards){
    if(check[n])return memo[n];
    check[n] = true;

    int score = getCount(parse(cards[n]));
    long long ret = 1;
    for(int i=n+1; i<=std::min(n+score, (int)(cards.size()-1)); i++){
        ret += dp(i, check, memo, cards);
    }

    memo[n] = ret;
    return ret;

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

    long long ans = 0;
    string s;

    if(!strcmp(argv[2], "1")){
        while(getline(f, s)){
            ans += getScore(parse(s));
        }
    }else {
        vector<string>cards;
        while(getline(f,s)){
            cards.push_back(s);
        }
        std::unordered_map<int, bool>memocheck;
        std::unordered_map<int, long long>memo;

        for(int i=0;i<cards.size();i++){
            ans += dp(i, memocheck, memo, cards);
        }
    }

    std::cout << "answer is " << ans << std::endl;

}