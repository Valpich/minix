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
 * The constant value
 */
const string History::defaultPath = ".shell_history";

string *History::getPath() {
    return path;
}

void History::setPath(string *value) {
    if (path != NULL) {
        delete path;
        path = NULL;
    }
    path = value;
}

void History::setFileManager(FileManager *value) {
    if (fileManager != NULL) {
        delete fileManager;
        fileManager = NULL;
    }
    fileManager = value;
}

FileManager *History::getFileManager() {
    return fileManager;
}

void History::logCommand(Command *command) {
    // Should never be false.
    if (this->getFileManager() == NULL) {
        // We create a new FileManager
        this->setFileManager(new FileManager());
    }
    // Should ever be true.
    if (this->getFileManager() != NULL) {
        vector<string> *commands = new vector<string>();
        // If the is a command
        if (command != NULL) {
            // We append the command name and params
            commands->push_back(*command->getName() + " " + *command->getParams());
            this->fileManager->appendToFile(*this->getPath(), *commands);
        }
    }
}

void History::getCommandHistory(vector<Command *> *commands){
    char buffer[300];
    char *answer = getcwd(buffer, sizeof(buffer));
    string s_cwd;
    if (answer){
        s_cwd = answer;
        remove(s_cwd.begin(), s_cwd.end(), ' ');
    }
    cout <<"Current path is " << s_cwd <<endl;
    /*
    if (commands != NULL) {
        getcwd(cCurrentPath, sizeof(cCurrentPath));
      vector<string> *paths = parseProfilePathContent();
#ifdef DEBUG
        cout << "History content parsed in find all commands";
#endif
        if (!paths->empty()) {
            for (string line: *paths) {
#ifdef DEBUG
                cout << "One command line was " << line << endl;
#endif

                            Command *command = new Command();
                           // command->setPath(new string(line));
                           // command->setName(new string(file));
                            commands->push_back(command);
                        }
                    }
                }
    }else{
        cout <<"You may have altered the source code, commands is not supposed to be NULL vector !"<<endl;
    }
    */
}

ostream &operator<<(ostream &os, const History &history) {
    os << "path: ";
    if(history.path == NULL){
        os <<"NULL";
    }else{
        os << *history.path;
    }
    os << " fileManager: ";
    if(history.fileManager == NULL){
        os <<"NULL";
    }else{
        os << *history.fileManager;
    }
    return os;
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