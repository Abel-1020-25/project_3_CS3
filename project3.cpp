/**************************************************************************
Class:			CSCI 3333.01 Data Structures and Algorithms
				Spring 2025
Project 3:		Finding Common Patterns from Large Text Documents
Name:			Abel Rodriguez
ID:             20627502
Description: use some combination of hash maps, lists, and sorting techniques 
	to discover the most common phrases and common paragraphs between two 
	large text documents: The King James version of the bible and 
	the Complete Works of William Shakespeare. 
Input file: bible.txt, WilliamShakespeare.txt
Output file: topPhrases.txt
Example format of output file:
	top 10 most frequent phrases of length 1
	phrase  		frequency in Bible		frequency in William Shakespeare
	trout			100						150
	snapper 		600						95	
	......
	top 10 most frequent phrases of length 2
	phrase  		frequency in Bible		frequency in William Shakespeare
	speckled trout	100						150
	red snapper 	600						95
	......
	++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	time used: 27 seconds
**************************************************************************/

#include <fstream>
#include <iomanip>
#include <string>
#include <map>
#include <unordered_map>
#include <vector>
#include <iostream>
#include <sstream>
#include <ctime>
#include <algorithm>

#include "project3_head.h"
using namespace std;

int main() {
    ifstream myfile;
    ofstream outfile;

    unordered_map<string, int> bk1, bk2; // store phrase frequencies
    vector<myPhraseType> topPhrases[10]; // top 10 for N = 1..10
    string str, word;

    time_t start, end;
    start = time(NULL);

    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    /*************************************************************
     * 1. Load the Bible and count all 1–10 word phrases
     *************************************************************/
    cout << "Loading the Bible..." << endl;
    myfile.open("bible.txt");
    bk1.clear();

    while (getline(myfile, str)) {
        clean(str);
        if (str.empty()) continue;

        vector<string> words;
        stringstream ss(str);
        while (ss >> word) words.push_back(word);

        // sliding window for 1–10 word phrases
        for (int n = 1; n <= 10; ++n) {
            if (words.size() < n) break;
            for (size_t i = 0; i + n <= words.size(); ++i) {
                string phrase = words[i];
                for (int j = 1; j < n; ++j)
                    phrase += " " + words[i + j];
                bk1[phrase]++;
            }
        }
    }
    myfile.close();
    cout << "Finished loading the Bible." << endl;

    /*************************************************************
     * 2. Load Shakespeare and count all 1–10 word phrases
     *************************************************************/
    cout << "Loading the Complete Works of William Shakespeare..." << endl;
    myfile.open("WilliamShakespeare.txt");
    bk2.clear();

    while (getline(myfile, str)) {
        clean(str);
        if (str.empty()) continue;

        vector<string> words;
        stringstream ss(str);
        while (ss >> word) words.push_back(word);

        for (int n = 1; n <= 10; ++n) {
            if (words.size() < n) break;
            for (size_t i = 0; i + n <= words.size(); ++i) {
                string phrase = words[i];
                for (int j = 1; j < n; ++j)
                    phrase += " " + words[i + j];
                bk2[phrase]++;
            }
        }
    }
    myfile.close();
    cout << "Finished loading Shakespeare." << endl;

    /*************************************************************
     * 3. Find the top 10 most common phrases for each N = 1..10
     *************************************************************/
    cout << "Finding top phrases..." << endl;

    for (int n = 1; n <= 10; ++n) {
        vector<myPhraseType> topList;

        // check all Bible phrases
        for (auto &entry : bk1) {
            const string &phrase = entry.first;
            if (findNumOfWords(phrase) != n) continue;

            int freq1 = entry.second;
            int freq2 = bk2.count(phrase) ? bk2[phrase] : 0;

            myPhraseType p;
            p.setPhrase(phrase, freq1 + freq2);
            insertPhrase(topList, p);
        }

        // check Shakespeare-only phrases
        for (auto &entry : bk2) {
            const string &phrase = entry.first;
            if (bk1.count(phrase)) continue;
            if (findNumOfWords(phrase) != n) continue;

            myPhraseType p;
            p.setPhrase(phrase, entry.second);
            insertPhrase(topList, p);
        }

        topPhrases[n - 1] = topList;
    }

    cout << "Finished finding top phrases." << endl;

    /*************************************************************
     * 4. Output the top 10 results to a text file
     *************************************************************/
    outfile.open("topPhrases.txt");

    for (int n = 1; n <= 10; ++n) {
        outfile << "Top 10 most frequent phrases of length " << n << "\n";
        outfile << left << setw(35) << "Phrase"
                << setw(20) << "Freq in Bible"
                << setw(20) << "Freq in Shakespeare" << "\n";
        outfile << string(75, '-') << "\n";

        for (auto &p : topPhrases[n - 1]) {
            outfile << left << setw(35) << p.getPhrase()
                    << setw(20) << bk1[p.getPhrase()]
                    << setw(20) << bk2[p.getPhrase()] << "\n";
        }
        outfile << "\n";
    }

    /*************************************************************
     * 5. Record and print the total runtime
     *************************************************************/
    end = time(NULL);
    outfile << "+++++++++++++++++++++++++++++++\n";
    outfile << "Time used: " << difftime(end, start) << " seconds\n";
    outfile.close();

    cout << "All done! Results written to topPhrases.txt" << endl;
    return 0;
}

