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

/*
Function to get the loction of a string in a vector
intputs:
    vec_to_index: vector containing the string
    strin_g: string to be searched
outpus:
    index: index of the string in the vector if found
             -1 if not found
*/
int Get_index(vector<string> vec_to_index, string strin_g)
{
    auto it = find(vec_to_index.begin(), vec_to_index.end(), strin_g);
    int index;
    if (it != vec_to_index.end())
    {
        index = it - vec_to_index.begin();
    }
    else
    {
        index = -1;
    }
    return index;
}

/*
Function to convert a decimal to binary
inputs:
    decimal: the decimal value to be converted
    len_: the expected length of the returned binary
output:
    binary__: the converted decimal to binary
*/
string To_binary(int decimal, int len_)
{
    std::string binary__;
    while (decimal != 0)
    {
        binary__ = (decimal % 2 == 0 ? "0" : "1") + binary__;
        decimal /= 2;
    }
    while (binary__.length() < len_)
    {
        binary__ = "0" + binary__;
    }
    return binary__;
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
Adds the bits to each other
inputs:
    s: reference string containing the bits stream
outputs:
    sum_: the total sum of each bit
*/
int AddBitStream(string &s)
{
    int sum_ = 0;
    for (char &i : s)
    {
        int k = i - '0';
        sum_ = sum_ + k;
    }
    return sum_;
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

/*
converts a string bit-stream into bit-stream
*/
bitset<32> to_bitset(std::string s)
{
    auto binary = [](char c)
    { return c == '0' || c == '1'; };
    auto not_binary = [binary](char c)
    { return !binary(c); };

    s.erase(std::remove_if(begin(s), end(s), not_binary), end(s));

    return std::bitset<32>(s);
}

/*
converts bit-stream into a string bit-stream
*/
string to_string(std::bitset<32> bs)
{
    return bs.to_string();
}

/*
Function to get the starting point of the bit mismatch
input:
    instruction_xored: a string which contains the instruction to be checked
output:
    loc: string which denotes the location by 5 bits
*/
string GetMisLocation(const string &instruction_xored)
{
    string loc;
    int i = 0;
    for (; i < instruction_xored.length(); i++)
    {
        if (instruction_xored[i] == '0')
        {
            continue;
        }
        else
        {
            break;
        }
    }
    loc = To_binary(i, 5);
    return loc;
}

/*
Function to check whether 2bits are in consective order
input: 
    str_2: string of bits 
output:
    consec_: boolean 
*/
bool Consecutive(const string &str_2)
{
    int cons_it = 0;
    bool consec_ = false;
    while (cons_it < str_2.length())
    {
        ++cons_it;
        if (str_2[cons_it] == '0')
        {
            continue;
        }
        else if (str_2[cons_it] == '1' && str_2[cons_it + 1] == '1')
        {
            consec_ = true;
            break;
        }
        else
        {
            consec_ = false;
            break;
        }
    }
    return consec_;
}

bool Consecutive_four(const string &str_4){
    bool found_ = false;
    return found_;
}

/*
Function to get the location of 2 "1" bits at different locations in a bit-stream
inputs:
    string_to_check: string which contains a bitstream
outputs:
    loc_ations: vector<int> contains the two locations of the "1" bits 
*/
vector<int> Get2LocationBits(const string &string_to_check)
{
    int first_loc = 0;
    int second_loc = 0;
    for (int po_s = 0; po_s < string_to_check.length(); po_s++)
    {
        if (string_to_check[po_s] == '0')
        {
            continue;
        }
        else if (first_loc == 0 && string_to_check[po_s] == '1')
        {
            first_loc = po_s;
        }
        else if (first_loc != 0 && string_to_check[po_s] == '1')
        {
            second_loc = po_s;
        }
    }
    vector<int> loc_cations{first_loc, second_loc};
    return loc_cations;
}

/*
Function to compress the instructions
inputs:
    code_to_compress: vector<string> contains the vector of instructions to be compressed
    dictionary: vector<string> contains the dictionary used for the compression
output:
    compressed_code: vector<string> contains a vector of compressed codes
*/
vector<string> Compression_algo(const vector<string> &code_to_compress, vector<string> &dictionary)
{
    vector<string> compressed_code;
    bool instruction_repeated = false;
    int rle_count = 0;
    string previous_instruction;

    vector<string> possible_bitmasks = {"1000",
                                        "1100",
                                        "1010",
                                        "1110",
                                        "1001",
                                        "1101",
                                        "1011",
                                        "1111"};
    vector<string> bitmasks = Get_bitmask(possible_bitmasks);
    int count__ = 0;
    int local_count = 0;

    for (const string &instru_ : code_to_compress)
    {
        string compression_method = "dictionary";
        instruction_repeated = previous_instruction == instru_;
        count__ += 1;
        // cout << instruction_repeated << "  instruction  " << instru_;
        // cout << instru_ << "   " << previous_instruction << endl;
        if (instruction_repeated && rle_count < 8)
        { //rle compression
            // cout << " by rle" << endl;
            string rle_position = To_binary(rle_count, 3);
            previous_instruction = instru_;
            string to_push = "001" + rle_position;
            compressed_code.push_back(to_push);
            ++rle_count;
            local_count += 1;
        }
        else if (!instruction_repeated || rle_count == 8)
        {
            bool two_bit_anywhere_present = false;
            bool bitmask_present = false;
            previous_instruction = instru_;
            // rle_count = 0;
            // local_count += 1;

            //dictionary
            if (compression_method == "dictionary")
            {

                int position = Get_index(dictionary, instru_);
                if (position >= 0)
                {
                    string dictionary_index = To_binary(position, 4);
                    string to_push = "111" + dictionary_index;
                    compressed_code.push_back(to_push);

                    // cout << " by dic" << endl;
                }
                else
                {
                    compression_method = "mismatch";
                }
            }

            //bit mismactch compression
            if (compression_method == "mismatch")
            {
                for (string &dic_entry : dictionary)
                {
                    string xor_instru = to_string(to_bitset(dic_entry) ^ to_bitset(instru_));
                    int bit_mismatch = AddBitStream(xor_instru); // check bit mismatches

                    // 1bit mismatch
                    if (bit_mismatch == 1)
                    {
                        int position = Get_index(dictionary, dic_entry);
                        string dictionary_index = To_binary(position, 4);
                        string mis_loc = GetMisLocation(xor_instru);
                        string to_push = "011" + mis_loc + dictionary_index;
                        compressed_code.push_back(to_push);
                        compression_method = "mismatch";
                        // cout << " by mismatch 1" << endl;
                        break;
                    }
                    //2bit mismatch
                    if (bit_mismatch == 2)
                    {
                        bool consec_ = Consecutive(xor_instru);
                        if (consec_) //2bit consecutive mismatches
                        {
                            int position = Get_index(dictionary, dic_entry);
                            string dictionary_index = To_binary(position, 4);
                            string mis_loc = GetMisLocation(xor_instru);
                            string to_push = "011" + mis_loc + dictionary_index;
                            compressed_code.push_back(to_push);
                            compression_method = "mismatch";
                            break;
                        }
                        else
                        { //2 bit mismatches anywhere
                            vector<int> loc_cations = Get2LocationBits(xor_instru);
                            string first_bit_position = To_binary(loc_cations[0], 5);
                            string second_bit_position = To_binary(loc_cations[1], 5);
                            int position = Get_index(dictionary, dic_entry);
                            string dictionary_index = To_binary(position, 4);
                            string to_push_two = "110" + first_bit_position + second_bit_position + dictionary_index;
                            two_bit_anywhere_present = true;
                            compression_method = "mismatch";

                            for (string &bit : bitmasks)
                            {
                                /* 
                                check whether if it is possible to do the bitmask compression 
                                for the compression of the 2bit anywhere compression
                                */
                                string xor_instru2 = to_string(to_bitset(bit) ^ to_bitset(instru_));
                                int position = Get_index(dictionary, xor_instru2);
                                if (position >= 0)
                                {
                                    rle_count = 0;
                                    int temp_location = Get_index(bitmasks, bit);
                                    int bitmask_location_temp = temp_location % 29;
                                    string bitmask_location = To_binary(bitmask_location_temp, 5);
                                    int bit_type_idx = floor(bitmask_location_temp / 29);
                                    string bitmask_type = To_binary(bit_type_idx, 4);
                                    string dictionary_index = To_binary(position, 4);
                                    string to_push_bit_mask = "010" + bitmask_location + bitmask_type + dictionary_index;
                                    compression_method = "bitmask";
                                    compressed_code.push_back(to_push_bit_mask);
                                    bitmask_present = true;
                                    break;
                                }
                                else
                                {
                                    compressed_code.push_back(to_push_two);
                                    break;
                                }
                            }
                            break;
                        }
                    }
                    if (bit_mismatch == 4)
                    { //4bit consecutive mismatches
                        int position = Get_index(dictionary, dic_entry);
                        string dictionary_index = To_binary(position, 4);
                        string mis_loc = GetMisLocation(xor_instru);
                        string to_push = "100" + mis_loc + dictionary_index;
                        compressed_code.push_back(to_push);
                        compression_method = "mismatch";
                        break;
                    }
                    else
                    {
                        compression_method = "bitmask";
                    }
                }
            }

            //bitmask
            if (compression_method == "bitmask")
            {
                for (string &bit : bitmasks)
                {
                    string xor_instru = to_string(to_bitset(bit) ^ to_bitset(instru_));
                    int position = Get_index(dictionary, xor_instru);
                    if (position >= 0)
                    {
                        rle_count = 0;
                        int temp_location = Get_index(bitmasks, bit);
                        int bitmask_location_temp = temp_location % 29;
                        string bitmask_location = To_binary(bitmask_location_temp, 5);
                        int bit_type_idx = floor(bitmask_location_temp / 29);
                        string bitmask_type = To_binary(bit_type_idx, 4);
                        string dictionary_index = To_binary(position, 4);
                        string to_push_bit_mask = "010" + bitmask_location + bitmask_type + dictionary_index;
                        compression_method = "bitmask";
                        compressed_code.push_back(to_push_bit_mask);
                        bitmask_present = true;
                        break;
                    }
                    else
                    {
                        compression_method = "uncompresses";
                    }
                }
            }
            //uncompressed
            if (compression_method == "uncompresses")
            {
                string to_push = "000" + instru_;
                compressed_code.push_back(to_push);
            }
        }
    }
    // for (string i : compressed_code)
    // {
    //     cout << i << endl;
    // }
    // cout << count__ << "  " << local_count << endl;
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