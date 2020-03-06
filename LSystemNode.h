#pragma once
#include <maya/MPxNode.h>
#include <maya/MStatus.h>
#include <maya/MFnNumericAttribute.h>
#include <maya/MDataBlock.h>
#include <maya/MObject.h>
#include <maya/MFnTypedAttribute.h>
#include <maya/MFnUnitAttribute.h>

#include "LSystem.h"

#define McheckErr(stat,msg)                     \
        if ( MS::kSuccess != stat ) {   \
                cerr << msg;                            \
                return MS::kFailure;            \
        }

class LSystemNode : public MPxNode 
{
public:
	LSystemNode() {};
	virtual ~LSystemNode() {};
	virtual MStatus compute(const MPlug& plug, MDataBlock& data);
	static void* creator();
	static MStatus initialize();

	static MTypeId id;

	// Attributes
	static MObject angle;
	static MObject stepSize;
	static MObject grammar;
	static MObject time;
	static MObject outputGeo;
	static MObject iteration;

	LSystem nodeLSystem;
};