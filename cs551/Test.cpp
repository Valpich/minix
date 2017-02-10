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
void Test::executeTest1() {
    cout << "First test started !! " << endl;
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
    cout << "End of first test !! " << endl;
}

void Test::executeTest2() {
    cout << "Second test started !! " << endl;
    Profile * profile = new Profile();
    cout << "Content " << endl;
    for(string line: *profile->getContent()) {
        cout << line << endl;
    }
    cout << "End of content " << endl;
    delete profile;
    cout << "End of second test !! " << endl;
}


void Test::executeTest3() {
    cout << "Third test started !! " << endl;
    Command * command = new Command();
    command->setName(new string("ls"));
    cout << command->execute("EXAMPLE=test env","-al") << endl;
    delete command;
    cout << "End of third test !! " << endl;
}