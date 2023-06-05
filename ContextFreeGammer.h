//
// Created by influx on 5/24/23.
//
#include <bits/stdc++.h>
#include "Base.h"
using namespace std;

#ifndef AUTOMATA_ContextFreeGrammer_H
#define AUTOMATA_ContextFreeGrammer_H
class ContextFreeGrammer
{
public:
    unordered_set<char> variables;
    unordered_set<char> terminals;
    char startVariable;
    unordered_map<char, unordered_set<string>> productionRules;
    ContextFreeGrammer *contextFreeGrammerForParsing;
    ContextFreeGrammer *chomsky;
    ContextFreeGrammer *greibach;
    bool lambda;

    ContextFreeGrammer(unordered_set<char> variables, unordered_set<char> terminals, char startVariable,
                       unordered_map<char, unordered_set<string>> productionRules)
    {
        this->variables = variables;
        this->terminals = terminals;
        this->startVariable = startVariable;
        this->productionRules = productionRules;
    }

    ContextFreeGrammer(unordered_set<char> variables, unordered_set<char> terminals, char startVariable)
    {
        this->variables = variables;
        this->terminals = terminals;
        this->startVariable = startVariable;
    }

    bool parsingCYK(string s)
    {
        if (chomsky == nullptr)
            makeChomsky();

        unordered_map<string, unordered_set<char>> cyk;
        for (int i = 0; i < s.length(); i++)
            for (pair<char, unordered_set<string>> p : chomsky->productionRules)
                if (p.second.find(string(1, s[i])) != p.second.end())
                    cyk[string(1, s[i])].insert(p.first);

        for (int i = 0; i < s.length(); i++)
            for (int j = i; j < s.length(); j++)
            {
                string m = s.substr(j - i, i + 1);
                for (int k = 1; k < i + 1; k++)
                {
                    string u = m.substr(0, k);
                    string v = m.substr(k);
                    for (char c1 : cyk[u])
                        for (char c2 : cyk[v])
                            for (pair<char, unordered_set<string>> p : chomsky->productionRules)
                                if (p.second.find(string(1, c1) + string(1, c2)) != p.second.end())
                                    cyk[m].insert(p.first);
                }
            }

        if (cyk[s].find(chomsky->startVariable) != cyk[s].end())
            return true;
        else
            return false;
    }

    // bool parsingRegular(string s)
    // {
    // }

    // private:
    char findVariable()
    {
        for (int i = 'A'; i <= 'Z'; i++)
            if (chomsky->variables.find(char(i)) == chomsky->variables.end())
                return char(i);
    }

    void removeLambda()
    {
        ContextFreeGrammer *cFG = this->contextFreeGrammerForParsing;
        cFG->lambda = productionRules[startVariable].find("$") != cFG->productionRules[startVariable].end();
        for (char variable : cFG->variables)
            if (cFG->productionRules[variable].find("$") != cFG->productionRules[variable].end())
                for (pair<char, unordered_set<string>> prRule : cFG->productionRules)
                {
                    unordered_set<string> newRule;
                    for (string s : prRule.second)
                    {
                        int pos = 0;
                        while (s.find(variable, pos) != -1)
                        {
                            string s1 = s.substr(0, s.find(variable, pos));
                            string s2 = s.substr(s.find(variable, pos) + 1);
                            pos = s.find(variable, pos) + 1;
                            string fin = s1 + s2;
                            if (fin.empty() || (fin.length() == 1 && fin[0] == prRule.first))
                                continue;
                            newRule.insert(fin);
                        }
                        if (s == "$" || (s.length() == 1 && s[0] == prRule.first))
                            continue;
                        newRule.insert(s);
                    }
                    cFG->productionRules[prRule.first] = newRule;
                }
    }

    void removeNotAvailable()
    {
        unordered_set<char> vTemp;

        queue<char> q;
        q.push(this->startVariable);

        while (!q.empty())
        {
            char u = q.front();
            q.pop();
            vTemp.insert(u);

            for (string s : productionRules[u])
                for (char c : s)
                    if (vTemp.find(c) == vTemp.end() && variables.find(c) != variables.end())
                        q.push(c);
        }

        contextFreeGrammerForParsing = new ContextFreeGrammer(vTemp, terminals, startVariable);

        for (char v : variables)
            if (vTemp.find(v) != vTemp.end())
                contextFreeGrammerForParsing->productionRules[v] = productionRules[v];
    }

    void removeUnitRules()
    {
        ContextFreeGrammer *cFG = this->contextFreeGrammerForParsing;
        for (char v : cFG->variables)
            for (string s : cFG->productionRules[v])
                if (s.size() == 1 && cFG->variables.find(s[0]) != cFG->variables.end())
                    for (pair<char, unordered_set<string>> prRule : cFG->productionRules)
                    {
                        unordered_set<string> newRule;
                        for (string su : prRule.second)
                        {
                            int pos = 0;
                            while (su.find(v, pos) != -1)
                            {
                                string s1 = su.substr(0, su.find(v, pos));
                                string s2 = su.substr(su.find(v, pos) + 1);
                                pos = su.find(v, pos) + 1;
                                string fin = s1 + s + s2;
                                newRule.insert(fin);
                            }
                            newRule.insert(su);
                        }
                        cFG->productionRules[prRule.first] = newRule;
                    }

        for (char v : cFG->variables)
        {
            unordered_set<string> tmp;
            for (string s : cFG->productionRules[v])
                if (s.size() == 1 && cFG->variables.find(s[0]) != cFG->variables.end())
                    tmp.insert(s);

            for (string s : tmp)
                cFG->productionRules[v].erase(s);
        }
    }

    void removeEmpty()
    {
        unordered_set<char> vNew;
        unordered_map<char, unordered_set<string>> prNew;
        unordered_set<char> vEmpties;

        for (char c : this->contextFreeGrammerForParsing->variables)
        {
            if (!this->contextFreeGrammerForParsing->productionRules[c].empty())
                vNew.insert(c);
        }

        for (pair<char, unordered_set<string>> pr : this->contextFreeGrammerForParsing->productionRules)
        {
            for (string s : pr.second)
            {
                bool flag = false;
                for (char c : s)
                {
                    if (variables.find(c) != variables.end() &&
                        vNew.find(c) == vNew.end())
                    {
                        flag = true;
                        break;
                    }
                }

                if (!flag)
                    prNew[pr.first].insert(s);
            }
        }

        this->contextFreeGrammerForParsing->variables = vNew;
        this->contextFreeGrammerForParsing->productionRules = prNew;
    }

    void makeContextFreeGrammerForParsing()
    {
        removeNotAvailable();
        removeLambda();
        removeUnitRules();
        removeEmpty();
    }

    void makeChomsky()
    {
        makeContextFreeGrammerForParsing();
        this->chomsky = new ContextFreeGrammer(variables, terminals, startVariable);
        this->chomsky->lambda = this->lambda;

        int tIdx = 0, vIdx = 0;
        unordered_map<char, char> TtoV;
        for (char t : terminals)
        {
            char u = findVariable();
            variables.insert(u);
            TtoV[t] = u;
            this->chomsky->productionRules[u].insert(string(1, t));
            chomsky->variables.insert(u);
        }

        ContextFreeGrammer *CFP = this->contextFreeGrammerForParsing;
        for (pair<char, unordered_set<string>> v : CFP->productionRules)
        {
            for (string s : v.second)
            {
                for (int i = 0; s.length() > 1 && i < s.length(); i++)
                {
                    if (CFP->terminals.find(s[i]) != CFP->terminals.end())
                        s[i] = TtoV[s[i]];
                }

                char lst;
                for (int i = 0; s.length() > 2 && i < s.length() - 1; i++)
                {
                    if (i == 0)
                    {
                        lst = findVariable();
                        chomsky->productionRules[v.first].insert(string(1, s[i]) + lst);
                    }
                    else if (i == s.length() - 2)
                    {
                        chomsky->productionRules[lst].insert(string(1, s[s.length() - 2]) + string(1, s[s.length() - 1]));
                        chomsky->variables.insert(lst);
                    }
                    else
                    {
                        chomsky->variables.insert(lst);
                        char nLst = findVariable();
                        chomsky->productionRules[lst].insert(string(1, s[i]) + string(1, nLst));
                        lst = nLst;
                    }
                }

                if (s.length() <= 2)
                    chomsky->productionRules[v.first].insert(s);
            }
        }
    }
};

#endif // AUTOMATA_ContextFreeGrammer_H
