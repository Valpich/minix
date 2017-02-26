/**
 * Project 1 Develop your own shell
 * @author Jules Baud / Valentin Pichavant / Antoine Regnier
 * @version 1.0
 */


#include "CommandFinder.h"

/**
 * CommandFinder implementation
 */

string *CommandFinder::getFolderPaths() {
    return folderPaths;
}

void CommandFinder::setFolderPaths(string *value) {
    if (folderPaths != NULL) {
        delete folderPaths;
        folderPaths = NULL;
    }
    folderPaths = value;
}

Profile *CommandFinder::getProfile() {
    return profile;
}

void CommandFinder::setProfile(Profile *value) {
    if (profile != NULL) {
        delete profile;
        profile = NULL;
    }
    profile = value;
}

void CommandFinder::findAllCommands(vector<Command *> *commands) {
    bool retry = true;
    if (commands != NULL) {
        parseProfile:
        vector<string> *paths = parseProfilePathContent();
#ifdef DEBUG
        cout << "Profile content parsed in find all commands";
#endif
        if (!paths->empty()) {
            for (string line: *paths) {
#ifdef DEBUG
                cout << "One path is " << line << endl;
#endif
                vector<string> *tempFolderFiles = new vector<string>();
                if (tempFolderFiles != NULL) {
                    getFilesInDirectory(tempFolderFiles, line);
                    for (string file: *tempFolderFiles) {
                        string filePath = line + "/" + file;
                        if (!access(filePath.c_str(), X_OK)) {
#ifdef DEBUG
                            cout << filePath << " is executable" << endl;
#endif
                            Command *command = new Command();
                            command->setPath(new string(line));
                            command->setName(new string(file));
                            commands->push_back(command);
                        } else {
#ifdef DEBUG
                            cout << "Command " << filePath << "is not executable " << endl;
#endif
                        }
                    }
                }
            }
        } else {
            // We regenerate the user profile from default and we retry one time to parse it
            // We exit with error code 2 if the program is unable to recover
            FileManager *fileManager = new FileManager();
            fileManager->replaceFileContent(*profile->getPath(), *profile->getDefaultContent());
            cout << "Profile was not found or cannot be opened" << endl;
            cout << "Default profile used, old profile replaced" << endl;
            profile->setContent(profile->getDefaultContent());
            delete fileManager;
            if (retry) {
                retry = false;
                goto parseProfile;

            } else {
                cout << "The program is not able to work correctly, be sure to have the write/read enable for files!"
                     << endl;
                cout << "Program shutdown with exit code 2 to prevent bad behaviors" << endl;
                exit(2);
            }
        }
    }
}

vector<string> *CommandFinder::parseProfilePathContent() {
    vector<string> *paths = new vector<string>();
    if (profile != NULL) {
#ifdef DEBUG
        cout << "Parsing profile" << endl;
#endif
        if (profile->getContent() != NULL) {
#ifdef DEBUG
            cout << "Parsing profile content" << endl;
#endif
            for (string line: *profile->getContent()) {
                size_t index = line.find("PATH");
                if (index == string::npos) {
#ifdef DEBUG
                    cout << "PATH line not found" << endl;
#endif
                } else {
#ifdef DEBUG
                    cout << "PATH line found at " << index << endl;
#endif
                    for (string tmp: *parseLinePath(line.substr(index, line.size()))) {
                        paths->push_back(tmp);
                    }
#ifdef DEBUG
                    cout << "PATH line parsed ! " << endl;
#endif
                }
            }
#ifdef DEBUG
            cout << "End of parsing profile content" << endl;
#endif
        }
#ifdef DEBUG
        cout << "End of parsing profile" << endl;
#endif
    }
    return paths;
}

vector<string> *CommandFinder::parseLinePath(string line) {
    vector<string> *paths = new vector<string>();
    size_t index = line.find("=");
    if (index == string::npos || line.substr(index + 1, line.size()).length() == 0) {
#ifdef DEBUG
        cout << "Path empty" << endl;
#endif
    } else {
#ifdef DEBUG
        cout << "Path found at " << index << endl;
#endif
        vector<string> v;
        split(line, ':', v);
        for (string tmp: v) {
#ifdef DEBUG
            cout << "Token is " << tmp << endl;
#endif
            string *validToken = validateToken(tmp);
            if (validToken != NULL) {
#ifdef DEBUG
                cout << "Valid token is " << *validToken << endl;
#endif
                paths->push_back(validToken->c_str());
            }
            delete validToken;
        }
#ifdef DEBUG
        cout << "Path line parsed ! " << endl;
#endif
    }
    return paths;
}

string *CommandFinder::validateToken(const string &token) {
    size_t index = token.find("/");
    string *validToken = NULL;
    if (index == string::npos || token.substr(index, token.size()).length() == 0) {
#ifdef DEBUG
        cout << "Token empty or invalid" << endl;
#endif
    } else {
#ifdef DEBUG
        cout << "Token found at " << index << endl;
#endif
        validToken = new string(token.substr(index, token.size()));
    }
    return validToken;
}


void CommandFinder::split(const string &s, char c, vector<string> &v) {
    string::size_type i = 0;
    string::size_type j = s.find(c);
    // While the string is not fully split
    while (j != string::npos) {
        // We add the string to the vector and get to the next one until the end
        v.push_back(s.substr(i, j - i));
        i = ++j;
        j = s.find(c, j);
        if (j == string::npos) {
            v.push_back(s.substr(i, s.length()));
        }
    }
}

void CommandFinder::getFilesInDirectory(vector<string> *listOfCommand, const string &directory) {
#ifdef DEBUG
    cout << "Getting all files in the directory " << directory << endl;
#endif
    DIR *dpdf;
    struct dirent *epdf;
    dpdf = opendir(directory.c_str());
    // If we didn't fail to open the directory
    if (dpdf != NULL) {
        while ((epdf = readdir(dpdf))) {
            // We add the found command to the list of potential commands
            listOfCommand->push_back(epdf->d_name);
        }
        closedir(dpdf);
    }
#ifdef DEBUG
    cout << "End of getting all files in the directory " << directory << endl;
#endif
}

ostream &operator<<(ostream &os, const CommandFinder &finder) {
    os << "folderPaths: ";
    if(finder.folderPaths == NULL){
        os <<"NULL";
    }else{
        os << *finder.folderPaths;
    }
    os << "profile: ";
    if(finder.profile == NULL){
        os <<"NULL";
    }else{
        os << *finder.profile;
    }
    return os;
}

CommandFinder::CommandFinder(void) {
    folderPaths = NULL;
    profile = NULL;
}

CommandFinder::~CommandFinder(void) {
#ifdef DEBUG
    cout << "Deleting folderPaths in CommandFinder" << endl;
#endif
    if (folderPaths != NULL) {
        delete folderPaths;
        folderPaths = NULL;
    }
#ifdef DEBUG
    cout << "folderPaths deleted in CommandFinder" << endl;
    cout << "Deleting profile in CommandFinder" << endl;
#endif
    if (profile != NULL) {
        delete profile;
        profile = NULL;
    }
#ifdef DEBUG
    cout << "profile deleted in CommandFinder" << endl;
#endif
}