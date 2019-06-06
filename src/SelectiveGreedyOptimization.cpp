#include "SelectiveGreedyOptimization.hpp"

#include <algorithm>
#include <iostream>

#include <boost/heap/fibonacci_heap.hpp>

using namespace boost::heap;
using namespace std;
void printConflictGraph2(vector<PointFeature*>* pointFeatures)
{
	for(vector<PointFeature*>::iterator it=pointFeatures->begin(); it!=pointFeatures->end(); it++)
	{
		PointFeature* pointFeature = *it;
		vector<PositionNode*>* positions = pointFeature->getPositions();
		cout<<"Point Feature Number: "<<pointFeature->getNo()<<endl;
		for(int i=0; i<positions->size(); i++)
		{
			PositionNode* positionNode = (*positions)[i];
			cout<<"\tPosition Number: "<<positionNode->getNo()<<" Conflict Size: "<<positionNode->getConflictingPositions()->size()<<endl;
			cout<<"\tConflicting Positions:";
			for(int j=0; j<positionNode->getConflictingPositions()->size(); j++)
			{
				PositionNode* conflictingPositionNode = positionNode->getConflictingPositions()->at(j);
				cout<<" "<<conflictingPositionNode->getNo();
			}
			cout<<endl;
		}
	}
}
double SelectiveGreedyOptimization::a;

bool SelectiveGreedyOptimization::ltPositionNode(PositionNode* pn1, PositionNode* pn2)
{
	return pn1->getWeightedConflictSize() < pn2->getWeightedConflictSize();
}
double SelectiveGreedyOptimization::minSelectedEval(PointFeature* pf1, PointFeature* pf2)
{
	double conflictSize1 = pf1->getPositions()->at(0)->getWeightedConflictSize();
	double conflictSize2 = pf2->getPositions()->at(0)->getWeightedConflictSize();
	return conflictSize1-conflictSize2;
}
double SelectiveGreedyOptimization::maxNonSelectedEval(PointFeature* pf1, PointFeature* pf2)
{
	double pf1Sum = 0;
	std::vector<PositionNode*>* pf1Positions = pf1->getPositions();
	for(int i=1; i<pf1Positions->size(); i++)
	{
		pf1Sum += pf1->getPositions()->at(i)->getWeightedConflictSize();
	}

	double pf2Sum = 0;
	std::vector<PositionNode*>* pf2Positions = pf2->getPositions();
	for(int i=1; i<pf2Positions->size(); i++)
	{
		pf2Sum += pf1->getPositions()->at(i)->getWeightedConflictSize();
	}

	return pf2Sum - pf1Sum;
}
double SelectiveGreedyOptimization::minSelectedEval2(PointFeature* pf1, PointFeature* pf2, int mode)
{
	double weightedConflictSize1 = pf1->getPositions()->at(0)->getWeightedConflictSize();
	double weightDiff1 = 1.0-1.0/(double)pf1->getPositions()->size();
	double eval1 =(1.0+weightDiff1)*weightedConflictSize1;
	
	double weightedConflictSize2 = pf2->getPositions()->at(0)->getWeightedConflictSize();
	double weightDiff2 = 1.0-1.0/(double)pf2->getPositions()->size();
	double eval2 =(1.0+weightDiff2)*weightedConflictSize2;
	double eval = eval1 - eval2;
	if(mode==1)
		if(eval==0)
			return maxNonSelectedEval2(pf1, pf2, false);
	return eval;
}
double SelectiveGreedyOptimization::maxNonSelectedEval2(PointFeature* pf1, PointFeature* pf2, int mode)
{
	double eval1 = 0;
	std::vector<PositionNode*>* pf1Positions = pf1->getPositions();
	for(int i=1; i<pf1Positions->size(); i++)
	{
		double weightedConflictSize1 = pf1->getPositions()->at(i)->getWeightedConflictSize();
		double weightDiff1 = 1.0/(double)pf1->getPositions()->size();
		eval1 +=(1.0+weightDiff1)*weightedConflictSize1;
	}

	double eval2 = 0;
	std::vector<PositionNode*>* pf2Positions = pf2->getPositions();
	for(int i=1; i<pf2Positions->size(); i++)
	{
		double weightedConflictSize2 = pf2->getPositions()->at(i)->getWeightedConflictSize();
		double weightDiff2 = 1.0/(double)pf2->getPositions()->size();
		eval2 +=(1.0+weightDiff2)*weightedConflictSize2;
	}
	double eval = eval2 - eval1;
	if(mode==1)
		if(eval==0)
			return minSelectedEval2(pf1, pf2, false);
	return eval;
}
double SelectiveGreedyOptimization::minSelectedEval3(PointFeature* pf1, PointFeature* pf2, int mode)
{
	PositionNode* position1 = pf1->getPositions()->at(0);
	PositionNode* position2 = pf2->getPositions()->at(0);
	double conflictSize1 = position1->getWeightedConflictSize();
	double conflictSize2 = position2->getWeightedConflictSize();
	double eval = conflictSize1-conflictSize2;
	if(mode==1 || mode==2)
	{
		if(eval==0)
			eval = maxNonSelectedEval3(pf1, pf2, 0);
	}
	if(mode==2)
	{
		if(eval == 0)
		{
			int conflictSizeOfConflictingPositions1 = 0;
			for(int i=0; i<position1->getConflictingPositions()->size(); i++)
			{
				conflictSizeOfConflictingPositions1 += position1->getConflictingPositions()->at(i)->getConflictSize();
			}
			int conflictSizeOfConflictingPositions2 = 0;
			for(int i=0; i<position2->getConflictingPositions()->size(); i++)
			{
				conflictSizeOfConflictingPositions2 += position2->getConflictingPositions()->at(i)->getConflictSize();
			}
			return conflictSizeOfConflictingPositions1-conflictSizeOfConflictingPositions2;
		}
	}
	return eval;
}
double SelectiveGreedyOptimization::maxNonSelectedEval3(PointFeature* pf1, PointFeature* pf2, int mode)
{
	double pf1Sum = 0;
	std::vector<PositionNode*>* pf1Positions = pf1->getPositions();
	for(int i=1; i<pf1Positions->size(); i++)
	{
		pf1Sum += pf1->getPositions()->at(i)->getWeightedConflictSize();
	}

	double pf2Sum = 0;
	std::vector<PositionNode*>* pf2Positions = pf2->getPositions();
	for(int i=1; i<pf2Positions->size(); i++)
	{
		pf2Sum += pf2->getPositions()->at(i)->getWeightedConflictSize();
	}
	double eval = pf2Sum - pf1Sum;
	if(mode==1 || mode==2)
	{
		if(eval==0)
			eval = minSelectedEval3(pf1, pf2, 0);
	}
	
	PositionNode* position1 = pf1->getPositions()->at(0);
	PositionNode* position2 = pf2->getPositions()->at(0);
	if(mode==2)
	{
		if(eval == 0)
		{
			int conflictSizeOfConflictingPositions1 = 0;
			for(int i=0; i<position1->getConflictingPositions()->size(); i++)
			{
				conflictSizeOfConflictingPositions1 += position1->getConflictingPositions()->at(i)->getConflictSize();
			}
			int conflictSizeOfConflictingPositions2 = 0;
			for(int i=0; i<position2->getConflictingPositions()->size(); i++)
			{
				conflictSizeOfConflictingPositions2 += position2->getConflictingPositions()->at(i)->getConflictSize();
			}
			return conflictSizeOfConflictingPositions1-conflictSizeOfConflictingPositions2;
		}
	}
	return eval;
}
double SelectiveGreedyOptimization::minSelectedEval4(PointFeature* pf1, PointFeature* pf2, int mode)
{
	PositionNode* position1 = pf1->getPositions()->at(0);
	PositionNode* position2 = pf2->getPositions()->at(0);
	double conflictSize1 = position1->getProbabilisticConflictSize();
	double conflictSize2 = position2->getProbabilisticConflictSize();
	double eval = conflictSize1-conflictSize2;
	if(mode==1)
	{
		if(eval==0)
			eval = maxNonSelectedEval4(pf1, pf2, 0);
	}
	return eval;
}
double SelectiveGreedyOptimization::maxNonSelectedEval4(PointFeature* pf1, PointFeature* pf2, int mode)
{
	double pf1Sum = 0;
	std::vector<PositionNode*>* pf1Positions = pf1->getPositions();
	for(int i=1; i<pf1Positions->size(); i++)
	{
		pf1Sum += pf1->getPositions()->at(i)->getProbabilisticConflictSize();
	}

	double pf2Sum = 0;
	std::vector<PositionNode*>* pf2Positions = pf2->getPositions();
	for(int i=1; i<pf2Positions->size(); i++)
	{
		pf2Sum += pf2->getPositions()->at(i)->getProbabilisticConflictSize();
	}
	double eval = pf2Sum - pf1Sum;
	if(mode==1)
	{
		if(eval==0)
			eval = minSelectedEval4(pf1, pf2, 0);
	}
	return eval;
}

double SelectiveGreedyOptimization::minSelectedEval5(PointFeature* pf1, PointFeature* pf2, int mode)
{
	PositionNode* position1 = pf1->getPositions()->at(0);
	double conflictSize1 = position1->getProbabilisticConflictSize();
	double position1Weight = 1.0/(double)position1->getPointFeature()->getPositions()->size();
	double position1NewWeight = 1.0;
	double conflictSizeDiff1 = 0;
	for(int i=0; i<position1->getConflictingPositions()->size(); i++)
	{
		PositionNode* conflictingPosition = position1->getConflictingPositions()->at(i);
		double probConfSize = conflictingPosition->getProbabilisticConflictSize();
		double newProbConfSize = 1 - (1 - probConfSize)/(1-position1Weight)*(1-position1NewWeight);
		conflictSizeDiff1 += newProbConfSize - probConfSize;
	}
	//cout<<"CS 1: "<<conflictSize1<<" CSD 1: "<<conflictSizeDiff1<<endl;
	double eval1 = conflictSize1;
	
	PositionNode* position2 = pf2->getPositions()->at(0);
	double conflictSize2 = position2->getProbabilisticConflictSize();
	double position2Weight = 1.0/(double)position2->getPointFeature()->getPositions()->size();
	double position2NewWeight = 1;
	double conflictSizeDiff2 = 0;
	for(int i=0; i<position2->getConflictingPositions()->size(); i++)
	{
		PositionNode* conflictingPosition = position2->getConflictingPositions()->at(i);
		double probConfSize = conflictingPosition->getProbabilisticConflictSize();
		double newProbConfSize = 1 - (1 - probConfSize)/(1-position2Weight)*(1-position2NewWeight);
		conflictSizeDiff1 += newProbConfSize - probConfSize;
	}
	double eval2 = conflictSize2;
	double eval = eval1 - eval2;
	/*if(eval == 0)
		eval = conflictSizeDiff1 - conflictSizeDiff2;*/
	
	if(mode==1)
	{
		if(eval==0)
			return maxNonSelectedEval5(pf1, pf2, 0);
	}
	return eval;
}

double SelectiveGreedyOptimization::maxNonSelectedEval5(PointFeature* pf1, PointFeature* pf2, int mode)
{
	double eval1 = 0;
	std::vector<PositionNode*>* pf1Positions = pf1->getPositions();
	for(int i=1; i<pf1Positions->size(); i++)
	{
		PositionNode* position1 = pf1Positions->at(i);
		double conflictSize1 = position1->getProbabilisticConflictSize();
		double position1Weight = 1.0/(double)position1->getPointFeature()->getPositions()->size();
		double position1NewWeight = 0;
		double conflictSizeDiff1 = 0;
		for(int i=0; i<position1->getConflictingPositions()->size(); i++)
		{
			PositionNode* conflictingPosition = position1->getConflictingPositions()->at(i);
			double probConfSize = conflictingPosition->getProbabilisticConflictSize();
			double newProbConfSize = 1 - (1 - probConfSize)/(1-position1Weight)*(1-position1NewWeight);
			conflictSizeDiff1 += newProbConfSize - probConfSize;
		}
		eval1 += conflictSize1 - conflictSizeDiff1;
	}
	
	double eval2 = 0;
	std::vector<PositionNode*>* pf2Positions = pf2->getPositions();
	for(int i=1; i<pf2Positions->size(); i++)
	{
		PositionNode* position2 = pf2Positions->at(i);
		double conflictSize2 = position2->getProbabilisticConflictSize();
		double position2Weight = 1.0/(double)position2->getPointFeature()->getPositions()->size();
		double position2NewWeight = 0;
		double conflictSizeDiff2 = 0;
		for(int i=0; i<position2->getConflictingPositions()->size(); i++)
		{
			PositionNode* conflictingPosition = position2->getConflictingPositions()->at(i);
			double probConfSize = conflictingPosition->getProbabilisticConflictSize();
			double newProbConfSize = 1 - (1 - probConfSize)/(1-position2Weight)*(1-position2NewWeight);
			conflictSizeDiff2 += newProbConfSize - probConfSize;
		}
		eval2 += conflictSize2 - conflictSizeDiff2;
	}
	
	double eval = eval2 - eval1;
	if(mode==1)
		if(eval==0)
			return minSelectedEval5(pf1, pf2, 0);
	return eval;
}
bool SelectiveGreedyOptimization::minSelectedLess::operator()(PointFeature* pf1, PointFeature* pf2) const
{
	return minSelectedEval3(pf1, pf2, 1) < 0;
}
bool SelectiveGreedyOptimization::maxNonSelectedLess::operator()(PointFeature* pf1, PointFeature* pf2) const
{
	return maxNonSelectedEval3(pf1, pf2, 1) < 0;
}
bool SelectiveGreedyOptimization::minAndMaxLess::operator()(PointFeature* pf1, PointFeature* pf2) const
{
	double val1 = minSelectedEval3(pf1, pf2, 0);
	double val2 = maxNonSelectedEval3(pf1, pf2, 0);
	return val1 + a*val2 < 0;
}
bool SelectiveGreedyOptimization::minSelectedGtr::operator()(PointFeature* pf1, PointFeature* pf2) const
{
	return minSelectedEval3(pf1, pf2, 1) > 0;
}
bool SelectiveGreedyOptimization::maxNonSelectedGtr::operator()(PointFeature* pf1, PointFeature* pf2) const
{
	return maxNonSelectedEval3(pf1, pf2, 1) > 0;
}
bool SelectiveGreedyOptimization::minAndMaxGtr::operator()(PointFeature* pf1, PointFeature* pf2) const
{
	double val1 = minSelectedEval3(pf1, pf2, 0);
	double val2 = maxNonSelectedEval3(pf1, pf2, 0);
	return val1 + a * val2 > 0;
}
bool SelectiveGreedyOptimization::minSelectedGtr2::operator()(PointFeature* pf1, PointFeature* pf2) const
{
	return minSelectedEval3(pf1, pf2, 2) >= 0;
}
bool SelectiveGreedyOptimization::maxNonSelectedGtr2::operator()(PointFeature* pf1, PointFeature* pf2) const
{
	return maxNonSelectedEval3(pf1, pf2, 2) >= 0;
}
bool SelectiveGreedyOptimization::minAndMaxGtr2::operator()(PointFeature* pf1, PointFeature* pf2) const
{
	double eval1 = minSelectedEval3(pf1, pf2, 0);
	double eval2 = maxNonSelectedEval3(pf1, pf2, 0);
	double eval = eval1 + a*eval2;
	PositionNode* position1 = pf1->getPositions()->at(0);
	PositionNode* position2 = pf2->getPositions()->at(0);
	if(eval == 0)
	{
		int conflictSizeOfConflictingPositions1 = 0;
		for(int i=0; i<position1->getConflictingPositions()->size(); i++)
		{
			conflictSizeOfConflictingPositions1 += position1->getConflictingPositions()->at(i)->getConflictSize();
		}
		int conflictSizeOfConflictingPositions2 = 0;
		for(int i=0; i<position2->getConflictingPositions()->size(); i++)
		{
			conflictSizeOfConflictingPositions2 += position2->getConflictingPositions()->at(i)->getConflictSize();
		}
		eval = conflictSizeOfConflictingPositions1-conflictSizeOfConflictingPositions2;
	}
	return eval >= 0;
}
bool SelectiveGreedyOptimization::minSelectedGtr3::operator()(PointFeature* pf1, PointFeature* pf2) const
{
	return minSelectedEval4(pf1, pf2, 1) > 0;
}
bool SelectiveGreedyOptimization::maxNonSelectedGtr3::operator()(PointFeature* pf1, PointFeature* pf2) const
{
	return maxNonSelectedEval4(pf1, pf2, 1) > 0;
}
bool SelectiveGreedyOptimization::minAndMaxGtr3::operator()(PointFeature* pf1, PointFeature* pf2) const
{
	double eval1 = minSelectedEval4(pf1, pf2, 0);
	double eval2 = maxNonSelectedEval4(pf1, pf2, 0);
	double eval = eval1 + a*eval2;
	return eval >= 0;
}
bool SelectiveGreedyOptimization::minAndMaxGtr4::operator()(PointFeature* pf1, PointFeature* pf2) const
{

	double min1 = pf1->getPositions()->at(0)->getProbabilisticConflictSize();
	double min2 = pf2->getPositions()->at(0)->getProbabilisticConflictSize();
	
	double max1 = 0;
	std::vector<PositionNode*>* pf1Positions = pf1->getPositions();
	for(int i=1; i<pf1Positions->size(); i++)
	{
		max1 += pf1->getPositions()->at(i)->getProbabilisticConflictSize();
	}

	double max2 = 0;
	std::vector<PositionNode*>* pf2Positions = pf2->getPositions();
	for(int i=1; i<pf2Positions->size(); i++)
	{
		max2 += pf2->getPositions()->at(i)->getProbabilisticConflictSize();
	}
	
	
	return min1/max1 > min2/max2;
}
Solution* SelectiveGreedyOptimization::findSolution(ConflictGraph* conflictGraph, int method)
{
	std::vector<PointFeature*>* pointFeatures = conflictGraph->getPointFeatures();
	Solution* solution = new Solution();
	std::vector<PositionNode*>* positions = new std::vector<PositionNode*>();
	positions->assign(pointFeatures->size(), NULL);
	solution->setPositions(positions);
	
	while(!pointFeatures->empty())
	{
		for(std::vector<PointFeature*>::iterator it = pointFeatures->begin(); it != pointFeatures->end(); it++)
		{
			PointFeature* pointFeature = *it;
			std::vector<PositionNode*>* pfPositions = pointFeature->getPositions();
			std::vector<PositionNode*>::iterator minPosIt = std::min_element(pfPositions->begin(), pfPositions->end(), ltPositionNode);
			std::iter_swap(pfPositions->begin(), minPosIt);
		}
	
		std::vector<PointFeature*>::iterator minPfIt;
		if(method==0)
			minPfIt = std::min_element(pointFeatures->begin(), pointFeatures->end(), minSelectedLess());
		else if(method==1)
			minPfIt = std::min_element(pointFeatures->begin(), pointFeatures->end(), maxNonSelectedLess());
		else if(method==2)
			minPfIt = std::min_element(pointFeatures->begin(), pointFeatures->end(), minAndMaxLess());
		
		PointFeature* bestPointFeature = *minPfIt;
		PositionNode* bestPosition = bestPointFeature->getPositions()->at(0);
		pointFeatures->erase(minPfIt);
		(*positions)[bestPointFeature->getNo()-1] = bestPosition;
		
		for(int i=1; i<PointFeature::numPositions; i++)
		{
			PositionNode* positionNode = bestPointFeature->getPositions()->at(i);
			std::vector<PositionNode*>* conflictingPositions1 = positionNode->getConflictingPositions();
			for(int j=0; j<conflictingPositions1->size(); j++)
			{
				PositionNode* conflictingPosition1 = conflictingPositions1->at(j);
				conflictingPosition1->removeConflictingPosition(positionNode);
			}
			delete positionNode;
		}
		bestPosition->setPointFeature(NULL);
		delete bestPointFeature->getPositions();
		bestPointFeature->setPositions(NULL);
		delete bestPointFeature;
	}
	conflictGraph->setPointFeatures(NULL);
	delete pointFeatures;
	delete conflictGraph;
	return solution;
}
Solution* SelectiveGreedyOptimization::findSolutionPQ(ConflictGraph* conflictGraph, int method, int hcParam)
{
	typedef fibonacci_heap<PointFeature*, compare<minSelectedGtr> > pq1;
	typedef fibonacci_heap<PointFeature*, compare<maxNonSelectedGtr> > pq2;
	typedef fibonacci_heap<PointFeature*, compare<minAndMaxGtr> > pq3;
	typedef fibonacci_heap<PointFeature*, compare<minSelectedGtr2> > pq4;
	typedef fibonacci_heap<PointFeature*, compare<maxNonSelectedGtr2> > pq5;
	typedef fibonacci_heap<PointFeature*, compare<minAndMaxGtr2> > pq6;
	typedef fibonacci_heap<PointFeature*, compare<minSelectedGtr3> > pq7;
	typedef fibonacci_heap<PointFeature*, compare<maxNonSelectedGtr3> > pq8;
	typedef fibonacci_heap<PointFeature*, compare<minAndMaxGtr3> > pq9;

	if(hcParam == 1)
	{
		ConflictGraph* conflictGraph2 = new ConflictGraph(*conflictGraph);
		Solution* partialSolution;
		if(method==0)
			partialSolution = findSolutionPQ<pq1>(conflictGraph, 1);
		else if(method==1)
			partialSolution = findSolutionPQ<pq2>(conflictGraph, 1);
		else if(method==2)
			partialSolution = findSolutionPQ<pq3>(conflictGraph, 1);
		
		std::vector<PointFeature*>* pointFeatures = conflictGraph2->getPointFeatures();
		std::vector<PositionNode*>* solutionPositions = partialSolution->getPositions();
		int countSolPositions = 0;
		for(int i=0; i<solutionPositions->size(); i++)
		{
			PositionNode* solutionPosition = solutionPositions->at(i);
			if(solutionPosition != NULL)
			{
				countSolPositions++;
				PointFeature* pointFeature;
				int pfIndex;
				for(int j=0; j<=pointFeatures->size(); j++)
				{
					pointFeature = pointFeatures->at(j);
					if(pointFeature->getNo() == i+1)
					{
						pfIndex = j;
						break;
					}
				}
				std::vector<PositionNode*>* positions = pointFeature->getPositions();
				for(int j=0; j<positions->size(); j++)
				{
					PositionNode* position = positions->at(j);
					if(position->getNo()!=solutionPosition->getNo())
						conflictGraph2->removePosition(positions->at(j));
				}
				//pointFeatures->erase(pointFeatures->begin()+pfIndex);
			}
		}
		//std::cout<<"Partial solution size: "<<countSolPositions<<std::endl;
		//std::cout<<"Partial solution conflict size: "<<partialSolution->getConflictSize()<<std::endl;
		conflictGraph = conflictGraph2;
	}
	if(method==0)
		return findSolutionPQ<pq1>(conflictGraph, 0);
	else if(method==1)
		return findSolutionPQ<pq2>(conflictGraph, 0);
	else if(method==2)
		return findSolutionPQ<pq3>(conflictGraph, 0);
}

template <typename PQ> 
Solution* SelectiveGreedyOptimization::findSolutionPQ(ConflictGraph* conflictGraph, int hcParam)
{
	std::vector<PointFeature*>* pointFeatures = conflictGraph->getPointFeatures();
	Solution* solution = new Solution();
	std::vector<PositionNode*>* positions = new std::vector<PositionNode*>();
	positions->assign(pointFeatures->size(), NULL);
	solution->setPositions(positions);
	
	int pfSize = pointFeatures->size();
	typedef typename PQ::handle_type handle_t;
	handle_t* pfHandles = new handle_t[pfSize];
	PQ pq;
	
	for(int i=0; i<pfSize; i++)
	{
		PointFeature* pointFeature = pointFeatures->at(i);
		std::vector<PositionNode*>* pfPositions = pointFeature->getPositions();
		std::vector<PositionNode*>::iterator minPosIt = std::min_element(pfPositions->begin(), pfPositions->end(), ltPositionNode);
		std::iter_swap(pfPositions->begin(), minPosIt);
		
		int pfIndex = pointFeature->getNo()-1;
		pfHandles[pfIndex] = pq.push(pointFeature);
	}
	while(!pq.empty())
	{
		PointFeature* bestPointFeature = pq.top();
		pq.pop();
		PositionNode* bestPosition = bestPointFeature->getPositions()->at(0);
		int bestPFIndex = bestPointFeature->getNo()-1;
		(*positions)[bestPFIndex] = bestPosition;

		std::vector<PointFeature*>* affectedPfs = new std::vector<PointFeature*>();
		for(int i=0; i<bestPointFeature->getPositions()->size(); i++)
		{
			PositionNode* position = bestPointFeature->getPositions()->at(i);
			std::vector<PositionNode*>* conflictingPositions = position->getConflictingPositions();
			for(int j=0; j<conflictingPositions->size(); j++)
			{
				PositionNode* conflictingPosition = conflictingPositions->at(j);
				PointFeature* conflictingPf = conflictingPosition->getPointFeature();
				if(conflictingPf!=NULL)
				{
					affectedPfs->push_back(conflictingPf);
				}
			}
		}
		for(int i=1; i<bestPointFeature->getPositions()->size();)
		{
			PositionNode* positionNode = bestPointFeature->getPositions()->at(i);
			std::vector<PointFeature*>* affectedPfs = conflictGraph->removePosition(positionNode);
			/*for(int j=1; j<affectedPfs->size(); j++)
			{
				PointFeature* affectedPf = affectedPfs->at(j);
				int affectedPFIndex = affectedPf->getNo()-1;
				std::vector<PositionNode*>* pfPositions = affectedPf->getPositions();
				std::vector<PositionNode*>::iterator minPosIt = std::min_element(pfPositions->begin(), pfPositions->end(), ltPositionNode);
				std::iter_swap(pfPositions->begin(), minPosIt);
				pq.update(pfHandles[affectedPFIndex]);
			}
			delete affectedPfs;*/
		}
		for(int i=0; i<affectedPfs->size(); i++)
		{
			PointFeature* affectedPf = affectedPfs->at(i);
			int affectedPFIndex = affectedPf->getNo()-1;
			std::vector<PositionNode*>* pfPositions = affectedPf->getPositions();
			std::vector<PositionNode*>::iterator minPosIt = std::min_element(pfPositions->begin(), pfPositions->end(), ltPositionNode);
			std::iter_swap(pfPositions->begin(), minPosIt);
			pq.update(pfHandles[affectedPFIndex]);
			
		}
		
		delete affectedPfs;
		
		if(hcParam == 1)
		{
			std::vector<PositionNode*>* conflictingPositions1 = bestPosition->getConflictingPositions();
			while(!conflictingPositions1->empty())
			{
				PositionNode* conflictingPosition1 = conflictingPositions1->at(0);
				std::vector<PointFeature*>* affectedPfs = conflictGraph->removePosition(conflictingPosition1);
				for(int j=0; j<affectedPfs->size(); j++)
				{
					PointFeature* affectedPf = affectedPfs->at(j);
					if(affectedPf->getNo()==bestPointFeature->getNo())
						continue;
					int affectedPFIndex = affectedPf->getNo()-1;
					std::vector<PositionNode*>* pfPositions = affectedPf->getPositions();
					if(pfPositions->size() == 0)
					{
						pq.erase(pfHandles[affectedPFIndex]);
					}
					else
					{
						std::vector<PositionNode*>::iterator minPosIt = std::min_element(pfPositions->begin(), pfPositions->end(), ltPositionNode);
						std::iter_swap(pfPositions->begin(), minPosIt);;
						pq.update(pfHandles[affectedPFIndex]);
					}
				}
				delete affectedPfs;
			}
		}
		
		bestPosition->setPointFeature(NULL);
		delete bestPointFeature->getPositions();
		bestPointFeature->setPositions(NULL);
		delete bestPointFeature;
	}
	conflictGraph->setPointFeatures(NULL);
	delete pointFeatures;
	delete conflictGraph;
	delete [] pfHandles;
	return solution;
}
