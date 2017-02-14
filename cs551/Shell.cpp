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
    }
    commandFinder = value;
}

bool Shell::run() {
    Command * command = new Command();
    cout << "Please, enter the command: " << endl;
    bool scanning = true;
    string commandString ="ls";
    while(scanning){
        // TODO: scanner la command et utiliser le tab
        if(1){
            scanning = false;
        }
    }
    History * history = new History();
    command->setName(new string(commandString));
    command->setEnv(new string("EXAMPLE=test env"));
    command->setParams(new string("-al"));
    cout << command->execute() << endl;
    history->logCommand(command);
    delete command;
    delete history;
    return false;
}

Shell::Shell(void){
    profile = new Profile();
    history = new History();
    commandList = NULL;
    commandFinder = NULL;
}

Shell::~Shell(void){
    if(profile != NULL){
        delete profile;
        profile = NULL;
    }
    if(history != NULL){
        delete history;
        history = NULL;
    }
    if(commandList != NULL){
        delete commandList;
        commandList = NULL;
    }
    if(commandFinder != NULL){
        delete commandFinder;
        commandFinder = NULL;
    }
}