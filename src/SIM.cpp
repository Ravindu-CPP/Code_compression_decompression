/*
Index number: 170006n
Assignment 5: Code compression and decompression
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
inputs:
    path_: string which denotes the path to the file
output:
    code_: vector<string> which contains string or 32 char long
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


*/
bool cmp(pair<string, int>& a,
         pair<string, int>& b)
{
    return a.second > b.second;
}



auto Sort_dictionary(const unordered_map<string,int> frequency_set, const vector<string> &vec_){
    vector< pair<string, int> > sorted_dictionary;
    vector< pair<string, int> > similar_frequency;

    for (auto& it : frequency_set) {
        sorted_dictionary.push_back(it);
    }
    sort(sorted_dictionary.begin(), sorted_dictionary.end(), cmp);

    for (auto &it : sorted_dictionary) {
  
        cout << it.first  << " " << it.second << endl;
    }
    
    cout << "print the similar frequency" << endl;

    int size_ = sorted_dictionary.size();
    for(int i = 0 ; i < size_ ; i++){
        if (sorted_dictionary[i].second == sorted_dictionary[i+1].second || sorted_dictionary[i].second == sorted_dictionary[i-1].second)
        {
            similar_frequency.push_back(sorted_dictionary[i]);
            // remove_copy(sorted_dictionary.begin(), sorted_dictionary.end(), similar_frequency.begin(), sorted_dictionary[i]);

        } 
    }
    int similar_size = similar_frequency.size();

    for(int i = 0 ; i < similar_size ; i++){
        remove(sorted_dictionary.begin(),sorted_dictionary.end(),similar_frequency[i]);
    }

    for (auto &it : similar_frequency) {
  
        cout << it.first  << " " << it.second << endl;
    }

    sorted_dictionary.resize(sorted_dictionary.size()-similar_frequency.size());

    // for( int i = 0; i < )

    return sorted_dictionary;
}

/* 
Function to get the frequency of the codes
inputs:
    vec: a reference vector<string> of the code to be compressed
outputs:
    sorted_dictionary: vector<pair<string, int> which contains the frequency for each unique instruction and sorted in the descending order
    insertion_order: contains the order of appearance of each unique instruction 
*/
auto GetFrequency(const vector<string> &vec)
{
    unordered_map<string, int> frequency_set;
    vector<string> insertion_order;
    vector<pair<string, int> > sorted_dictionary;


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
    sorted_dictionary = Sort_dictionary(frequency_set,insertion_order);

    // for (auto& it : frequency_set) {
    //     sorted_dictionary.push_back(it);
    // }
    // sort(sorted_dictionary.begin(), sorted_dictionary.end(), cmp);

    // for (auto &it : sorted_dictionary) {
  
    //     cout << it.first  << " " << it.second << endl;
    // }
    // int si = sorted_dictionary.size();
    
    
    // for
    // for (auto& i : sorted_dictionary){
    //     int val << i.second;
    //     cout << i.first << endl;
    //     for (int j ; j < si; j++)
    // }
    // cout << "after swaping" << endl;
    // for (auto &it : sorted_dictionary) {
        
    //     cout << it.first  << " " << it.second << endl;
    // }
    // cout << "Frequency" << "\n";
    // for (auto const &pair: frequency_set) {
    //     std::cout  << pair.first << ": " << pair.second << "\n";
    // }
    return sorted_dictionary, insertion_order;
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