/**
 * Project 1 Develop your own shell
 * @author Jules Baud / Valentin Pichavant / Antoine Regnier
 * @version 1.0
 */


#ifndef _HISTORY_H
#define _HISTORY_H

#include <string>
#include <vector>

#include "FileManager.h"
#include "Command.h"

using namespace std;

class History {
public:

    /**
     * A string which represent the default path of the profile
     */
    static const string defaultPath;

    /**
     * Construct an History instance
     */
    History();

    /**
     * Destroy an History instance
     */
    ~History();

    /**
     * @return The history path string
     */
    string * getPath();

    /**
     * Set the path string of the history and remove the previous one if exists
     * @param value The new value of history path
     */
    void setPath(string * value);

    /**
     * @return The file manager of the history
     */
    FileManager * getFileManager();

    /**
     * Set the file manager of the history and remove the previous one if exists
     * @param value The new value of the file manager
     */
    void setFileManager(FileManager * value);

    /**
     * Log a command in the history
     * @param command The command to be logged in the history
     */
    void logCommand(Command * command);

private:
    string * path;
    FileManager * fileManager;
};

#endif //_HISTORY_H
