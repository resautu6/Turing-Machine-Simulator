#include <iostream>
#include <fstream>
#include <sstream>
#include <math.h>

#include "TuringSimulator.h"
using namespace std;

TuringSimulator::TuringSimulator(bool verbose, string file_name, string input_str){
    this->verbose = verbose;
    this->turing_path = file_name;
    this->input_str = input_str;
    syntaxTable[0] = "Q";
    syntaxTable[1] = "S";
    syntaxTable[2] = "G";
    syntaxTable[3] = "q0";
    syntaxTable[4] = "B";
    syntaxTable[5] = "F";
    syntaxTable[6] = "N";

    readTuringMachine();

    for(int i = 0; i < input_str.size(); i++){
        if(!inputSymbols.count(input_str.substr(i, 1))){
            if(verbose){
                if(verbose) cerr << "Input: " << input_str << '\n';
                cerr << ERR;
                cerr << "error: Symbol \"" << input_str.substr(i, 1) << "\" in input is not defined in the set of input symbols\n";
                cerr << "Input: " << input_str << '\n';
                string pos = "";
                for(int j = 0; j < string("Input: ").length() + i; j++) pos.push_back(' ');
                pos += "^\n";
                cerr << pos;
                cerr << END;
            }
            else cerr << "illegal input string\n";
            exit(1);
        } 
    }

    heads = vector<int>(ntapes, 0);
    tapes = vector<deque<tapeElement>>(ntapes, deque<tapeElement>());
    isAccept = false;
    step = 0;
    state = start_state;
    for(int i = 0; i < ntapes; i++){
        heads[i] = 0;
        if(i == 0 && !input_str.empty()) continue;
        tapes[i].push_back({0, blank_symbol, false});
        
    }
    for(int i = 0; i < input_str.size(); i++){
        tapes[0].push_back({i, input_str.substr(i, 1), false});
    }
}

void TuringSimulator:: throwError(string error){
    cerr << error << '\n';
    exit(1);
}

vector<string> TuringSimulator::split(const string& str, const char delim){
    vector<string> res;
    if("" == str) return res;
    stringstream ss(str);
    string token;
    while(getline(ss, token, delim)){
        if(!token.empty())
            res.push_back(token);
    }
 
    return res;
}

string TuringSimulator::trim(const string& str){
    string::size_type pos = str.find_first_not_of(' ');
    if(pos == string::npos) return str;
    string::size_type pos2 = str.find_last_not_of(' ');
    if(pos2 != string::npos)
        return str.substr(pos, pos2 - pos + 1);
    return str.substr(pos);
}

string TuringSimulator::unTrim(const string& str, int len){
    string res(str);
    for(int i = res.length(); i < len; i++){
        res.push_back(' ');
    }
    return res;
}

string TuringSimulator::removeCR(const string& str){
    string res;
    for(auto& ch : str){
        if(ch != '\r') res.push_back(ch);
    }
    return res;
}

void TuringSimulator::readTuringMachine(){
    fstream fin(turing_path);
    if(!fin.is_open()) throwError("Cannot open file: " + turing_path);
    string line;
    int tag = 0;
    while(getline(fin, line)){
        line = trim(line);
        line = removeCR(line);
        if(line.empty() || line == "\r") continue;
        auto pos = line.find(';');
        if(pos != string::npos){
            line = line.substr(0, pos);
        }
        if(line.empty()) continue;
        auto tokens = split(line, ' ');
        if(tag <= 6) {
            if(line[0] != '#') throwError("Syntax error.");
            else if(tokens.size() != 3) throwError("Syntax error.");
            else if(tokens[1] != "=") throwError("Syntax error.");
            else if((tag == 0 || tag == 1 || tag == 2 || tag == 5) && (tokens[2][0] != '{' || tokens[2][tokens[2].size() - 1] != '}')) throwError("Syntax error.");
            else if(tokens[0].substr(1) != syntaxTable[tag]) throwError("Syntax error.");

            switch (tag)
            {
            case 3:
                start_state = tokens[2];
                break;
            case 4:
                blank_symbol = tokens[2];
                if(blank_symbol.size() != 1) throwError("Syntax error.");
                break;
            case 6:{
                try{
                    ntapes = stoi(tokens[2]);
                } catch(exception& e){
                    throwError("Syntax error.");
                }
            }
            case 0:
            case 1:
            case 2:
            case 5:{
                auto tmp = tokens[2].substr(1, tokens[2].size() - 2);
                auto tmp_tokens = split(tmp, ',');
                for(auto& token : tmp_tokens){
                    if(token.empty()) throwError("Syntax error.");
                    switch (tag)
                    {
                    case 0:
                        states.insert(token);
                        break;
                    case 1:
                        if(token.size() != 1 || token[0] == ' ' || token[0] == ',' || token[0] == ';' || token[0] == '*' || token[0] == '_' || token[0] == '{' || token[0] == '}') throwError("Syntax error.");
                        inputSymbols.insert(token);
                        break;
                    case 2:
                        if(token.size() != 1 || token[0] == ' ' || token[0] == ',' || token[0] == ';' || token[0] == '*' || token[0] == '{' || token[0] == '}') throwError("Syntax error.");
                        tapeSymbols.insert(token);
                        break;
                    case 5:
                        finalStates.insert(token);
                        break;
                    default:
                        break;
                    }
                }
                break;
            }
            default:
                throwError("Syntax error.");
                break;
            }
        }
        else{
            if(tokens.size() != 5) throwError("Syntax error.");
            else if(!states.count(tokens[0]) || !states.count(tokens[4])) throwError("Syntax error.");
            else if(tokens[1].size() != ntapes || tokens[2].size() != ntapes || tokens[3].size() != ntapes) throwError("Syntax error.");

            transFuction tf;
            for(int i = 0; i < ntapes; i++){
                string tmp = tokens[1].substr(i, 1);
                
                if(!tapeSymbols.count(tmp) && tmp != "*") throwError("Syntax error.");
                tf.input.push_back(tmp);

                tmp = tokens[2].substr(i, 1);
                if(!tapeSymbols.count(tmp) && tmp != "*") throwError("Syntax error.");
                else if(tmp == "*" && tokens[1].substr(i, 1) != "*") throwError("Syntax error.");
                tf.output.push_back(tmp);


                tmp = tokens[3].substr(i, 1);
                if(tmp != "l" && tmp != "r" && tmp != "*") throwError("Syntax error.");
                tf.direction.push_back(tmp);
            }

            tf.state = tokens[0];
            tf.next_state = tokens[4];
            transFuctions[tokens[0]].push_back({tokens[1], tf});
        }
        tag++;
    }
}

bool TuringSimulator::canMatch(string str, string pattern){
    if(str.size() != pattern.size()) return false;
    for(int i = 0; i < str.size(); i++){
        if(pattern[i] != '*' && str[i] != pattern[i] || pattern[i] == '*' && str[i] == blank_symbol[0]) return false;
    }
    return true;
}

pair<int, int> TuringSimulator::findBound(int tape_index){
    int begin_out = -1, end_out = -1;
    for(int i = 0; i < tapes[tape_index].size(); i++){
        if(tapes[tape_index][i].symbol != blank_symbol){
            begin_out = i;
            break;
        }
    }
    for(int i = tapes[tape_index].size() - 1; i >= 0; i--){
        if(tapes[tape_index][i].symbol != blank_symbol){
            end_out = i;
            break;
        }
    }
    return {begin_out, end_out};
}

void TuringSimulator:: printLog(){
    int max_len = string("Index" + to_string(ntapes - 1)).length();
    cout << unTrim("Step", max_len) << " : " << step << '\n';
    cout << unTrim("State", max_len) << " : " << state << '\n';
    cout << unTrim("Acc", max_len) << " : " << (isAccept ? "Yes" : "No") << '\n';
    for(int i = 0; i < ntapes; i++) {
        auto tape_bound = findBound(i);
        if(tape_bound.first == -1) tape_bound = {heads[i], heads[i]};
        tape_bound.first = min(tape_bound.first, heads[i]);
        tape_bound.second = max(tape_bound.second, heads[i]);

        string indexs = "", tape_symbols = "", head_pos = "";
        for(int j = tape_bound.first; j <= tape_bound.second; j++){
            int cur_index = abs(tapes[i][j].index);
            string index_tmp = to_string(cur_index);
            int cur_maxlen = index_tmp.length();
            indexs.append(index_tmp).append(" ");
            tape_symbols.append(unTrim(tapes[i][j].symbol, cur_maxlen)).append(" ");
            if(j <= heads[i]) head_pos.append((heads[i] == j ? "^" : unTrim("", cur_maxlen))).append(" ");
        }
        indexs.pop_back(); tape_symbols.pop_back(); head_pos.pop_back();
        cout << unTrim("Index" + to_string(i), max_len).append(" : ").append(indexs) << '\n';
        cout << unTrim("Tape" + to_string(i), max_len).append(" : ").append(tape_symbols) << '\n';
        cout << unTrim("Head" + to_string(i), max_len).append(" : ").append(head_pos) << '\n';
    }
    cout << inter_symbol;   
}

void TuringSimulator::run(){
    if(verbose){
        cout << "Input: " << input_str << '\n';
        cout << RUN;
    } 

    while(true) {
        if(finalStates.count(state)){
            isAccept = true;
        }
        if(verbose) printLog();

        string cur_symbols = "";
        for(int i = 0; i < ntapes; i++){
            cur_symbols += tapes[i][heads[i]].symbol;
        }
        if(!transFuctions.count(state)){
            break;
        }

        string nex_symbols = "";
        transFuction tf;
        tf.state = "";
        for(auto &ele : transFuctions[state]){
            if(canMatch(cur_symbols, ele.first)){
                tf = ele.second;
                break;
            }
        }
        if(tf.state.empty()) break;
        state = tf.next_state;
        for(int i = 0; i < ntapes; i++){
            if(tf.output[i] != "*"){
                tapes[i][heads[i]].symbol = tf.output[i];
            }
            int cur_index = tapes[i][heads[i]].index;
            if(tf.direction[i] == "l"){
                cur_index--;
                heads[i]--;
                if(heads[i] < 0){
                    tapes[i].push_front({cur_index, blank_symbol, cur_index < 0});
                    heads[i] = 0;
                }
            } else if(tf.direction[i] == "r"){
                heads[i]++;
                if(heads[i] >= tapes[i].size()){
                    cur_index++;
                    tapes[i].push_back({cur_index, blank_symbol, cur_index < 0});
                }
            }
        }
        
        step++;

    }
    if(isAccept){
        if(verbose) cout << "ACCEPTED\n";
        else cout << "(ACCEPTED) ";
    } else{
        if(verbose) cout << "UNACCEPTED\n";
        else cout << "(UNACCEPTED) ";
    }
    if(verbose) cout << "Result: ";

    auto tape_bound = findBound(0);
    int begin_out = tape_bound.first, end_out = tape_bound.second;
    
    for(int i = begin_out; i <= end_out; i++){
        if(verbose) cout << tapes[0][i].symbol;
        else cout << tapes[0][i].symbol;
    }
    if(verbose) cout << endl << END;
    else cout << endl;
}