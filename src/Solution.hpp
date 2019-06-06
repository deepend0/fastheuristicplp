#ifndef _SOLUTION_HPP_
#define _SOLUTION_HPP_

#include <vector>

#include "ConflictGraphElements.hpp"

class Solution
{
	private:
	std::vector<PointFeature*>* problem;
	std::vector<PositionNode*>* positions;
	
	public:
	Solution();
	Solution(Solution& solution);
	~Solution();
	std::vector<PointFeature*>* getProblem();
	void setProblem(std::vector<PointFeature*>* problem);
	std::vector<PositionNode*>* getPositions();
	void setPositions(std::vector<PositionNode*>* positions);
	void addPosition(PositionNode* position, int pfIndex);
	void removePosition(int pfIndex);
	int getConflictSize();
	int getConflictExistanceSize();
	void print();
};

#endif
