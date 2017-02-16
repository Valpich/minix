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
string *CommandFinder::getFolderPaths() {
    return folderPaths;
}

/**
 * @param value
 */
void CommandFinder::setFolderPaths(string *value) {
    if (folderPaths != NULL) {
        delete folderPaths;
        folderPaths = NULL;
    }
    folderPaths = value;
}


/**
 * @return vector<string>
 */
Profile *CommandFinder::getProfile() {
    return profile;
}

/**
 * @param value
 */
void CommandFinder::setProfile(Profile *value) {
    if (profile != NULL) {
        delete profile;
        profile = NULL;
    }
    profile = value;
}

/**
 * @return vector<Command>
 */
void CommandFinder::findAllCommands(vector<Command *> * commands) {
    vector<string> *paths = parseProfileContent();
    cout << "Profile content parsed in find all commands";
    if(!paths->empty()){
        for (string line: *paths) {
            cout << "One path is " << line << endl;
            vector<string> * tempFolderFiles = new vector<string>();
            if(tempFolderFiles != NULL){
                getFilesInDirectory(tempFolderFiles,line);
                for(string file: *tempFolderFiles){
                    string filePath = line+"/"+file;
                    if (! access (filePath.c_str(), X_OK)){
                        cout<< filePath <<" is executable" << endl;
                        Command * command = new Command();
                        command->setPath(new string(line));
                        command->setName(new string(file));
                        commands->push_back(command);
                    } else {
                        cout << "Command "<< filePath <<"is not executable " << endl;
                    }
                }
            }
        }
    }else{
        // RE-GENERATE DEFAULT PROFILE
    }
}

vector<string> *CommandFinder::parseProfileContent() {
    vector<string> *paths = new vector<string>();
    if (profile != NULL) {
        cout << "Parsing profile" << endl;
        if (profile->getContent() != NULL) {
            cout << "Parsing profile content" << endl;
            for (string line: *profile->getContent()) {
                size_t index = line.find("PATH");
                if (index == string::npos) {
                    cout << "PATH line not found" << endl;
                } else {
                    cout << "PATH line found at " << index << endl;
                    for (string tmp: *parseLinePath(line.substr(index, line.size()))) {
                        paths->push_back(tmp);
                    }
                    cout << "PATH line parsed ! " << endl;
                }
            }
            cout << "End of parsing profile content" << endl;
        }
        cout << "End of parsing profile" << endl;
    }
    return paths;
}

vector<string> *CommandFinder::parseLinePath(string line) {
    vector<string> *paths = new vector<string>();
    size_t index = line.find("=");
    if (index == string::npos || line.substr(index + 1, line.size()).length() == 0) {
        cout << "Path empty" << endl;
    } else {
        cout << "Path found at " << index << endl;
        vector<string> v;
        split(line, ':', v);
        for (string tmp: v) {
            cout << "Token is " << tmp << endl;
            string *validToken = validateToken(tmp);
            if (validToken != NULL) {
                cout << "Valid token is " << *validToken << endl;
                paths->push_back(validToken->c_str());
            }
            delete validToken;
        }
        cout << "Path line parsed ! " << endl;
    }
    return paths;
}

string *CommandFinder::validateToken(const string &token) {
    size_t index = token.find("/");
    string *validToken = NULL;
    if (index == string::npos || token.substr(index, token.size()).length() == 0) {
        cout << "Token empty or invalid" << endl;
    } else {
        cout << "Token found at " << index << endl;
        validToken = new string(token.substr(index, token.size()));
    }
    return validToken;
}


void CommandFinder::split(const string &s, char c, vector<string> &v) {
    string::size_type i = 0;
    string::size_type j = s.find(c);
    while (j != string::npos) {
        v.push_back(s.substr(i, j - i));
        i = ++j;
        j = s.find(c, j);
        if (j == string::npos) {
            v.push_back(s.substr(i, s.length()));
        }
    }
}

void CommandFinder::getFilesInDirectory(vector<string> * listOfCommand,const string &directory) {
    cout <<"Getting all files in the directory "<<directory<<endl;
    DIR * dpdf;
    struct dirent * epdf;
    dpdf = opendir(directory.c_str());
    if (dpdf != NULL){
        while ((epdf = readdir(dpdf))){
            listOfCommand->push_back(epdf->d_name);
        }
        closedir(dpdf);
    }
    cout <<"End of getting all files in the directory "<<directory<<endl;
}

CommandFinder::CommandFinder(void) {
    folderPaths = NULL;
    profile = NULL;
}

CommandFinder::~CommandFinder(void) {
    cout << "Deleting folderPaths in CommandFinder" << endl;
    if (folderPaths != NULL) {
        delete folderPaths;
        folderPaths = NULL;
    }
    cout << "folderPaths deleted in CommandFinder" << endl;
    cout << "Deleting profile in CommandFinder" << endl;
    if (profile != NULL) {
        delete profile;
        profile = NULL;
    }
    cout << "profile deleted in CommandFinder" << endl;
}