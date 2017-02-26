/**
 * Project 1 Develop your own shell
 * @author Jules Baud / Valentin Pichavant / Antoine Regnier
 * @version 1.0
 */


#ifndef _TEST_H
#define _TEST_H

#include <string>

#include "Command.h"
#include "CommandFinder.h"
#include "CommandManager.h"
#include "FileManager.h"
#include "History.h"
#include "Profile.h"
#include "Shell.h"

using namespace std;

class Test {
public:

    Test();

    ~Test();

    /**
     * Execute all tests cases
     */
    void executeTestSuite();

    bool waitingAlarm;
private:

    void executeTest1();

    void executeTest2();

    void executeTest3();

    void executeTest4();

    void executeTest5();

    void executeTest6();

    void executeTest7();

    void executeTest8();

    void executeTest9();
};

#endif //_TEST_H
