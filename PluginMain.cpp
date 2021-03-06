#include <maya/MPxCommand.h>
#include <maya/MFnPlugin.h>
#include <maya/MIOStream.h>
#include <maya/MString.h>
#include <maya/MArgList.h>
#include <maya/MGlobal.h>
#include <maya/MSimple.h>
#include <maya/MDoubleArray.h>
#include <maya/MPoint.h>
#include <maya/MPointArray.h>
#include <maya/MFnNurbsCurve.h>
#include <maya/MDGModifier.h>
#include <maya/MPlugArray.h>
#include <maya/MVector.h>
#include <maya/MFnDependencyNode.h>
#include <maya/MStringArray.h>
#include <list>
#include <string>
#include <fstream>
#include <streambuf>

#include "LSystemCmd.h"
#include "LSystemNode.h"


MStatus initializePlugin( MObject obj )
{
    MStatus   status = MStatus::kSuccess;
    MFnPlugin plugin( obj, "MyPlugin", "1.0", "Any");

    // Register Command
    status = plugin.registerCommand( "LSystemCmd", LSystemCmd::creator, LSystemCmd::newSyntax);
    if (!status) 
    {
        status.perror("registerCommand");
        return status;
    }
    
    // Register Node
    status = plugin.registerNode("LSystemNode", LSystemNode::id, LSystemNode::creator, LSystemNode::initialize);
    if (!status) 
    {
        status.perror("registerNode");
        return status;
    }

    //char buffer[2048]; 
    //sprintf_s(buffer, 2048, "menu -label \"LSystemCmd\" -tearOff true"); 
    //MGlobal::executeCommand(buffer, true);
    std::ifstream melFileRead("menu.mel");
    std::string str((std::istreambuf_iterator<char>(melFileRead)),
					 std::istreambuf_iterator<char>());
    MGlobal::executeCommand(str.c_str());

    return status;
}

MStatus uninitializePlugin( MObject obj)
{
    MStatus status = MStatus::kSuccess;
    MFnPlugin plugin( obj );

    status = plugin.deregisterCommand( "LSystemCmd" );
    if (!status) 
    {
	    status.perror("deregisterCommand");
	    return status;
    }

    status = plugin.deregisterNode(LSystemNode::id);
    if (!status) 
    {
        status.perror("deregisterNode");
        return status;
    }

    return status;
}


