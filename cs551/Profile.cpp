/**
 * Project 1 Develop your own shell
 * @author Jules Baud / Valentin Pichavant / Antoine Regnier
 * @version 1.0
 */


#include "Profile.h"

/**
 * Profile implementation
 */


/**
 * @return string
 */
string Profile::getPath() {
    return *path;
}

/**
 * @param value
 */
void Profile::setPath(string value) {
	path = &value;
}

/**
 * @return string
 */
string Profile::getHome() {
    return *home;
}

/**
 * @param value
 */
void Profile::setHome(string value) {
	home = &value;
}

/**
 * @param value
 */
void Profile::setFileManager(FileManager value) {
    if(fileManager != NULL){
        delete fileManager;
    }
	fileManager = &value;
}

/**
 * @return FileManager
 */
FileManager Profile::getFileManager() {
    return *fileManager;
}

Profile::Profile(void){

}

Profile::~Profile(void){

}