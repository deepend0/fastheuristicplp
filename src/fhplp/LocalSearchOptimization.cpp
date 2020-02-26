#include "LocalSearchOptimization.hpp"

#include <iostream>
#include <set>

namespace fhplp {
Solution* LocalSearchOptimization::optimize(Solution* solution, bool first)
{
	Solution* newSolution = solution;
	int numPointFeatures = newSolution->getProblem()->size();
	while(true)
	{
		bool improved = false;
		int improvement = 0;
		Solution *improvingNeighbor = new Solution(*newSolution);
		
		for(int i=0; i<numPointFeatures; i++)
		{
			PointFeature* pointFeature = (*newSolution->getProblem())[i];
			PositionNode* positionNode = (*newSolution->getPositions())[i];
			//std::cout<<"LocalSearchOptimization::optimize >"<<" Point: "<<pointFeature->getNo()<<" Position: "<<positionNode->getNo()<<std::endl;
			for(int j=0; j<PointFeature::numPositions; j++)
			{
				int otherPositionNo = pointFeature->getPositions()->at(j)->getNo();
				if(otherPositionNo != positionNode->getNo())
				{
					//std::cout<<"Replacing position node conflict size: "<<otherPositionNode->getConflictingPositions()->size()<<std::endl;
					//std::cout<<"LocalSearchOptimization::optimize >"<<" Other Position: "<<otherPositionNode->getNo()<<std::endl;
					PositionNode* otherPositionNode = new PositionNode(otherPositionNo);
					Solution* neighbor = new Solution(*newSolution);
					neighbor->removePosition(i);
					neighbor->addPosition(otherPositionNode, i);
					
					if(improvingNeighbor->getConflictSize() > neighbor->getConflictSize())
					{
						//std::cout<<"D5"<<std::endl;
						delete improvingNeighbor;
						improvingNeighbor = neighbor;
						improved=true;
						improvement = improvingNeighbor->getConflictSize() - neighbor->getConflictSize();
						if(first)
						{
							std::cout<<"B1"<<std::endl;
							break;
						}
					}
					else
					{
						//std::cout<<"D6"<<std::endl;
						delete neighbor;
					}
				}
			}
			if(improved && first)
			{
				std::cout<<"B2"<<std::endl;
				break;
			}
		}
		if(improved)
		{
			iterationImprovements.push_back(improvement);
			newSolution = improvingNeighbor;
		}
		else
			break;
	};
	return newSolution;
}
Solution* LocalSearchOptimization::localSearchFirst(Solution* solution)
{
	Solution* newSolution = solution;
	std::vector<PointFeature*>* problem = solution->getProblem();
	
	std::set<int> pfsToCheck;
	for(int i=0; i<problem->size(); i++)
	{
		pfsToCheck.insert(i);
	}
	/*std::vector<int> pfsToCheck;
	for(int i=problem->size()-1; i>=0; i--)
	{
		pfsToCheck.push_back(i);
	}*/
	while(!pfsToCheck.empty())
	{
		int pfi = *pfsToCheck.begin();
		pfsToCheck.erase(pfsToCheck.begin());
		PointFeature* pointFeature = problem->at(pfi);
		PositionNode* positionNode = newSolution->getPositions()->at(pfi);
		//std::cout<<"LocalSearchOptimization::optimize >"<<" Point: "<<pointFeature->getNo()<<" Position: "<<positionNode->getNo()<<std::endl;
		for(int j=0; j<PointFeature::numPositions; j++)
		{
			int improvement = 0;
			PositionNode* problemPositionNode = pointFeature->getPositions()->at(j);
			int otherPositionNo = problemPositionNode->getNo();
			if(otherPositionNo != positionNode->getNo())
			{
				//std::cout<<"Replacing position node conflict size: "<<otherPositionNode->getConflictingPositions()->size()<<std::endl;
				//std::cout<<"LocalSearchOptimization::optimize >"<<" Other Position: "<<otherPositionNode->getNo()<<std::endl;
				PositionNode* otherPositionNode = new PositionNode(otherPositionNo);
				Solution* neighbor = new Solution(*newSolution);
				neighbor->removePosition(pfi);
				neighbor->addPosition(otherPositionNode, pfi);
				
				if(newSolution->getConflictSize() > neighbor->getConflictSize())
				{
					improvement = newSolution->getConflictSize() - neighbor->getConflictSize();
					//std::cout<<"D5"<<std::endl;
					delete newSolution;
					newSolution = neighbor;
					for(int k=0; k<problemPositionNode->getConflictingPositions()->size(); k++)
					{
						PositionNode* conflictingPosition = problemPositionNode->getConflictingPositions()->at(k);
						int pfIndex = (conflictingPosition->getNo()-1)/4;
						bool found = false;
						/*for(std::vector<int>::iterator it2 = pfsToCheck.begin(); it2!=pfsToCheck.end(); it2++)
						{
							if(pfIndex==*it2)
							{
								found = true;
								break;
							}
						}
						if(!found)*/
						pfsToCheck.insert(pfIndex);
					}
					for(int k=0; k<otherPositionNode->getConflictingPositions()->size(); k++)
					{
						PositionNode* conflictingPosition = otherPositionNode->getConflictingPositions()->at(k);
						int pfIndex = (conflictingPosition->getNo()-1)/4;
						bool found = false;
						pfsToCheck.insert(pfIndex);
					}
				}
				else
				{
					//std::cout<<"D6"<<std::endl;
					delete neighbor;
				}
				iterationImprovements.push_back(improvement);
			}
		}
	}
	return newSolution;
}
}
