#ifndef CreateLSystemCmd_H_
#define CreateLSystemCmd_H_

#include <maya/MPxCommand.h>
#include <maya/MSyntax.h>
#include <string>

class LSystemCmd : public MPxCommand
{
public:
    LSystemCmd();
    virtual ~LSystemCmd();
    static void* creator() { return new LSystemCmd(); }
    MStatus doIt( const MArgList& args );
    static MSyntax newSyntax() 
    {
        MSyntax syntax;

        // Step size
        syntax.addFlag("-s", "-step", MSyntax::kDouble);

        // Angle
        syntax.addFlag("-a", "-angle", MSyntax::kDouble);

        // Grammar
        syntax.addFlag("-g", "-grammar", MSyntax::kString);

        // Iteration
        syntax.addFlag("-i", "-iter", MSyntax::kLong);

        return syntax;
    }
};

#endif