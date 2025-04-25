#ifndef RECCHECK
// For debugging
#include <iostream>
// For std::remove
#include <algorithm> 
#include <map>
#include <set>
#include <vector>
#endif

#include "wordle.h"
#include "dict-eng.h"
using namespace std;


// Add prototypes of helper functions here
/*
bool isValid(std::vector<char> requiredLetters, std::map<int, char> keyWord, std::string entryWord, int correctSize) {
    std::map<int, char>::iterator it;
    if (entryWord.length() != correctSize) { //Check if length is the same
        return false;
    }
    for (it = keyWord.begin(); it != keyWord.end(); it++) { //Check if entryWord has letters in correct placement
        int idx = it->first;
        char correctChar = it->second;
        if (entryWord[idx] != correctChar) {
            return false;
        }
    }
    std::vector<bool> marked;
    for (size_t i = 0; i < entryWord.length(); i++) {
        marked.push_back(false);
    }
    for (size_t j = 0; j < requiredLetters.size(); j++) { //Ensures required letters are present
        char currChar = requiredLetters[j];
        bool found = false;
        for (size_t i = 0; i < entryWord.length(); i++) {
            if (entryWord[i] == currChar && marked[i] != true) {
                found = true;
                marked[i] = true;
                break;
            }
        }
        if (found == false) {
            return false;
        }
    }
    return true;
}*/
void recursiveWordle(std::string currWord, const std::string& in, std::vector<char>& requiredLetters, const std::set<std::string>& dict, std::set<std::string>& output, int idx) {
    if (requiredLetters.size() > (in.size() - idx)) {
        return;
    }

    if (idx == in.size()) {
        if (currWord.length() == in.length() && dict.find(currWord) != dict.end() && requiredLetters.empty()) {
            output.insert(currWord);
        }
        return;
    }
    if (in[idx] != '-') { //If the in[idx] is equal to a valid letter, add that letter to currWord
        currWord.push_back(in[idx]);
        recursiveWordle(currWord, in, requiredLetters, dict, output, idx + 1);
        currWord.pop_back();
    }
    else {
        for (char currChar = 'a'; currChar <= 'z'; currChar++) { //Iterate through alphabet
            bool isPresent = false;
            auto it = std::find(requiredLetters.begin(), requiredLetters.end(), currChar);
            if (it != requiredLetters.end()) {
                requiredLetters.erase(it);
                isPresent = true;
            }
            currWord.push_back(currChar);
            recursiveWordle(currWord, in, requiredLetters, dict, output, idx + 1);
            currWord.pop_back();
            if (isPresent == true) {
                requiredLetters.insert(it, currChar);
            }
        }
    }
}

// Definition of primary wordle function
std::set<std::string> wordle(
    const std::string& in,
    const std::string& floating,
    const std::set<std::string>& dict)
{
    // Add your code here
    std::set<std::string> output;
    std::vector<char> requiredLetters; //Keeps track of what letters are required
    for (size_t i = 0; i < floating.length(); i++) {
        requiredLetters.push_back(floating[i]);
    }
    recursiveWordle("", in, requiredLetters, dict, output, 0);
    return output;

    //Disregard everything below here. I found out much later in my process that I was limited in how I solved this/I didn't read the instructions carefully enough. Though it would've worked this way too
    /*
    std::map<int, char> keyWord; //Puts in the location of the non-dash character in first and the actual character in second
    for (size_t i = 0; i < in.length(); i++) {
        if(in[i] != '-') {
            keyWord[i] = in[i];
        }
    }
    int correctSize = in.length();
    std::set<std::string>::iterator it;
    for (it = dict.begin(); it != dict.end(); it++) {
        std::string currWord = *it;
        if (isValid(requiredLetters, keyWord, currWord, correctSize) == true) {
            output.insert(currWord);
        }
    }
    return output;*/
}

// Define any helper functions here
