#ifndef _SELECTIVEGREEDYOPTIMIZATION_HPP_
#define _SELECTIVEGREEDYOPTIMIZATION_HPP_

#include <vector>

#include "ConflictGraphElements.hpp"
#include "Solution.hpp"

class SelectiveGreedyOptimization
{
	public:
	static double a;
	static bool ltPositionNode(PositionNode* pn1, PositionNode* pn2);
	static double minSelectedEval(PointFeature* pf1, PointFeature* pf2);
	static double maxNonSelectedEval(PointFeature* pf1, PointFeature* pf2);
	static double minSelectedEval2(PointFeature* pf1, PointFeature* pf2, int mode);
	static double maxNonSelectedEval2(PointFeature* pf1, PointFeature* pf2, int mode);
	static double minSelectedEval3(PointFeature* pf1, PointFeature* pf2, int mode);
	static double maxNonSelectedEval3(PointFeature* pf1, PointFeature* pf2, int mode);
	static double minSelectedEval4(PointFeature* pf1, PointFeature* pf2, int mode);
	static double maxNonSelectedEval4(PointFeature* pf1, PointFeature* pf2, int mode);
	static double minSelectedEval5(PointFeature* pf1, PointFeature* pf2, int mode);
	static double maxNonSelectedEval5(PointFeature* pf1, PointFeature* pf2, int mode);
	struct minSelectedLess
	{
		bool operator()(PointFeature* pf1, PointFeature* pf2) const;
	};
	struct maxNonSelectedLess
	{
		bool operator()(PointFeature* pf1, PointFeature* pf2) const;
	};
	struct minAndMaxLess
	{
		bool operator()(PointFeature* pf1, PointFeature* pf2) const;
	};
	struct minSelectedGtr
	{
		bool operator()(PointFeature* pf1, PointFeature* pf2) const;
	};
	struct maxNonSelectedGtr
	{
		bool operator()(PointFeature* pf1, PointFeature* pf2) const;
	};
	struct minAndMaxGtr
	{
		bool operator()(PointFeature* pf1, PointFeature* pf2) const;
	};
	struct minSelectedGtr2
	{
		bool operator()(PointFeature* pf1, PointFeature* pf2) const;
	};
	struct maxNonSelectedGtr2
	{
		bool operator()(PointFeature* pf1, PointFeature* pf2) const;
	};
	struct minAndMaxGtr2
	{
		bool operator()(PointFeature* pf1, PointFeature* pf2) const;
	};
	struct minSelectedGtr3
	{
		bool operator()(PointFeature* pf1, PointFeature* pf2) const;
	};
	struct maxNonSelectedGtr3
	{
		bool operator()(PointFeature* pf1, PointFeature* pf2) const;
	};
	struct minAndMaxGtr3
	{
		bool operator()(PointFeature* pf1, PointFeature* pf2) const;
	};
	struct minAndMaxGtr4
	{
		bool operator()(PointFeature* pf1, PointFeature* pf2) const;
	};
	Solution* findSolution(ConflictGraph* conflictGraph, int method);
	Solution* findSolutionPQ(ConflictGraph* conflictGraph, int method, int hcParam);
	template <typename PQ1>
	Solution* findSolutionPQ(ConflictGraph* conflictGraph, int hcParam);
};

#endif
