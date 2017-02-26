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

    /**
     * Construct a FileManager instance
     */
    FileManager();

    /**
     * Destroy a FileManager instance
     */
    ~FileManager();

    /**
     * Add at the end of a file the text represented as a vector a string (a string is a line)
     * @return True if the append succeed else false
     */
    bool appendToFile(string filePath, vector<string> textToAppend);

    /**
     * Replace the file content by the text represented as a vector a string (a string is a line)
     * @return True if the replace succeed else false
     */
    bool replaceFileContent(string filePath, vector<string> newFileContent);

    /**
     * Read a file content and returns the text represented as a vector a string (a string is a line)
     * @return A vector of string containing the file content (can be empty if the file is empty or cannot be read)
     */
    vector<string> readFileToString(string filePath);

    /**
     * Overload the ostream operator to display as a string a FileManager
     */
    friend ostream &operator<<(ostream &os, const FileManager &manager);

private:
    ofstream *outStream;
    ifstream *inStream;
};

#endif //_FILEMANAGER_H
