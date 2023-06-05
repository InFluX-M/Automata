#include <bits/stdc++.h>
#include "/media/influx/Programming/Projects/Automata/Headers/DFA.h"
#include "/media/influx/Programming/Projects/Automata/Headers/NFA.h"
#include "/media/influx/Programming/Projects/Automata/Headers/Base.h"
#include "/media/influx/Programming/Projects/Automata/Headers/RegularExpression.h";
#include "/media/influx/Programming/Projects/Automata/Headers/ContextFreeGammer.h"
using namespace std;

p
int main()
{
    string s = "(a).((a).(b))*.(b)";
    ContextFreeGrammer *aim = CV(s);

    cout << 5;
    return 0;
}

// int main()
// {
//     string s = "(a.b)*.a";
//     RegularExpression *re = new RegularExpression(s);
//     NFA *nfa = re->convertToNFA();
//     ContextFreeGrammer *gfg = nfa->convertToGFG();
//     cout << 5;

//     // unordered_set<char> variables = {'S', 'A', 'B'};
//     // unordered_set<char> terminals = {'a', 'b'};
//     // char startVariable = 'S';
//     // unordered_map<char, unordered_set<string>> productionRules;
//     // productionRules['S'] = {"AB"};
//     // productionRules['A'] = {"a", "BB"};
//     // productionRules['B'] = {"AB", "b"};
//     // ContextFreeGrammer *cfg = new ContextFreeGrammer(variables, terminals, startVariable, productionRules);
//     // cout << cfg->parsingCYK("aab");
//     return 0;
// }
