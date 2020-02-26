#include "EliminativeGreedyOptimization.hpp"

#include <algorithm>
#include <iostream>
#include <boost/heap/fibonacci_heap.hpp>

using namespace boost::heap;
using namespace std;

namespace fhplp {
double  EliminativeGreedyOptimization::a;
bool EliminativeGreedyOptimization::gtPositionNode(PositionNode* pn1, PositionNode* pn2)
{
	return pn1->getWeightedConflictSize() > pn2->getWeightedConflictSize();
}
double EliminativeGreedyOptimization::maxEliminatedEval(PointFeature* pf1, PointFeature* pf2)
{
	double conflictSize1 = pf1->getPositions()->at(0)->getWeightedConflictSize();
	double conflictSize2 = pf2->getPositions()->at(0)->getWeightedConflictSize();
	return conflictSize2 - conflictSize1;
}
double EliminativeGreedyOptimization::minNonEliminatedEval(PointFeature* pf1, PointFeature* pf2)
{
	double pf1Avg = 0;
	std::vector<PositionNode*>* pf1Positions = pf1->getPositions();
	for(int i=1; i<pf1Positions->size(); i++)
	{
		pf1Avg += pf1Positions->at(i)->getWeightedConflictSize();
	}
	pf1Avg /= pf1Positions->size()-1;
	
	double pf2Avg = 0;
	std::vector<PositionNode*>* pf2Positions = pf2->getPositions();
	for(int i=1; i<pf2Positions->size(); i++)
	{
		pf2Avg += pf2Positions->at(i)->getWeightedConflictSize();
	}
	pf2Avg /= pf2Positions->size()-1;
	return pf1Avg - pf2Avg;
}
double EliminativeGreedyOptimization::maxEliminatedEval2(PointFeature* pf1, PointFeature* pf2, int mode)
{
	double weightedConflictSize1 = pf1->getPositions()->at(0)->getWeightedConflictSize();
	double weightDiff1 = 1.0/(double)pf1->getPositions()->size();
	double eval1 = (1.0+weightDiff1)*weightedConflictSize1;
	double weightedConflictSize2 = pf2->getPositions()->at(0)->getWeightedConflictSize();
	double weightDiff2 = 1.0/(double)pf2->getPositions()->size();
	double eval2 = (1.0+weightDiff2)*weightedConflictSize2;
	double eval = eval2 - eval1;
	if(mode==1 || mode==2)
	{
		if(eval==0)
			return minNonEliminatedEval2(pf1, pf2, 0);
	}
	return eval;
}
double EliminativeGreedyOptimization::minNonEliminatedEval2(PointFeature* pf1, PointFeature* pf2, int mode)
{
	double eval1 = 0;
	std::vector<PositionNode*>* pf1Positions = pf1->getPositions();
	for(int i=1; i<pf1Positions->size(); i++)
	{
		double weightedConflictSize1 = pf1Positions->at(i)->getWeightedConflictSize();
		double weightDiff1 = 1.0/(double)(pf1->getPositions()->size()-1) - 1.0/(double)pf1->getPositions()->size();
		eval1 += (1.0+weightDiff1)*weightedConflictSize1;
	}
	eval1 /= pf1Positions->size()-1;
	
	double eval2 = 0;
	std::vector<PositionNode*>* pf2Positions = pf2->getPositions();
	for(int i=1; i<pf2Positions->size(); i++)
	{
		double weightedConflictSize2 = pf2Positions->at(i)->getWeightedConflictSize();
		double weightDiff2 = 1.0/(double)(pf2->getPositions()->size()-1) - 1.0/(double)pf2->getPositions()->size();
		eval2 += (1.0+weightDiff2)*weightedConflictSize2;
	}
	eval2 /= pf2Positions->size()-1;
	
	double eval = eval1 - eval2;
	if(mode==1)
		if(eval==0)
			return maxEliminatedEval2(pf1, pf2, 0);
	return eval;
}
double EliminativeGreedyOptimization::maxEliminatedEval3(PointFeature* pf1, PointFeature* pf2, int mode)
{
	PositionNode* position1 = pf1->getPositions()->at(0);
	PositionNode* position2 = pf2->getPositions()->at(0);
	double conflictSize1 = position1->getWeightedConflictSize();
	double conflictSize2 = position2->getWeightedConflictSize();
	double eval = conflictSize2 - conflictSize1;
	if(mode==1 || mode==2)
	{
		if(eval==0)
			return minNonEliminatedEval3(pf1, pf2, 0);
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
			eval = conflictSizeOfConflictingPositions2-conflictSizeOfConflictingPositions1;
		}
	}
	return eval;
}
double EliminativeGreedyOptimization::minNonEliminatedEval3(PointFeature* pf1, PointFeature* pf2, int mode)
{
	double pf1Avg = 0;
	std::vector<PositionNode*>* pf1Positions = pf1->getPositions();
	for(int i=1; i<pf1Positions->size(); i++)
	{
		pf1Avg += pf1Positions->at(i)->getWeightedConflictSize();
	}
	pf1Avg /= pf1Positions->size()-1;
	
	double pf2Avg = 0;
	std::vector<PositionNode*>* pf2Positions = pf2->getPositions();
	for(int i=1; i<pf2Positions->size(); i++)
	{
		pf2Avg += pf2Positions->at(i)->getWeightedConflictSize();
	}
	pf2Avg /= pf2Positions->size()-1;
	double eval = pf1Avg - pf2Avg;
	if(mode==1 || mode==2)
	{
		if(eval==0)
			return maxEliminatedEval3(pf1, pf2, 0);
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
			eval = conflictSizeOfConflictingPositions2-conflictSizeOfConflictingPositions1;
		}
	}
	
	return eval;
}

double EliminativeGreedyOptimization::maxEliminatedEval4(PointFeature* pf1, PointFeature* pf2, int mode)
{
	PositionNode* position1 = pf1->getPositions()->at(0);
	PositionNode* position2 = pf2->getPositions()->at(0);
	double conflictSize1 = position1->getProbabilisticConflictSize();
	double conflictSize2 = position2->getProbabilisticConflictSize();
	double eval = conflictSize2 - conflictSize1;
	if(mode==1)
	{
		if(eval==0)
			return minNonEliminatedEval4(pf1, pf2, 0);
	}
	return eval;
}

double EliminativeGreedyOptimization::minNonEliminatedEval4(PointFeature* pf1, PointFeature* pf2, int mode)
{
	double pf1Avg = 0;
	std::vector<PositionNode*>* pf1Positions = pf1->getPositions();
	for(int i=1; i<pf1Positions->size(); i++)
	{
		pf1Avg += pf1Positions->at(i)->getProbabilisticConflictSize();
	}
	pf1Avg /= pf1Positions->size()-1;
	
	double pf2Avg = 0;
	std::vector<PositionNode*>* pf2Positions = pf2->getPositions();
	for(int i=1; i<pf2Positions->size(); i++)
	{
		pf2Avg += pf2Positions->at(i)->getProbabilisticConflictSize();
	}
	pf2Avg /= pf2Positions->size()-1;
	double eval = pf1Avg - pf2Avg;
	if(mode==1)
	{
		if(eval==0)
			return maxEliminatedEval4(pf1, pf2, 0);
	}
		
	return eval;
}
double EliminativeGreedyOptimization::maxEliminatedEval5(PointFeature* pf1, PointFeature* pf2, int mode)
{
	PositionNode* position1 = pf1->getPositions()->at(0);
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
	double eval1 = conflictSize1 - conflictSizeDiff1;
	
	PositionNode* position2 = pf2->getPositions()->at(0);
	double conflictSize2 = position2->getProbabilisticConflictSize();
	double position2Weight = 1.0/(double)position2->getPointFeature()->getPositions()->size();
	double position2NewWeight = 0;
	double conflictSizeDiff2 = 0;
	for(int i=0; i<position2->getConflictingPositions()->size(); i++)
	{
		PositionNode* conflictingPosition = position2->getConflictingPositions()->at(i);
		double probConfSize = conflictingPosition->getProbabilisticConflictSize();
		double newProbConfSize = 1 - (1 - probConfSize)/(1-position2Weight)*(1-position2NewWeight);
		conflictSizeDiff1 += newProbConfSize - probConfSize;
	}
	double eval2 = conflictSize2 - conflictSizeDiff2;
	double eval = eval2 - eval1;
	if(mode==1)
	{
		if(eval==0)
			return minNonEliminatedEval5(pf1, pf2, 0);
	}
	return eval;
}

double EliminativeGreedyOptimization::minNonEliminatedEval5(PointFeature* pf1, PointFeature* pf2, int mode)
{
	double eval1 = 0;
	std::vector<PositionNode*>* pf1Positions = pf1->getPositions();
	for(int i=1; i<pf1Positions->size(); i++)
	{
		PositionNode* position1 = pf1Positions->at(i);
		double conflictSize1 = position1->getProbabilisticConflictSize();
		double position1Weight = 1.0/(double)position1->getPointFeature()->getPositions()->size();
		double position1NewWeight = 1.0/(double)(position1->getPointFeature()->getPositions()->size()-1);
		double conflictSizeDiff1 = 0;
		for(int i=0; i<position1->getConflictingPositions()->size(); i++)
		{
			PositionNode* conflictingPosition = position1->getConflictingPositions()->at(i);
			double probConfSize = conflictingPosition->getProbabilisticConflictSize();
			double newProbConfSize = 1 - (1 - probConfSize)/(1-position1Weight)*(1-position1NewWeight);
			conflictSizeDiff1 += newProbConfSize - probConfSize;
		}
		eval1 += conflictSize1 + conflictSizeDiff1;
	}
	eval1 /= pf1Positions->size()-1;
	
	double eval2 = 0;
	std::vector<PositionNode*>* pf2Positions = pf2->getPositions();
	for(int i=1; i<pf2Positions->size(); i++)
	{
		PositionNode* position2 = pf2Positions->at(i);
		double conflictSize2 = position2->getProbabilisticConflictSize();
		double position2Weight = 1.0/(double)position2->getPointFeature()->getPositions()->size();
		double position2NewWeight = 1.0/(double)(position2->getPointFeature()->getPositions()->size()-1);
		double conflictSizeDiff2 = 0;
		for(int i=0; i<position2->getConflictingPositions()->size(); i++)
		{
			PositionNode* conflictingPosition = position2->getConflictingPositions()->at(i);
			double probConfSize = conflictingPosition->getProbabilisticConflictSize();
			double newProbConfSize = 1 - (1 - probConfSize)/(1-position2Weight)*(1-position2NewWeight);
			conflictSizeDiff2 += newProbConfSize - probConfSize;
		}
		eval2 += conflictSize2 + conflictSizeDiff2;
	}
	eval2 /= pf2Positions->size()-1;
	
	double eval = eval1 - eval2;
	if(mode==1)
		if(eval==0)
			return maxEliminatedEval5(pf1, pf2, 0);
	return eval;
}
bool EliminativeGreedyOptimization::maxEliminatedLess::operator()(PointFeature* pf1, PointFeature* pf2) const
{
	return maxEliminatedEval3(pf1, pf2, 1) < 0;
}
bool EliminativeGreedyOptimization::minNonEliminatedLess::operator()(PointFeature* pf1, PointFeature* pf2) const
{
	return minNonEliminatedEval3(pf1, pf2, 1) < 0;
}
bool EliminativeGreedyOptimization::maxAndMinLess::operator()(PointFeature* pf1, PointFeature* pf2) const
{
	double val1 = maxEliminatedEval3(pf1, pf2, 0);
	double val2 = minNonEliminatedEval3(pf1, pf2, 0);
	double eval = val1 + a*val2;
	return  eval < 0;
}
bool EliminativeGreedyOptimization::maxEliminatedGtr::operator()(PointFeature* pf1, PointFeature* pf2) const
{
	return !maxEliminatedLess()(pf1, pf2);
}
bool EliminativeGreedyOptimization::minNonEliminatedGtr::operator()(PointFeature* pf1, PointFeature* pf2) const
{
	return !minNonEliminatedLess()(pf1, pf2);
}
bool EliminativeGreedyOptimization::maxAndMinGtr::operator()(PointFeature* pf1, PointFeature* pf2) const
{
	return !maxAndMinLess()(pf1, pf2);
}
bool EliminativeGreedyOptimization::maxEliminatedGtr2::operator()(PointFeature* pf1, PointFeature* pf2) const
{
	return maxEliminatedEval3(pf1, pf2, 2) >= 0;
}
bool EliminativeGreedyOptimization::minNonEliminatedGtr2::operator()(PointFeature* pf1, PointFeature* pf2) const
{
	return minNonEliminatedEval3(pf1, pf2, 2) >= 0;
}
bool EliminativeGreedyOptimization::maxAndMinGtr2::operator()(PointFeature* pf1, PointFeature* pf2) const
{
	double val1 = maxEliminatedEval3(pf1, pf2, 0);
	double val2 = minNonEliminatedEval3(pf1, pf2, 0);
	double eval = val1 + a*val2;
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
		eval = conflictSizeOfConflictingPositions2-conflictSizeOfConflictingPositions1;
	}
	return  eval >= 0;
}
bool EliminativeGreedyOptimization::maxEliminatedGtr3::operator()(PointFeature* pf1, PointFeature* pf2) const
{
	return maxEliminatedEval4(pf1, pf2, 1) >= 0;
}
bool EliminativeGreedyOptimization::minNonEliminatedGtr3::operator()(PointFeature* pf1, PointFeature* pf2) const
{
	return minNonEliminatedEval4(pf1, pf2, 1) >= 0;
}
bool EliminativeGreedyOptimization::maxAndMinGtr3::operator()(PointFeature* pf1, PointFeature* pf2) const
{
	double val1 = maxEliminatedEval4(pf1, pf2, 0);
	double val2 = minNonEliminatedEval4(pf1, pf2, 0);
	double eval = val1 + a*val2;
	return  eval >= 0;
}
bool EliminativeGreedyOptimization::maxAndMinGtr4::operator()(PointFeature* pf1, PointFeature* pf2) const
{
	double max1 = pf1->getPositions()->at(0)->getProbabilisticConflictSize();
	double max2 = pf2->getPositions()->at(0)->getProbabilisticConflictSize();

	double min1 = 0;
	std::vector<PositionNode*>* pf1Positions = pf1->getPositions();
	for(int i=1; i<pf1Positions->size(); i++)
	{
		min1 += pf1Positions->at(i)->getProbabilisticConflictSize();
	}
	min1 /= pf1Positions->size()-1;
	
	double min2 = 0;
	std::vector<PositionNode*>* pf2Positions = pf2->getPositions();
	for(int i=1; i<pf2Positions->size(); i++)
	{
		min2 += pf2Positions->at(i)->getProbabilisticConflictSize();
	}
	min2 /= pf2Positions->size()-1;
	
	return max2/min2 > max1/min1;
}
Solution* EliminativeGreedyOptimization::findSolution(ConflictGraph* conflictGraph, int method)
{
	std::vector<PointFeature*>* pointFeatures = conflictGraph->getPointFeatures();
	
	Solution* solution = new Solution();
	std::vector<PositionNode*>* solutionPositions = new std::vector<PositionNode*>();
	solutionPositions->assign(pointFeatures->size(), NULL);
	solution->setPositions(solutionPositions);
		
	while(!pointFeatures->empty())
	{
		for(std::vector<PointFeature*>::iterator it = pointFeatures->begin(); it != pointFeatures->end(); it++)
		{
			PointFeature* pointFeature = *it;
			std::vector<PositionNode*>* pfPositions = pointFeature->getPositions();
			std::vector<PositionNode*>::iterator minPosIt = std::min_element(pfPositions->begin(), pfPositions->end(), gtPositionNode);
			std::iter_swap(pfPositions->begin(), minPosIt);
		}
	
		std::vector<PointFeature*>::iterator minPfIt;
		if(method==0)
			minPfIt = std::min_element(pointFeatures->begin(), pointFeatures->end(), maxEliminatedLess());
		else if(method==1)
			minPfIt = std::min_element(pointFeatures->begin(), pointFeatures->end(), minNonEliminatedLess());
		else if(method==2)
			minPfIt = std::min_element(pointFeatures->begin(), pointFeatures->end(), maxAndMinLess());
	
		
		PointFeature* pointFeature = *minPfIt;
		std::vector<PositionNode*>* pfPositions = pointFeature->getPositions();
		PositionNode* position = pfPositions->at(0);
		pfPositions->erase(pfPositions->begin());
		if(pfPositions->size()==0)
		{
			(*solutionPositions)[pointFeature->getNo()-1] = position;
			position->setPointFeature(NULL);
			pointFeatures->erase(minPfIt);
			delete pointFeature;
		}
		else
		{
			std::vector<PositionNode*>* conflictingPositions1 = position->getConflictingPositions();
			for(int j=0; j<conflictingPositions1->size(); j++)
			{
				PositionNode* conflictingPosition1 = conflictingPositions1->at(j);
				conflictingPosition1->removeConflictingPosition(position);
				PointFeature* conflictingPointFeature = conflictingPosition1->getPointFeature();
				if(conflictingPointFeature != NULL)
					std::sort(conflictingPointFeature->getPositions()->begin(), conflictingPointFeature->getPositions()->end(), gtPositionNode);
			}
			delete position;
		}
	}
	
	conflictGraph->setPointFeatures(NULL);
	delete pointFeatures;
	delete conflictGraph;
	return solution;
}
Solution* EliminativeGreedyOptimization::findSolutionPQ(ConflictGraph* conflictGraph, int method, int hcParam)
{
	typedef fibonacci_heap<PointFeature*, compare<maxEliminatedGtr> > pq1;
	typedef fibonacci_heap<PointFeature*, compare<minNonEliminatedGtr> > pq2;
	typedef fibonacci_heap<PointFeature*, compare<maxAndMinGtr4> > pq3;
	typedef fibonacci_heap<PointFeature*, compare<maxEliminatedGtr2> > pq4;
	typedef fibonacci_heap<PointFeature*, compare<minNonEliminatedGtr2> > pq5;
	typedef fibonacci_heap<PointFeature*, compare<maxAndMinGtr2> > pq6;
	typedef fibonacci_heap<PointFeature*, compare<maxEliminatedGtr3> > pq7;
	typedef fibonacci_heap<PointFeature*, compare<minNonEliminatedGtr3> > pq8;
	typedef fibonacci_heap<PointFeature*, compare<maxAndMinGtr3> > pq9;
	
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
Solution* EliminativeGreedyOptimization::findSolutionPQ(ConflictGraph* conflictGraph, int hcParam)
{
	std::vector<PointFeature*>* pointFeatures = conflictGraph->getPointFeatures();
	Solution* solution = new Solution();
	std::vector<PositionNode*>* solutionPositions = new std::vector<PositionNode*>();
	solutionPositions->assign(pointFeatures->size(), NULL);
	solution->setPositions(solutionPositions);
		
	int pfSize = pointFeatures->size();
	typedef typename PQ::handle_type handle_t;
	handle_t* pfHandles = new handle_t[pfSize];
	PQ pq;
	
	for(int i=0; i<pfSize; i++)
	{
		PointFeature* pointFeature = pointFeatures->at(i);
		std::vector<PositionNode*>* pfPositions = pointFeature->getPositions();
		std::vector<PositionNode*>::iterator minPosIt = std::min_element(pfPositions->begin(), pfPositions->end(), gtPositionNode);
		std::iter_swap(pfPositions->begin(), minPosIt);
		
		int pfIndex = pointFeature->getNo()-1;
		pfHandles[pfIndex] = pq.push(pointFeature);
	}
	while(!pq.empty())
	{
		PointFeature* pointFeature = pq.top();
		int pfIndex = pointFeature->getNo()-1;
		std::vector<PositionNode*>* pfPositions = pointFeature->getPositions();
		PositionNode* position = pfPositions->at(0);
		if(pfPositions->size()==1)
		{
			pq.pop();
			(*solutionPositions)[pointFeature->getNo()-1] = position;
			if(hcParam == 1)
			{
				std::vector<PositionNode*>* conflictingPositions1 = position->getConflictingPositions();
				while(!conflictingPositions1->empty())
				{
					PositionNode* conflictingPosition1 = conflictingPositions1->at(0);
					std::vector<PointFeature*>* affectedPfs = conflictGraph->removePosition(conflictingPosition1);
					for(int j=0; j<affectedPfs->size(); j++)
					{
						PointFeature* affectedPf = affectedPfs->at(j);
						if(affectedPf->getNo()==pointFeature->getNo())
							continue;
						int affectedPFIndex = affectedPf->getNo()-1;
						std::vector<PositionNode*>* pfPositions = affectedPf->getPositions();
						if(pfPositions->size() == 0)
						{
							pq.erase(pfHandles[affectedPFIndex]);
						}
						else
						{
							std::vector<PositionNode*>::iterator minPosIt = std::min_element(pfPositions->begin(), pfPositions->end(), gtPositionNode);
							std::iter_swap(pfPositions->begin(), minPosIt);;
							pq.update(pfHandles[affectedPFIndex]);
						}
					}
					delete affectedPfs;
				}
			}
			
			position->setPointFeature(NULL);
			pfPositions->erase(pfPositions->begin());
			delete pointFeature;
		}
		else
		{
			pfPositions->erase(pfPositions->begin());
			
			/*std::vector<PointFeature*>* affectedPfs = new std::vector<PointFeature*>();
			for(int i=0; i<pointFeature->getPositions()->size(); i++)
			{
				PositionNode* position = pointFeature->getPositions()->at(i);
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
			}*/
			std::vector<PointFeature*>* affectedPfs = conflictGraph->removePosition(position);
			for(int j=1; j<affectedPfs->size(); j++)
			{
				PointFeature* affectedPf = affectedPfs->at(j);
				int affectedPFIndex = affectedPf->getNo()-1;
				std::vector<PositionNode*>* pfPositions = affectedPf->getPositions();
				std::vector<PositionNode*>::iterator minPosIt = std::min_element(pfPositions->begin(), pfPositions->end(), gtPositionNode);
				std::iter_swap(pfPositions->begin(), minPosIt);
				pq.update(pfHandles[affectedPFIndex]);
			}
			delete affectedPfs;
			
			/*for(int i=0; i<affectedPfs->size(); i++)
			{
				PointFeature* affectedPf = affectedPfs->at(i);
				int affectedPFIndex = affectedPf->getNo()-1;
				std::vector<PositionNode*>* pfPositions = affectedPf->getPositions();
				std::vector<PositionNode*>::iterator minPosIt = std::min_element(pfPositions->begin(), pfPositions->end(), ltPositionNode);
				std::iter_swap(pfPositions->begin(), minPosIt);
				pq.update(pfHandles[affectedPFIndex]);
			
			}
		
			delete affectedPfs;*/
			
			if(pfPositions->size()>1)
			{
				std::vector<PositionNode*>::iterator minPosIt = std::min_element(pfPositions->begin(), pfPositions->end(), gtPositionNode);
				std::iter_swap(pfPositions->begin(), minPosIt);
				pq.update(pfHandles[pfIndex]);
			}
		}
	}
	conflictGraph->setPointFeatures(NULL);
	delete pointFeatures;
	delete conflictGraph;
	delete [] pfHandles;
	return solution;
}
}
