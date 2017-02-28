/**
 * Project 1 Develop your own shell
 * @author Jules Baud / Valentin Pichavant / Antoine Regnier
 * @version 1.0
 */


#include "Main.h"


#ifdef TEST
Test *test = new Test();
#endif

/**
 * Main implementation
 */

jmp_buf buf;

/**
 * The main class of the program
 */
Main *mainClass = new Main();

Main::Main() {
    shell = new Shell();
}

Main::~Main() {
#ifdef DEBUG
    cout << "Deleting shell in Main" << endl;
#endif
    if (shell != NULL) {
        delete shell;
        shell = NULL;
    }
#ifdef DEBUG
    cout << "shell deleted in Main" << endl;
#endif
}

Shell *Main::getShell() {
    return shell;
}

void Main::signalHandler(int signum) {
    // If we are stuck in a command
    if (signum == SIGINT) {
        //Disable cursive mode if still enabled
        endwin();
#ifdef DEBUG
        cout << "\nCTRL+C INTERCEPTED." << endl;
#endif
        bool commandKilled = false;
        if (mainClass != NULL) {
#ifdef DEBUG
            cout << "mainClass != NULL" << endl;
#endif
            if (mainClass->getShell() != NULL) {
#ifdef DEBUG
                cout << "mainClass->getShell() != NULL" << endl;
#endif
                if (mainClass->getShell()->getCommand() != NULL) {
#ifdef DEBUG
                    cout << "mainClass->getShell()->getCommand() != NULL" << endl;
#endif
                    if(mainClass->getShell()->getCommand()->isRunning()){
#ifdef DEBUG
                        cout << "mainClass->getShell()->getCommand()->isRunning() true" << endl;
#endif
                        kill(mainClass->shell->getCommand()->getPid(), SIGKILL);
                        mainClass->shell->setCommand(NULL);
                        commandKilled = true;
                        cout << "Kill command " << endl;
                   }
                }
            }
            if (!commandKilled) {
#ifdef DEBUG
                cout << "Command::isRunning() false" << endl;
#endif
                cout << "Kill main " << endl;
                delete mainClass;
                mainClass = NULL;
                exit(signum);
            }
        } else {
            cout << "Exit " << endl;
            exit(signum);
        }
    } else if (signum == SIGALRM) {
        bool scanning = true;
        int c;
        cout << "Do you want to kill the command ? Y/N ?" << endl;
        while (scanning) {
            c = getchar();
#ifdef DEBUG
            cout << "Scanned " << c << endl;
#endif
            // If Y/y
            if (c == 89 || c == 121) {
                if (mainClass != NULL) {
                    if (mainClass->shell != NULL) {
                        if (mainClass->shell->getCommand() != NULL) {
#ifdef DEBUG
                            cout << "Send kill" << endl;
#endif
                            kill(mainClass->shell->getCommand()->getPid(), SIGKILL);
                            mainClass->shell->setCommand(NULL);
                            //Disable cursive mode if still enabled
                            endwin();
                            cout << "\nSIGALRM INTERCEPTED." << endl;
                            //     exit(1);
#ifdef DEBUG
                            cout << "kill send" << endl;
#endif
                        }
                    }
                }
                // in case no command has been typed yet
                scanning = false;
            }
            // If N/n
            if (c == 78 || c == 110) {
                // Else back to normal
                scanning = false;
                cout << "Command will run until you usr CTRL+C or termination" << endl;
            }
            fflush(stdin);
        }
#ifdef TEST
        test->waitingAlarm = false;
#endif
    } else {
        endwin();
    }
}

/**
 * @return int The return code of the program, should always be 0
 */
int main() {
#ifdef TEST
    test->executeTestSuite();
    delete test;
#else
    // Registering all 22 signal of POSIX
    /*
    for (int i = 0; i <= 22; i++) {
        signal(i, Main::signalHandler);
    }*/
    signal(SIGINT, Main::signalHandler);
    signal(SIGALRM, Main::signalHandler);
    signal(SIGSEGV, Main::signalHandler);
    bool exit = false;
#ifdef DEBUG_ALARM
    Command * cmd = new Command();
    mainClass->getShell()->setCommand(cmd);
    cmd->executeWithExecve();
#endif
    // Save the program state before running the shell
    setjmp(buf);
    // We try to run the shell and we catch every possible exceptions to avoid crash
    try {
        // While the user use the shell
        while (exit == false) {
            exit = mainClass->getShell()->run();
        }
    } catch (...) {
#ifdef DEBUG
        cout << "Exception catch" << endl;
#endif
        // If the main class is not null then we delete the main class
        if (mainClass != NULL) delete mainClass;
        // We create it back as a new one to avoid that the issue that caused the crash happen again
        mainClass = new Main();
        // We restore the state of the program to the previous setjmp
        longjmp(buf, 1);
    }
#endif
    return 0;
}
