#include <iostream>
#include <fstream>
#include <vector>
#include <iterator>
#include <algorithm>
#include <cassert>
#include <chrono>
#include <thread>

#include "bulk.h"

using namespace std;

int main(int argc, const char **argv)
{
    auto  cmds = make_unique<Commands>( (argc<2) ? 3 : static_cast<size_t>(atoi(argv[1])) );
    auto ConsoleObs = make_shared<ConsoleObserver>();
    ConsoleObs->JustNotConstructor(cmds);

    auto LocalFileObs = std::make_shared<LocalFileObserver>();
    LocalFileObs->JustNotConstructor(cmds);

    string line;
    while (getline(std::cin, line))
    {
        #if (defined WIN32) || (defined WIN64)
            std::this_thread::sleep_for(1.0s);
            cout << line << endl;
        #endif
        cmds->AnalyzeCommand(line);
    }
    cmds->ExecForAllSubs(true);
    return 0;
}