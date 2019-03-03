// Kevin Sittser
// 525003900
// CSCE 420
// Due: March 3, 2019
// hw3pr2.cpp


#include <iostream>
#include <string>
#include <algorithm>
#include <vector>
#include <unordered_map>
#include <unordered_set>
using namespace std;

struct Literal {
    // variable with sign
    char symbol;
    bool sign;  // whether literal is negated in expression
    enum Value {F,T,UNASSIGNED,NOSIGN};
    Value value(unordered_map<char,Literal::Value>& model) {
        // value according to model, with sign accounted for
        if (model[symbol] == UNASSIGNED || model[symbol] == NOSIGN)
            return model[symbol];
        else if (model[symbol] == T)
            return (sign ? T : F);
        else //if (model[symbol] == F)
            return (sign ? F : T);
    }
    Literal(char sym,bool sn) : symbol(sym),sign(sn)/*,value(UNASSIGNED)*/ {}
};

struct Clause {
    // disjunction of literals
    vector<Literal> literals;
    enum Value {F,T,UNASSIGNED,NOSIGN};
    Clause() : literals(vector<Literal>()) {}
    Clause(vector<Literal> lits) : literals(lits) {}
    Value value(unordered_map<char,Literal::Value>& model) {
        bool allFalse = true;
        for (Literal l : literals) {
            if (l.value(model) == Literal::T)
                return T;
            else if (l.value(model) != Literal::F) {
                allFalse = false;
                break;
            }
        }
        return (allFalse ? F : UNASSIGNED);
    }
};

pair<char,Literal::Value> findPureSymbol(unordered_set<char>& symbols,vector<Clause>& clauses,unordered_map<char,Literal::Value>& model) {
    // find any symbol that has the same sign in all clauses
    
    unordered_map<char,Literal::Value> signPurity;
    for (char c : symbols)
        signPurity[c] = Literal::UNASSIGNED;
    for (Clause c : clauses) {
        if (c.value(model) == Clause::T)
            continue;
        for (Literal l : c.literals) {
            if (symbols.find(l.symbol) == symbols.end())
                continue;
            if (signPurity[l.symbol] == Literal::UNASSIGNED)
                signPurity[l.symbol] = (l.sign ? Literal::T : Literal::F);
            else if (signPurity[l.symbol] == Literal::T || signPurity[l.symbol] == Literal::F)
                if ((l.sign ? Literal::T : Literal::F) != signPurity[l.symbol])
                    signPurity[l.symbol] = Literal::NOSIGN;
        }
    }
    for (pair<char,Literal::Value> p : signPurity)
        if (p.second == Literal::T || p.second == Literal::F)
            return p;
    return make_pair('\0',Literal::NOSIGN);  // return null character if no pure symbols found
}

pair<char,Literal::Value> findUnitClause(vector<Clause>& clauses,unordered_map<char,Literal::Value>& model) {
    // find a clause with no more than one false literal
    
    for (Clause c : clauses) {
        bool nonFalseFound = false;
        Literal lit('\0',Literal::NOSIGN);
        for (Literal l : c.literals)
            if (l.value(model) != Literal::F)
                if (nonFalseFound) {
                    nonFalseFound = false;
                    break;
                }
                else {
                    nonFalseFound = true;
                    lit = l;
                }
        // don't find this clause if updating it wouldn't actually change it 
        if (model[lit.symbol] == lit.sign ? Literal::T : Literal::F)
            continue;
        if (nonFalseFound)
            return make_pair(lit.symbol,(lit.sign ? Literal::T : Literal::F));
    }
    return make_pair('\0',Literal::NOSIGN);
}

bool DPLL(vector<Clause>& clauses,unordered_set<char>& symbols,unordered_map<char,Literal::Value>& model) {
    
    bool allTrue = true, anyFalse = false;
    for (Clause& c : clauses) {
        if (c.value(model) == Clause::F)
            anyFalse = true;
        if (c.value(model) != Clause::T)
            allTrue = false;
    }
    if (allTrue)
        return true;
    if (anyFalse)
        return false;
    pair<char,Literal::Value> pureSym = findPureSymbol(symbols,clauses,model);
    if (pureSym.first != '\0') {
        symbols.erase(pureSym.first);
        model[pureSym.first] = pureSym.second;
        return DPLL(clauses,symbols,model);
    }
    pair<char,Literal::Value> unitClause = findUnitClause(clauses,model);
    if (unitClause.first != '\0') {
        symbols.erase(unitClause.first);
        model[unitClause.first] = unitClause.second;
        return DPLL(clauses,symbols,model);
    }
    char sym = *symbols.begin();
    symbols.erase(symbols.begin());
    unordered_map<char,Literal::Value> model1 = model,model2 = model;
    model1[sym] = Literal::T;
    model2[sym] = Literal::F;
    
    return DPLL(clauses,symbols,model1) || DPLL(clauses,symbols,model2);
}

/*bool DPLL_satisfiable(string s) {
    
}*/

int main() {
    vector<Clause> clauses;
    unordered_set<char> symbols;
    unordered_map<char,Literal::Value> model;
    string ss = "";
    while (true) {  // loop once for each clause
        string s = "";
        while (s.size() == 0 || s.at(s.size()-1) != '.') {
            cin >> ss;
            s += ss;
        }
        vector<Literal> literals;
        
        if (s.at(0) == '?') {
            // parse a query
            literals.push_back(Literal(s.at(1),false));
            if (find(symbols.begin(),symbols.end(),s.at(1)) == symbols.end())
                symbols.insert(s.at(1));
        }
        else if (s.at(0) != ':') {
            // get left side of clause
            literals.push_back(Literal(s.at(0),true));
            if (find(symbols.begin(),symbols.end(),s.at(0)) == symbols.end())
                symbols.insert(s.at(0));
        }
        if (s.at(1) == ':') {
            for (int i=3; i<s.size(); i+=2) {
                literals.push_back(Literal(s.at(i),false));
                if (find(symbols.begin(),symbols.end(),s.at(i)) == symbols.end())
                    symbols.insert(s.at(i));
            }
        }
        else if (s.at(0) == ':') {
            // clauses with no left side
            for (int i=2; i<s.size(); i+=2) {
                literals.push_back(Literal(s.at(i),false));
                if (find(symbols.begin(),symbols.end(),s.at(i)) == symbols.end())
                    symbols.insert(s.at(i));
            }
        }
        clauses.push_back(Clause(literals));
        if (s.at(0) == '?')
            break;
    }
    for (char c : symbols)
        model[c] = Literal::UNASSIGNED;
    cout << (!DPLL(clauses,symbols,model) ? "TRUE" : "FALSE") << endl;
}