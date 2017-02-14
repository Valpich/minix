/**
 * Project 1 Develop your own shell
 * @author Jules Baud / Valentin Pichavant / Antoine Regnier
 * @version 1.0
 */


#include "CommandManager.h"

/**
 * Command implementation
 */


void CommandManager::executeInParallelWithWait(vector <Command *> commands) {
    vector<thread> threads;
    for (Command * command: commands) {
        shared_ptr<Command> ptr_command(command);
        cout << " Command name " << *command->getName() <<endl;
        thread tmp(bind(&Command::execute, ptr_command));
        threads.push_back(move(tmp));
    }
    cout<< "Threads started" <<endl;
    unsigned int size = threads.size();
    cout<< "Threads size is "<< size <<endl;
    for(unsigned int i=0; i<size; ++i){
        threads.at(i).join();
    }
    cout<< "Threads joined" <<endl;
}

void CommandManager::executeInParallelWithoutWait(vector <Command *> commands) {
    for (Command * command: commands) {
        shared_ptr<Command> ptr_command(command);
        cout << " Command name " << *command->getName() <<endl;
        thread tmp(bind(&Command::execute, ptr_command));
        tmp.detach();
    }
    cout<< "Threads started" <<endl;
}

CommandManager::CommandManager(void){

}

CommandManager::~CommandManager(void) {

}
