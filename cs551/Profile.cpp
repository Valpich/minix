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
        {"PATH=",
         "PATH=/sbin:/usr/sbin:/bin:/usr/bin:/usr/pkg/sbin:/usr/pkg/bin",
         "PATH=${PATH}:/usr/X11R7/bin:/usr/X11R6/bin:/usr/local/sbin:/usr/local/bin",
         "ALARM=ON", "HOST=\"$(hostname)\""};

string *Profile::getPath() {
    return path;
}

void Profile::setPath(string *value) {
    if (path != NULL) {
        delete path;
        path = NULL;
    }
    path = value;
}

vector<string> *Profile::getContent() {
    return content;
}

void Profile::setContent(vector<string> *value) {
    if (content != NULL) {
        delete content;
        content = NULL;
    }
    content = value;
}

void Profile::setFileManager(FileManager *value) {
    if (fileManager != NULL) {
        delete fileManager;
        fileManager = NULL;
    }
    fileManager = value;
}

FileManager *Profile::getFileManager() {
    return fileManager;
}

vector<string> *Profile::getDefaultContent() {
    vector<string> *defaultContent = new vector<string>();
    for (string line : defaultProfile) {
        defaultContent->push_back(line);
    }
    return defaultContent;
}

void Profile::setAlarmStatus() {
    bool alarmFind = false;
    // We try to find the alarm in the profile content for each line
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
            string alarmString = line.substr(index + 5, line.size());
            // If the value contains ON
            if (alarmString.find("ON") == 1) {
                // If we have already find an alarm status we inform the user, the
                if (alarmFind) {
                    cout << "Multiple definition detected of ALARM on the .profile file!" << endl;
                    cout << "Last definition is used." << endl;
                }
#ifdef DEBUG
                cout << "ALARM line is ON ! " << endl;
#endif
                alarmFind = true;
                Command::setAlarmEnabled(true);
            } else if (alarmString.find("OFF") == 1) {
                // If we have already find an alarm status we inform the user, the
                if (alarmFind) {
                    cout << "Multiple definition detected of ALARM on the .profile file!" << endl;
                    cout << "Last definition is used." << endl;
                }
#ifdef DEBUG
                cout << "ALARM line is OFF ! " << endl;
#endif
                alarmFind = true;
                Command::setAlarmEnabled(false);

            }
#ifdef DEBUG
            cout << "AlarmString is " << alarmString << endl;
            cout << "ALARM line parsed ! " << endl;
#endif
        }
    }
    // If we didn't find the alarm status, the alarm is activated by default
    if (!alarmFind) {
        Command::setAlarmEnabled(true);
        cout
                << "Alarm status not found or corrupted on the .profile file, be sure to follow the following format ALARM=ON || ALARM=OFF"
                << endl;
        cout << "Alarm is activated by default" << endl;
    }
#ifdef DEBUG
    cout << "Alarm is set to " << Command::isAlarmEnabled() << endl;
#endif
}

ostream &operator<<(ostream &os, const Profile &profile) {
    os << "path: ";
    if (profile.path == NULL) {
        os << "NULL";
    } else {
        os << *profile.path;
    }
    os << " content: ";
    if (profile.content == NULL) {
        os << "NULL";
    } else {
        for (string stringTmp : *profile.content)
            os << stringTmp;
    }
    os << " fileManager: ";
    if (profile.fileManager == NULL) {
        os << "NULL";
    } else {
        os << *profile.fileManager;
    }
    return os;
}

Profile::Profile(void) {
    // We create a new file manager
    fileManager = new FileManager();
    path = new string(".profile");
    // We try to read the profile
    vector<string> findProfile = fileManager->readFileToString(*path);
    // If their is no profile then we use the default profile and try to replace the previous content by the default profile
    if (findProfile.empty()) {
        vector<string> *defaultContent = new vector<string>();
        for (string line : defaultProfile) {
            defaultContent->push_back(line);
        }
        fileManager->replaceFileContent(*path, *defaultContent);
        cout << "Profile was not found or cannot be opened" << endl;
        cout << "Default profile used, old profile replaced" << endl;
        content = defaultContent;
    } else {
        // Else we get the found profile
        vector<string> *profile = new vector<string>();
        for (string line : findProfile) {
            profile->push_back(line);
        }
        content = profile;
    }
    // We set up or not the alarm using the profile content
    setAlarmStatus();
}

Profile::~Profile(void) {
#ifdef CLEAN
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
#endif
}