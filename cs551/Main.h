/**
 * Project 1 Develop your own shell
 * @author Jules Baud / Valentin Pichavant / Antoine Regnier
 * @version 1.0
 */


#ifndef _MAIN_H
#define _MAIN_H

#include "Command.h"
#include "CommandFinder.h"
#include "FileManager.h"
#include "History.h"
#include "Profile.h"
#include "Shell.h"

using namespace std;

class Main {
public:

    Main();

    ~Main();

    static void signalHandler(int signum);

    Shell * getShell();

private:

 Shell * shell;

};

#endif //_MAIN_H