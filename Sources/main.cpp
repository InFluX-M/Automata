#include <bits/stdc++.h>
#include "../Headers/DFA.h"
#include "../Headers/NFA.h"
#include "../Headers/Base.h"
#include "../Headers/RegularExpression.h"
#include "../Headers/ContextFreeGammer.h"
#include <iostream>
#include <fstream>
#include <string>

using namespace std;

int main()
{
    cout << "Input Regex: :))))\n";
    
    string regex;
    cin >> regex;

    RegularExpression *r = new RegularExpression(regex);
    ContextFreeGrammer *g = r->CV(regex);

    ofstream myfile;
    myfile.open("../Files/results.txt");

    string line;
    ifstream myfiles("../Files/strings.txt");
    if (myfiles.is_open())
    {
        while (getline(myfiles, line))
        {
            if (g->parsingCYK(line))
                myfile << "\"" + line + "\"" + "\n";
            else
                myfile << line + "\n";
        }
        myfiles.close();
        myfile.close();
    }
    else
        cout << "Unable to open file";

    return 0;
}