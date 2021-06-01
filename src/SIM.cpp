#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <cassert>
#include <stdlib.h>

using namespace std;

vector<string> ReadFile(string path_){
    ifstream file_;
    file_.open(path_);
    assert(file_);
    vector<string> code_;
    if(file_){
        string line;
        while (getline(file_,line))
        {
            code_.push_back(line);
        }
    }
    // cout << code_.size();
    return code_;    
}


int main(int argc,char** argv){
    int argument = strtol(argv[1], NULL,10);
    if( argument == 1){
        vector<string> code_to_compress;
        code_to_compress = ReadFile("original.txt");
        
        
        // for (string i: code_to_compress){
        //     cout << i  << "\n";
        // }  
    }
    if( argument == 2){
        vector<string> code_to_decompress;
        code_to_decompress = ReadFile("compressed.txt");
        
        // for (string i: code_to_decompress){
        //     cout << i  << "\n";
        // }
        
        // cout << "this is argc 2" << "\n";
    }
    return 0;
}