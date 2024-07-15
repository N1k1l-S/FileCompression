#include "huffman-tree.cpp"
#include "huffman-zip.cpp"
#include "lz77.cpp"
#include "rle.cpp"
#include "constants.h"
using std::cout, std::endl, std::fstream;
using std::string, std::ios;

string next_block(fstream &f){
    string block; char c;
    for (int i = 0; i < BLOCK_SIZE && !f.eof(); i++){
        // if (f.eof()) break;
        f.get(c);
        block += c;
    } return block;
}

// Function to trim trailing whitespace
string rstrip(const string &s) {
    // Find the last non-whitespace character
    size_t end = s.length();
    while (end > 0 && std::isspace(s[end - 1])) {
        --end;
    }

    // Extract the substring from the start to the end
    return s.substr(0, end);
}


int main(int argc, char **argv){
    if (argc != 3) {
        cout << "Usage: " << argv[0] << " <input-file> <part>" << endl;
        return 1;
    }
    string infile = argv[1];
    fstream inf(infile);

    string block, comp, recon;
    ulg insz = 0, outsz = 0;

    // utilities for compression
    tree *huffman_tree;
    rle rlenc(4);

    int part = atoi(argv[2]);
    bool use_huffman=false, use_lz77=false, use_rle=false;
    switch (part) {
        case 3: use_lz77 = false; // notice the missing break statement. Why is the fall-through useful here?
        case 2: use_huffman = true;
        case 1: use_rle = false;
    }

    while (!inf.eof()) {
        block = next_block(inf);
        // std::cout<<block<<'\n'; // DUMMY
        insz += block.size();

        // compression
        comp = block;

        // cout << block << " 1\n"; // dummy

        if (use_lz77) {
            comp = deflate_lz77(comp);
            // cout << comp << " 2\n"; // dummy
            fstream out("comps/c1.txt", ios::app); out << comp;
        } 
        
        if (use_huffman) {
            comp = deflate(comp, huffman_tree);
            // cout << comp << " " << comp.size()<< " 3\n"; // dummy
            fstream out("comps/c2.txt", ios::app); out << comp;
        }

        if (use_rle) {
            comp = rlenc.compress(comp);
            // cout << comp << " " << comp.size()<<" 4\n"; // dummy
            fstream out("comps/c3.txt", ios::app); out << comp;
        }

        outsz += comp.size();

        // decompression
        recon = comp;

        if (use_rle) {
            recon = rlenc.decompress(recon);
            // std::cout << recon << " " << recon.size()<< " 5\n"; // dummy
            fstream out("decomps/d1.txt", ios::app); out << recon;
        }

        if (use_huffman) {
            recon = inflate(recon, huffman_tree);
            // std::cout << recon << " 6\n"; // dummy
            fstream out("decomps/d2.txt", ios::app); out << recon;
        }

        if (use_lz77) {
            recon = inflate_lz77(recon);
            // std::cout << recon << " 7\n"; // dummy
            fstream out("decomps/d3.txt", ios::app); out << recon;
        }

        block = rstrip(block);
        recon = rstrip(recon);

        cout << (block == recon ? "Success!" : "Failure!") << endl;
    }
    // print gain in bits
    if (part != 1) cout << "Ratio of final/original: " << outsz << "/" << 8*insz << " = " << 100.0 * ((double)outsz / (8*(double)insz)) << "\%\n";
    else cout << "Ratio of final/original: " << outsz << "/" << insz << " = " << 100.0 * ((double)outsz / ((double)insz)) << "\%\n";
}