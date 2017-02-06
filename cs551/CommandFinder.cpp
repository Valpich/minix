/**
 * Project 1 Develop your own shell
 * @author Jules Baud / Valentin Pichavant / Antoine Regnier
 * @version 1.0
 */


#include "CommandFinder.h"

/**
 * CommandFinder implementation
 */


/**
 * @return vector<string>
 */
vector<string> CommandFinder::getFolderPaths() {
    return folderPaths;
}

/**
 * @param value
 */
void CommandFinder::setFolderPaths(vector<string> value) {
	folderPaths = value;
}

/**
 * @return vector<Command>
 */
vector<Command> CommandFinder::findAllCommands() {
	vector<Command> commands;
    return commands;
}

CommandFinder::CommandFinder(void){

}

CommandFinder::~CommandFinder(void){
	
}