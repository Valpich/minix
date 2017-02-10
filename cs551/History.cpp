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
 * @return string
 */
string * History::getPath() {
    return path;
}

/**
 * @param value
 */
void History::setPath(string * value) {
	path = value;
}

/**
 * @param value
 */
void History::setFileManager(FileManager * value) {
    if(fileManager != NULL){
        delete fileManager;
    }
	fileManager = value;
}

/**
 * @return FileManager
 */
FileManager * History::getFileManager() {
    return fileManager;
}

History::History(void){
    fileManager = NULL;
}

History::~History(void){
	if(fileManager != NULL){
        delete fileManager;
        fileManager = NULL;
    }
}