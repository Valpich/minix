/**
 * Project 1 Develop your own shell
 * @author Jules Baud / Valentin Pichavant / Antoine Regnier
 * @version 1.0
 */


#include "Profile.h"
#include "Command.h"

/**
 * Profile implementation
 */


const char *Profile::defaultProfile[] =
        {"export PATH=",
         "export PATH=/sbin:/usr/sbin:/bin:/usr/bin:/usr/pkg/sbin:/usr/pkg/bin",
         "export PATH=${PATH}:/usr/X11R7/bin:/usr/X11R6/bin:/usr/local/sbin:/usr/local/bin",
         "export ALARM=ON", "export HOST=\"$(hostname)\""};

/**
 * @return string
 */
string *Profile::getPath() {
    return path;
}

/**
 * @param value
 */
void Profile::setPath(string *value) {
    if (path != NULL) {
        delete path;
        path = NULL;
    }
    path = value;
}

/**
 * @return string
 */
vector<string> *Profile::getContent() {
    return content;
}

/**
 * @param value
 */
void Profile::setContent(vector<string> *value) {
    if (content != NULL) {
        delete content;
        content = NULL;
    }
    content = value;
}

/**
 * @param value
 */
void Profile::setFileManager(FileManager *value) {
    if (fileManager != NULL) {
        delete fileManager;
        fileManager = NULL;
    }
    fileManager = value;
}

/**
 * @return FileManager
 */
FileManager *Profile::getFileManager() {
    return fileManager;
}

Profile::Profile(void) {
    fileManager = new FileManager();
    path = new string(".profile");
    vector<string> findProfile = fileManager->readFileToString(*path);
    if (findProfile.empty()) {
        vector<string> *defaultContent = new vector<string>();
        for (string line : defaultProfile) {
            defaultContent->push_back(line);
        }
        fileManager->replaceFileContent(*path, *defaultContent);
        cout << "Profile was not found or cannot be opened" << endl;
        cout << "Default profile used" << endl;
        content = defaultContent;
    } else {
        vector<string> *profile = new vector<string>();
        for (string line : findProfile) {
            profile->push_back(line);
        }
        content = profile;
    }
    setAlarmStatus();
}

void Profile::setAlarmStatus() {
    bool alarmFind = false;
    for (string line: *content) {
        size_t index = line.find("ALARM");
        if (index == string::npos) {
#ifdef DEBUG
            cout << "ALARM line not found" << endl;
#endif
        } else {
#ifdef DEBUG
            cout << "ALARM line found at " << index << endl;
#endif
            string alarmString = line.substr(index+5, line.size());
            if(alarmString.find("ON")==1){
                if(alarmFind){
                    cout <<"Multiple definition detected of ALARM on the .profile file!"<<endl;
                }
#ifdef DEBUG
                cout << "ALARM line is ON ! " << endl;
#endif
                alarmFind = true;
                Command::setAlarmEnabled(true);
            }
            if(alarmString.find("OFF") == 1){
                if(alarmFind) {
                    cout << "Multiple definition detected of ALARM on the .profile file!" << endl;
                }
#ifdef DEBUG
                cout << "ALARM line is OFF ! " << endl;
#endif
                alarmFind = true;
                Command::setAlarmEnabled(false);

            }
            cout<< "AlarmString is " << alarmString <<endl;
#ifdef DEBUG
            cout << "ALARM line parsed ! " << endl;
#endif
        }
    }
    if(!alarmFind){
        Command::setAlarmEnabled(true);
        cout << "Alarm status not found or corrupted on the .profile file, be sure to follow the following format ALARM=ON || ALARM=OFF" << endl;
        cout << "Alarm is activated by default" << endl;
    }
#ifdef DEBUG
    cout << "Alarm is set to "<< Command::isAlarmEnabled() <<endl;
#endif
}

Profile::~Profile(void) {
#ifdef DEBUG
    cout << "Deleting fileManager in profile" << endl;
#endif
    if (fileManager != NULL) {
        delete fileManager;
        fileManager = NULL;
    }
#ifdef DEBUG
    cout << "fileManager deleted in profile" << endl;
    cout << "Deleting path in profile" << endl;
#endif
    if (path != NULL) {
        delete path;
        path = NULL;
    }
#ifdef DEBUG
    cout << "path deleted in profile" << endl;
    cout << "Deleting content in profile" << endl;
#endif
    if (content != NULL) {
        delete content;
        content = NULL;
    }
#ifdef DEBUG
    cout << "content deleted in profile" << endl;
#endif
}