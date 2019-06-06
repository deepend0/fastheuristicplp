#ifndef _CONFLICTGRAPHELEMENTS_HPP_
#define _CONFLICTGRAPHELEMENTS_HPP_

#include <vector>
#include <string>

class PositionNode;

class PointFeature
{
	private:
	int no;
	std::vector<PositionNode*>* positions;

	public:
	static int numPositions;
	PointFeature();
	PointFeature(int no);
	~PointFeature();
	int getNo();
	void setNo(int no);
	std::vector<PositionNode*>* getPositions();
	void setPositions(std::vector<PositionNode*>* positions);
};

class PositionNode
{
	private:
	PointFeature* pointFeature;
	int no;
	std::vector<PositionNode*>* conflictingPositions;

	public:
	PositionNode();
	PositionNode(int no);
	PositionNode(int no, int pfNo);
	~PositionNode();
	PointFeature* getPointFeature();
	void setPointFeature(PointFeature* pointFeature);
	int getNo();
	void setNo(int no);
	int getPosIndex();
	int getConflictSize();
	double getWeight();
	double getWeightedConflictSize();
	double getWeightedOccuringConflictSize();
	double getProbabilisticConflictSize();
	
	std::vector<PositionNode*>* getConflictingPositions();
	void setConflictingPositions(std::vector<PositionNode*>* conflictingPositions);
	int conflicts(PositionNode* position);
	void removeConflictingPosition(PositionNode* positionNode);
};

class ConflictGraph
{
	private:
	int numPositions;
	std::vector<PointFeature*>* pointFeatures;
	
	public:
	ConflictGraph();
	ConflictGraph(ConflictGraph& conflictGraph);
	~ConflictGraph();
	int getNumPositions();
	void setNumPositions(int numPositions);
	std::vector<PointFeature*>* getPointFeatures();
	void setPointFeatures(std::vector<PointFeature*>* pointFeatures);
	std::vector<PointFeature*>* removePosition(PositionNode* position);
};

#endif
