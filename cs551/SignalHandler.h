/**
 * Project 1 Develop your own shell
 * @author Jules Baud / Valentin Pichavant / Antoine Regnier
 * @version 1.0
 */


#ifndef _SIGNALHANDLER_H
#define _SIGNALHANDLER_H

#include <csignal>

class SignalHandler {
protected: 
    
    SignalHandler();
    
	~SignalHandler();

    void signalHandler(int sigNumber);
};

#endif //_SIGNALHANDLER_H