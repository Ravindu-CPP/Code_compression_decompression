/*

*/
#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <cassert>
#include <stdlib.h>
#include <map>
#include <algorithm>
#include <iterator>

using namespace std;

// Function to read the file and return it as a vector
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




// Function to get the frequency of the codes
map<string, int> GetFrequency(vector<string> &vec, int &size_){

    map<string, int> frequency_set;
    



    return frequency_set;
}



int main(int argc,char** argv){

    int argument = strtol(argv[1], NULL,10);



    if( argument == 0){
        vector<string> code_to_compress, unique_occurance;
        map<string, int> frequency_list;
        int original_size;

      

        code_to_compress = ReadFile("original.txt");
        unique_occurance = code_to_compress;
        original_size = code_to_compress.size();
        cout << unique_occurance.size() << " " << "\n";
        cout << &unique_occurance << " " << "\n";
        cout << &code_to_compress << " " << "\n";

        vector<int>::iterator ip;

        ip = unique(unique_occurance.begin(), unique_occurance.end());
        unique_occurance.resize(distance(unique_occurance.begin(), ip));

        frequency_list = GetFrequency(code_to_compress, original_size);
        // cout << frequency_list.size();
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