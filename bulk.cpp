#include <iostream>
#include <cassert>
#include <fstream>
#include <chrono>
#include <thread>

#include "bulk.h"

using namespace std;


void TestFile(const char *file_name)
{
    cout << "File: " << file_name << endl;

    ifstream i_stream = ifstream(file_name);
    if (!i_stream)
    {
        cout << "My error: the input file not found" << endl;
        exit(0);
    }
    auto cmds = make_unique<Commands>(3);
    auto ConsoleObs = make_shared<ConsoleObserver>();
    ConsoleObs->JustNotConstructor(cmds);
    auto LocalFileObs = std::make_shared<LocalFileObserver>();
    LocalFileObs->JustNotConstructor(cmds);
    string line;
    while (getline(i_stream, line))
    {
        std::this_thread::sleep_for(0.6s);
        cout << line << endl;
        cmds->AnalyzeCommand(line);
    }    
    cmds->ExecForAllSubs(true);

    i_stream.close();
    cout << endl << endl;
}