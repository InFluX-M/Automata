#include <bits/stdc++.h>
#include "../Headers/DFA.h"
#include "../Headers/NFA.h"
#include "../Headers/Base.h"
#include "../Headers/RegularExpression.h"
#include "../Headers/ContextFreeGammer.h"
using namespace std;

int main()
{
    string s = "((b)+(a))*.((a)+(b))";
    RegularExpression *r = new RegularExpression(s);
    ContextFreeGrammer *g = r->CV(s);
    g->makeContextFreeGrammerForParsing();
    g->makeChomsky();
    while (true)
    {
        string y;
        cin >> y;
        cout << g->parsingCYK(y) << endl;
    }
    return 0;
}