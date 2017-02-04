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
 * @return std::string
 */
std::string Profile::getPath() {
    return path;
}

/**
 * @param value
 */
void Profile::setPath(std::string value) {
	path = value;
}

/**
 * @return std::string
 */
std::string Profile::getHome() {
    return home;
}

/**
 * @param value
 */
void Profile::setHome(std::string value) {
	home = value;
}

/**
 * @param value
 */
void Profile::setFileManager(FileManager value) {
	fileManager = value;
}

/**
 * @return FileManager
 */
FileManager Profile::getFileManager() {
    return fileManager;
}

Profile::Profile(void){

}

Profile::~Profile(void){

}