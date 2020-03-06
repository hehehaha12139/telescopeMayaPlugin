#include "LSystemNode.h"
#include <maya/MTime.h>
#include <maya/MFnMeshData.h>
#include <maya/MFloatPointArray.h>
#include <maya/MFnMesh.h>
#include "cylinder.h"

MTypeId LSystemNode::id(0x80001);
MObject LSystemNode::angle;
MObject LSystemNode::stepSize;
MObject LSystemNode::grammar;
MObject LSystemNode::time;
MObject LSystemNode::outputGeo;
MObject LSystemNode::iteration;

void* LSystemNode::creator() 
{
	return new LSystemNode;
}

MStatus LSystemNode::initialize() 
{
	MFnNumericAttribute numAttr;
	MFnUnitAttribute unitAttr;
	MFnTypedAttribute typeAttr;

	MStatus returnStatus;

	cout << "InitializeNode!" << endl;

	LSystemNode::angle = numAttr.create("angle", "an", MFnNumericData::kDouble, 30.0, &returnStatus);
	McheckErr(returnStatus, "ERROR creating lSystem angle attribute\n");

	LSystemNode::stepSize = numAttr.create("step", "st", MFnNumericData::kDouble, 1.0, &returnStatus);
	McheckErr(returnStatus, "ERROR creating lSystem step size attribute\n");

	LSystemNode::grammar = typeAttr.create("grammar", "grm", MFnData::kString, &returnStatus);
	McheckErr(returnStatus, "ERROR creating lSystem grammar attribute\n");

	LSystemNode::outputGeo = typeAttr.create("outGeo", "oGe", MFnData::kMesh, &returnStatus);
	McheckErr(returnStatus, "ERROR creating lSystem output geometry attribute\n");

	LSystemNode::time = unitAttr.create("time", "tm", MFnUnitAttribute::kTime, 0.0, &returnStatus);
	McheckErr(returnStatus, "ERROR creating lSystem time attribute\n");

	LSystemNode::iteration = numAttr.create("iteration", "it", MFnNumericData::kInt, 1, &returnStatus);
	McheckErr(returnStatus, "ERROR creating lSystem iteration attribute");

	typeAttr.setStorable(false);

	returnStatus = addAttribute(LSystemNode::angle);
	McheckErr(returnStatus, "ERROR adding angle attribute\n");

	returnStatus = addAttribute(LSystemNode::stepSize);
	McheckErr(returnStatus, "ERROR adding step size attribute\n");

	returnStatus = addAttribute(LSystemNode::grammar);
	McheckErr(returnStatus, "ERROR adding grammar attribute\n");

	returnStatus = addAttribute(LSystemNode::time);
	McheckErr(returnStatus, "ERROR adding time attribute\n");

	returnStatus = addAttribute(LSystemNode::outputGeo);
	McheckErr(returnStatus, "ERROR adding outputGeo attribute\n");

	returnStatus = attributeAffects(LSystemNode::time, LSystemNode::outputGeo);
	McheckErr(returnStatus, "ERROR in attributeAffects\n");

	returnStatus = addAttribute(LSystemNode::iteration);
	McheckErr(returnStatus, "ERROR adding iteration attribute\n");

	return MS::kSuccess;
}

MStatus LSystemNode::compute(const MPlug& plug, MDataBlock& data) 
{
	MStatus status;

	if (plug == outputGeo)
	{
		MDataHandle timeData = data.inputValue(time, &status);
		McheckErr(status, "Error getting time handle");
		MTime curTime = timeData.asTime();

		MDataHandle geoData = data.outputValue(outputGeo, &status);
		McheckErr(status, "Error getting output geo handle");

		MDataHandle angleData = data.inputValue(angle, &status);
		McheckErr(status, "Error getting angle handle");
		double nodeAngle = angleData.asDouble();

		MDataHandle stepData = data.inputValue(stepSize, &status);
		McheckErr(status, "Error getting step size handle");
		double nodeStep = stepData.asDouble();

		MDataHandle gramData = data.inputValue(grammar, &status);
		McheckErr(status, "Error getting grammar handle");
		MString nodeGrammar = gramData.asString();

		MDataHandle iterData = data.inputValue(iteration, &status);
		McheckErr(status, "Error getting iteration handle");
		int iteration = iterData.asInt();

		nodeLSystem.loadProgramFromString(nodeGrammar.asChar());
		nodeLSystem.setDefaultAngle(nodeAngle);
		nodeLSystem.setDefaultStep(nodeStep);

		string insn = nodeLSystem.getIteration(iteration);
		std::vector<LSystem::Branch> branches;
		nodeLSystem.process(iteration, branches);

		// Set mesh
		int numVertices, frame, numFaces;
		float meshSize = 0.5f;
		MPointArray points;
		MIntArray faceFinalCount;
		MIntArray faceFinalConnects;
		MFnMesh meshFS;

		frame = (int)curTime.as(MTime::kFilm);
		if (frame == 0) 
			frame = 1;
		
		
		for (int i = 0; i < branches.size(); i++) 
		{
			LSystem::Branch curBranch = branches.at(i);
			vec3 first = curBranch.first;
			vec3 second = curBranch.second;

			CylinderMesh cyMesh = CylinderMesh(MPoint(first[0], first[2], first[1]), 
				                               MPoint(second[0], second[2], second[1]));
			
			cyMesh.appendToMesh(points, faceFinalCount, faceFinalConnects);
		}
		numVertices = points.length();
		numFaces = faceFinalCount.length();

		MFnMeshData dataCreator;
		MObject newOutputData = dataCreator.create(&status);
		McheckErr(status, "ERROR creating outputData");

		MObject newMesh = meshFS.create(numVertices, numFaces, points, 
									    faceFinalCount, faceFinalConnects, 
			                            newOutputData, &status);

		geoData.set(newOutputData);
		data.setClean(plug);
	}
	else
		return MS::kUnknownParameter;

	return MS::kSuccess;
}