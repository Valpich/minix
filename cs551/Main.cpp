/**
 * Project 1 Develop your own shell
 * @author Jules Baud / Valentin Pichavant / Antoine Regnier
 * @version 1.0
 */


#include "Main.h"

/**
 * Main implementation
 */

Main * mainClass = new Main();

Main::Main(){
    shell = new Shell();
}

Main::~Main(){
#ifdef DEBUG
    cout << "Deleting shell in Main" << endl;
#endif
    if(shell!=NULL){
        delete shell;
        shell = NULL;
    }
#ifdef DEBUG
    cout << "shell deleted in Main" << endl;
#endif
}

Shell * Main::getShell() {
    return shell;
}

void Main::signalHandler(int signum) {
    //Disable cursive mode if still enabled
    endwin();

#ifdef DEBUG
    cout << "\nInterrupt signal (" << signum << ") received." << endl;
#endif
    // TODO : Delete all dynamics objects
    if(signum == SIGINT){
        if(mainClass != NULL){
            delete mainClass;
            mainClass = NULL;
        }
        exit(signum);
    }
}

/**
 * @return int
 */
int main() {
#ifdef TEST
    Test *test = new Test();
    test->executeTestSuite();
    delete test;
#endif
    // Registering all 22 signal of POSIX
    for(int i = 0 ; i<=220; i++){
        signal(i, Main::signalHandler);
    }
    jmp_buf buf;
    bool exit = false;
    setjmp(buf);
    try {
        while (exit == false) {
            exit = mainClass->getShell()->run();
        }
    } catch (...) {
#ifdef DEBUG
        cout << "Exception catched" << endl;
#endif
        if (mainClass != NULL) delete mainClass;
        mainClass = new Main();
        longjmp(buf, 1);
    }
}