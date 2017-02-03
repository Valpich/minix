/**
 * Project 1 Develop your own shell
 * @author Jules Baud / Valentin Pichavant / Antoine Regnier
 * @version 1.0
 */


#ifndef _FILEMANAGER_H
#define _FILEMANAGER_H

#include <string>

class FileManager {
public: 
    
    FileManager();

    ~FileManager();

    std::string getFilePath();
    
    /**
     * @param value
     */
    void setFilePath(std::string value);
private: 
    std::string filePath;
};

#endif //_FILEMANAGER_H
