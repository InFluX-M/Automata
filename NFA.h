//
// Created by influx on 5/28/23.
//
#include <bits/stdc++.h>
#include "Base.h"
using namespace std;

#ifndef AUTOMATA_NFA_H
#define AUTOMATA_NFA_H
class TransferNFATODFA
{
public:
    set<State *> q;
    char c;

    TransferNFATODFA(set<State*> q, char c)
    {
        this->q = q;
        this->c = c;
    }
};
class NFATODFA
{
public:
    set<set<State *>> Q;
    set<char> alphabet;
    set<State*> q0;
    set<set<State *>> finals;
    map<TransferNFATODFA *, set<State *>> transferFunctions;

    NFATODFA(set<set<State *>> &q, set<char> &alphabet, set<State*>& q0, set<set<State *>> &finals,
             map<TransferNFATODFA *, set<State *>> &transferFunctions) : Q(q), alphabet(alphabet), q0(std::move(q0)), finals(finals),
    transferFunctions(transferFunctions) {}
};

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
            if(t.first==nullptr)continue;
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

    void setQ0NFAToDFA(set<State*>& q01, State* q)
    {
        for(State* state : this->transferFunctions[search(q, '$')])
        {
            q01.insert(state);
            setQ0NFAToDFA(q01, state);
        }
    }

    TransferNFATODFA* transferNfatodfa1(set<State*>& s, char c, map<TransferNFATODFA *, set<State *>>& transferFunctions1)
    {
        for(pair<TransferNFATODFA *, set<State *>> t : transferFunctions1)
            if(t.first->q == s && t.first->c == c)
                return t.first;

        return nullptr;
    }

    /*DFA *NFAToDFA()
    {
        map<set<State*>, bool> status;
        set<set<State *>> Q1;
        set<char> alphabet1 = alphabet;
        set<State*> q01;
        set<set<State *>> finals1;
        map<TransferNFATODFA *, set<State *>> transferFunctions1;

        q01.insert(q0);
        setQ0NFAToDFA(q01, q0);

        NFATODFA* nfaTodfa = new NFATODFA(Q1, alphabet1, q01, finals1, transferFunctions1);

        nfaTodfa->Q.insert(nfaTodfa->q0);
        queue<set<State*>> queue;
        queue.push(nfaTodfa->q0);

        while (!queue.empty())
        {
            set<State*> t = queue.front();
            status[t] = true;

            for(char c : alphabet1)
            {
                set<State*> all;
                for(State* state : t)
                {
                    for(State* state1 : transferFunctions[search(state, c)])
                        all.insert(state1);
                }

                if(nfaTodfa->Q.find(all) == nfaTodfa->Q.end())
                {
                    nfaTodfa->Q.insert(all);
                    queue.push(all);

                    for(char c1 : alphabet1)
                    {
                        TransferNFATODFA* transferNfatodfa = new TransferNFATODFA(all, c1);
                        nfaTodfa->transferFunctions[transferNfatodfa] = {};
                    }

                    nfaTodfa->transferFunctions[transferNfatodfa1(t, c, transferFunctions1)] = all;
                }
                else
                {
                    nfaTodfa->transferFunctions[transferNfatodfa1(t, c, transferFunctions1)] = all;
                    if(!status[all])
                        queue.push(all);
                }
            }

        }

        cout << "Hi";
    }*/
};
#endif //AUTOMATA_NFA_H
