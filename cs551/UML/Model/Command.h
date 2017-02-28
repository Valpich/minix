/**
 * Project 1 Develop your own shell
 * @author Jules Baud / Valentin Pichavant / Antoine Regnier
 * @version 1.0
 */


#ifndef _COMMAND_H
#define _COMMAND_H

class Command {
public:

    std::string getName();

    /**
     * @param value
     */
    void setName(std::string value);

private:
    std::string name;
    std::string path;
};

#endif //_COMMAND_H