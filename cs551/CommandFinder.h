/**
 * Project 1 Develop your own shell
 * @author Jules Baud / Valentin Pichavant / Antoine Regnier
 * @version 1.0
 */


#ifndef _COMMANDFINDER_H
#define _COMMANDFINDER_H

#include <string>
#include <vector>
#include <dirent.h>
#include <unistd.h>
#include <ostream>

#include "Command.h"
#include "Profile.h"

using namespace std;

class CommandFinder {
public:

    /**
     * Construct a CommandFinder instance
     */
    CommandFinder();

    /**
     * Destroy a CommandFinder instance
     */
    ~CommandFinder();

    /**
     * @return The folder paths string
     */
    string *getFolderPaths();

    /**
     * Set the folder paths string and remove the previous one if exists
     * @param value The new value of the folder paths
     */
    void setFolderPaths(string *value);

    /**
     * @return The profile used to parse in the CommandFinder
     */
    Profile *getProfile();

    /**
     * Set the new profile  and remove the previous one if exists
     * @param value The new value of the profile
     */
    void setProfile(Profile *value);

    /**
     * Convert all potentials commands found in the path(s) and add them to the vector givin as parameter
     * @param commands The vector used to contains all the commands
     */
    void findAllCommands(vector<Command *> *commands);

    /**
     * Overload the ostream operator to display as a string a CommandFinder
     */
    friend ostream &operator<<(ostream &os, const CommandFinder &finder);

private:
    string *folderPaths;
    Profile *profile;

    /**
     * Utility method used to parse the profile content to find the PATH values
     * @return The paths as a vector of string for the profile
     */
    vector<string> *parseProfilePathContent();

    /**
     * Utility method used to parse the a line of path which can contains multiple paths into a vector of path as string
     * @param line The line to parse
     * @return The vector of possible paths for a line
     */
    vector<string> *parseLinePath(string line);

    /**
     * Utility method used to valide if a token is (a path into a list of paths) is valid or not
     * @param token The token to be validate
     * @return NULL if the token is invalid else the valid token
     */
    string *validateToken(const string &token);

    /**
     * Utility method used to split a string into a vector of string using a separator
     * @param s The string to split
     * @param c The separator character
     * @param v The vector of splitted string
     */
    void split(const string &s, char c, vector<string> &v);

    /**
     * Utility method used to get all files in a directory
     * @param listOfCommand The possible list of commands
     * @param directory The directory where to look if their is commands
     */
    void getFilesInDirectory(vector<string> *listOfCommand, const string &directory);
};

#endif //_COMMANDFINDER_H
