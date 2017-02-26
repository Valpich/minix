/**
 * Project 1 Develop your own shell
 * @author Jules Baud / Valentin Pichavant / Antoine Regnier
 * @version 1.0
 */


#ifndef _PROFILE_H
#define _PROFILE_H

#include <string>
#include <vector>

#include "FileManager.h"

using namespace std;

class Profile {
public:

    /**
     * A string array which represent the default profile
     */
    static const char *defaultProfile[];

    /**
     * Construct a Profile instance
     */
    Profile();

    /**
     * Destroy a Profile instance
     */
    ~Profile();

    /**
     * @return The profile path string
     */
    string * getPath();

    /**
     * Set the path string of the profile and remove the previous one if exists
     * @param value The new value of profile path
     */
    void setPath(string * value);

    /**
      * @return The profile content as an array of string (one string per line)
      */
    vector<string> * getContent();

    /**
     * Set the content of the profile and remove the previous one if exists
     * @param value The new value of the content
     */
    void setContent(vector<string> * value);

    /**
     * @return The file manager of the profile
     */
    FileManager * getFileManager();

    /**
     * Set the file manager of the profile and remove the previous one if exists
     * @param value The new value of the file manager
     */
    void setFileManager(FileManager * value);

    /**
     * @return The default content of the profile
     */
    vector<string> *getDefaultContent();
    
private: 
    string * path;
    vector<string> * content;
    FileManager * fileManager;

    /**
     * Utility method used to set if the shell uses or not an alarm of 5 second after starting a command
     */
    void setAlarmStatus();
};

#endif //_PROFILE_H
