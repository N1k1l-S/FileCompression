#include "huffman-tree.cpp"
#include "constants.h"

// given a compressed binary string, convert it to the original string using the huffman tree
string inflate(const string &compressed, tree *huffman_tree){
    // std::cout<<"inflate"<<std::endl; // DUMMY
    // std::cout<<"cp1"<<std::endl; // DUMMY
    // map<char, string> encoding;
    // huffman_tree->get_codes(encoding);
    // std::cout<<encoding.size()<<std::endl; // DUMMY

    // huffman_tree->print();

    int i=0;
    tree* t = huffman_tree;
    string s;
    while(i < compressed.size()){
        if(compressed[i] == '0'){
            t = t->left;
        }
        else{
            t = t->right;
        }
        if(t->isLeaf()){
            s += t->value;
            t = huffman_tree;
        }
        i++;
    }
    return s;
}

// given a string, compress it using the huffman tree and return the resulting binary string
string deflate(const string &buf, tree* &huffman_tree){
    // std::cout<<"deflate"<<std::endl; // DUMMY
    map<char, double> freq_map = get_freqs(buf);
    // std::cout<<freq_map.size()<<std::endl; // DUMMY
    huffman_tree = huffman(freq_map);
    // tree *huff = huffman(freq_map);

    // std::cout<<freq_map.size()<<std::endl; // DUMMY
    // std::cout<<"done with huffman"<<std::endl; // DUMMY
    map<char, string> encoding;
    // std::cout<<encoding.size()<<std::endl; // DUMMY
    huffman_tree->get_codes(encoding);
    // std::cout<<"encoding size : "<<encoding.size()<<std::endl; // DUMMY
    string s;
    for(auto i:buf){
        s += encoding[i];
    }
    std::cout<<s.size()<<std::endl; // DUMMY
    return s;
}