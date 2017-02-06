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
 * @return string
 */
string Command::getName() {
    return name;
}

/**
 * @param value
 */
void Command::setName(string value) {
	name = value;
}

/**
 * @return string
 */
string Command::getPath() {
    return path;
}

/**
 * @param value
 */
void Command::setPath(string value) {
	path = value;
}

Command::Command(void){

}

Command::~Command(void){
	
}