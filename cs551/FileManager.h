/**
 * Project 1 Develop your own shell
 * @author Jules Baud / Valentin Pichavant / Antoine Regnier
 * @version 1.0
 */


#ifndef _FILEMANAGER_H
#define _FILEMANAGER_H

#include <iostream>
#include <string>
#include <fstream>
#include <vector>

using namespace std;

class FileManager {
public: 
    
    FileManager();

    ~FileManager();

    bool appendToFile(string filePath, vector<string> textToAppend);

    bool replaceFileContent(string filePath, vector<string> newFileContent);

    vector<string> readFileToString(string filePath);

private:
    ofstream * outStream;
    ifstream * inStream;
};

#endif //_FILEMANAGER_H
