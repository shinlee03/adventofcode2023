#include <iostream>
#include <fstream>

#include <string>
#include <vector>

#include "helper.cpp"

using std::string;
using std::vector;

#include <unordered_map>

int recursively_solve(parsedData d, int idx){
    
    if(idx == d.phrase.size()){
        if(verify(d.phrase, d.rules, 0, 0, false)){
            return 1;
        } else return 0;
    }

    if(d.phrase[idx] != '?') {
        int res = recursively_solve(d, idx+1);
        return res;
    }

    //is a question mark
    d.phrase[idx] = '.';
    int ret = 0;
    ret = ret + recursively_solve(d, idx+1);
    d.phrase[idx] = '#';
    ret = ret + recursively_solve(d, idx+1);

    if(ret == 3){
        d.phrase[idx] = '?';
        std::cout << d.phrase << " is " << 3 << "\n";
    }
    return ret;
}


long long calculate(parsedData d, int count, bool take_sharp, bool take_dot){
    if(count == 5){
        if(take_sharp || take_dot){
            printf("(%d,%d) = %d\n", count, take_sharp||take_dot ?1:0, 0);
            return 0;
        }
        //printf("result is 1");
        
        printf("(%d,%d) = %d\n", count, take_sharp||take_dot?1:0, 1);
        return 1;
    }

    parsedData dd = d;
    
    if(take_sharp){
        //I CAN ONLY ADD SHARP TO MYSELF IF THE PREVIOUS ONE DIDNT END IN SHARP.
        //BECAUSE PADDING WAS GIVEN TO ME, I CAN DO IT AS LONG AS LAST CHAR OF 

    }

    long long temp = 0;

    // I take the question mark
    
    // CASE 1: ADD DOT.
    long long res1 =  recursively_solve(dd, 0);

    long long res2=  calculate(d, count+1, false);
    temp+=res1*res2;

    // Next one takes the question mark
    dd.phrase = (take_q? "?" : "") + d.phrase;
    long long res3= recursively_solve(dd, 0);
    long long res4= calculate(d, count+1, true);
    temp+=res3*res4;

    printf("(%d, %d) = %lld * (%d, %d) = %lld + %lld * (%d, %d) = %lld = %lld\n", count, take_q?1:0, res1, count+1, 0, res2, res3, count+1, 1, res4, temp);

    
    printf("(%d,%d) = %lld\n", count, take_q?1:0, temp);
    return temp;
}

int main(int argc, char** argv){
    std::ifstream f = std::ifstream(argv[1]);

    string line;

    //test_validity(); //DEBUG

    long long ans = 0;
    int idx = 0;


    /*
    //part 1:
    while(getline(f, line)){
        int res = recursively_solve(parse(line), 0);

        std::cout << "BATCH " << idx++ << "/ 1000\n";
        //std::cout << "GOT " << res << "\n";
        ans += res;
    }
    */
 
   //DEBUG
   parsedData d = {{1,1,3},"????.###?"};
   if(recursively_solve(d, 0) != 2){
    std::cerr << "Expected 2, but got " << recursively_solve(d, 0) << "\n";
    exit(1);
   }

   while(getline(f, line)){
        long long res = calculate(parse(line), 0, false);

        std::cout << "BATCH " << idx++ << "/ 1000\n";
        
        std::cout << "got: " << res << "\n";
        ans += res;
         
    }


    std::cout << "ANSWER: " << ans << std::endl;

}