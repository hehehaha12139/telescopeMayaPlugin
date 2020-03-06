#include <stdio.h>
#include <math.h>
#include <iostream>
#include "LSystem.h"

int main(int argc, char **argv)
{
    LSystem system;
    system.loadProgramFromString("X\nX->F[+X]F[-X]+X\nF->FF");
    system.setDefaultAngle(25.7f);
    system.setDefaultStep(1.0f);
    std::vector<LSystem::Branch> branches;
    for (int i = 0; i < 2; i++)
    {
        std::string insn = system.getIteration(i);
        std::cout << insn << std::endl;

        
        system.process(i, branches);
    }
    return 0;      
}
