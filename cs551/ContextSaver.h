/**
 * Project 1 Develop your own shell
 * @author Jules Baud / Valentin Pichavant / Antoine Regnier
 * @version 1.0
 */


#ifndef _CONTEXTSAVER_H
#define _CONTEXTSAVER_H

#include <csetjmp>

class ContextSaver {
public: 
    
    ContextSaver();

    ~ContextSaver();

    std::jmp_buf getJumpBuffer();
    
    /**
     * @param value
     */
    void setJumpBuffer(std::jmp_buf value);
private: 
    std::jmp_buf jumpBuffer;
};

#endif //_CONTEXTSAVER_H
