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

    if (signum == SIGINT) {
        //Disable cursive mode if still enabled
        endwin();
#ifdef DEBUG
        cout << "\nCTRL+C INTERCEPTED." << endl;
#endif
        if (mainClass != NULL) {
            delete mainClass;
            mainClass = NULL;
        }
        exit(signum);
    }else if (signum == SIGALRM) {
        bool scanning = true;
        int c;
        cout << "Do you want to kill the command ?" << endl;
        
         while (scanning) {
          // TODO: Scan the reponse
             // If Y/y
             c = getch();
             if(c == 89 || c == 121){
                 if(mainClass->shell != NULL){
                       //cout << mainClass->shell->getCommand() << endl; 
                       if(mainClass->shell->getCommand()!= NULL){
                         kill(mainClass->shell->getCommand()->getPid(),SIGKILL);
                     }
                 }
              // in case no command has been typed yet
              scanning = false; 
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
    }else {
        longjmp(buf, signum);
    }
}

ostream &operator<<(ostream &os, const Main &main1) {
    os << "shell: " << main1.shell;
    return os;
}

/**
 * @return int The return code of the program, should always be 0
 */
int main() {
#ifdef TEST
    test->executeTestSuite();
    delete test;
#endif
    // Registering all 22 signal of POSIX
    for (int i = 0; i <= 22; i++) {
        signal(i, Main::signalHandler);
    }
    bool exit = false;
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
    return 0;
}
