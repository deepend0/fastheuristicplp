#include "ConflictGraphElements.hpp"
#include <iostream>

namespace fhplp {

int PointFeature::numPositions;
PointFeature::PointFeature()
{
	no = 0;
	positions = NULL;
}
PointFeature::PointFeature(int no)
{
	positions = NULL;
	this->no = no;
}
PointFeature::~PointFeature()
{
	if(positions!=NULL)
	{
		for(int i=0; i<positions->size(); i++)
		{
			PositionNode* position = (*positions)[i];
			if(position!=NULL)
			{
				//std::cout<<"D1"<<std::endl;
				delete position;
			}
		}
		//std::cout<<"D17"<<std::endl;
		delete positions;
	}
}
int PointFeature::getNo()
{
	return no;
}
void PointFeature::setNo(int no)
{
	this->no = no;
}
std::vector<PositionNode*>* PointFeature::getPositions()
{
	return positions;
}
void PointFeature::setPositions(std::vector<PositionNode*>* positions)
{
	this->positions = positions;
}
PositionNode::PositionNode()
{
	conflictingPositions = new std::vector<PositionNode*>();
}
PositionNode::PositionNode(int no)
{
	this->no = no;
	conflictingPositions = new std::vector<PositionNode*>();
}
PositionNode::PositionNode(int no, int pfNo)
{
	this->no = no;
	this->pointFeature = new PointFeature(pfNo);
	conflictingPositions = new std::vector<PositionNode*>();
}
PositionNode::~PositionNode()
{
	//std::cout<<"D2"<<std::endl;
	delete conflictingPositions;
}
PointFeature* PositionNode::getPointFeature()
{
	return pointFeature;
}
void PositionNode::setPointFeature(PointFeature* pointFeature)
{
	this->pointFeature = pointFeature;
}
int PositionNode::getNo()
{
	return no;
}
void PositionNode::setNo(int no)
{
	this->no = no;
}
int PositionNode::getPosIndex()
{
	return (no-1)%PointFeature::numPositions;
}
int PositionNode::getConflictSize()
{
	return conflictingPositions->size();
}
double PositionNode::getWeight()
{
	return 1.0/(double)pointFeature->getPositions()->size();
}
double PositionNode::getWeightedConflictSize()
{
	double weightedConflictSize = 0;
	for(int i=0; i<conflictingPositions->size(); i++)
	{
		PositionNode* conflictingPosition = conflictingPositions->at(i);
		PointFeature* pf = conflictingPosition->getPointFeature();
		double pfPositionSize = 1;
		if(pf != NULL)
			pfPositionSize = pf->getPositions()->size();
		weightedConflictSize += (double)1 / pfPositionSize;
	}
	return weightedConflictSize;
}
double PositionNode::getWeightedOccuringConflictSize()
{
	return getWeight()*getWeightedConflictSize();
}
std::vector<PositionNode*>* PositionNode::getConflictingPositions()
{
	return conflictingPositions;
}


double PositionNode::getProbabilisticConflictSize()
{
	double probabilisticConflictSize = 1;
	for(int i=0; i<conflictingPositions->size(); i++)
	{
		PositionNode* conflictingPosition = conflictingPositions->at(i);
		PointFeature* pf = conflictingPosition->getPointFeature();
		double pfPositionSize = 1;
		if(pf != NULL)
			pfPositionSize = pf->getPositions()->size();
		probabilisticConflictSize *= (1.0-(double)1 / pfPositionSize);
	}
	probabilisticConflictSize = 1.0 - probabilisticConflictSize;
	return probabilisticConflictSize;
}

void PositionNode::setConflictingPositions(std::vector<PositionNode*>* conflictingPositions)
{
	this->conflictingPositions = conflictingPositions;
}
int PositionNode::conflicts(PositionNode* position)
{
	for(int i=0; i<conflictingPositions->size(); i++)
	{
		PositionNode* conflictingPosition = conflictingPositions->at(i);
		if(position->getNo() == conflictingPosition->getNo())
			return i;
	}
	return -1;
}
void PositionNode::removeConflictingPosition(PositionNode* positionNode)
{
	for(int k=0; k<conflictingPositions->size(); k++)
	{
		PositionNode* conflictingPosition = conflictingPositions->at(k);
		if(conflictingPosition->getNo() == positionNode->getNo())
		{
			conflictingPositions->erase(conflictingPositions->begin() + k);
			break;
		}
	}
}

ConflictGraph::ConflictGraph()
{
}
ConflictGraph::ConflictGraph(ConflictGraph& conflictGraph)
{
	pointFeatures = new std::vector<PointFeature*>();
	numPositions = conflictGraph.getNumPositions();
	std::vector<PointFeature*>* sourcePfs = conflictGraph.getPointFeatures();
	int allPositionsCount = sourcePfs->size() * numPositions;
	PositionNode** allPositions = new PositionNode*[allPositionsCount];
	for(int i=0; i<sourcePfs->size(); i++)
	{
		PointFeature* sourcePf = sourcePfs->at(i);
		PointFeature* pointFeature = new PointFeature(sourcePf->getNo());
		pointFeatures->push_back(pointFeature);
		std::vector<PositionNode*>* sourcePositions = sourcePf->getPositions();
		std::vector<PositionNode*>* positions = new std::vector<PositionNode*>();
		pointFeature->setPositions(positions);
		for(int j=0; j<numPositions; j++)
		{
			PositionNode* sourcePn = sourcePositions->at(j);
			PositionNode* position = new PositionNode(sourcePn->getNo());
			position->setPointFeature(pointFeature);
			positions->push_back(position);
			allPositions[position->getNo()-1] = position;
		}
	}
	for(int i=0; i<sourcePfs->size(); i++)
	{
		PointFeature* sourcePf = sourcePfs->at(i);
		PointFeature* pointFeature = pointFeatures->at(i);
		std::vector<PositionNode*>* sourcePositions = sourcePf->getPositions();
		std::vector<PositionNode*>* positions = pointFeature->getPositions();
		for(int j=0; j<numPositions; j++)
		{
			PositionNode* sourcePn = sourcePositions->at(j);
			PositionNode* position = positions->at(j);
			std::vector<PositionNode*>* sourceConflictingPositions = sourcePn->getConflictingPositions();
			std::vector<PositionNode*>* conflictingPositions = position->getConflictingPositions();
			for(int k=0; k<sourceConflictingPositions->size(); k++)
			{
				PositionNode* conflictingPosition = sourceConflictingPositions->at(k);
				conflictingPositions->push_back(allPositions[conflictingPosition->getNo()-1]);
			}
		}
	}
	delete [] allPositions;
}
ConflictGraph::~ConflictGraph()
{
	if(pointFeatures != NULL)
	{
		for(std::vector<PointFeature*>::iterator it = pointFeatures->begin(); it != pointFeatures->end(); it++)
		{
			PointFeature* pointFeature = *it;
			//std::cout<<"D3"<<std::endl;
			delete pointFeature;
		}
		//std::cout<<"D4"<<std::endl;
		delete pointFeatures;
	}

}
int ConflictGraph::getNumPositions()
{
	return numPositions;
}
void ConflictGraph::setNumPositions(int numPositions)
{
	this->numPositions = numPositions;
}
std::vector<PointFeature*>* ConflictGraph::getPointFeatures()
{
	return pointFeatures;
}
void ConflictGraph::setPointFeatures(std::vector<PointFeature*>* pointFeatures)
{
	this->pointFeatures = pointFeatures;
}
std::vector<PointFeature*>* ConflictGraph::removePosition(PositionNode* position)
{
	std::vector<PointFeature*>* affectedPfs = new std::vector<PointFeature*>();
	PointFeature* pointFeature1 = position->getPointFeature();
	affectedPfs->push_back(pointFeature1);
	std::vector<PositionNode*>* positions = pointFeature1->getPositions();
	for(int i=0; i<positions->size(); i++)
	{
		if(positions->at(i)->getNo() == position->getNo())
		{
			positions->erase(pointFeature1->getPositions()->begin()+i);
			break;
		}
	}
	std::vector<PositionNode*>* conflictingPositions = position->getConflictingPositions();
	for(int i=0; i<conflictingPositions->size(); i++)
	{
		PositionNode* conflictingPosition = conflictingPositions->at(i);
		conflictingPosition->removeConflictingPosition(position);
		PointFeature* conflictingPf = conflictingPosition->getPointFeature();
		if(conflictingPf!=NULL)
		{
			affectedPfs->push_back(conflictingPf);
		}
	}
	delete position;
	return affectedPfs;
}
}
