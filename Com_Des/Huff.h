#pragma once
#include <iostream>
#include <fstream>
#include <queue>
#include <string>
#include <vector>

using namespace std;

/**
 * @file Huff.h
 * @brief Header file for Huffman compression and decompression.
 */

class Huffman {
private:
    /**
     * @brief Represents a node in the Huffman tree.
     */
    struct Node {
        int count = 0;        ///< Count of occurrences of the character.
        char Id;               ///< The character.
        string ids = "";       ///< String representation of the character.
        Node* left = NULL;     ///< Pointer to the left child node.
        Node* right = NULL;    ///< Pointer to the right child node.
    };

    /**
     * @brief Functor for comparing two nodes based on their counts.
     */
    class CompareNode {
    public:
        /**
         * @brief Compares two nodes based on their counts.
         * @param a First node.
         * @param b Second node.
         * @return True if the count of the first node is greater than the count of the second node.
         */
        bool operator()(const Node& a, const Node& b) const {
            return a.count > b.count;
        }
    };

    vector<Node> Znaky;                                  ///< Vector of nodes representing characters and their counts.
    priority_queue<Node, vector<Node>, CompareNode> PQ;  ///< Priority queue of nodes used for building the Huffman tree.

public:
    void createPriorityQueue(vector<Node> Znaky);
    void readNodeFromFile(Node& node, std::istream& infile);
    void writeNodeToFile(const Node& node, std::ostream& outfile);    
    void Compres(string name, string name2);
    void Decompres(string name, string name2);
    void encode(Node* node, string& output);
    void encodeCharacter(char c, Node root, vector<int>& output);
    void decodeSequence(vector<int>& sequence, Node root, ofstream& output);
    long getFileSize(const std::string& fileName);
};