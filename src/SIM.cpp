/*

*/
#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <cassert>
#include <stdlib.h>
#include <unordered_map>
#include <algorithm>

using namespace std;

/*

Function to read the file and return it as a vector

*/
vector<string> ReadFile(string path_)
{
    ifstream file_;
    file_.open(path_);
    assert(file_);
    vector<string> code_;
    if (file_)
    {
        string line;
        while (getline(file_, line))
        {
            code_.push_back(line);
        }
    }
    // cout << code_.size();
    return code_;
}

/* 

Function to get the frequency of the codes

*/
auto GetFrequency(vector<string> &vec)
{
    unordered_map<string, int> frequency_set;
    vector<string> insertion_order;
    for (const string &line_ : vec)
    {

        frequency_set[line_]++;
        if (find(insertion_order.begin(), insertion_order.end(), line_) != insertion_order.end())
        {
            continue;
        }
        else
        {
            insertion_order.push_back(line_);
        }
    }

    // cout << "Frequency" << "\n";
    // for (auto const &pair: frequency_set) {
    //     std::cout  << pair.first << ": " << pair.second << "\n";
    // }
    return frequency_set, insertion_order;
}

int main(int argc, char **argv)
{

    int argument = strtol(argv[1], NULL, 10);

    if (argument == 0)
    {
        vector<string> code_to_compress, insersion_order;
        unordered_map<string, int> frequency_list;

        code_to_compress = ReadFile("original.txt");

        frequency_list, insersion_order = GetFrequency(code_to_compress);
        // cout << frequency_list.size();
        // for (string i: code_to_compress){
        //     cout << i  << "\n";
        // }
    }

    if (argument == 2)
    {
        vector<string> code_to_decompress;

        code_to_decompress = ReadFile("compressed.txt");

        // for (string i: code_to_decompress){
        //     cout << i  << "\n";
        // }

        // cout << "this is argc 2" << "\n";
    }
    return 0;
}