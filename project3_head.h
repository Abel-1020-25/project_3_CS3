/**************************************************************************
Class:			CSCI 3333.01 Data Structures and Algorithms
				Spring 2025
Project 3:		Finding Common Patterns from Large Text Documents
Name:			Abel Rodriguez
ID:             20627502
Description:	In this header file for Project 3, we define myPhraseType, 
				and several functions. 
**************************************************************************/
#ifndef HW2
#define HW2

#include <iostream>
#include <fstream>
#include <iomanip>
#include <string>
#include <map>
#include <vector>
#include <algorithm>
#include <sstream>
#include <cctype>

using namespace std;

/************************************************************
 * Function: findNumOfWords
 * Purpose : Count how many words exist in a string
 ************************************************************/
int findNumOfWords(string str) {
    stringstream ss(str);
    string w;
    int count = 0;
    while (ss >> w) count++;
    return count;
}

/************************************************************
 * Function: takeOutStangers
 * Purpose : Remove non-alphanumeric characters (except space)
 ************************************************************/
void takeOutStangers(string& str) {
    for (char& c : str) {
        if (!isalnum(static_cast<unsigned char>(c)) && c != ' ')
            c = ' ';
    }
}

/************************************************************
 * Function: clean
 * Purpose : Normalize text:
 *           - remove punctuation
 *           - lowercase all characters
 *           - trim leading/trailing spaces
 ************************************************************/
void clean(string& str) {
    takeOutStangers(str);
    transform(str.begin(), str.end(), str.begin(), ::tolower);

    // trim leading spaces
    while (!str.empty() && isspace(static_cast<unsigned char>(str.front())))
        str.erase(str.begin());

    // trim trailing spaces
    while (!str.empty() && isspace(static_cast<unsigned char>(str.back())))
        str.pop_back();
}

/************************************************************
 * Class: myPhraseType
 * Purpose: store a phrase, its frequency, and word count
 ************************************************************/
class myPhraseType {
private:
    string str;      // phrase
    int count;       // frequency
    int numOfWords;  // number of words in phrase

public:
    myPhraseType() : str(""), count(0), numOfWords(0) {}
    myPhraseType(const myPhraseType& p) = default;

    // set phrase and its frequency
    void setPhrase(string s, int c) {
        str = s;
        count = c;
        numOfWords = findNumOfWords(s);
    }

    // assignment operator
    myPhraseType& operator=(const myPhraseType& p) = default;

    // comparison operators for sorting
    bool operator<(const myPhraseType& p) const { return count < p.count; }
    bool operator<=(const myPhraseType& p) const { return count <= p.count; }
    bool operator==(const myPhraseType& p) const { return str == p.str; }

    // getters
    int getNumOfWords() const { return numOfWords; }
    int getFreq() const { return count; }
    string getPhrase() const { return str; }

    void clear() { str.clear(); count = numOfWords = 0; }
};

/************************************************************
 * Function: insertPhrase
 * Purpose : Maintain a vector of top 10 phrases in descending
 *           order by frequency.
 ************************************************************/
void insertPhrase(vector<myPhraseType>& list, const myPhraseType& p) {
    // check if phrase already exists in the list
    for (auto& item : list) {
        if (item.getPhrase() == p.getPhrase()) {
            // if it already exists, update frequency if higher
            if (p.getFreq() > item.getFreq())
                item = p;
            sort(list.begin(), list.end(),
                 [](const myPhraseType& a, const myPhraseType& b) {
                     return a.getFreq() > b.getFreq();
                 });
            return;
        }
    }

    // otherwise insert and sort
    list.push_back(p);
    sort(list.begin(), list.end(),
         [](const myPhraseType& a, const myPhraseType& b) {
             return a.getFreq() > b.getFreq();
         });

    // keep only top 10
    if (list.size() > 10)
        list.pop_back();
}

#endif
