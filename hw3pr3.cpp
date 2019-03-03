// Kevin Sittser
// 525003900
// CSCE 420
// Due: March 3, 2019
// hw3pr3.cpp


#include <iostream>
#include <string>
#include <algorithm>
#include <vector>
#include <cmath>
#include <sstream>
using namespace std;

string ullToBinaryString(unsigned long long orig) {
    // convert ull to string of its binary representation
    
    string res = "";
    if (orig == 0)
        res = "0";
    for (unsigned long long i=(orig > pow(2,63) ? pow(2,63) : pow(2,(int)log2(orig))); i>=1; i/=2) {
        // ternary here because for some reason log2(2^64-1) (for clause being 6 variables ORed together) was giving 64 instead of 63-- probably log2 didn't work right with ull
        if (orig >= i) {
            res += "1";
            orig -= i;
        }
        else
            res += "0";
    }
    return res;
}

unsigned long long findTruth(string disj,vector<pair<char,unsigned long long>>& vars) {
    // output bitmask of which variable combinations are true for the given disjunction
    
    unsigned long long disjTruth = 0;
    vector<string> literals;
    
    // convert string to list of literals (X or -X)
    for (char& c : disj)
        if (c == '+')
            c = ' ';
    stringstream ss;
    string s;
    for (int i=0; i<count(disj.begin(),disj.end(),' ')+1; i++)
        ss << disj << " ";
    for (int i=0; i<count(disj.begin(),disj.end(),' ')+1; i++) {
        ss >> s;
        literals.push_back(s);
    }
    
    // create bitmask
    for (string lit : literals) {
        int idx = -1;
        for (int i=0; i<vars.size(); i++)
            if (vars.at(i).first == lit.at(lit.size()-1)) {
                idx = i;
                break;
            }
        if (lit.at(0) == '-')
            disjTruth |= ((unsigned long long)pow(2,pow(2,vars.size()))-1 - vars.at(idx).second);
        else
            disjTruth |= vars.at(idx).second;
    }
    return disjTruth;
}

int main() {
    string ss;
    vector<string> formula;
    vector<pair<char,unsigned long long>> vars;
    
    // read in
    while (getline(cin,ss)) {
        string s = "";
        for (char c : ss)
            if (!isspace(c))
                s += c;
        formula.push_back(s);
        for (char c : s) {
            bool found = false;
            for (pair<char,unsigned long long> p : vars)
                if (p.first == c) {
                    found = true;
                    break;
                }
            if (c != '+' && c != '-' && !found)
                vars.push_back(make_pair(c,0));
        }
    }
    sort(vars.begin(),vars.end());
    
    // set truth table columns for each var (e.g., for vars P,Q, P is 0011 and Q is 0101)
    for (int i=0; i<vars.size(); i++)
        for (int j=0; j<pow(2,vars.size()); j++) {
            if ( (j / ((int)pow(2,vars.size())/2/(int)pow(2,i))) % 2)
                vars.at(i).second |= ((unsigned long long)1 << (unsigned long long)(pow(2,vars.size())-1-j));
        }
    
    // find truth value for each disjunction and AND together
    unsigned long long formulaTruth = (unsigned long long)pow(2,pow(2,vars.size()))-1;
    for (string disj : formula)
        formulaTruth &= findTruth(disj,vars);
    string formulaTruthString = ullToBinaryString(formulaTruth);
    for (char& c : formulaTruthString)
        if (c == '0')
            c = 'F';
        else if (c == '1')
            c = 'T';
    while (formulaTruthString.size() < pow(2,vars.size()))
        formulaTruthString = "F" + formulaTruthString;
    
    // print table
    for (pair<char,unsigned long long> c : vars)
        cout << c.first << "  ";
    cout << "Formula" << endl;
    for (pair<char,unsigned long long> c : vars)
        cout << "---";
    cout << "-------" << endl;
    for (int i=0; i<pow(2,vars.size()); i++) {
        string row = ullToBinaryString(i);
        for (char& c : row)
            if (c == '0')
                c = 'F';
            else if (c == '1')
                c = 'T';
        while (row.size() < vars.size())
            row = "F" + row;
        for (char c : row)
            cout << c << "  ";
        cout << "   " << formulaTruthString.at(i) << endl;
    }
}