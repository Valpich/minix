/**
 * Project 1 Develop your own shell
 * @author Jules Baud / Valentin Pichavant / Antoine Regnier
 * @version 1.0
 */


#ifndef _PROFILE_H
#define _PROFILE_H

#include <string>

#include "FileManager.h"

using namespace std;

class Profile {
public: 
    
    Profile();

    ~Profile();

    string getPath();
    
    /**
     * @param value
     */
    void setPath(string value);
    
    string getHome();
    
    /**
     * @param value
     */
    void setHome(string value);
    
    /**
     * @param value
     */
    void setFileManager(FileManager value);
    
    FileManager getFileManager();
private: 
    string * path;
    string * home;
    FileManager * fileManager;
};

#endif //_PROFILE_H
