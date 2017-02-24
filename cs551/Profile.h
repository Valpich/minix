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

    static const char *defaultProfile[];

    Profile();

    ~Profile();

    string * getPath();
    
    /**
     * @param value
     */
    void setPath(string * value);
    
    vector<string> * getContent();
    
    /**
     * @param value
     */
    void setContent(vector<string> * value);
    
    /**
     * @param value
     */
    void setFileManager(FileManager * value);
    
    FileManager * getFileManager();
private: 
    string * path;
    vector<string> * content;
    FileManager * fileManager;

    void setAlarmStatus();
};

#endif //_PROFILE_H
