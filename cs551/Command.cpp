/**
 * Project 1 Develop your own shell
 * @author Jules Baud / Valentin Pichavant / Antoine Regnier
 * @version 1.0
 */


#include "Command.h"

/**
 * Command implementation
 */


/**
 * @return std::string
 */
std::string Command::getName() {
    return name;
}

/**
 * @param value
 */
void Command::setName(std::string value) {
	name = value;
}

/**
 * @return std::string
 */
std::string Command::getPath() {
    return path;
}

/**
 * @param value
 */
void Command::setPth(std::string value) {
	path = value;
}

Command::Command(void){

}

Command::~Command(void){
	
}