/**
 * Project 1 Develop your own shell
 * @author Jules Baud / Valentin Pichavant / Antoine Regnier
 * @version 1.0
 */


#ifndef _COMMANDFINDER_H
#define _COMMANDFINDER_H

#include <string>
#include <vector>

#include "Command.h"
#include "Profile.h"

using namespace std;

class CommandFinder {
public: 
    
    CommandFinder();

    ~CommandFinder();

    string * getFolderPaths();

    /**
     * @param value
     */
    void setFolderPaths(string * value);


    Profile * getProfile();

    /**
     * @param value
     */
    void setProfile(Profile * value);
    
    vector<Command> * findAllCommands();
private: 
    string * folderPaths;
    Profile * profile;

    vector<string> * parseProfileContent();
};

#endif //_COMMANDFINDER_H
