#ifndef TURINGSIMULATOR
#define TURINGSIMULATOR

#include <string>
#include <unordered_set>
#include <unordered_map>
#include <deque>
#include <vector>
#include <map>
using namespace std;

struct transFuction{
    string state;
    vector<string> input;
    vector<string> output;
    vector<string> direction;
    string next_state;
};

struct tapeElement{
    int index;
    string symbol;
    bool isNegative;
};

class TuringSimulator{

private:
    bool verbose;
    string turing_path, input_str;

    map<int, string> syntaxTable;
    const string ERR = "==================== ERR ====================\n";
    const string END = "==================== END ====================\n";
    const string RUN = "==================== RUN ====================\n";
    const string inter_symbol = "---------------------------------------------\n";
    unordered_set<string> states;
    unordered_set<string> inputSymbols;
    unordered_set<string> tapeSymbols;
    string start_state;
    string blank_symbol;
    unordered_set<string> finalStates;
    unordered_map<string, vector<pair<string, transFuction>>> transFuctions;
    int ntapes;

    vector<int> heads;
    vector<deque<tapeElement>> tapes;
    bool isAccept;
    int step;
    string state;
    

private:
    void throwError(string error);
    vector<string> split(const string& str, const char delim);
    string trim(const string& str);
    string unTrim(const string& str, int len);
    string removeCR(const string& str);
    void readTuringMachine();
    bool canMatch(string str, string pattern);
    pair<int, int> findBound(int tape_index);
    void printLog();
public:
    TuringSimulator(bool verbose, string file_name, string input_str);
    
    void run();
};

#endif