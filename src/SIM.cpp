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
#include <bitset>
#include <cmath>

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
vector<string> Sort_dictionary(const map<string, int> frequency_set, vector<string> &vec_, int &dictionary_size)
{
    vector<pair<string, int>> sorted_dictionary;
    vector<pair<string, int>> similar_frequency;

    for (auto &it : frequency_set)
    { //sort the dictionary in the descending order
        sorted_dictionary.push_back(it);
    }
    sort(sorted_dictionary.begin(), sorted_dictionary.end(), cmp);

    int size_ = sorted_dictionary.size();
    for (int i = 0; i < size_; i++)
    { // get the similar frequency dictionary values to another vector
        if (sorted_dictionary[i].second == sorted_dictionary[i + 1].second || sorted_dictionary[i].second == sorted_dictionary[i - 1].second)
        {
            similar_frequency.push_back(sorted_dictionary[i]);
        }
    }

    std::map<int, unsigned int> h; // get the number of occurances of each frequency
    for (auto const &x : sorted_dictionary)
    {
        ++h[x.second];
    }

    vector<string> dictionary; // define the dictionary to be returned
    for (auto &it : sorted_dictionary)
    {
        int count_ = h[it.second]; // it.first is the instruction code being checked
        if (count_ > 1)
        {
            int k = it.second;
            int added = 0;
            for (int i = 0; i < count_; i++)
            {
                for (string j : vec_)
                {

                    if (auto it1 = any_of(similar_frequency.begin(), similar_frequency.end(), [&j](const pair<string, int> &similar_frequency)
                                          { return similar_frequency.first == j; }))
                    {
                        int index_ = 0;
                        for (const auto &pair : similar_frequency)
                        {
                            if (pair.first == j)
                            {
                                break;
                            }
                            index_++;
                        }
                        int temp_freq = similar_frequency[index_].second;
                        if (temp_freq == k)
                        {
                            dictionary.push_back(j);
                            remove(vec_.begin(), vec_.end(), j); //remove j from the vec_
                            added = 1;
                            break;
                        }
                        else
                        {
                            continue;
                        }
                    }
                }
            }
        }
        else
        {
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

int Get_index(vector<string> v, string K)
{
    auto it = find(v.begin(), v.end(), K);
    int index;
    if (it != v.end())
    {
        index = it - v.begin();
    }
    else
    {
        index = -1;
    }
    return index;
}

// string In_dictionary(const string &instruction,const vector<string> &dictionary){
//     string compressed_instuction;

//     return compressed_instuction;
// }

std::string To_binary(int n, int len_)
{
    std::string r;
    while (n != 0)
    {
        r = (n % 2 == 0 ? "0" : "1") + r;
        n /= 2;
    }
    while (r.length() < len_)
    {
        r = "0" + r;
    }
    return r;
}

string operator*(string a, unsigned int b)
{
    string output = "";
    while (b--)
    {
        output += a;
    }
    return output;
}

/*
Generates the bitmask patterns to be used for bitmask based compression
intput: 
    vector<string>: type of the bitmask
outputs:
    bitmask: vector<string> of all possible bitmasks
*/
vector<string> Get_bitmask(const vector<string> &vec_)
{
    string str = "0";
    vector<string> bitmask;
    string temp_holder;
    for (string pattern : vec_)
    {
        for (int i = 0; i < 29; i++)
        {
            temp_holder = (str * i) + pattern + (str * (28 - i));
            bitmask.push_back(temp_holder);
        }
    }
    return bitmask;
}

bitset<32> to_bitset(std::string s)
{
    auto binary = [](char c)
    { return c == '0' || c == '1'; };
    auto not_binary = [binary](char c)
    { return !binary(c); };

    s.erase(std::remove_if(begin(s), end(s), not_binary), end(s));

    return std::bitset<32>(s);
}

string to_string(std::bitset<32> bs)
{
    return bs.to_string();
}

vector<string> Compression_algo(vector<string> &code_to_commpress, vector<string> dictionary)
{
    vector<string> compressed_code;
    bool instruction_repeated = false;
    int rle_count = 0;
    string previous_instruction;

    string not_compressed_header = "000";
    string rle_header = "001";
    string bitmask_header = "010";
    string one_bit_header = "011";
    string two_bit_cons_header = "100";
    string four_bit_cons_header = "101";
    string two_bit_anywhere_header = "110";
    string direct_header = "111";

    vector<string> possible_bitmasks = {"1000",
                                        "1100",
                                        "1010",
                                        "1110",
                                        "1001",
                                        "1101",
                                        "1011",
                                        "1111"};
    vector<string> bitmasks = Get_bitmask(possible_bitmasks);
    // cout << bitmasks.size() << endl;
    // for (string i : bitmasks){
    //     cout << i << endl;
    // }
    
    for (const string &instru_ : code_to_commpress)
    {   bool by_dictionary = false;
        bool by_rle = false;
        instruction_repeated = previous_instruction == instru_;
        // cout << instruction_repeated << "  instruction  " << instru_;
        // cout << instru_ << "   " << previous_instruction << endl;
        if (instruction_repeated && rle_count < 8)
        { // r/e
            // cout << " by rle" << endl;
            string rle_position = To_binary(rle_count, 3);
            previous_instruction = instru_;
            // send instruction to rle algo
            // cout << rle_position << endl;
            string to_push = "001" + rle_position;
            compressed_code.push_back(to_push);
            ++rle_count;
            by_rle = true;
        }
        else if (!instruction_repeated)
        { //dictionary
            int position = Get_index(dictionary, instru_);
            if (position >= 0)
            {
                rle_count = 0;
                string dictionary_index = To_binary(position, 4);
                string to_push = "111" + dictionary_index;
                compressed_code.push_back(to_push);
                previous_instruction = instru_;
                by_dictionary = true;
                // cout << " by dic" << endl;
            }
        }
        if (!instruction_repeated && !by_dictionary && !by_rle)
        { // bitmask
            for (string &bit : bitmasks)
            {   
                
                string xor_instru = to_string(to_bitset(bit) ^ to_bitset(instru_));
                int position = Get_index(dictionary, instru_);
                if (position >= 0){
                // {   cout << "I was here too" << endl; 
                    rle_count = 0;
                    int temp_location = Get_index(bitmasks,bit);
                    int bitmask_location_temp = temp_location%29;
                    string bitmask_location = To_binary(bitmask_location_temp,5);
                    int bit_type_idx = floor(bitmask_location_temp/29);
                    string bitmask_type = To_binary(bit_type_idx,4);
                    string dictionary_index = To_binary(position, 4);
                    string to_push2 = "010" + bitmask_location + bitmask_type + dictionary_index;
                    compressed_code.push_back(to_push2);
                    previous_instruction = instru_;
                    // cout << " by bitmask" << endl;
                }
            }
        }
        // else
        // {   
        //     previous_instruction = instru_;
        //     rle_count = 0;
        // }
        // cout << endl;
    }
    for(string i : compressed_code){
        cout << i << endl;
    }
    return compressed_code;
}

int main(int argc, char **argv)
{

    int argument = strtol(argv[1], NULL, 10);

    if (argument == 0)
    {
        vector<string> code_to_compress, dictionary, compressed_code;

        code_to_compress = ReadFile("original.txt"); // read the file to be compressed

        dictionary = GetFrequency(code_to_compress); // get the dictionary

        compressed_code = Compression_algo(code_to_compress, dictionary);
    }

    if (argument == 2)
    {
        vector<string> code_to_decompress;

        code_to_decompress = ReadFile("compressed.txt");
    }
    return 0;
}