/**
 * Project 1 Develop your own shell
 * @author Jules Baud / Valentin Pichavant / Antoine Regnier
 * @version 1.0
 */


#ifndef _MAIN_H
#define _MAIN_H

#include <ostream>
#include "Command.h"
#include "CommandFinder.h"
#include "FileManager.h"
#include "History.h"
#include "Profile.h"
#include "Shell.h"

using namespace std;

class Main {
public:

    /**
     * Construct a Main instance
     */
    Main();

    /**
     * Desotry a Main instance
     */
    ~Main();

    /**
     * Static method used to handle signals
     * @param signum The signal number of the signal to be registred
     */
    static void signalHandler(int signum);

    /**
     * @return The shell of the main
     */
    Shell *getShell();

    /**
     * Overload the ostream operator to display as a string a Shell
     */
    friend ostream &operator<<(ostream &os, const Main &main1);

private:

    Shell *shell;

};

#endif //_MAIN_H