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
// using std::bitset;
// using std::cerr;
// using std::cout;
// using std::endl;
// using std::ifstream;
// using std::map;
// using std::ofstream;
// using std::pair;
// using std::string;
// using std::vector;

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

bool Compare_(pair<string, int> &a, pair<string, int> &b)
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
    sort(sorted_dictionary.begin(), sorted_dictionary.end(), Compare_);

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

bool Consecutive_four(const string &str_4)
{
    int cons_it = 0;
    bool consec_4 = false;
    while (cons_it < str_4.length())
    {
        ++cons_it;
        if (str_4[cons_it] == '0')
        {
            continue;
        }
        else if (str_4[cons_it] == '1' && str_4[cons_it + 1] == '1' && str_4[cons_it + 2] == '1' && str_4[cons_it + 3] == '1')
        {
            consec_4 = true;
            break;
        }
        else
        {
            consec_4 = false;
            break;
        }
    }
    return consec_4;
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

    vector<string> possible_bitmasks = {
        "1000",
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
    string to_push_rle;
    string rle_position;
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
            rle_position = To_binary(rle_count, 3);
            previous_instruction = instru_;
            to_push_rle = "001" + rle_position;
            // compressed_code.push_back(to_push);
            ++rle_count;
            local_count += 1;
        }
        else if (!instruction_repeated || rle_count == 8)
        {
            if (to_push_rle.length() == 6)
            {
                compressed_code.push_back(to_push_rle);
                to_push_rle = "";
            }
            previous_instruction = instru_;
            rle_count = 0;
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
                        { 
                            compression_method = "bitmask";
                            break;
                        }
                    }
                    if (bit_mismatch == 4)
                    { //4bit consecutive mismatches
                        bool check_4 = Consecutive_four(xor_instru);
                        if (check_4)
                        {
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
                        break;
                    }
                    else
                    {
                        compression_method = "2-bit-anywhere";
                    }
                }
            }
            if (compression_method == "2-bit-anywhere")
            {
                for (string &dic_entry : dictionary)
                {
                    string xor_instru = to_string(to_bitset(dic_entry) ^ to_bitset(instru_));
                    int bit_mismatch = AddBitStream(xor_instru); // check bit mismatches
                    bool consec_ = Consecutive(xor_instru);
                    if (bit_mismatch == 2 && !consec_)
                    {
                        vector<int> loc_cations = Get2LocationBits(xor_instru);
                        string first_bit_position = To_binary(loc_cations[0], 5);
                        string second_bit_position = To_binary(loc_cations[1], 5);
                        int position = Get_index(dictionary, dic_entry);
                        string dictionary_index = To_binary(position, 4);
                        string to_push_two = "110" + first_bit_position + second_bit_position + dictionary_index;
                        compressed_code.push_back(to_push_two);
                        compression_method = "mismatch";
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

string Concat_(const vector<string> &compressed__code)
{
    string cont;
    for (string i : compressed__code)
    {
        cont += i;
    }
    return cont;
}

/*
remove this function
*/
void Write_file(string &to_write1, const string &to_write2)
{
    ofstream output_;
    string to_str = "0";
    int to_stuff = to_write1.size() % 32;
    to_write1 = to_write1 + (to_str * (32 - to_stuff));

    output_.open("cout.txt");
    if (!output_)
    {
        cerr << "Error: file could not be opened" << endl;
        exit(1);
    }

    for (int i = 0; i < to_write1.length(); ++i)
    {
        if (i % 32 != 0 || i == 0)
        {
            output_ << to_write1[i];
        }
        else
        {
            output_ << "\n"
                    << to_write1[i];
        }
    }
    output_ << "\n"
            << "xxxx" << endl;

    for (int i = 0; i < to_write2.length(); ++i)
    {
        if (i % 32 != 0 || i == 0)
        {
            output_ << to_write2[i];
        }
        else
        {
            output_ << "\n"
                    << to_write2[i];
        }
    }
    output_.close();
}

/////// remove this function
// auto Separate_x(const string &to_seperate)
// {
//     string compressed_code, dict;
//     map<string, string> dictionary_;
//     bool to_break = false;

//     for (int i = 0; i < to_seperate.length(); i++)
//     {
//         if (to_seperate[i] != 'x' && !to_break)
//         {
//             compressed_code += to_seperate[i];
//         }
//         else if (to_seperate[i] == 'x')
//         {
//             to_break = true;
//         }
//         else if (to_break)
//         {
//             dict += to_seperate[i];
//         }
//     }
//     int cnt = 0;
//     string temp_dict;
//     for (int i = 0; i < dict.length(); i++)
//     {
//         if (i == 0 || i % 32 != 0)
//         {
//             temp_dict += dict[i];
//         }
//         else
//         {
//             string key_ = To_binary(cnt, 4);
//             dictionary_.insert(pair<string, string>(key_, temp_dict));
//             ++cnt;
//             temp_dict = dict[i];
//         }
//     }
//     string key_ = To_binary(cnt, 4);
//     dictionary_.insert(pair<string, string>(key_, temp_dict));

//     // for (auto itr = dictionary_.begin(); itr != dictionary_.end(); ++itr) {
//     //     cout << itr->first
//     //          << '\t' << itr->second << '\n';
//     // }

//     return compressed_code, dictionary_;
// }

string Get_Bitmask_Decompressed(string &start_location, string &bit__mask, string Dict_inst)
{
    string pad = "0";
    string bitmask__ = (pad * stoi(start_location, 0, 2)) + bit__mask + (pad * (28 - stoi(start_location, 0, 2)));
    string decompressed_bitmask = to_string(to_bitset(bitmask__) ^ to_bitset(Dict_inst));

    return decompressed_bitmask;
}

string One_Mismatch(string &start_location, string Dict_inst)
{
    string one_decompressed__co, one_miss;
    string pad = "0";
    one_miss = (pad * stoi(start_location, 0, 2)) + "1" + (pad * (31 - stoi(start_location, 0, 2)));
    one_decompressed__co = to_string(to_bitset(one_miss) ^ to_bitset(Dict_inst));

    return one_decompressed__co;
}

string Two_Mismatch(string &start_location, string Dict_inst)
{
    string two_decompressed__co, two_miss;
    string pad = "0";
    two_miss = (pad * stoi(start_location, 0, 2)) + "11" + (pad * (30 - stoi(start_location, 0, 2)));
    two_decompressed__co = to_string(to_bitset(two_miss) ^ to_bitset(Dict_inst));

    return two_decompressed__co;
}

string Four_Mismatch(string &start_location, string Dict_inst)
{
    string four_decompressed__co, four_miss;
    string pad = "0";
    four_miss = (pad * stoi(start_location, 0, 2)) + "1111" + (pad * (28 - stoi(start_location, 0, 2)));
    four_decompressed__co = to_string(to_bitset(four_miss) ^ to_bitset(Dict_inst));

    return four_decompressed__co;
}

string Two_Mismatch_any(string &start_location, string &start_location2, string Dict_inst)
{
    string two_any_decompressed, two_miss_any;
    int st1 = stoi(start_location, 0, 2);
    int st2 = stoi(start_location2, 0, 2) - st1 - 1;
    int fin = (30 - (st1 + st2));
    string pad = "0";
    two_miss_any = (pad * st1) + "1" + (pad * st2) + "1" + (pad * fin);

    two_any_decompressed = to_string(to_bitset(two_miss_any) ^ to_bitset(Dict_inst));

    return two_any_decompressed;
}

vector<string> Decompression_algo(string &compressed_code, map<string, string> &dictionary_)
{
    vector<string> decompressed_code;
    int type_ = 0;
    string opcode;
    string previous_instruction;
    int idx_string_c = 0;
    for (idx_string_c; idx_string_c < compressed_code.length();)
    {
        if (type_ < 3)
        {
            opcode += compressed_code[idx_string_c];
            type_ += 1;
            idx_string_c++;
        }
        else if (type_ == 3)
        {
            type_ = 0;

            string compressed_code_temp, info_component, start_location, start_location2, bit__mask, DictionarY_index, dicitonary_val;

            // uncompressed
            if (opcode == "000")
            {
                opcode = "";
                if (idx_string_c + 32 <= compressed_code.length())
                {
                    compressed_code_temp = compressed_code.substr(idx_string_c, 32);

                    decompressed_code.push_back(compressed_code_temp);
                    previous_instruction = compressed_code_temp;
                }
                else
                {
                    break;
                }
                idx_string_c += 32;
            }

            //rle decompression
            else if (opcode == "001")
            {
                opcode = "";
                if (idx_string_c + 3 <= compressed_code.length())
                {
                    string num_of_times = compressed_code.substr(idx_string_c, 3);
                    for (int times_ = 0; times_ <= stoi(num_of_times, 0, 2); times_++)
                    {
                        decompressed_code.push_back(previous_instruction);
                    }
                }
                else
                {
                    break;
                }
                idx_string_c += 3;
            }

            //bitmask
            else if (opcode == "010")
            {
                opcode = "";
                if (idx_string_c + 13 <= compressed_code.length())
                {
                    info_component = compressed_code.substr(idx_string_c, 13);
                    start_location = info_component.substr(0, 5);
                    bit__mask = info_component.substr(5, 4);
                    DictionarY_index = info_component.substr(9, 4);
                    dicitonary_val = dictionary_[DictionarY_index];
                    compressed_code_temp = Get_Bitmask_Decompressed(start_location, bit__mask, dicitonary_val);

                    decompressed_code.push_back(compressed_code_temp);
                    previous_instruction = compressed_code_temp;
                }
                else
                {
                    break;
                }
                idx_string_c += 13;
            }

            // 1 bit mismatch
            else if (opcode == "011")
            {
                if (idx_string_c + 9 <= compressed_code.length())
                {
                    opcode = "";
                    info_component = compressed_code.substr(idx_string_c, 9);
                    start_location = info_component.substr(0, 5);
                    DictionarY_index = info_component.substr(5, 4);
                    dicitonary_val = dictionary_[DictionarY_index];

                    compressed_code_temp = One_Mismatch(start_location, dicitonary_val);

                    decompressed_code.push_back(compressed_code_temp);
                    previous_instruction = compressed_code_temp;
                }
                else
                {
                    break;
                }
                idx_string_c += 9;
            }

            // 2-bit consecutive mismatch
            else if (opcode == "100")
            {
                if (idx_string_c + 9 <= compressed_code.length())
                {
                    opcode = "";
                    info_component = compressed_code.substr(idx_string_c, 9);
                    start_location = info_component.substr(0, 5);
                    DictionarY_index = info_component.substr(5, 4);
                    dicitonary_val = dictionary_[DictionarY_index];

                    compressed_code_temp = Two_Mismatch(start_location, dicitonary_val);

                    decompressed_code.push_back(compressed_code_temp);
                    previous_instruction = compressed_code_temp;
                }
                else
                {
                    break;
                }
                idx_string_c += 9;
            }
            // 4 bit consecutive mismatch
            else if (opcode == "101")
            {
                if (idx_string_c + 9 <= compressed_code.length())
                {
                    opcode = "";
                    info_component = compressed_code.substr(idx_string_c, 9);
                    start_location = info_component.substr(0, 5);
                    DictionarY_index = info_component.substr(5, 4);
                    dicitonary_val = dictionary_[DictionarY_index];

                    compressed_code_temp = Four_Mismatch(start_location, dicitonary_val);

                    decompressed_code.push_back(compressed_code_temp);
                    previous_instruction = compressed_code_temp;
                }
                else
                {
                    break;
                }
                idx_string_c += 9;
            }

            //2-bit anywhere
            else if (opcode == "110")
            {
                if (idx_string_c + 14 <= compressed_code.length())
                {
                    opcode = "";
                    info_component = compressed_code.substr(idx_string_c, 14);
                    start_location = info_component.substr(0, 5);
                    start_location2 = info_component.substr(5, 5);
                    DictionarY_index = info_component.substr(10, 4);
                    dicitonary_val = dictionary_[DictionarY_index];

                    compressed_code_temp = Two_Mismatch_any(start_location, start_location2, dicitonary_val);

                    decompressed_code.push_back(compressed_code_temp);
                    previous_instruction = compressed_code_temp;
                }
                else
                {
                    break;
                }
                idx_string_c += 14;
            }

            // dictionary
            else if (opcode == "111")
            {
                if (idx_string_c + 4 <= compressed_code.length())
                {
                    opcode = "";
                    DictionarY_index = compressed_code.substr(idx_string_c, 4);
                    dicitonary_val = dictionary_[DictionarY_index];

                    compressed_code_temp = dicitonary_val;

                    decompressed_code.push_back(compressed_code_temp);
                    previous_instruction = compressed_code_temp;
                }
                else
                {
                    break;
                }
                idx_string_c += 4;
            }
        }
    }
    return decompressed_code;
}

int main(int argc, char **argv)
{

    int argument = strtol(argv[1], NULL, 10);

    if (argument == 1)
    {
        vector<string> code_to_compress, dictionary, compressed_code;

        code_to_compress = ReadFile("original.txt"); // read the file to be compressed

        dictionary = GetFrequency(code_to_compress); // get the dictionary

        compressed_code = Compression_algo(code_to_compress, dictionary); // compress the instructions

        string to_print = Concat_(compressed_code);
        string to_print2 = Concat_(dictionary);

        Write_file(to_print, to_print2); // write the compressed code to cout.txt
    }

    if (argument == 0)
    {
        vector<string> code_to_decompress, uncompressed_code;
        map<string, string> dictionary_;
        string compressed_code, dict;

        code_to_decompress = ReadFile("compressed.txt");
        string total_ = Concat_(code_to_decompress);

        /*
        Seperate the dictionary and the compressed code
        */
        bool to_break = false;
        for (int i = 0; i < total_.length(); i++)
        {
            if (total_[i] != 'x' && !to_break)
            {
                compressed_code += total_[i];
            }
            else if (total_[i] == 'x')
            {
                to_break = true;
            }
            else if (to_break)
            {
                dict += total_[i];
            }
        }
        int cnt = 0;
        string temp_dict;
        for (int i = 0; i < dict.length(); i++)
        {
            if (i == 0 || i % 32 != 0)
            {
                temp_dict += dict[i];
            }
            else
            {
                string key_ = To_binary(cnt, 4);
                dictionary_.insert(pair<string, string>(key_, temp_dict));
                ++cnt;
                temp_dict = dict[i];
            }
        }
        string key_ = To_binary(cnt, 4);
        dictionary_.insert(pair<string, string>(key_, temp_dict));
        uncompressed_code = Decompression_algo(compressed_code, dictionary_);

        //write the decompressed code to a text file
        ofstream de_out;

        de_out.open("dout.txt");
        if (!de_out)
        {
            cerr << "Error: file could not be opened" << endl;
            exit(1);
        }
        for (string &i : uncompressed_code)
        {
            de_out << i << endl;
        }
        de_out.close();
    }
    return 0;
}