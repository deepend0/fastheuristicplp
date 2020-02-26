#ifndef _LOCALSEARCHOPTIMIZATION_HPP_
#define _LOCALSEARCHOPTIMIZATION_HPP_

#include <vector>

#include "ConflictGraphElements.hpp"
#include "Solution.hpp"

namespace fhplp {
class LocalSearchOptimization
{
	public:
	std::vector<int> iterationImprovements;
	Solution* optimize(Solution* solution, bool firstOrBest);
	Solution* localSearchFirst(Solution* solution);
};
}
#endif
