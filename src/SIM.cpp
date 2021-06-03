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
#include <map>
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
bool cmp(pair<string, int> &a,
         pair<string, int> &b)
{
    return a.second > b.second;
}


/*
Sorts the dictionary in the descending order of the frequency of occurance;
if two dictionary entries have the same occuarance rate give priority to the entry that came first
inputs:
    frequency_set: map<string,int> which contains the frequency of each instruction
    vec_ : vector<string> contains the order of occurance of each unique instruction
    dictionary_siz: int which tells the size of the dictionary to be compressed
*/
vector<string> Sort_dictionary(const map<string, int> frequency_set, vector<string> &vec_ , int &dictionary_size)
{ 
    vector<pair<string, int>> sorted_dictionary;
    vector<pair<string, int>> similar_frequency;
    
    for (auto &it : frequency_set)
    {   //sort the dictionary in the descending order
        sorted_dictionary.push_back(it);
    }
    sort(sorted_dictionary.begin(), sorted_dictionary.end(), cmp);


    int size_ = sorted_dictionary.size();  
    for (int i = 0; i < size_; i++)
    {   // get the similar frequency dictionary values to another vector
        if (sorted_dictionary[i].second == sorted_dictionary[i + 1].second || sorted_dictionary[i].second == sorted_dictionary[i - 1].second)
        {
            similar_frequency.push_back(sorted_dictionary[i]);
        }
    }

    std::map<int , unsigned int> h;          // get the number of occurances of each frequency
    for (auto const & x : sorted_dictionary)
    {
        ++h[x.second];
    }

    vector<string> dictionary;  // define the dictionary to be returned
    for(auto &it : sorted_dictionary){
        int count_ = h[it.second];  // it.first is the instruction code being checked
        if (count_> 1){
            int k = it.second ;
            int added = 0;
            for(int i = 0; i < count_; i++){
                for (string j: vec_){

                    if(auto it1 = any_of(similar_frequency.begin(), similar_frequency.end(), [&j](const pair<string, int>& similar_frequency)
                        { return similar_frequency.first == j; })){
                            int index_ = 0;
                            for(const auto& pair : similar_frequency) {
                                if(pair.first == j ) {
                                break;
                                }
                                index_++;
                            }
                            int temp_freq = similar_frequency[index_].second;
                            if(temp_freq == k){
                                dictionary.push_back(j);
                                remove(vec_.begin(), vec_.end(), j); //remove j from the vec_
                                added = 1;
                                break;
                            }
                            else{
                                continue;
                            }
                            }
                            
                        }
            }
        }
        else{
            dictionary.push_back(it.first);
        }
    }
    dictionary.resize(dictionary_size); // get the size of the desired dictionary

    return dictionary;
}

/* 
Function to get the frequency of the codes
inputs:
    vec: a reference vector<string> of the code to be compressed
outputs:
    sorted_dictionary: vector<pair<string, int> which contains the frequency for each unique instruction and sorted in the descending order
    insertion_order: contains the order of appearance of each unique instruction 
*/
vector<string> GetFrequency(const vector<string> &vec)
{
    map<string, int> frequency_set;
    vector<string> insertion_order;
    vector<string> dictionary;
    int dictionary_size = 16;

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
    dictionary = Sort_dictionary(frequency_set, insertion_order, dictionary_size);

    return dictionary;
}

int main(int argc, char **argv)
{

    int argument = strtol(argv[1], NULL, 10);

    if (argument == 0)
    {
        vector<string> code_to_compress, dictionary;
        
        code_to_compress = ReadFile("original.txt");  // read the file to be compressed

        dictionary = GetFrequency(code_to_compress);  // get the dictionary
    }

    if (argument == 2)
    {
        vector<string> code_to_decompress;

        code_to_decompress = ReadFile("compressed.txt");

    }
    return 0;
}