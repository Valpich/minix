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
Profile * Shell::getProfile() {
    return profile;
}

/**
 * @param value
 */
void Shell::setProfile(Profile * value) {
    if(profile != NULL){
        delete profile;
        profile = NULL;
    }
    profile = value;
}

/**
 * @return History
 */
History * Shell::getHistory() {
    return history;
}

/**
 * @param value
 */
void Shell::setHistory(History * value) {
    if(history != NULL){
        delete history;
        history = NULL;
    }
    history = value;
}

/**
 * @return vector<Command>
 */
vector<Command> * Shell::getCommandList() {
	return commandList;
}

/**
 * @param value
 */
void Shell::setCommandList(vector<Command> * value) {
    if(commandList != NULL){
        delete commandList;
        commandList = NULL;
    }
    commandList = value;
}

/**
 * @return CommandFinder
 */
CommandFinder * Shell::getCommandFinder() {
    return commandFinder;
}

/**
 * @param value
 */
void Shell::setCommandFinder(CommandFinder * value) {
    if(commandFinder != NULL){
        delete commandFinder;
        commandFinder = NULL;
    }
    commandFinder = value;
}

bool Shell::run() {
    bool scanning = true;
    while(scanning){
            int c;
            initscr();    /* Start curses mode */
            //One-character-a-time.
            cbreak();
            //No echo.
            noecho();
            //Special keys. In order to capture special keystrokes like Backspace, Delete and the four arrow keys by getch()
            keypad(stdscr, TRUE);
            cout << "Please, enter the command: " << '\r'<< endl;
            while (10 != (c = getch())) {
                if (halfdelay(1) != ERR) {   /* getch function waits 1 tenths of a second */
                    while (getch() == c)
                        if (halfdelay(1) == ERR) /* getch function waits 1 tenth of a second */
                            break;
                }
                if (c == 9) {
                    //TODO: Auto complete
#ifdef DEBUG
                    cout << "Tab pressed" << '\r' << endl;
#endif
                } else {
#ifdef DEBUG
                    cout << static_cast<char>(c) << " pressed" << '\r'<< endl;
                    cout << c << " pressed" << '\r'<< endl;
#endif
                }
                cbreak();
            }
            scanning = false;
            endwin();
#ifdef DEBUG
            cout << "Scan done!" << '\r'<< endl;
#endif
    }
    return true;
}

Shell::Shell(void){
    profile = new Profile();
    history = new History();
    commandList = NULL;
    commandFinder = NULL;
}

Shell::~Shell(void){
#ifdef DEBUG
    cout << "Deleting profile in shell" << endl;
#endif
    if(profile != NULL){
        delete profile;
        profile = NULL;
    }
#ifdef DEBUG
    cout << "profile deleted in shell" << endl;
    cout << "Deleting history in shell" << endl;
#endif
    if(history != NULL){
        delete history;
        history = NULL;
    }
#ifdef DEBUG
    cout << "history deleted in shell" << endl;
    cout << "Deleting commandList in shell" << endl;
#endif
    if(commandList != NULL){
        delete commandList;
        commandList = NULL;
    }
#ifdef DEBUG
    cout << "commandList deleted in shell" << endl;
    cout << "Deleting commandFinder in shell" << endl;
#endif
    if(commandFinder != NULL){
        delete commandFinder;
        commandFinder = NULL;
    }
#ifdef DEBUG
    cout << "commandFinder deleted in shell" << endl;
#endif
}