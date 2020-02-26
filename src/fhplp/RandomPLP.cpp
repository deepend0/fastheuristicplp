/*
 * RandomPLP.cpp
 *
 *  Created on: Aug 11, 2019
 *      Author: oakile
 */

#include "RandomPLP.hpp"

#include <random>
#include <vector>

namespace fhplp {

RandomPLP::RandomPLP(int numPoints, int numPositionsPerPoint) {
	this->numPoints = numPoints;
	this->numPositionsPerPoint=numPositionsPerPoint;
}

RandomPLP::~RandomPLP() {
}


Solution* RandomPLP::produceSolution(ConflictGraph* conflictGraph) {
	Solution* solution = new Solution();
	std::vector<PositionNode*>* positions = new std::vector<PositionNode*>();
	solution->setPositions(positions);

	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<> positionGen(0, numPositionsPerPoint-1);

	for(int i=0; i<numPoints; i++) {
		positions->push_back(NULL);
	}

	for(int i=0; i<numPoints; i++) {
		int positionNo = i*numPositionsPerPoint + positionGen(gen)+1;
		PositionNode* positionNode = new PositionNode(positionNo);
		(*positions)[i] = positionNode;
	}

	for(int i=0; i<numPoints; i++) {
		PositionNode* positionNode = (*positions)[i];
		std::vector<PositionNode*>* conflictingPositions = new std::vector<PositionNode*>();
		positionNode->setConflictingPositions(conflictingPositions);

		int posIndex = (positionNode->getNo()-1) % numPositionsPerPoint;
		PointFeature* pointFeature = (*conflictGraph->getPointFeatures())[i];
		PositionNode* positionNodeCG = (*pointFeature->getPositions())[posIndex];
		std::vector<PositionNode*>* conflictingPositionsCG = positionNodeCG->getConflictingPositions();
		for(std::vector<PositionNode*>::iterator it=conflictingPositionsCG->begin(); it!=conflictingPositionsCG->end(); it++) {
			PositionNode* conflictingPositionCG = *it;
			int conflictingPositionPointIx = (conflictingPositionCG->getNo()-1) / numPositionsPerPoint;
			PositionNode* conflictingPosition=(*positions)[conflictingPositionPointIx];
			if(conflictingPosition->getNo()==conflictingPositionCG->getNo()) {
				conflictingPositions->push_back(conflictingPosition);
			}
		}
	}
	return solution;
}

} /* namespace fhplp */
