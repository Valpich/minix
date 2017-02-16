/**
 * Project 1 Develop your own shell
 * @author Jules Baud / Valentin Pichavant / Antoine Regnier
 * @version 1.0
 */


#include "History.h"

/**
 * History implementation
 */

const string History::defaultPath = ".shell_history";

/**
 * @return string
 */
string *History::getPath() {
    return path;
}

/**
 * @param value
 */
void History::setPath(string *value) {
    if (path != NULL) {
        delete path;
        path = NULL;
    }
    path = value;
}

/**
 * @param value
 */
void History::setFileManager(FileManager *value) {
    if (fileManager != NULL) {
        delete fileManager;
        fileManager = NULL;
    }
    fileManager = value;
}

/**
 * @return FileManager
 */
FileManager *History::getFileManager() {
    return fileManager;
}

/**
 * @return FileManager
 */
void History::logCommand(Command *command) {
    // Should never be false.
    if (this->getFileManager() == NULL) {
        this->setFileManager(new FileManager());
    }
    // Should ever be true.
    if (this->getFileManager() != NULL) {
        vector<string> * commands = new vector<string>();
        commands->push_back(*command->getName());
        this->fileManager->appendToFile(*this->getPath(),*commands);
    }
}

History::History(void) {
    fileManager = new FileManager();
    path = new string(defaultPath);
}

History::~History(void) {
#ifdef DEBUG
    cout << "Deleting fileMananger in History" << endl;
#endif
    if (fileManager != NULL) {
        delete fileManager;
        fileManager = NULL;
    }
    #ifdef DEBUG
    cout << "fileMananger delete in History" << endl;
    cout << "Deleting path in History" << endl;
    #endif
    if (path != NULL) {
        delete path;
        path = NULL;
    }
    #ifdef DEBUG
    cout << "path deleted in History" << endl;
    #endif
}