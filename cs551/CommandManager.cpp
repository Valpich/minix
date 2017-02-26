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
    //If we are not on minix
#ifndef MINIX
    vector<thread> threads;
    for (Command *command: commands) {
        shared_ptr<Command> ptr_command(command);
#ifdef DEBUG
        cout << " Command name " << *command->getName() << endl;
#endif
        thread tmp(bind(&Command::execute, ptr_command));
        threads.push_back(move(tmp));
    }
#ifdef DEBUG
    cout << "Threads started" << endl;
#endif
    unsigned int size = threads.size();
#ifdef DEBUG
    cout << "Threads size is " << size << endl;
#endif
    for (unsigned int i = 0; i < size; ++i) {
        threads.at(i).join();
    }
#ifdef DEBUG
    cout << "Threads joined" << endl;
#endif
#endif
    // TODO: the same with forks and wait
}

void CommandManager::executeInParallelWithoutWaitAndPopen(vector<Command *> commands) {
    //If we are not on minix
#ifndef MINIX
    for (Command *command: commands) {
        shared_ptr<Command> ptr_command(command);
#ifdef DEBUG
        cout << " Command name " << *command->getName() << endl;
#endif
        thread tmp(bind(&Command::execute, ptr_command));
        tmp.detach();
    }
#ifdef DEBUG
    cout << "Threads started" << endl;
#endif
#endif
    // TODO: the same with forks without wait
}

ostream &operator<<(ostream &os, const CommandManager &manager) {
    return os;
}

CommandManager::CommandManager(void) {

}

CommandManager::~CommandManager(void) {

}
