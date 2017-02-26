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
#define DELETE_PRESSED 263 
#define ESC_PRESSED 27

using namespace std;

class Shell {
public:

    /**
     * A string array which represent name of the shell
     */
    static const string name;

    /**
     * Construct a Shell instance
     */
    Shell();

    /**
     * Destroy a Shell instance
     */
    ~Shell();

    /**
     * @return The profile used by the shell
     */
    Profile * getProfile();

    /**
     * Set the profile and remove the previous one if exists
     * @param value The new value of the profile
     */
    void setProfile(Profile * value);

    /**
     * @return The history used by the shell
     */
    History * getHistory();

    /**
      * Set the history and remove the previous one if exists
      * @param value The new value of the history
      */
    void setHistory(History * value);

    /**
     * @return The available command list of commands used by the shell
     */
    vector<Command> * getCommandList();

    /**
      * Set the coammd list and remove the previous one if exists
      * @param value The new value of the command list
      */
    void setCommandList(vector<Command> * value);

    /**
     * @return The command finder used by the shell
     */
    CommandFinder * getCommandFinder();

    /**
      * Set the command finder and remove the previous one if exists
      * @param value The new value of the command finder
      */
    void setCommandFinder(CommandFinder * value);

    /**
     * @return The name of the shell
     */
    static const string &getName();

    /**
     * @return The command currently used by the shell
     */
    Command *getCommand() const;

    /**
      * Set the command and remove the previous one if exists
      * @param value The new value of the command
      */
    void setCommand(Command *command);

    /**
     * @return The result of the running execution of the shell
     */
    bool run();

private: 
    Profile * profile;
    Command * command;
    History * history;
    vector<Command> * commandList;
    CommandFinder * commandFinder;
};

#endif //_SHELL_H
