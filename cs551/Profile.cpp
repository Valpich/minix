/**
 * Project 1 Develop your own shell
 * @author Jules Baud / Valentin Pichavant / Antoine Regnier
 * @version 1.0
 */


#include "Profile.h"

/**
 * Profile implementation
 */


const char * Profile::defaultProfile[] =
        {"export PATH=/sbin:/usr/sbin:/bin:/usr/bin:/usr/pkg/sbin:/usr/pkg/bin"
                ,"export PATH=${PATH}:/usr/X11R7/bin:/usr/X11R6/bin:/usr/local/sbin:/usr/local/bin",
        "export BLOCKSIZE=1k", "export HOST=\"$(hostname)\""};

/**
 * @return string
 */
string * Profile::getPath() {
    return path;
}

/**
 * @param value
 */
void Profile::setPath(string * value) {
    if(path != NULL){
        delete path;
        path = NULL;
    }
	path = value;
}

/**
 * @return string
 */
vector<string> * Profile::getContent() {
    return content;
}

/**
 * @param value
 */
void Profile::setContent(vector<string> * value) {
    if(content != NULL){
        delete content;
        content = NULL;
    }
    content = value;
}

/**
 * @param value
 */
void Profile::setFileManager(FileManager * value) {
    if(fileManager != NULL){
        delete fileManager;
        fileManager = NULL;
    }
	fileManager = value;
}

/**
 * @return FileManager
 */
FileManager * Profile::getFileManager() {
    return fileManager;
}

Profile::Profile(void){
    fileManager = new FileManager();
    path = new string(".profile");
    vector<string> findProfile = fileManager->readFileToString(*path);
    if(findProfile.empty()){
        vector<string> * defaultContent = new vector<string>();
        for(string line : defaultProfile){
            defaultContent->push_back(line);
        }
        fileManager->replaceFileContent(*path,*defaultContent);
        content = defaultContent;
    }else{
        vector<string> * profile = new vector<string>();
        for(string line : findProfile){
            profile->push_back(line);
        }
        content = profile;
    }
}

Profile::~Profile(void){
    cout << "Deleting fileManager in profile" << endl;
    if(fileManager != NULL){
        delete fileManager;
        fileManager = NULL;
    }
    cout << "fileManager deleted in profile" << endl;
    cout << "Deleting path in profile" << endl;
    if(path != NULL){
        delete path;
        path = NULL;
    }
    cout << "path deleted in profile" << endl;
    cout << "Deleting content in profile" << endl;
    if(content != NULL){
        delete content;
        content = NULL;
    }
    cout << "content deleted in profile" << endl;
}