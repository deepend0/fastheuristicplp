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

namespace fhplp {
struct FastHeuristicPLPParameters {
	int numPoints;
	int numPositionsPerPoint;
	int greedyMethod;
	int greedySubmethod;
	double a;
	double b;
	bool hc;
	bool localSearch;
	FastHeuristicPLPParameters();
	FastHeuristicPLPParameters(int numPoints, int numPositionsPerPoint, int greedyMethod, int greedySubmethod, double a, double b, bool hc, bool localSearch);
};

struct FastHeuristicPLPResults {
	int greedyConflictSize;
	int greedyTime;
	int localSearchConflictSize;
	int localSearchTime;
};

class FastHeuristicPLP {
private:
	FastHeuristicPLPParameters parameters;
	FastHeuristicPLPResults results;
public:
	FastHeuristicPLP();
	virtual ~FastHeuristicPLP();
	void init(FastHeuristicPLPParameters& parameters);
	FastHeuristicPLPResults& getResults();
	Solution* optimize(ConflictGraph* conflictGraph);
};
}
#endif /* _FASTHEURISTICPLP_HPP_ */
