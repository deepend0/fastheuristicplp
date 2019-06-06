/*
 * FastHeuristicPLP.cpp
 *
 *  Created on: Jun 6, 2019
 *      Author: oakile
 */

#include "FastHeuristicPLP.hpp"

#include <vector>

using namespace std;

FastHeuristicPLPParameters::FastHeuristicPLPParameters() {
	this->numPoints = -1;
	this->greedyMethod = -1;
	this->greedySubmethod = -1;
	this->a = 0;
	this->b = 0;
	this->hc = -1;
}
FastHeuristicPLPParameters::FastHeuristicPLPParameters(int numPoints, int greedyMethod, int greedySubmethod, double a, double b, int hc){
	this->numPoints = numPoints;
	this->greedyMethod = greedyMethod;
	this->greedySubmethod = greedySubmethod;
	this->a = a;
	this->b = b;
	this->hc = hc;
}

FastHeuristicPLP::FastHeuristicPLP() {

}

FastHeuristicPLP::~FastHeuristicPLP() {
}

Solution* FastHeuristicPLP::optimize(ConflictGraph* conflictGraph) {
	ConflictGraph* conflictGraph2 = new ConflictGraph(*conflictGraph);
	vector<PointFeature*>* pointFeatures1 = conflictGraph->getPointFeatures();
	int instanceSize = pointFeatures1->size();
	Solution* solution = NULL;
	clock_t startG = clock();
	if(parameters.greedyMethod==1)
	{
		SelectiveGreedyOptimization selectiveGreedy;
		SelectiveGreedyOptimization::a = parameters.a;
		solution = selectiveGreedy.findSolutionPQ(conflictGraph, parameters.greedySubmethod, parameters.hc);
	}
	else if(parameters.greedyMethod==2)
	{
		EliminativeGreedyOptimization eliminativeGreedy;
		EliminativeGreedyOptimization::a = parameters.a;
		solution = eliminativeGreedy.findSolutionPQ(conflictGraph, parameters.greedySubmethod, parameters.hc);
	}
	clock_t endG = clock();
	int gConflictSize = solution->getConflictSize();
	int gTimeCost = 1000 * (endG - startG) / CLOCKS_PER_SEC;

	//cout<<"Greedy Solution:";
	//solution->print();
	//cout<<"Conflict Size: "<<gConflictSize<<endl;

	vector<PointFeature*>* problem = conflictGraph2->getPointFeatures();
	conflictGraph2->setPointFeatures(NULL);
	delete conflictGraph2;
	solution->setProblem(problem);
	clock_t startLS = clock();
	LocalSearchOptimization localSearch;
	solution = localSearch.localSearchFirst(solution);
	//solution = localSearch.optimize(solution, false);
	clock_t endLS = clock();
	int lsConflictSize = solution->getConflictSize();
	int lsTimeCost = 1000 * (endLS - startLS) / CLOCKS_PER_SEC;
	return solution;

}
