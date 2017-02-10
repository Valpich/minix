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
    test->executeTest1();
    test->executeTest2();
    test->executeTest3();
    delete test;
#endif
    volatile bool exceptionInput = false;
    signal(SIGINT, Main::signalHandler);
    jmp_buf buf;
    int errorValue = 0;
    setjmp(buf);
    int scanned = 0;
    try{
        while(1){
            cout << "Enter a number:" << endl;
            int i;
            scanned = scanf ("%d",&i);
            while(getchar() != '\n');
            if(scanned == 0){
                exceptionInput = true;
                throw 1;
            }
            cout << "Scanned is " << scanned << endl;
            cout << "Your number is " << i << endl;
#ifdef DEBUG

#endif
        }
    }catch(...){
        if(exceptionInput == true){
            cout << "You put an invalid input" << endl;
            errorValue = 1;
        }
        longjmp(buf,errorValue);
    }
}