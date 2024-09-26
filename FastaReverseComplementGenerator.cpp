#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <algorithm>

using namespace std;

// Function to read FASTA file and determine the maximum characters per line
pair<vector<pair<string, string>>, int> read_fasta(const string& filepath) {
    vector<pair<string, string>> sequences;
    ifstream infile(filepath);
    string line, sequence, description;
    int max_chars_per_line = 0;

    while (getline(infile, line)) {
        if (line[0] == '>') {
            if (!sequence.empty()) {
                transform(sequence.begin(), sequence.end(), sequence.begin(), ::toupper);
                sequences.push_back({ description, sequence });
                sequence.clear();
            }
            description = line.substr(1);
        }
        else {
            sequence += line;
            if (max_chars_per_line == 0) {
                max_chars_per_line = line.length();
            }
        }
    }
    if (!sequence.empty()) {
        transform(sequence.begin(), sequence.end(), sequence.begin(), ::toupper);
        sequences.push_back({ description, sequence });
    }
    return { sequences, max_chars_per_line };
}

string reverseComplement(const string& seq) {
    string rev_comp_seq = seq;
    for (char& base : rev_comp_seq) {
        switch (base) {
        case 'A': base = 'T'; break;
        case 'T': base = 'A'; break;
        case 'C': base = 'G'; break;
        case 'G': base = 'C'; break;
        }
    }
    reverse(rev_comp_seq.begin(), rev_comp_seq.end());
    return rev_comp_seq;
}

int main() {
    string fastaFile;
    cout << "Enter the path to the input FASTA file: ";
    getline(cin, fastaFile);

    auto result = read_fasta(fastaFile);
    vector<pair<string, string>> sequences = result.first;
    int max_chars_per_line = result.second;

    //// Remove the key-value pairs where the key contains the substring "mitochondrion"
    //sequences.erase(remove_if(sequences.begin(), sequences.end(),
    //    [](const pair<string, string>& p) {
    //        return p.first.find("mitochondrion") != string::npos;
    //    }),
    //    sequences.end());

    string outputFileName = fastaFile + "_REVERSE_STRAND.fasta";
    ofstream outputFile(outputFileName);
    if (!outputFile) {
        cerr << "Failed to create the output file." << endl;
        return 1;
    }

    // Generate reverse complement for each sequence and write to the output file
    for (const auto& entry : sequences) {
        outputFile << ">" << entry.first << " Reverse strand\n";
        string rev_comp_seq = reverseComplement(entry.second);
        for (size_t i = 0; i < rev_comp_seq.length(); i += max_chars_per_line) {
            outputFile << rev_comp_seq.substr(i, max_chars_per_line) << "\n";
        }
    }

    outputFile.close();

    cout << "Reverse complement sequences generated successfully." << endl;
    cout << "Output file: " << outputFileName << endl;

    return 0;
}
