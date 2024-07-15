#include <string>
#include <iostream>
#include <fstream>
#include <bitset>
#include "constants.h"
using std::string, std::bitset;
using namespace std;
typedef unsigned long ulg;

// a struct for the backreference. A casting operator to string that can used (it is not optimal) as encoding is provided for convenience.
struct backref{
    ulg offset, length;
    backref(const ulg &offset, const ulg &length): offset(offset), length(length){}
    operator string() const{
        return encode_backref();
    }
    string encode_backref() const{
        return string(1, -1) + bitset<15>(offset).to_string() + string(1, -1) + bitset<8>(length-3).to_string() + string(1, -1);
    }
};

// find the longest match for buf[i:i+lookahead_size] as a substring in the window buf[max(i-window_size,0):] and return appropriate backreference. Note that the longest match can exceed buf[i], that is, for buf=aaaa the longest match at i = 1 returns (0, 3).
backref find_longest_match(const string &buf, ulg i, ulg window_size, ulg lookahead_size){
    ulg st = 0;
    ulg maxlen = 0, maxind = 0;
    if(i > window_size) st = i - window_size;
    for(ulg j = st;j < i; j++) {
        ulg k = 0;
        while(true){
            if(i+k >= buf.size()){
                break;
            }
            if(buf[j+k] == buf[i+k]) k++;
            else {
                break;
            }
        }
        if(k >= lookahead_size){
            if(k > maxlen){
                maxlen = k;
                maxind = j;
            }
        }
    }
    if(maxlen == 0) return backref(0, 0);
    return backref(i-maxind,maxlen);
}

// compress the string using lz77 and return the compressed binary string containing characters and string encodings of backreferences.
string deflate_lz77(const string &infile_str){
    if(infile_str.size() < 4) return infile_str;
    string res;
    res += infile_str[0];
    backref br(0,0);
    for(int i = 1;i < infile_str.size();i++){
        br = find_longest_match(infile_str,i,216,3);
        if(br.offset == 0) res += infile_str[i];
        else{
            i += br.length-1;
            res += br.encode_backref();
        }
    }
    // cout << res << endl;
    return res;
}

ulg bs2num(string bs){
    // cout << bs << "\n"; // dummy
    ulg n = 0, m = 1;
    for(int i=bs.size()-1;i>=0;i--){
        // cout << i << "\n"; // dummy
        if(bs[i] == '1') n += m;
        m *= 2;
    }
    // cout << n << "\n"; // dummy
    return n;
}

// decompress the string (contains string encodings of backreferences, etc) using lz77 and return the original string.
string inflate_lz77(const string &compressed){
    // cout << "---\n"; // dummy
    string res;
    // cout << compressed << "\n";
    char x = 255;
    ulg st,l;
    for(int i=0;i<compressed.size();i++){
        if(compressed[i] != x) res += compressed[i];
        else{
            st = res.size() - bs2num(compressed.substr(1+i,15));
            l = bs2num(compressed.substr(1+i+15+1,8)) + 3;
            // cout << i << "\n";
            while(l--){
                res += res[st++];
            }
            i += 25;;
        }
    }
    return res;
}

// string inflate_lz772(const string &compressed){
//     string res;
//     char delimiter = '\xFF';  // Use a valid character as a delimiter, '\xFF' represents 255 in ASCII
//     size_t i = 0;
//     while (i < compressed.size()) {
//         if (compressed[i] != delimiter) {
//             res += compressed[i];
//             i++;
//         } else {
//             if (i + 25 >= compressed.size()) {
//                 cerr << "Error: Invalid backreference detected at index " << i << "\n";
//                 break;
//             }
//             ulg offset = bs2num(compressed.substr(i + 1, 15));
//             ulg length = bs2num(compressed.substr(i + 17, 8)) + 3;
//             if (offset > res.size()) {
//                 cerr << "Error: Backreference offset exceeds current result size at index " << i << "\n";
//                 break;
//             }
//             size_t start_pos = res.size() - offset;
//             for (size_t j = 0; j < length; j++) {
//                 if (start_pos + j >= res.size()) {
//                     cerr << "Error: Invalid backreference length at index " << i << "\n";
//                     break;
//                 }
//                 res += res[start_pos + j];
//             }
//             i += 26;  // Move past the backreference
//         }
//     }
//     return res;
// }
