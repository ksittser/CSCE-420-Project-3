// Kevin Sittser
// 525003900
// CSCE 420
// Due: March 3, 2019
// hw3pr1.cpp


#include <iostream>
#include <string>
#include <unordered_map>
#include <algorithm>
#include <sstream>
#include <vector>
using namespace std;

bool checkSum(string str1,string str2,string sumStr,unordered_map<char,int> soln) {
    // check if letters chosen so far are valid in equation, based on solving digit sums from right to left; if reaches a letter whose value hasn't been determined yet, also returns true
    bool carry1 = false;
    for (int i=0; i<sumStr.size(); i++) {
        if ((i < str1.size() && soln[str1.at(str1.size()-1-i)] == -1) || (i < str2.size() && soln[str2.at(str2.size()-1-i)] == -1) || soln[sumStr.at(sumStr.size()-1-i)] == -1)
            // if reaches a letter with undetermined value
            return true;
        int sum = carry1;
        if (i < str1.size())
            sum += soln[str1.at(str1.size()-1-i)];
        if (i < str2.size())
            sum += soln[str2.at(str2.size()-1-i)];
        if (sum % 10 != soln[sumStr.at(sumStr.size()-1-i)])
            return false;
        carry1 = (sum > 9);
    }
    return true;
}

string sortByPosition(string toSort,string str1,string str2,string sumStr) {
    // sort chars in input string by their positions in the equation, from left to right
    string sorted = "";
    for (int i=0; i<sumStr.size(); i++) {
        if (i < str1.size() && sorted.find(str1.at(str1.size()-1-i)) == string::npos)
            sorted += str1.at(str1.size()-1-i);
        if (i < str2.size() && sorted.find(str2.at(str2.size()-1-i)) == string::npos)
            sorted += str2.at(str2.size()-1-i);
        if (sorted.find(sumStr.at(sumStr.size()-1-i)) == string::npos)
            sorted += sumStr.at(sumStr.size()-1-i);
    }
    return sorted;
}

string getDistinctLetters(string str) {
    // return string containing all distinct letters in str, alphabetized
    sort(str.begin(),str.end());
    string distLetters = "";
    for (char c : str)
        if (!distLetters.size() || c != distLetters.at(distLetters.size()-1))
            distLetters += c;
    return distLetters;
}

bool checkSolution(string str1,string str2,string sumStr,unordered_map<char,int> soln) {
    // check if solution is valid
    
    // first letter of word shouldn't be zero
    if (soln[str1.at(0)] == 0 || soln[str2.at(0)] == 0 || soln[sumStr.at(0)] == 0)
        return false;
    string str1New = "", str2New = "", sumStrNew = "";
    for (char c : str1)
        str1New += to_string(soln[c]);
    for (char c : str2)
        str2New += to_string(soln[c]);
    for (char c : sumStr)
        sumStrNew += to_string(soln[c]);
    stringstream ss;
    ss << str1New << " " << str2New << " " << sumStrNew;
    int int1,int2,sumInt;
    ss >> int1 >> int2 >> sumInt;
    return (int1 + int2 == sumInt);
}
//int digitsTried = 0;
unordered_map<char,int> solveStrs(string str1,string str2,string sumStr,unordered_map<char,int>& result,string distLetters,int letterIdx) {
    // recursive function to find solution to cryptarithmetic problem
    
    for (int i=0; i<=9; i++) {  // for each digit 0-9
        // if third string is longer than both of others, its first digit must be 1
        if (distLetters.at(letterIdx) == sumStr.at(0) && sumStr.size() > str1.size() && sumStr.size() > str2.size())
            if (i != 1)
                continue;
        // if letter is in bottom row and letters above it are already assigned, this letter must be sum of those values, or 1 higher due to carrying
        for (int j=0; j<min(str1.size(),str2.size()); j++) {
            if (sumStr.at(sumStr.size()-1-j) == distLetters.at(letterIdx) && result[str1.at(str1.size()-1-j)] > -1 && result[str2.at(str2.size()-1-j)] > -1)
                if (i != result[str1.at(str1.size()-1-j)] + result[str2.at(str2.size()-1-j)] && i != result[str1.at(str1.size()-1-j)] + result[str2.at(str2.size()-1-j)] +1)
                    continue;
        }
        bool alreadyUsed = false;
        // don't use a number that is already assigned to a letter
        for (char c : distLetters.substr(0,letterIdx+1))
            if (result[c] == i) {
                alreadyUsed = true;
                break;
            }
        if (!alreadyUsed) {
            result[distLetters.at(letterIdx)] = i;
            if (!checkSum(str1,str2,sumStr,result))  // if sum check fails
                result[distLetters.at(letterIdx)] = -1;
            else if (letterIdx == distLetters.size()-1) {  // if last letter has been assigned
                if (checkSolution(str1,str2,sumStr,result))  // solution found!
                    return result;
                else
                    result[distLetters.at(letterIdx)] = -1;
            }
            else {  // if letter successfully inserted, try the next letter
                //digitsTried++;
                unordered_map<char,int> resTemp = solveStrs(str1,str2,sumStr,result,distLetters,letterIdx+1);
                if (resTemp != unordered_map<char,int>())
                    return resTemp;
                else
                    result[distLetters.at(letterIdx)] = -1;
            }
        }
    }
    return unordered_map<char,int>();
}

int main() {
    string str1,str2,sumStr;
    cin >> str1 >> str2 >> sumStr;
    unordered_map<char,int> result;
    string distLetters = sortByPosition(getDistinctLetters(str1+str2+sumStr),str1,str2,sumStr);
    for (char c : distLetters)
        result[c] = -1;
    result = solveStrs(str1,str2,sumStr,result,distLetters,0);
    if (result == unordered_map<char,int>())
        cout << "No solution" << endl;
    else {
        for (int i=0; i<sumStr.size()-str1.size(); i++)
            cout << " ";
        for (char c : str1)
            cout << result[c];
        cout << endl;
        for (int i=0; i<sumStr.size()-str2.size(); i++)
            cout << " ";
        for (char c : str2)
            cout << result[c];
        cout << endl;
        for (char c : sumStr)
            cout << result[c];
        cout << endl;
        //cout << "digitsTried: " << digitsTried << endl;
    }
}