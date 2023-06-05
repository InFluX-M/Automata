//
// Created by influx on 5/24/23.
//
#include <bits/stdc++.h>
#include "Base.h"
using namespace std;

#ifndef AUTOMATA_DFA_H
#define AUTOMATA_DFA_H
class DFA {
public:
    set<State*> Q;
    set<char> alphabet;
    State* q0;
    set<State*> finals;
    map<Transfer*, State*> transferFunctions;

    DFA(const set<State *> &q, const set<char> &alphabet, State *q0, const set<State *> &finals,
        const map<Transfer *, State *> &transferFunctions) : Q(q), alphabet(alphabet), q0(q0), finals(finals),
                                                             transferFunctions(transferFunctions) {}

    Transfer* search(State* state, char c)
    {
        for (pair<Transfer*, State*> t : transferFunctions)
        {
            if(t.first->q == state && t.first->c == c)
                return t.first;
        }
    }

    bool acceptance(string s)
    {
        State* q = q0;
        for(char i : s)
        {
            Transfer* t = search(q, i);
            q = transferFunctions[t];
        }

        if(q->final)
            return true;
        else
            return false;

    }
};
#endif //AUTOMATA_DFA_H
