/*
 * RandomPLP.hpp
 *
 *  Created on: Aug 11, 2019
 *      Author: oakile
 */

#ifndef _RANDOMPLP_HPP_
#define _RANDOMPLP_HPP_

#include "ConflictGraphElements.hpp"
#include "Solution.hpp"

namespace fhplp {

class RandomPLP {
public:
	RandomPLP(int numPoints, int numPositionsPerPoint);
	virtual ~RandomPLP();
	Solution* produceSolution(ConflictGraph* conflictGraph);
private:
	int numPoints;
	int numPositionsPerPoint;
};

} /* namespace fhplp */

#endif /* _RANDOMPLP_HPP_ */
