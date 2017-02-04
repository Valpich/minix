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
 * @return vector<std::string>
 */
std::vector<std::string> CommandFinder::getFolderPaths() {
    return folderPaths;
}

/**
 * @param value
 */
void CommandFinder::setFolderPaths(std::vector<std::string> value) {
	folderPaths = value;
}

/**
 * @return vector<Command>
 */
std::vector<Command> CommandFinder::findAllCommands() {
	std::vector<Command> commands;
    return commands;
}

CommandFinder::CommandFinder(void){

}

CommandFinder::~CommandFinder(void){
	
}