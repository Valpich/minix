/**
 * Project 1 Develop your own shell
 * @author Jules Baud / Valentin Pichavant / Antoine Regnier
 * @version 1.0
 */


#include "History.h"

/**
 * History implementation
 */


/**
 * @return std::string
 */
std::string History::getPath() {
    return path;
}

/**
 * @param value
 */
void History::setPath(std::string value) {
	path = value;
}

/**
 * @param value
 */
void History::setFileManager(FileManager value) {
	fileManager = value;
}

/**
 * @return FileManager
 */
FileManager History::getFileManager() {
    return fileManager;
}

History::History(void){

}

History::~History(void){
	
}