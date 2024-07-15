#include "rle.h"
#include "bits/stdc++.h"

// string int2binstring(int n, int gamma){
//     std::cout << "int2bin\n";
//     string s = "";
//     int sz=0;
//     while(n != 0){
//         s = std::to_string(n%2) + s;
//         n /= 2;
//         sz++;
//     }
//     while(sz < gamma){
//         s = "0" + s;
//         sz++;
//     }
//     return s;
// }

// int binstr2int(string s){
//     std::cout << "bin2int\n";
//     int n = 0,m = 1;
//     for(int i=s.size()-1; i>=0; i--){
//         if(s[i] == '1') n += m*(s[i]);
//         m *= 2;
//     }
//     return n;
// }

// Constructor
rle::rle(int gamma) : gamma(gamma) {}

// Convert integer to gamma-bit binary string
string rle::f(int n)
{
    // std::cout << "f\n";
    string s = "";
    int G = pow(2,gamma)-1;
    string _0s(gamma,'0');
    string _1s(gamma,'1');
    while(n >= G){
        s += _1s + _0s;
        n -= G;
    }
    if(n == 0) return s.substr(0,s.size()-gamma);
    int sz = 0;
    string t = "";
    while (n > 0){
        t = std::to_string(n%2) + t;
        sz++;
        n /= 2;
    }
    while (sz < gamma){
        t = "0" + t;
        sz++;
    }
    s += t;
    return s;
}

// Convert gamma-bit binary string to integer
int rle::g(string s)
{
    // std::cout << "g\n";
    int n = 0,m = 1;
    for(int i=s.size()-1;i>=0;i--){
        // n += m*(s[i]-'0');
        if(s[i] == '1') n += m;
        m *= 2;
    }
    // std::cout<<"output of g" << n << std::endl; 
    return n;
}

// Compress the input file
string rle::compress(string input_string)
{
    // std::cout << "compress\n";
    string res;
    char c = input_string[0];
    if (c == '0') res = "0";
    else if(c == '1') res = "1";
    int n = 0,i = 0;
    // std::cout<<input_string.size()<<"\n";
    while(i < input_string.size()){
        // sleep(1);
        // std::cout<<i<<"\n";
        while(i < input_string.size() && input_string[i] == c){
            i++;
            n++;
        }       
        res += f(n); 
        if (c == '0') c = '1';
        else if (c == '1') c = '0';
        n = 0;
        if(i >= input_string.size() || (input_string[i] != '0' && input_string[i] != '1')) break;
    }
    // std::cout<<"output of compress"<<res<<std::endl;
    return res;
}

// Decompress the input file
string rle::decompress(string input_string)
{
    // std::cout << "decompress\n";
    char cur = input_string[0];
    string out = "";
    string res(0,cur);
    for(int i=1;i<input_string.size();i+=gamma){
        string temp(g(input_string.substr(i,gamma)),cur);
        // std::cout<<"temp"<<temp<<std::endl;
        res += temp;
        if(cur == '0') cur = '1';
        else if(cur == '1') cur = '0';
    }
    // std::cout<<"output of decompress"<<res<<std::endl;
    return res;
}