//
// Created by influx on 5/28/23.
//
#include <bits/stdc++.h>
#include "Base.h"
#include "ContextFreeGammer.h"

using namespace std;

#ifndef AUTOMATA_NFA_H
#define AUTOMATA_NFA_H
class NFA
{
public:
    set<State *> Q;
    set<char> alphabet;
    State *q0;
    set<State *> finals;
    map<Transfer *, set<State *>> transferFunctions;

    NFA(const set<State *> &q, const set<char> &alphabet, State *q0, const set<State *> &finals,
        const map<Transfer *, set<State *>> &transferFunctions) : Q(q), alphabet(alphabet), q0(q0), finals(finals), transferFunctions(transferFunctions) {}

    Transfer *search(State *state, char c)
    {
        for (pair<Transfer *, set<State *>> t : transferFunctions)
        {
            if (t.first == nullptr)
                continue;
            if (t.first->q == state && t.first->c == c)
                return t.first;
        }
        return nullptr;
    }

    bool acceptance(string s, int i, State *q)
    {
        if (i == s.size())
            return q->final;

        bool accept = false;
        for (State *state : transferFunctions[search(q, s[i])])
            if (acceptance(s, i + 1, state))
                accept = true;

        return accept;
    }

    ContextFreeGrammer *convertToGFG()
    {
        unordered_set<char> variables;
        unordered_set<char> terminals;
        char startVariable;
        unordered_map<char, unordered_set<string>> productionRules;

        for (State *s : this->Q)
            variables.insert(s->id[0]);

        for (char c : this->alphabet)
            terminals.insert(c);

        startVariable = this->q0->id[0];

        for (pair<Transfer *, set<State *>> tS : this->transferFunctions)
            for (State *sT : tS.second)
                productionRules[tS.first->q->id[0]].insert(string(1, tS.first->c) + string(1, sT->id[0]));

        for (State *s : this->Q)
            if (s->final)
                productionRules[s->id[0]].insert("$");

        return new ContextFreeGrammer(variables, terminals, startVariable, productionRules);
    }
};
#endif // AUTOMATA_NFA_H
