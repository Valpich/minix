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

    if(signum == SIGINT){
        //Disable cursive mode if still enabled
        endwin();
#ifdef DEBUG
        cout << "\nCTRL+C INTERCEPTED." << endl;
#endif
        if(mainClass != NULL){
            delete mainClass;
            mainClass = NULL;
        }
        exit(signum);
    }
    if(signum == SIGALRM){
        bool scanning = true;
        int c;
        cout <<"Do you want to kill the command ?" <<endl;
        while (scanning) {
         // TODO: Scan the reponse
            // If Y/y
            if(c == 89 || c == 121){
                if(mainClass->shell != NULL){
                    if(mainClass->shell->getCommand()!= NULL){
                        if(mainClass->shell->getCommand()->getPid()!= NULL){
                            kill(mainClass->shell->getCommand()->getPid(),SIGKILL);
                        }
                    }
                }
            }
            // If N/n
            if(c == 78 || c == 110){
                // Else back to normal
                scanning = false;
            }
            sleep(1);
        }
#ifdef TEST
        test->waitingAlarm = false;
#endif
#ifdef DEBUG
        //Disable cursive mode if still enabled
        endwin();
        cout << "\nSIGALRM INTERCEPTED." << endl;
#endif
    }
}


/**
 * @return int
 */
int main() {
#ifdef TEST
    test->executeTestSuite();
    delete test;
#endif
    // Registering all 22 signal of POSIX
    for(int i = 0 ; i<=22; i++){
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
        cout << "Exception catch" << endl;
#endif
        if (mainClass != NULL) delete mainClass;
        mainClass = new Main();
        longjmp(buf, 1);
    }
}