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
#ifdef MINIX
#include <thread>
#endif
#include "Command.h"

using namespace std;

class CommandManager {
public:

    CommandManager();

    ~CommandManager();

    void executeInParallelWithWaitAndPopen(vector<Command *> commands);

    void executeInParallelWithoutWaitAndPopen(vector<Command *> commands);

private: 
};

#endif //_COMMAND_MANAGER_H
