/**
 * Project 1 Develop your own shell
 * @author Jules Baud / Valentin Pichavant / Antoine Regnier
 * @version 1.0
 */


#include "ContextSaver.h"

/**
 * ContextSaver implementation
 */


/**
 * @return std::jmp_buf
 */
std::jmp_buf ContextSaver::getJumpBuffer() {
    return jumpBuffer;
}

/**
 * @param value
 */
void ContextSaver::setJumpBuffer(std::jmp_buf value) {
	jumpBuffer = value;
}

ContextSaver::ContextSaver(void){

}

ContextSaver::~ContextSaver(void){
	
}