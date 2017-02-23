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
#ifdef DEBUG
    cout << "\nInterrupt signal (" << signum << ") received." << endl;
#endif
    // TODO : Delete all dynamics objects

    //Disable cursive mode if still enabled
    endwin();
    exit(signum);
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
    Shell *shell = NULL;
    setjmp(buf);
    try {
        shell = new Shell();
        while (exit == false) {
            exit = shell->run();
        }
    } catch (...) {
#ifdef DEBUG
        cout << "Exception catched" << endl;
#endif
        if (shell != NULL) delete shell;
        longjmp(buf, 1);
    }
}