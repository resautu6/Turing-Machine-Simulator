#include <iostream>
#include <string>
#include <memory>

#include "TuringSimulator.h"
using namespace std;


string help_info = "usage: turing [-v|--verbose] [-h|--help] <tm> <input>\n";
int main(int argc, char* argv[]){
    if(argc == 1) {
        cout << help_info;
        return 0;
    }
    string sarg = argv[0];
    bool is_verbose = false;
    string file_name, input_str;
    for(int i = 1; i < argc; i++) {
        sarg = argv[i];
        if(sarg[0] == '-'){
            if(sarg == "-h" || sarg == "--help") {
                cout << help_info;
                return 0;
            } else if(sarg == "-v" || sarg == "--verbose") {
                is_verbose = true;
            } else{
                cerr << "Unknown command: " << sarg << '\n';
                return 1;
            }
            continue;
        }
        if(file_name.empty()) {
            file_name = sarg;
        } else if(input_str.empty()) {
            input_str = sarg;
        } else{
            cerr << "Unknown command: " << sarg << '\n';
            return 1;
        }
    }
    if(file_name.empty()) {
        cerr << help_info;
        return 1;
    }
    unique_ptr<TuringSimulator> turingSimulator(new TuringSimulator(is_verbose, file_name, input_str));

    turingSimulator->run();

    return 0;
}