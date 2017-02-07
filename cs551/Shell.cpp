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