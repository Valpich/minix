/**
 * Project 1 Develop your own shell
 * @author Jules Baud / Valentin Pichavant / Antoine Regnier
 * @version 1.0
 */


#include "Main.h"

/**
 * Main implementation
 */


void Main::signalHandler(int signum) {
    cout << "\nInterrupt signal (" << signum << ") received." << endl;

    // TODO : Delete all dynamics objects

    exit(signum);
}

/**
 * @return int
 */
int main () {
#ifdef TEST
    Test * test = new Test();
    test->executeTestSuite();
    delete test;
#endif
    signal(SIGINT, Main::signalHandler);
    jmp_buf buf;
    bool exit = false;
    Shell * shell = NULL;
    setjmp(buf);
    try{
        shell = new Shell();
        while(exit == false){
        //   exit = shell->run();
        }
    }catch(...){
        cout << "Exception catched" << endl;
        if(shell != NULL) delete shell;
        longjmp(buf,1);
    }
}