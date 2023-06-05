//
// Created by influx on 5/24/23.
//
#include <bits/stdc++.h>
using namespace std;

#ifndef AUTOMATA_BASE_H
#define AUTOMATA_BASE_H
class State {
public:
    string id;
    bool final;

    State(string id, bool final) : id(std::move(id)), final(final) {}
};

class Transfer {
public:
    State* q;
    char c;

    Transfer(State* q, char c)
    {
        this->q = q;
        this->c = c;
    }
};
#endif //AUTOMATA_BASE_H
