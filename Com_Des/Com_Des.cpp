#include <iostream>
#include "Huff.h"

using namespace std;

int main(int argc, char** argv) {
    // Check if the correct number of command-line arguments is provided
    if (argc != 4) {
        cout << "Error, nezadano spravne" << endl;
    }
    else {
        // Extract operation and file names from command-line arguments
        string str1 = argv[1];
        string str2 = "c";
        string str3 = "d";

        // Check if the operation is compression ("c")
        if (str1.compare(str2) == 0) {
            // Perform compression
            Huffman h;
            h.Compres(argv[2], argv[3]);
            cout << "Compression operation performed." << endl;
            cout << "Input file: " << argv[2] << endl;
            cout << "Output file: " << argv[3] << endl;
        }
        // Check if the operation is decompression ("d")
        else if (str1.compare(str3) == 0) {
            // Perform decompression
            Huffman h;
            h.Decompres(argv[2], argv[3]);
            cout << "Decompression operation performed." << endl;
            cout << "Input file: " << argv[2] << endl;
            cout << "Output file: " << argv[3] << endl;
        }
        // Invalid operation
        else {
            cout << "Spatna operace" << endl;
        }
    }

    return 0;
}
