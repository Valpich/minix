/**
 * Project 1 Develop your own shell
 * @author Jules Baud / Valentin Pichavant / Antoine Regnier
 * @version 1.0
 */


#ifndef _COMMAND_MANAGER_H
#define _COMMAND_MANAGER_H

#include <cstdio>
#include <iostream>
#include <memory>
#include <stdexcept>
#include <string>
#include <array>
#include <vector>

#ifndef MINIX

#include <thread>

#endif

#include "Command.h"

using namespace std;

class CommandManager {
public:

    /**
     * Construct a CommandManager instance
     */
    CommandManager();

    /**
     * Destroy a CommandManager instance
     */
    ~CommandManager();

    /**
     * Execute a list of command using Popen with a wait to the threads to join
     * CANNOT BE EXECUTED ON MINIX BECAUSE "pthread.h" IS NOT IMPLEMENTED, WORKS ON UNIX BASED OS
     * @param commands The list of commands to be executed
     */
    void executeInParallelWithWaitAndPopen(vector<Command *> commands);

    /**
     * Execute a list of command using Popen without waiting the threads to join
     * CANNOT BE EXECUTED ON MINIX BECAUSE "pthread.h" IS NOT IMPLEMENTED, WORKS ON UNIX BASED OS
     * @param commands The list of commands to be executed
     */
    void executeInParallelWithoutWaitAndPopen(vector<Command *> commands);

    /**
     * Overload the ostream operator to display as a string a CommandManager
     */
    friend ostream &operator<<(ostream &os, const CommandManager &manager);

private:
};

#endif //_COMMAND_MANAGER_H
