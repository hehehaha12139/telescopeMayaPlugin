#include "LSystemCmd.h"
#include <maya/MArgDatabase.h>
#include <maya/MGlobal.h>
#include <list>
#include <fstream>
#include <sstream>
#include <streambuf>

LSystemCmd::LSystemCmd() : MPxCommand()
{
}

LSystemCmd::~LSystemCmd() 
{
}

MStatus LSystemCmd::doIt( const MArgList& args )
{
	// message in Maya output window
    cout<<"Implement Me!"<<endl;
	
	double stepSize = 1.0;
	double angle = 30.0;
	MString grammar = "\"F\\nF->F[+F]F[-F]F\"";
	int iteration = 2;

	MArgDatabase argData(syntax(), args);

	// Get step size
	if (argData.isFlagSet("-s"))
		argData.getFlagArgument("-s", 0, stepSize);

	// Get angle
	if (argData.isFlagSet("-a"))
		argData.getFlagArgument("-a", 0, angle);

	// Get grammar string
	if (argData.isFlagSet("-g"))
		argData.getFlagArgument("-g", 0, grammar);

	// Get iteration
	if (argData.isFlagSet("-i"))
		argData.getFlagArgument("-i", 0, iteration);

	std::ifstream melFileRead("ui.mel");
	if (!melFileRead.is_open()) 
	{
		cout << "Error opening file"; exit(1);
	}

	std::stringstream ss;
	ss << stepSize;
	std::string stepString = ss.str();
	ss.str("");
	ss.clear();

	ss << angle;
	std::string angleString = ss.str();
	ss.str("");
	ss.clear();

	ss << iteration;
	std::string iterString = ss.str();

	std::string grammarString = grammar.asChar();

	std::string outPutGeo = "curve - d 1 - p 1 1 1 - p 10 10 10 - k 0 - k 1 - name curve1;\n"
		"circle - nr 0 0 1 - c 0 0 0 - name \"nurbsCircle1\";\n"
		"select - r \"nurbsCircle1\" curve1;\n"
		"extrude - ch true - rn false - po 1 - et 2 - ucp 1 - fpt 1 - upn 1 - rotation 0 - scale 1 - rsp 1 \"nurbsCircle1\" \"curve1\";\n";

	MGlobal::executeCommand(outPutGeo.c_str());

	/*std::string str((std::istreambuf_iterator<char>(melFileRead)),
					 std::istreambuf_iterator<char>());*/

	std::string cmd = "if (`window - exists lSystemWindow`) deleteUI lSystemWindow;\n"
                   "if (`windowPref - exists lSystemWindow`) windowPref - remove lSystemWindow;\n"
                   "string $content;\n"
                   "proc browseFiles()\n"
                   "{\n"
                   "    string $content = \"\";\n"
                   "    string $fileName[] = `fileDialog2 - fm 1 - fileFilter \"Grammar Files(*.txt)\"`;\n"
                   "    int $fileId = fopen($fileName[0], \"r\");\n"
                   "    string $nextLine = `fgetline $fileId`;\n"
                   "    while (size($nextLine) > 0)\n"
                   "    {\n"
"        $content = $content + $nextLine;\n"
"        $nextLine = `fgetline $fileId`;\n"
"    }"
"    scrollField - edit - cl grmText;\n"
"    scrollField - edit - text $content grmText;\n"
"}\n"

"proc string getText(string $default)\n"
"{\n"
"    global string $content;\n"
"    if ($content == \"\")\n"
"    {\n"
"return $default;\n"
"}\n"
"return $content;\n"
"}\n"

"proc updateNode()\n"
"{\n"
"if (`objExists lSystem`) delete lSystem;\n"
"if (`objExists lSystemShape`) delete lSystemShape;\n"
"if (`objExists lSystemNode`) delete lSystemNode;\n"
"if (`objExists lSystemTime`) delete lSystemTime;\n"

"createNode transform - n lSystem;\n"
"createNode mesh - n lSystemShape - p lSystem;\n"
"sets - add initialShadingGroup lSystemShape;\n"
"createNode LSystemNode - n lSystemNode;\n"
"createNode time - n lSystemTime;\n"
"connectAttr lSystemTime.outTime lSystemNode.time;\n"
"connectAttr lSystemNode.outGeo lSystemShape.inMesh;\n"
"float $angle = `floatSlider - q - v angleSlider`;\n"
"float $stepSize = `floatSlider - q - v stepSlider`;\n"
"int $iter = `intSlider - q - v iterSlider`;\n"
"string $grammar = `scrollField - q - text grmText`;\n"
"setAttr lSystemNode.angle $angle;\n"
"setAttr lSystemNode.step $stepSize;\n"
"setAttr lSystemNode.iteration $iter;\n"
"setAttr lSystemNode.grammar - type \"string\" $grammar;\n"
"}\n"
"proc updateAngleSlider()\n"
"{\n"
"float $value = `floatField - q - v angleField`;\n"
"floatSlider - edit - v $value angleSlider;\n"
"}\n"
"proc updateAngleField()\n"
"{\n"
"float $value = `floatSlider - q - v angleSlider`;\n"
" floatField - edit - v $value angleField;\n"
"}\n"
"proc updateStepSlider()\n"
"{\n"
"float $value = `floatField - q - v stepField`;\n"
"floatSlider - edit - v $value stepSlider;\n"
"}\n"
"proc updateStepField()\n"
"{\n"
"float $value = `floatSlider - q - v stepSlider`;\n"
" floatField - edit - v $value stepField;\n"
"}\n"
"proc updateIterSlider()\n"
"{\n"
"int $value = `intField - q - v iterField`;\n"
"intSlider - edit - v $value iterSlider;\n"
"}\n"
"proc updateIterField()\n"
"{\n"
"int $value = `intSlider - q - v iterSlider`;\n"
"intField - edit - v $value iterField;\n"
"}\n"
"window - title \"L-System Tree Generator\" - resizeToFitChildren true lSystemWindow;\n"
"columnLayout - adjustableColumn true - rowSpacing 10;\n"
"rowLayout - numberOfColumns 2 - columnWidth2 100 540 - columnAttach 1 \"left\" 10 - columnAttach 2 \"right\" 0;\n"
"text - label \"Browse File\";\n"
"button - label \"Browse..\" - command \"browseFiles()\";\n"
"setParent ..;\n"
"scrollField - width 500 - height 300 - text \"" + grammarString +"\" grmText;\n"
"frameLayout - label \"L-System Parameters\" - collapsable true - collapse true - borderStyle \"etchedIn\";\n"
"columnLayout - adjustableColumn true;\n"
"rowLayout - numberOfColumns 3 - columnWidth3 120 80 120 - columnAttach 1 \"left\" 28 - columnAttach 2 \"right\" 5 - columnAttach 3 \"left\" 0;\n"
"text - label Iterations;\n"
"intField - value "+ iterString + " - width 60 - changeCommand \"updateIterSlider()\" iterField;\n"
"intSlider - value " + iterString + "- minValue 0 - maxValue 10 - step 1 - changeCommand \"updateIterField()\" iterSlider;\n"
"setParent ..;\n"
"rowLayout - numberOfColumns 3 - columnWidth3 120 80 120 - columnAttach 1 \"left\" 28 - columnAttach 2 \"right\" 5 - columnAttach 3 \"left\" 0;\n"
"text - label Step Size;\n"
"floatField - value "+ stepString +" - minValue 1.0 - maxValue 10.0 - changeCommand \"updateStepSlider()\" stepField;\n"
"floatSlider - value " + stepString + " - minValue 1.0 - maxValue 10.0 - step 0.5 - changeCommand \"updateStepField()\" stepSlider;\n"
"setParent ..;\n"
"rowLayout - numberOfColumns 3 - columnWidth3 120 80 120 - columnAttach 1 \"left\" 28 - columnAttach 2 \"right\" 5 - columnAttach 3 \"left\" 0;\n"
"text - label angle;\n"
"floatField - value " + angleString + " - minValue 0.0 - maxValue 90.0 - changeCommand \"updateAngleSlider()\" angleField;\n"
"floatSlider - value " + angleString + " - minValue 0.0 - maxValue 90.0 - step 1.0 - changeCommand \"updateAngleField()\" angleSlider;\n"
"setParent ..;\n"
"setParent ..;\n"
"setParent ..;\n"
"rowLayout - numberOfColumns 2 - columnWidth2 100 540 - columnAttach 1 \"left\" 10 - columnAttach 2 \"right\" 0;\n"
"button - label \"Create\" - command \"updateNode()\";\n"
"button - label \"Cancel\";\n"
"setParent ..;\n"
"showWindow lSystemWindow;\n";

	std::cout.flush();
	MGlobal::executeCommand(cmd.c_str());
	// message in scriptor editor
	MGlobal::displayInfo("Implement Me!");

    return MStatus::kSuccess;
}



