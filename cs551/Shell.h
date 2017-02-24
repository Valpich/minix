/**
 * Project 1 Develop your own shell
 * @author Jules Baud / Valentin Pichavant / Antoine Regnier
 * @version 1.0
 */


#ifndef _SHELL_H
#define _SHELL_H


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

#define TAB_PRESSED 9
#define ENTER_PRESSED 10
#define DOWN_ARROW_PRESSED 258
#define UP_ARROW_PRESSED 259
#define LEFT_ARROW_PRESSED 260
#define RIGHT_ARROW_PRESSED 261
#define DELETE_PRESSED 127
#define ESC_PRESSED 27

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
    Command * command;
public:
    static const string &getName();

    Command *getCommand() const;

    void setCommand(Command *command);

private:
    History * history;
    vector<Command> * commandList;
    CommandFinder * commandFinder;
};

#endif //_SHELL_H
