/**
 * Project 1 Develop your own shell
 * @author Jules Baud / Valentin Pichavant / Antoine Regnier
 * @version 1.0
 */


#include "Shell.h"

/**
 * Shell implementation
 */


/**
 * @return std::string
 */
std::string Shell::getName() {
    return "Empty";
}

/**
 * @param value
 */
void Shell::setName(std::string value) {

}

/**
 * @return Profile
 */
Profile Shell::getProfile() {
	Profile profile;
    return profile;
}

/**
 * @param value
 */
void Shell::setProfile(Profile value) {

}

/**
 * @return History
 */
History Shell::getHistory() {
	History history;
    return history;
}

/**
 * @return std::vector<Command>
 */
std::vector<Command> Shell::getCommandList() {
    std::vector<std::string> commandList;
	return commandList;
}

/**
 * @param value
 */
void Shell::setCommandList(std::vector<Command> value) {

}

/**
 * @return CommandFinder
 */
CommandFinder Shell::getCommandFinder() {
	CommandFinder cf;
    return cf;
}

/**
 * @param value
 */
void Shell::setCommandFinder(CommandFinder value) {

}

Shell::Shell(void){

}

Shell::~Shell(void){

}

/**
 * @return int
 */
int main () {
    std::cout << "Hello !" << std::endl;
}
