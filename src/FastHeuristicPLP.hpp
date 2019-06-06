/*
 * FastHeuristicPLP.hpp
 *
 *  Created on: Jun 6, 2019
 *      Author: oakile
 */

#ifndef _FASTHEURISTICPLP_HPP_
#define _FASTHEURISTICPLP_HPP_

#include "SelectiveGreedyOptimization.hpp"
#include "EliminativeGreedyOptimization.hpp"
#include "LocalSearchOptimization.hpp"

struct FastHeuristicPLPParameters {
	int numPoints;
	int greedyMethod;
	int greedySubmethod;
	double a;
	double b;
	int hc;
	FastHeuristicPLPParameters();
	FastHeuristicPLPParameters(int numPoints, int greedyMethod, int greedySubmethod, double a, double b, int hc);
};

class FastHeuristicPLP {
private:
	FastHeuristicPLPParameters parameters;
public:
	FastHeuristicPLP();
	virtual ~FastHeuristicPLP();
	void init(FastHeuristicPLPParameters& parameters);
	Solution* optimize(ConflictGraph* conflictGraph);
};

#endif /* _FASTHEURISTICPLP_HPP_ */
