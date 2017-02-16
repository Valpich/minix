/**
 * Project 1 Develop your own shell
 * @author Jules Baud / Valentin Pichavant / Antoine Regnier
 * @version 1.0
 */


#include "CommandManager.h"

/**
 * CommandManager implementation
 */


void CommandManager::executeInParallelWithWaitAndPopen(vector<Command *> commands) {
#ifndef MINIX
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
#endif
    // TODO: the same with forks and wait
}

void CommandManager::executeInParallelWithoutWaitAndPopen(vector<Command *> commands) {
#ifndef MINIX
    for (Command * command: commands) {
        shared_ptr<Command> ptr_command(command);
        cout << " Command name " << *command->getName() <<endl;
        thread tmp(bind(&Command::execute, ptr_command));
        tmp.detach();
    }
    cout<< "Threads started" <<endl;
#endif
    // TODO: the same with forks without wait
}

CommandManager::CommandManager(void){

}

CommandManager::~CommandManager(void) {

}
