/**
 * Project 1 Develop your own shell
 * @author Jules Baud / Valentin Pichavant / Antoine Regnier
 * @version 1.0
 */


#include "Shell.h"
/**
 * Shell implementation
 */

const string Shell::name = "SHELL NAME";
/**
 * @return Profile
 */
Profile Shell::getProfile() {
    return *profile;
}

/**
 * @param value
 */
void Shell::setProfile(Profile value) {
    if(profile != NULL){
        delete profile;
    }
    profile = &value;
}

/**
 * @return History
 */
History Shell::getHistory() {
    return *history;
}

/**
 * @param value
 */
void Shell::setHistory(History value) {
    if(history != NULL){
        delete history;
    }
    history = &value;
}

/**
 * @return vector<Command>
 */
vector<Command> Shell::getCommandList() {
	return *commandList;
}

/**
 * @param value
 */
void Shell::setCommandList(vector<Command> value) {
    if(commandList != NULL){
        delete commandList;
    }
    commandList = &value;
}

/**
 * @return CommandFinder
 */
CommandFinder Shell::getCommandFinder() {
    return *commandFinder;
}

/**
 * @param value
 */
void Shell::setCommandFinder(CommandFinder value) {
    if(commandFinder != NULL){
        delete commandFinder;
    }
    commandFinder = &value;
}

Shell::Shell(void){

}

Shell::~Shell(void){

}

void signalHandler(int signum) {
   cout << "\nInterrupt signal (" << signum << ") received." << endl;

   // TODO: Close all files / destroy all dynamics objects

   exit(signum); 
}

/**
 * @return int
 */
int main () {
#ifdef TEST
    Test * test = new Test();
    test->executeTestOne();
#endif
    volatile bool exceptionInput = false;
    signal(SIGINT, signalHandler);
    jmp_buf env;
    int errorValue = 0;
    setjmp(env);
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
        longjmp(env,errorValue);
    }
    return 0;
}
