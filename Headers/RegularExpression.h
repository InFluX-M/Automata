//
// Created by influx on 5/28/23.
//
#include <bits/stdc++.h>
#include "Base.h"
#include "NFA.h"
#include "ContextFreeGammer.h";

using namespace std;

#ifndef AUTOMATA_RegularExpression_H
#define AUTOMATA_RegularExpression_H
class RegularExpression
{
public:
    string regex;
    RegularExpression(string s)
    {
        this->regex = s;
    }

    pair<vector<string>, vector<char>> process(string s)
    {
        vector<string> a;
        vector<char> op;

        stack<char> st;
        st.push(s[0]);

        string temp;
        for (int i = 1; i < s.length(); i++)
        {
            if (s[i] == ')')
            {
                st.pop();
                if (st.empty())
                {
                    a.push_back(temp);
                    temp.clear();
                }
                else
                {
                    temp.push_back(')');
                }
            }
            else if (s[i] == '(')
            {
                if (!st.empty())
                    temp.push_back('(');

                st.push('(');
            }
            else if (st.empty())
                op.push_back(s[i]);
            else
                temp.push_back(s[i]);
        }

        return make_pair(a, op);
    }

    void kleeneStar(ContextFreeGrammer *cfg)
    {
        unordered_map<char, unordered_set<string>> productionRules;

        for (pair<char, unordered_set<string>> p : cfg->productionRules)
        {
            for (string s : p.second)
            {
                if (s.size() == 1)
                {
                    productionRules[p.first].insert(s + cfg->startVariable);
                }
                productionRules[p.first].insert(s);
            }
        }
        productionRules[cfg->startVariable].insert("$");
        cfg->productionRules = productionRules;
    }

    char findFreeVariable(unordered_set<char> v1, unordered_set<char> v2)
    {
        for (int i = 'A'; i <= 'Z'; i++)
            if (v1.find(char(i)) == v1.end() && v2.find(char(i)) == v2.end())
                return char(i);
    }

    void kleeneConcat(ContextFreeGrammer *cfg, ContextFreeGrammer *cfg1)
    {
        for (char t : cfg1->terminals)
            cfg->terminals.insert(t);
        for (char v : cfg1->variables)
        {
            if (cfg->variables.find(v) != cfg->variables.end())
            {
                char nVar = findFreeVariable(cfg->variables, cfg1->variables);
                cfg->variables.insert(nVar);
                if (cfg1->startVariable == v)
                    cfg1->startVariable = nVar;

                for (pair<char, unordered_set<string>> p : cfg1->productionRules)
                {
                    for (string s : p.second)
                    {
                        if (s.back() == v)
                        {
                            s[s.size() - 1] = nVar;
                        }
                    }
                }

                unordered_set<string> ss = cfg1->productionRules[v];
                cfg1->productionRules.erase(v);
                cfg1->productionRules[nVar] = ss;
            }
            else
                cfg->variables.insert(v);
        }

        unordered_map<char, unordered_set<string>> productionRules;
        for (pair<char, unordered_set<string>> p : cfg->productionRules)
        {
            for (string s : p.second)
            {
                if (s.size() == 1 && (cfg->terminals.find(s[0]) != cfg->terminals.end() || s[0] == '$'))
                {
                    if (s == "$")
                        productionRules[p.first].insert(string(1, cfg1->startVariable));
                    else
                        productionRules[p.first].insert(s + string(1, cfg1->startVariable));
                }
                else
                    productionRules[p.first].insert(s);
            }
        }

        for (pair<char, unordered_set<string>> p : cfg1->productionRules)
        {
            for (string s : p.second)
            {
                productionRules[p.first].insert(s);
            }
        }

        cfg->productionRules = productionRules;
    }

    void kleeneChoose(ContextFreeGrammer *cfg, ContextFreeGrammer *cfg1)
    {
        for (char t : cfg1->terminals)
            cfg->terminals.insert(t);
        for (char v : cfg1->variables)
        {
            if (cfg->variables.find(v) != cfg->variables.end())
            {
                char nVar = findFreeVariable(cfg->variables, cfg1->variables);
                cfg->variables.insert(nVar);
                if (cfg1->startVariable == v)
                    cfg1->startVariable = nVar;
                cfg1->variables.erase(v);
                cfg1->variables.insert(nVar);

                for (pair<char, unordered_set<string>> p : cfg->productionRules)
                {
                    for (string s : p.second)
                    {
                        if (s.back() == v)
                        {
                            s[s.size() - 1] = nVar;
                        }
                    }
                }
            }
            else
                cfg->variables.insert(v);
        }

        for (pair<char, unordered_set<string>> p : cfg1->productionRules)
        {
            for (string s : p.second)
            {
                cfg->productionRules[p.first].insert(s);
            }
        }

        char nStart = findFreeVariable(cfg->variables, cfg1->variables);

        char s1 = cfg->startVariable, s2 = cfg1->startVariable;
        cfg->startVariable = nStart;
        cfg->variables.insert(nStart);

        cfg->productionRules[nStart].insert(string(1, s1));
        cfg->productionRules[nStart].insert(string(1, s2));
    }

    ContextFreeGrammer *CV(string s)
    {
        if (s.length() == 1)
        {
            unordered_set<char> variables;
            unordered_set<char> terminals;
            char startVariable = 'A';
            unordered_map<char, unordered_set<string>> productionRules;
            variables.insert('A');
            terminals.insert(s[0]);
            startVariable = 'A';
            productionRules['A'].insert(string(1, s[0]));
            ContextFreeGrammer *cfg = new ContextFreeGrammer(variables, terminals, startVariable, productionRules);
            return cfg;
        }
        pair<vector<string>, vector<char>> p = process(s);

        vector<ContextFreeGrammer *> grammers;
        for (string r : p.first)
        {
            grammers.push_back(CV(r));
        }

        for (int i = 0; i < grammers.size(); i++)
        {
            if (p.second[i] == '*')
            {
                kleeneStar(grammers[i]);
                p.second.erase(p.second.begin() + i);
            }
        }

        for (int i = 0; i < grammers.size() && grammers.size() > 1; i++)
        {
            if (p.second[i] == '.')
            {
                kleeneConcat(grammers[i], grammers[i + 1]);
                p.second.erase(p.second.begin() + i);
                grammers.erase(grammers.begin() + i + 1);
                i--;
            }
        }

        for (int i = 0; i < grammers.size(); i++)
        {
            if (p.second[i] == '+')
            {
                kleeneChoose(grammers[i], grammers[i + 1]);
                p.second.erase(p.second.begin() + i);
                grammers.erase(grammers.begin() + i + 1);
                i--;
            }
        }

        return grammers[0];
    }
};
#endif // AUTOMATA_RegularExpression_H
