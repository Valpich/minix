/**
 * Project 1 Develop your own shell
 * @author Jules Baud / Valentin Pichavant / Antoine Regnier
 * @version 1.0
 */


#include "Test.h"

/**
 * Test implementation
 */

Test::Test(void) {

}

Test::~Test(void) {

}

/**
 * @param value
 */
void Test::executeTestOne() {
    cout << "First " << endl;
    FileManager *fm = new FileManager();
    vector<string> fileStrings = fm->readFileToString("test.txt");
    cout << "Second " << endl;
    vector<string> fileStringsTwo;
    fileStringsTwo.push_back("Hi,");
    fileStringsTwo.push_back("This is a test.");
    fm->replaceFileContent("test.txt", fileStringsTwo);
    cout << "Third " << endl;
    fileStrings = fm->readFileToString("test.txt");
    cout << "Fourth " << endl;
    vector<string> fileStringsFour;
    fileStringsFour.push_back("At the end");
    fm->appendToFile("test.txt", fileStringsFour);
    cout << "File content " << endl;
    fm->readFileToString("test.txt");
    cout << "End " << endl;
    delete fm;
    remove("test.txt");
    bool failed = !ifstream("test.txt");
    if (!failed) {
        cerr << "Error deleting file" << endl;
    }
}

