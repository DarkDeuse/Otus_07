#pragma once

#include <iostream>
#include <fstream>

#include <vector>
#include <iterator>
#include <algorithm>
#include <memory>
#include <ctime>


using namespace std;
#if ((defined WIN32) || (defined WIN64)) && (defined _MSC_VER)
#else
#define MY_P_FUNC __PRETTY_FUNCTION__
#endif

#if ((defined NDEBUG) || (defined _NDEBUG))
#define MY_DEBUG_ONLY(x)
#else
#define MY_DEBUG_ONLY(x) (x)
#endif

void TestFile(const char *file_name);
class Observer
{
private:

public:

    virtual void Do(const std::vector<std::string> &cmds, time_t t) = 0;
    virtual ~Observer() = default;
};
class Commands
{
private:
    std::vector<shared_ptr<Observer>> subs;

    size_t N = 3;
    size_t BracketOpenLevel = 0;
    std::vector<std::string> cmds;
    time_t timeFirst;

public:

    Commands(size_t _N) : N(_N) {}
    void subscribe(const shared_ptr<Observer> &obs)
    {
        subs.push_back(obs);
    }

    void AnalyzeCommand(const std::string &str)
    {
        if (str != "{" && str != "}")
        {
            if (cmds.empty())
                timeFirst = std::time(nullptr);
            cmds.push_back(str);
        }
        if (cmds.size() == N && !BracketOpenLevel)
        {
            ExecForAllSubs(false);
        }
        if (str == "{")
        {
            BracketOpenLevel++;
            if (BracketOpenLevel == 1 && !cmds.empty())
                ExecForAllSubs(false);

        }
        if (str == "}")
        {
            BracketOpenLevel--;
            if (BracketOpenLevel == 0 && !cmds.empty())
            {
                ExecForAllSubs(false);
            }
        }

    }

    void ExecForAllSubs(bool isFinished)
    {
        if ( !cmds.empty() &&  ( BracketOpenLevel == 0 || (BracketOpenLevel == 1 && !isFinished) ) )
        {
            for (auto &s : subs)
            {
                s->Do(cmds, timeFirst);
            }

            cmds.clear();
        }
    }

};

class ConsoleObserver : public std::enable_shared_from_this<ConsoleObserver>, public Observer
{
public:
    void JustNotConstructor(const unique_ptr<Commands> &_cmds)
    {
        auto t = shared_from_this();
        _cmds->subscribe(t);
    }

    void Do(const std::vector<std::string> &cmds, [[maybe_unused]]time_t t) override
    {
        std::cout << "bulk: ";
        size_t cmds_size = cmds.size();
        for (size_t i = 0; i < cmds_size; i++)
            std::cout << cmds[i] << (  (i<(cmds_size-1)) ? ", " : "\n");
    }
};

class LocalFileObserver : public Observer, public std::enable_shared_from_this<LocalFileObserver>
{
public:
    void JustNotConstructor(const unique_ptr<Commands> &_cmds)
    {
        _cmds->subscribe(shared_from_this());
    }

    void Do(const std::vector<std::string> &cmds, time_t t) override
    {
        ofstream f( std::string("bulk") + std::to_string(t) + std::string(".log") );

        size_t cmds_size = cmds.size();
        for (size_t i = 0; i < cmds_size; i++)
            f << cmds[i] << std::endl;

        f.close();
    }
};