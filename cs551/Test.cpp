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

void Test::executeTestSuite() {
    executeTest1();
    executeTest2();
    executeTest3();
    executeTest4();
    executeTest5();
    executeTest6();
    executeTest7();
    executeTest8();
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
    command->setEnv(new string("EXAMPLE=test env"));
    command->setParams(new string("-al"));
    cout << command->execute() << endl;
    delete command;
    cout << "End of third test !! " << endl;
}

void Test::executeTest4() {
    cout << "Test four started !! " << endl;
    Command * command = new Command();
    History * history = new History();
    command->setName(new string("ls"));
    command->setEnv(new string("EXAMPLE=test env"));
    command->setParams(new string("-al"));
    cout << command->execute() << endl;
    history->logCommand(command);
    delete command;
    delete history;
    cout << "End test four !! " << endl;
}


void Test::executeTest5() {
    cout << "Test five started !! " << endl;
    vector<Command *> commands;
    CommandManager * commandManager = new CommandManager();
    Command * command = new Command();
    command->setName(new string("ls"));
    command->setEnv(new string("EXAMPLE=test env"));
    command->setParams(new string("-al"));
    commands.push_back(command);
    Command * commandTwo = new Command();
    commandTwo->setName(new string("pwd"));
    commandTwo->setEnv(new string("EXAMPLE=test env"));
    commandTwo->setParams(new string(""));
    commands.push_back(commandTwo);
    Command * commandThree = new Command();
    commandThree->setName(new string("ls"));
    commandThree->setEnv(new string("EXAMPLE=test env"));
    commandThree->setParams(new string(""));
    commands.push_back(commandThree);
    commandManager->executeInParallelWithWaitAndPopen(commands);
    delete commandManager;
    // DO NOT DELETE THIS POINTER, THREAD WILL DO IT
    // delete command;
    // delete commandTwo;
    // delete commandThree;
    cout << "End test five !! " << endl;
}


void Test::executeTest6() {
    cout << "Test six started !! " << endl;
    vector<Command *> commands;
    CommandManager * commandManager = new CommandManager();
    Command * command = new Command();
    command->setName(new string("ls"));
    command->setEnv(new string("EXAMPLE=test env"));
    command->setParams(new string("-al"));
    commands.push_back(command);
    Command * commandTwo = new Command();
    commandTwo->setName(new string("pwd"));
    commandTwo->setEnv(new string("EXAMPLE=test env"));
    commandTwo->setParams(new string(""));
    commands.push_back(commandTwo);
    Command * commandThree = new Command();
    commandThree->setName(new string("ls"));
    commandThree->setEnv(new string("EXAMPLE=test env"));
    commandThree->setParams(new string(""));
    commands.push_back(commandThree);
    commandManager->executeInParallelWithoutWaitAndPopen(commands);
    cout << "Hi" <<endl;
   delete commandManager;
    // DO NOT DELETE THIS POINTER, THREAD WILL DO IT
    // delete command;
    // delete commandTwo;
    // delete commandThree;
    cout << "End test six !! " << endl;
}

void Test::executeTest7() {
    cout << "Test seven started !! " << endl;
    waitingAlarm = true;
    Command * command = new Command();
    command->setName(new string("ls"));
    command->setEnv(new string("EXAMPLE=test env"));
    command->setParams(new string("-al"));
    command->executeWithExecve();
    delete command;
   // while(waitingAlarm);
    cout << "End test seven !! " << endl;
}

void Test::executeTest8() {
    cout << "Test 8 started !! " << endl;
    Profile * profile = new Profile();
    cout << "Content " << endl;
    for(string line: *profile->getContent()) {
        cout << line << endl;
    }
    cout << "End of content " << endl;
    CommandFinder * commandFinder = new CommandFinder();
    commandFinder->setProfile(profile);
    vector<Command *> * commands = new vector<Command *> ();
    commandFinder->findAllCommands(commands);
    cout << "The finded commands are" << endl;
    for(Command * command : *commands){
        cout<< *command <<endl;
    }
    cout << "End of the commands are" << endl;
    delete commandFinder;
    cout << "End test 8 !! " << endl;
}
