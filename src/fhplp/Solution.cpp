#include "Solution.hpp"

#include <iostream>

namespace fhplp {
Solution::Solution()
{
	this->problem = NULL;
	this->positions = NULL;
}
Solution::Solution(Solution& solution)
{
	this->problem = solution.getProblem();
	std::vector<PositionNode*>* positions = solution.getPositions();
	std::vector<PositionNode*>* newPositions = new std::vector<PositionNode*>();
	this->positions = newPositions;
	
	int numPointFeatures = positions->size();
	PositionNode* positionNodeTable[numPointFeatures*PointFeature::numPositions];
	
	for(int i=0; i<numPointFeatures; i++)
	{
		PositionNode* positionNode = positions->at(i);
		PositionNode* newPositionNode = new PositionNode(positionNode->getNo());
		newPositions->push_back(newPositionNode);
		positionNodeTable[positionNode->getNo()-1] = newPositionNode;
	}
	for(int i=0; i<numPointFeatures; i++)
	{
		PositionNode* positionNode = positions->at(i);
		PositionNode* newPositionNode = newPositions->at(i);
		std::vector<PositionNode*>* conflictingPositions = positionNode->getConflictingPositions();
		std::vector<PositionNode*>* newConflictingPositions = newPositionNode->getConflictingPositions();
		for(int j=0; j<conflictingPositions->size(); j++)
		{
			PositionNode* conflictingPosition = conflictingPositions->at(j);
			PositionNode* newConflictingPosition = positionNodeTable[conflictingPosition->getNo()-1];
			newConflictingPositions->push_back(newConflictingPosition);
		}
	}
}
Solution::~Solution()
{
	for(std::vector<PositionNode*>::iterator it = positions->begin(); it != positions->end(); it++)
	{
		PositionNode* positionNode = *it;
		//std::cout<<"D8"<<std::endl;
		delete positionNode;
	}
	//std::cout<<"D11"<<std::endl;
	delete positions;
}
std::vector<PointFeature*>* Solution::getProblem()
{
	return problem;
}
void Solution::setProblem(std::vector<PointFeature*>* problem)
{
	this->problem = problem;
}
std::vector<PositionNode*>* Solution::getPositions()
{
	return positions;
}
void Solution::setPositions(std::vector<PositionNode*>* positions)
{
	this->positions = positions;
}
void Solution::addPosition(PositionNode* positionNode, int pfIndex)
{
	(*positions)[pfIndex] = positionNode;
	int posIndex = positionNode->getPosIndex();
	//std::cout<<"Solution::addPosition >"<<" Size of Current Point Features: "<<problem->size()<<std::endl;
	//std::cout<<"Solution::addPosition >"<<" Point Feature: "<<positionNode->getPointFeature()->getNo()<<" Position Index: "<<posIndex<<std::endl;
	PositionNode* problemPositionNode = problem->at(pfIndex)->getPositions()->at(posIndex);
	
	for(int i=0; i<positions->size(); i++)
	{
		PositionNode* existingPositionNode = positions->at(i);
		if(existingPositionNode != NULL)
		{
			int conflictIndex = problemPositionNode->conflicts(existingPositionNode);
			if(conflictIndex!=-1)
			{
				//std::cout<<"Solution::addPosition >"<<" Position conflicts to existing position: "<<existingPositionNode->getNo()<<std::endl;
				existingPositionNode->getConflictingPositions()->push_back(positionNode);
				positionNode->getConflictingPositions()->push_back(existingPositionNode);
			}
		}
	}
}
void Solution::removePosition(int pfIndex)
{
	PositionNode* positionNode = (*positions)[pfIndex];
	(*positions)[pfIndex] = NULL;
	for(int i=0; i<positions->size(); i++)
	{
		PositionNode* existingPositionNode = positions->at(i);
		if(existingPositionNode != NULL)
		{
			int conflictIndex = existingPositionNode->conflicts(positionNode);
			if(conflictIndex != -1)
			{
				std::vector<PositionNode*>* conflictingPositions = existingPositionNode->getConflictingPositions();
				conflictingPositions->erase(conflictingPositions->begin() + conflictIndex);
			}
		}
	}
	//std::cout<<"D9"<<std::endl;
	delete positionNode;
}
int Solution::getConflictSize()
{
	int conflictSize = 0;
	for(int i=0; i<positions->size(); i++)
	{
		PositionNode* positionNode = positions->at(i);
		if(positionNode!=NULL)
		{
			int positionConflictSize = positionNode->getConflictingPositions()->size();
			if(positionConflictSize>0)
				conflictSize++;
		}
	}
	return conflictSize;
}
void Solution::print()
{
	for(std::vector<PositionNode*>::iterator it1=positions->begin(); it1!=positions->end(); it1++)
	{
		PositionNode* positionNode = *it1;
		std::cout<<" ("<<positionNode->getNo()<<", "<<positionNode->getConflictingPositions()->size()<<") ";
		for(int j=0; j<positionNode->getConflictingPositions()->size(); j++)
		{
			PositionNode* conflictingPositionNode = positionNode->getConflictingPositions()->at(j);
			if(conflictingPositionNode!=NULL)
				std::cout<<" "<<conflictingPositionNode->getNo();
		}
		std::cout<<std::endl;
	}
}
}
