#include "InstanceLoader.hpp"

#include <stdlib.h>

#include <fstream>
#include <sstream>
#include <iostream>

#include <boost/algorithm/string.hpp>
#include <boost/algorithm/string/regex_find_format.hpp>


using namespace boost;
using namespace std;
ConflictGraph* InstanceLoader::load(std::string filePath)
{
	ConflictGraph* conflictGraph = new ConflictGraph();
	std::vector<PointFeature*>* pointFeatures = new std::vector<PointFeature*>();
	conflictGraph->setPointFeatures(pointFeatures);
	
	std::ifstream ifs;
	ifs.open(filePath.c_str(), std::ifstream::in);
	
	if(ifs.fail())
		std::cout<<"Opening instance data file is failed."<<std::endl;
		
	std::string line;
	std::istringstream iss;
	
	std::getline(ifs, line); //Skip first line as it is empty.
	
	std::getline(ifs, line); //Line of point feature count
	iss.str(line);
	std::getline(iss, line, ' ');
	int pfCount = atoi(line.c_str());
	
	std::getline(ifs, line); //Line of label places count
	iss.str(line);
	std::getline(iss, line, ' ');
	int lpCount = atoi(line.c_str());
	PointFeature::numPositions = lpCount;
	conflictGraph->setNumPositions(lpCount);
	int allPositionsCount = pfCount*lpCount;
	int** conflictingPositionsNumbers = new int*[allPositionsCount];
	int* conflictingPositionsNumbersSize = new int[allPositionsCount];
	
	PositionNode** allPositions = new PositionNode*[allPositionsCount];
	for(int i=0; i<pfCount; i++)
	{
		//std::cout<<"i :"<<i<<std::endl;
		PointFeature* pointFeature = new PointFeature();
		pointFeature->setNo(i+1);
		std::vector<PositionNode*>* positions = new std::vector<PositionNode*>();
		
		for(int j=0; j<lpCount; j++)
		{
			//std::cout<<"j :"<<j<<std::endl;
			positions->push_back(new PositionNode());
		}
		pointFeature->setPositions(positions);
		pointFeatures->push_back(pointFeature);
		
		for(int j=0; j<lpCount; j++)
		{
			//std::cout<<"j :"<<j<<std::endl;
			int positionNo = i*lpCount + j + 1;
			allPositions[positionNo-1] = positions->at(j);
			positions->at(j)->setPointFeature(pointFeature);
			positions->at(j)->setNo(positionNo);
			
			std::getline(ifs, line); //Line of potential conflict count
			iss.str(line);
			std::getline(iss, line, ' ');
			int pcCount = atoi(line.c_str());
			int conflictSize = pcCount-lpCount+1;
			conflictingPositionsNumbersSize[positionNo-1] = conflictSize;
			
			int* conflictingPositionNumbers = NULL;
			if(conflictSize > 0)
				conflictingPositionNumbers = new int[conflictSize];
			conflictingPositionsNumbers[positionNo-1] = conflictingPositionNumbers;
			std::getline(ifs, line); //Line of potential conflict label numbers
			iss.str(line);
			
			typedef find_iterator<string::iterator> string_find_iterator;
			regex numRegex("[0-9]+");
    			string_find_iterator sfit=make_find_iterator(line, regex_finder(numRegex));
			for(int k=0; k<conflictSize;)
			{
				//std::cout<<"k :"<<k<<std::endl;
				std::string labelNumStr;
				//ALTERNATIVE 1
				//std::cout<<"line " <<iss.str()<<std::endl;
				//int foundPos = line.find(" ", startPos);
				//labelNumStr=line.substr(startPos, foundPos);
				//startPos = foundPos;
				//ALTERNATIVE 2
				//std::getline(iss, labelNumStr, ' ');
				
				labelNumStr = copy_range<std::string>(*sfit++);
								
				int conflictingPositionNo = atoi(labelNumStr.c_str());
				if((conflictingPositionNo-1)/lpCount != (positionNo-1)/lpCount)
				{
					conflictingPositionNumbers[k] = conflictingPositionNo;
					k++;
				}
			}
		}
	}
	for(int i=0; i<pfCount; i++)
	{
		PointFeature* pointFeature = pointFeatures->at(i);
		std::vector<PositionNode*>* positions = pointFeature->getPositions();
		
		for(int j=0; j<lpCount; j++)
		{
			PositionNode* position = positions->at(j);
			
			std::vector<PositionNode*>* conflictingPositions = new std::vector<PositionNode*>();
			position->setConflictingPositions(conflictingPositions);

			int conflictSize = conflictingPositionsNumbersSize[position->getNo()-1];
			int* conflictingPositionNumbers = conflictingPositionsNumbers[position->getNo()-1];
			for(int k=0; k<conflictSize; k++)
			{
				int conflictingPositionNo = conflictingPositionNumbers[k];
				conflictingPositions->push_back(allPositions[conflictingPositionNo-1]);
			}
		}
	}
		
	ifs.close();
	delete [] conflictingPositionsNumbersSize;
	delete [] conflictingPositionsNumbers;
	return conflictGraph;
}
