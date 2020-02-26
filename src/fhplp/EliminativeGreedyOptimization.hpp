#ifndef _ELIMINATIVEGREEDYOPTIMIZATION_HPP_
#define _ELIMINATIVEGREEDYOPTIMIZATION_HPP_

#include <vector>

#include "ConflictGraphElements.hpp"
#include "Solution.hpp"

namespace fhplp {
class EliminativeGreedyOptimization
{	
	public:
	static double a;
	static bool gtPositionNode(PositionNode* pn1, PositionNode* pn2);
	static double maxEliminatedEval(PointFeature* pf1, PointFeature* pf2);
	static double minNonEliminatedEval(PointFeature* pf1, PointFeature* pf2);
	static double maxEliminatedEval2(PointFeature* pf1, PointFeature* pf2, int mode);
	static double minNonEliminatedEval2(PointFeature* pf1, PointFeature* pf2, int mode);
	static double maxEliminatedEval3(PointFeature* pf1, PointFeature* pf2, int mode);
	static double minNonEliminatedEval3(PointFeature* pf1, PointFeature* pf2, int mode);
	static double maxEliminatedEval4(PointFeature* pf1, PointFeature* pf2, int mode);
	static double minNonEliminatedEval4(PointFeature* pf1, PointFeature* pf2, int mode);
	static double maxEliminatedEval5(PointFeature* pf1, PointFeature* pf2, int mode);
	static double minNonEliminatedEval5(PointFeature* pf1, PointFeature* pf2, int mode);
	struct maxEliminatedLess
	{
		bool operator()(PointFeature* pf1, PointFeature* pf2) const;
	};
	struct minNonEliminatedLess
	{
		bool operator()(PointFeature* pf1, PointFeature* pf2) const;
	};
	struct maxAndMinLess
	{
		bool operator()(PointFeature* pf1, PointFeature* pf2) const;
	};
	struct maxEliminatedGtr
	{
		bool operator()(PointFeature* pf1, PointFeature* pf2) const;
	};
	struct minNonEliminatedGtr
	{
		bool operator()(PointFeature* pf1, PointFeature* pf2) const;
	};
	struct maxAndMinGtr
	{
		bool operator()(PointFeature* pf1, PointFeature* pf2) const;
	};
	struct maxEliminatedGtr2
	{
		bool operator()(PointFeature* pf1, PointFeature* pf2) const;
	};
	struct minNonEliminatedGtr2
	{
		bool operator()(PointFeature* pf1, PointFeature* pf2) const;
	};
	struct maxAndMinGtr2
	{
		bool operator()(PointFeature* pf1, PointFeature* pf2) const;
	};
	struct maxEliminatedGtr3
	{
		bool operator()(PointFeature* pf1, PointFeature* pf2) const;
	};
	struct minNonEliminatedGtr3
	{
		bool operator()(PointFeature* pf1, PointFeature* pf2) const;
	};
	struct maxAndMinGtr3
	{
		bool operator()(PointFeature* pf1, PointFeature* pf2) const;
	};
	struct maxAndMinGtr4
	{
		bool operator()(PointFeature* pf1, PointFeature* pf2) const;
	};
	Solution* findSolution(ConflictGraph* conflictGraph, int method);
	Solution* findSolutionPQ(ConflictGraph* conflictGraph, int method, int hcParam);
	template <typename PQ> 
	Solution* findSolutionPQ(ConflictGraph* conflictGraph, int hcParam);
};
}
#endif
