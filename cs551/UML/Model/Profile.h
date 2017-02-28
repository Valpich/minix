/**
 * Project 1 Develop your own shell
 * @author Jules Baud / Valentin Pichavant / Antoine Regnier
 * @version 1.0
 */


#ifndef _PROFILE_H
#define _PROFILE_H

#include "FileManager.h"


class Profile {
public:

    std::string getPath();

    /**
     * @param value
     */
    void setPath(std::string value);

    std::string getHome();

    /**
     * @param value
     */
    void setHome(std::string value);

    /**
     * @param value
     */
    void setFileManager(FileManager value);

    FileManager getFileManager();

private:
    std::string path;
    std::string home;
    FileManager fileManager;
};

#endif //_PROFILE_H