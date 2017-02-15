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
 * @return string
 */
string * CommandFinder::getFolderPaths() {
    return folderPaths;
}

/**
 * @param value
 */
void CommandFinder::setFolderPaths(string * value) {
    if(folderPaths != NULL){
        delete folderPaths;
        folderPaths = NULL;
    }
	folderPaths = value;
}


/**
 * @return vector<string>
 */
Profile * CommandFinder::getProfile() {
    return profile;
}

/**
 * @param value
 */
void CommandFinder::setProfile(Profile * value) {
    if(profile != NULL){
        delete profile;
        profile = NULL;
    }
    profile = value;
}

/**
 * @return vector<Command>
 */
vector<Command> * CommandFinder::findAllCommands() {
	vector<Command> * commands = new vector<Command>();
    vector<string> * paths = parseProfileContent();
    return commands;
}

vector<string> * CommandFinder::parseProfileContent() {
    if(profile != NULL){
        cout << "Parsing profile" << endl;
        if(profile->getContent() != NULL) {
            cout << "Parsing profile content" << endl;
            for (string line: *profile->getContent()) {
                size_t index = line.find("PATH");
                if (index == string::npos) {
                    cout << "PATH not found" << endl;
                }else {
                    cout << "found at " << index << endl;
                    string parsed = line.substr(index,line.size());
                    cout << "parsed in " << parsed << endl;
                }
            }
            cout << "End of parsing profile content" << endl;
        }
        cout << "End of parsing profile" << endl;
    }
    return NULL;
}

CommandFinder::CommandFinder(void){
    folderPaths = NULL;
    profile = NULL;
}

CommandFinder::~CommandFinder(void){
    cout << "Deleting folderPaths in CommandFinder" << endl;
    if(folderPaths != NULL){
        delete folderPaths;
        folderPaths = NULL;
    }
    cout << "folderPaths deleted in CommandFinder" << endl;
    cout << "Deleting profile in CommandFinder" << endl;
    if(profile != NULL){
        delete profile;
        profile = NULL;
    }
    cout << "profile deleted in CommandFinder" << endl;
}