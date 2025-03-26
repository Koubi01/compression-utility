#include "Huff.h"
/**
 * @file Huff.cpp
 * @brief Source code file for Huffman compression and decompression.
 */

/**
 * @brief Creates a priority queue from a vector of Node objects.
 * @param Znaky Vector of Node objects representing characters and their counts.
 */
void Huffman::createPriorityQueue(vector<Node> Znaky) {
    for (int i = 0; i < Znaky.size(); i++) {
        PQ.push(Znaky[i]);
    }
}

/**
 * @brief Reads a Node from a binary file.
 * @param node Reference to a Node object where the data will be stored.
 * @param infile Input stream to read from.
 */
void Huffman::readNodeFromFile(Node& node, std::istream& infile) {
    infile.read(reinterpret_cast<char*>(&node.count), sizeof(int));
    infile.read(reinterpret_cast<char*>(&node.Id), sizeof(char));
}

/**
 * @brief Writes a Node to a binary file.
 * @param node Const reference to a Node object containing the data to be written.
 * @param outfile Output stream to write to.
 */
void Huffman::writeNodeToFile(const Node& node, std::ostream& outfile) {
    outfile.write(reinterpret_cast<const char*>(&node.count), sizeof(int));
    outfile.write(reinterpret_cast<const char*>(&node.Id), sizeof(char));
}

/**
 * @brief Gets the size of a file.
 * @param fileName Name of the file.
 * @return Size of the file in bytes.
 */
long Huffman::getFileSize(const std::string& fileName) {
    std::ifstream file(fileName, std::ios::binary | std::ios::ate);
    return file.tellg();
}

/**
 * @brief Compresses a file using Huffman coding.
 * @param name Input file name.
 * @param name2 Output compressed file name.
 */

void Huffman::Compres(string name, string name2) {
    // Variables for reading input file
    string line;
    ifstream FILE(name);
    int count = 0;

    // Check if the input file is open
    if (FILE.is_open()) {
        bool flag2 = false;

        // Read input file line by line
        while (getline(FILE, line)) {
            // Process each character in the line
            for (char c : line) {
                bool flag = false;

                // If the Znaky vector is empty, create a new node for the character
                if (Znaky.empty()) {
                    Node Pis;
                    Pis.Id = c;
                    Pis.ids = c;
                    Pis.count = 1;
                    Znaky.push_back(Pis);
                    count = 1;
                }
                else {
                    // Check if the character is already in the Znaky vector
                    for (int i = 0; i < count; i++) {
                        if (Znaky[i].Id == c) {
                            Znaky[i].count++;
                            flag = true;
                            break;
                        }
                    }

                    // If the character is not in the Znaky vector, create a new node
                    if (!flag) {
                        Node Pis;
                        Pis.Id = c;
                        Pis.ids = c;
                        Pis.count = 1;
                        Znaky.push_back(Pis);
                        count++;
                    }
                }
            }

            // Update count for newline character
            if (flag2) {
                for (int i = 0; i < count; i++) {
                    if (Znaky[i].Id == '\n') {
                        Znaky[i].count++;
                        break;
                    }
                }
            }
            else {
                // Add newline character to Znaky vector
                Node end;
                end.Id = '\n';
                end.ids = "\n";
                Znaky.push_back(end);
                count++;
                flag2 = true;
            }
        }

        // Create priority queue from Znaky vector
        createPriorityQueue(Znaky);

        // Build Huffman tree using priority queue
        while (PQ.size() > 1) {
            Node left = PQ.top();
            PQ.pop();
            Node right = PQ.top();
            PQ.pop();

            Node newNode;
            newNode.count = left.count + right.count;
            newNode.ids = left.ids + right.ids + left.Id + right.Id;
            newNode.left = new Node(left);
            newNode.right = new Node(right);
            PQ.push(newNode);
        }

        // The Huffman tree root is now at the top of the priority queue
        Node root = PQ.top();

        // Encode the input file and write to the output file
        string input;
        FILE.close();
        FILE.open(name);
        ofstream outfile(name2, ios::binary);
        int velikost = Znaky.size();

        // Write the size of the Znaky vector to the output file
        outfile.write(reinterpret_cast<char*>(&velikost), sizeof(int));

        // Write the Znaky vector to the output file
        for (const auto& node : Znaky) {
            writeNodeToFile(node, outfile);
        }

        // Encode each character in the input file and write to the output file
        vector<int> bits;
        while (getline(FILE, input)) {
            for (char c : input) {
                encodeCharacter(c, root, bits);
            }
            encodeCharacter('\n', root, bits);
        }

        // Write the encoded bits to the output file
        int num = 0;
        int i = 0;
        for (int bit : bits) {
            num |= (bit << i);
            i++;

            if (i == 32) {
                outfile.write(reinterpret_cast<char*>(&num), sizeof(int));
                num = 0;
                i = 0;
            }
        }

        // Write any remaining bits to the output file
        if (i != 0) {
            outfile.write(reinterpret_cast<char*>(&num), sizeof(int));
        }

        // Close input and output files
        FILE.close();
        outfile.close();

        // Calculate and display compression information
        long fileSize1 = getFileSize(name);
        long fileSize2 = getFileSize(name2);

        double compressionRatio = 0;

        if (fileSize2 != 0) {
            compressionRatio = (static_cast<double>(fileSize2) / fileSize1) * 100;
        }

        std::cout << "Kompresni pomer: " << compressionRatio << "%" << std::endl;

        cout << "Vstupni soubor: " << fileSize1 << " bits" << endl;
        cout << "Vystupni soubor: " << fileSize2 << " bits" << endl;
    }
    else {
        // Display an error message if the input file cannot be opened
        cout << "Nejde otvorit" << endl;
    }

    // Clear the priority queue
    priority_queue<Node, vector<Node>, CompareNode> emptyPQ;
    swap(PQ, emptyPQ);
}


/**
 * @brief Decompresses a file that was compressed using Huffman coding.
 * @param name Compressed input file name.
 * @param name2 Output decompressed file name.
 */

void Huffman::Decompres(string name, string name2) {
    // Open the file in binary mode
    ifstream File(name, ios::binary);

    // Check if the file is open
    if (File.is_open()) {
        int count = 0;
        vector<Node> Znaky1;

        // Read the count of nodes from the file
        File.read(reinterpret_cast<char*>(&count), sizeof(int));

        // Read nodes from the file and create Znaky1 vector
        for (int i = 0; i < count; ++i) {
            Node node;
            readNodeFromFile(node, File);
            node.ids = node.Id;
            node.left = NULL;
            node.right = NULL;
            Znaky1.push_back(node);
        }

        // Create priority queue from Znaky1 vector
        createPriorityQueue(Znaky1);

        // Build Huffman tree using priority queue
        while (PQ.size() > 1) {
            Node left = PQ.top();
            PQ.pop();
            Node right = PQ.top();
            PQ.pop();

            Node newNode;
            newNode.count = left.count + right.count;
            newNode.ids = left.ids + right.ids + left.Id + right.Id;
            newNode.left = new Node(left);
            newNode.right = new Node(right);
            PQ.push(newNode);
        }

        // The Huffman tree root is now at the top of the priority queue
        Node root = PQ.top();

        // Read encoded bits from the file and decode
        vector<int> bits;
        int num;
        while (File.read(reinterpret_cast<char*>(&num), sizeof(int))) {
            // Extract individual bits from the integer
            for (int i = 0; i < 32; i++) {
                bits.push_back((num >> i) & 1);
            }
        }

        // Close the input file
        File.close();

        // Open the output file for writing
        ofstream Output(name2);

        // Decode the bits and write to the output file
        decodeSequence(bits, root, Output);

        // Close the output file
        Output.close();

        // Calculate and display compression information
        long fileSize1 = getFileSize(name);
        long fileSize2 = getFileSize(name2);

        double compressionRatio = 0;

        if (fileSize2 != 0) {
            compressionRatio = (static_cast<double>(fileSize2) / fileSize1) * 100;
        }

        std::cout << "Kompresni pomer: " << compressionRatio << "%" << std::endl;

        cout << "Vstupni soubor: " << fileSize1 << " bits" << endl;
        cout << "Vystupni soubor: " << fileSize2 << " bits" << endl;
    }
    else {
        // Display an error message if the file cannot be opened
        cout << "Error: Cannot open the file." << endl;
    }
}


/**
 * @brief Recursively generates Huffman codes for each character in the tree.
 * @param node Pointer to the current node in the tree.
 * @param output String representing the Huffman code for the current node.
 */

void Huffman::encode(Node* node, string& output) {
    // Recursively create Huffman code for the left and right subtrees.

    if (node->left != NULL && node->right != NULL) {
        // Continue creating Huffman code for the left and right subtrees.
        string leftOutput = output + "0";
        string rightOutput = output + "1";

        encode(node->left, leftOutput);
        encode(node->right, rightOutput);
    }
    else if (node->left == NULL && node->right == NULL) {
        // Output Huffman code for the given character.
        cout << node->Id << " : " << output << endl;
    }
}


/**
 * @brief Encodes a character using Huffman coding and appends the bits to the output vector.
 * @param c Character to be encoded.
 * @param root Root of the Huffman tree.
 * @param output Vector to store the encoded bits.
 */

void Huffman::encodeCharacter(char c, Node root, vector<int>& output) {
    // Recursive process that traverses the Huffman tree until it finds the desired character.

    // Check if the current node contains the desired character
    if (root.Id == c) {
        // Encode the found character using the output from the recursion.
        return;
    }
    else if (root.left->ids.find(c) != string::npos) {
        // The character is found in the left subtree, add 0 to the output.
        output.push_back(0);

        // Continue creating Huffman code for the left subtree.
        encodeCharacter(c, *root.left, output);
    }
    else if (root.right->ids.rfind(c) != string::npos) {
        // The character is found in the right subtree, add 1 to the output.
        output.push_back(1);

        // Continue creating Huffman code for the right subtree.
        encodeCharacter(c, *root.right, output);
    }
}


/**
 * @brief Decodes a sequence of bits using Huffman coding and writes the characters to the output file.
 * @param sequence Vector of bits representing the Huffman-encoded sequence.
 * @param root Root of the Huffman tree.
 * @param output Output stream to write the decoded characters.
 */

void Huffman::decodeSequence(vector<int>& sequence, Node root, ofstream& output) {
    int count = 0;
    int i = 0;
    Node* currentNode = &root;

    // Loop through the encoded sequence
    while (i < sequence.size()) {
        // Traverse left or right in the Huffman tree based on the current bit
        if (sequence[i] == 0) {
            currentNode = currentNode->left;
        }
        else if (sequence[i] == 1) {
            currentNode = currentNode->right;
        }

        // Check if a leaf node (character) is reached
        if (currentNode->left == NULL && currentNode->right == NULL) {
            // Check if the current node represents the end of a character's encoding
            if (currentNode->count == 0) {
                return;  // End of encoding
            }
            else {
                // Write the decoded character to the output file
                output << currentNode->Id;
            }

            // Update count, decrement, and reset the current node to the root
            count = currentNode->count;
            currentNode->count--;
            currentNode = &root;
        }
        i++;
    }
}



