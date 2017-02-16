/**
 * Project 1 Develop your own shell
 * @author Jules Baud / Valentin Pichavant / Antoine Regnier
 * @version 1.0
 */


#ifndef _SHELL_H
#define _SHELL_H

#define TEST 1

#ifdef TEST
#include "Test.h"
#endif

#include <iostream>
#include <string>
#include <vector>
#include <csetjmp>
#include <csignal>

#include <curses.h>

#include "Profile.h"
#include "History.h"
#include "Command.h"
#include "CommandFinder.h"
#include "FileManager.h"

using namespace std;

class Shell {
public:

    static const string name;

    Shell();

    ~Shell();

    Profile * getProfile();
    
    /**
     * @param value
     */
    void setProfile(Profile * value);
    
    History * getHistory();

    /**
     * @param value
     */
    void setHistory(History * value);
    
    vector<Command> * getCommandList();
    
    /**
     * @param value
     */
    void setCommandList(vector<Command> * value);
    
    CommandFinder * getCommandFinder();
    
    /**
     * @param value
     */
    void setCommandFinder(CommandFinder * value);

    bool run();

private: 
    Profile * profile;
    History * history;
    vector<Command> * commandList;
    CommandFinder * commandFinder;
};

#endif //_SHELL_H
