#ifndef HUFFMAN
#define HUFFMAN
#include <bits/stdc++.h>
#include <vector>
#include <string>
#include <map>
#include <set>
#include <iostream>
using std::cout, std::endl, std::string;
using std::map, std::set, std::pair, std::make_pair;

// a node of the huffman tree. Already written, observe what is going on.
class tree{
public:
    string value = "┐"; // ┐/0/1/character. for now we just use just ┐.
    tree *left = nullptr, *right = nullptr;

    tree(){}
    tree(char value): value(1, value){}
    tree(tree *left): left(left){}
    tree(tree *left, tree *right): left(left), right(right){}
    
    static tree *merge_trees(tree *left, tree *right){
        return new tree(left, right);
    }

    void print(const string &prefix="", bool isLeftChild=false){
        cout << prefix << (prefix==""?"└──":(isLeftChild? "├─0":"└─1"));
        // cout << "value:" << value << "|" << endl;
        if (isLeaf()) cout << "─[" << value << "]" << endl; else cout << value << endl;
        if (left) left->print(prefix + (isLeftChild?"│  ":"   "), true);
        if (right) right->print(prefix + (isLeftChild?"│  ":"   "), false);
    }
    bool isLeaf() const{ return !(left && right); }
    void get_codes(map<char, string> &map_in_progress, const string &prefix="") const{
        if (left){
            if (left->isLeaf()) map_in_progress[left->value[0]] = prefix + "0";
            else left->get_codes(map_in_progress, prefix + "0");
        }
        if (right){ 
            if (right->isLeaf()) map_in_progress[right->value[0]] = prefix + "1";
            else right->get_codes(map_in_progress, prefix + "1");
        }
    }
    ~tree() {
        delete left; delete right;
    }
};

tree* buildtree(std::map<tree*,int>& nodes){
    if(nodes.size() == 1) return nodes.begin()->first;
    // std::cout<<"buildtree : "<<nodes.size()<<std::endl; // DUMMY

    int min1 = INT_MAX;
    int min2 = INT_MAX;
    tree* min1Key = nullptr;
    tree* min2Key = nullptr;

    for (const auto& entry : nodes) {
        int value = nodes[entry.first];
        tree* key = entry.first;

        if (value < min1) {
            min2 = min1;
            min2Key = min1Key;

            min1 = value;
            min1Key = key;
        } else if (value < min2) {
            min2 = value;
            min2Key = key;
        }
    }

    tree* t = new tree(min1Key,min2Key);
    nodes[t] = min1+min2;
    nodes.erase(min1Key);
    nodes.erase(min2Key);

    return buildtree(nodes);

}

// this returns the huffman tree using passed map of character to frequency
tree *huffman(const map<char, double> &freq_map){
    // std::cout<<"huffman"<<std::endl; // DUMMY
    // std::map<char,tree*> nodes
    std::map<tree*,int> nodes; 
    // std::cout<<nodes.size()<<std::endl; // DUMMY

    for(auto el:freq_map){
        tree* c = new tree(el.first);
        // tree* t = new tree(c);
        nodes[c] = el.second;
    }
    // std::cout<<nodes.size()<<std::endl; // DUMMY
    // std::cout<<"cp1"<<std::endl; // DUMMY

    tree* head = buildtree(nodes);
    // std::cout<<nodes.size()<<std::endl; // DUMMY

    return head;
}

// compute the frequencies of characters in the string. For part 3, make sure to skip over backreferences and also set highest priority to the special character.
map<char, double> get_freqs(const string &buf) {
    // std::cout<<"get_freq"<<std::endl; // DUMMY
    map<char, double> mymap;
    for(auto i:buf){
        mymap[i]++;
    }
    return mymap;
}

#endif