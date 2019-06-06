#include "../InstanceLoader.hpp"
#include "../SelectiveGreedyOptimization.hpp"
#include "../EliminativeGreedyOptimization.hpp"
#include "../LocalSearchOptimization.hpp"
#include "../FastHeuristicPLP.hpp"

#include <cstdlib>
#include <ctime>
#include <iostream>
#include <boost/algorithm/string.hpp>
#include <boost/algorithm/string/regex_find_format.hpp>
#include <boost/filesystem.hpp>
#include <boost/regex.hpp> 
#include <map>
#include <string>

using namespace std;
using namespace boost;
using namespace boost::filesystem;

void printConflictGraph(vector<PointFeature*>* pointFeatures);

int main(int argc, char **argv)
{
	string probSizeStr = argv[1];
	int probSize = atoi(argv[1]);
	int greedyMethod = atoi(argv[2]);
	int greedySubmethod = atoi(argv[3]);
	double paramA = atof(argv[4]);
	int paramB = 0;
	int hcParam = atoi(argv[5]);
	
	cout<<"Prob Size: "<<probSizeStr<<" Greedy Method: "<<greedyMethod<<" Greedy Submethod: "<<greedySubmethod<<" Param A: "<<paramA<<" Param B: "<<paramB<<endl;
	path p;
	
	if(probSize == 0)
		p = path("problem_instance");
	else if(probSize > 0)
		p = path("problem_instance/d" + probSizeStr);

	vector<vector<int>* > lsImprovementsReduced;
	vector<int* > performanceResults;
	for (recursive_directory_iterator i = recursive_directory_iterator(p); i != recursive_directory_iterator(); i++)
	{
		if (!is_directory(i->path())) //we eliminate directories
		{
			ConflictGraph* conflictGraph1 = InstanceLoader::load(i->path().string());
			/*int totalConflictSize=0;
			for(auto it=conflictGraph1->getPointFeatures()->begin(); it!=conflictGraph1->getPointFeatures()->end(); it++)
			{
				PointFeature* pf = *it;
				for(auto it2=pf->getPositions()->begin(); it2!=pf->getPositions()->end(); it2++)
				{
					PositionNode* pn = *it2;
					if(pn->getConflictingPositions()->size()>0)
						totalConflictSize++;
				}
			}
			cout<<totalConflictSize<<endl;*/
			ConflictGraph* conflictGraph2 = new ConflictGraph(*conflictGraph1);
			vector<PointFeature*>* pointFeatures1 = conflictGraph1->getPointFeatures();
			int instanceSize = pointFeatures1->size();
			Solution* solution = NULL;
			clock_t startG = clock();
			if(greedyMethod==1)
			{
				SelectiveGreedyOptimization selectiveGreedy;
				SelectiveGreedyOptimization::a = paramA;
				solution = selectiveGreedy.findSolutionPQ(conflictGraph1, greedySubmethod, hcParam);
			}
			else if(greedyMethod==2)
			{
				EliminativeGreedyOptimization eliminativeGreedy;
				EliminativeGreedyOptimization::a = paramA;
				solution = eliminativeGreedy.findSolutionPQ(conflictGraph1, greedySubmethod, hcParam);
			}
			clock_t endG = clock();
			int gConflictSize = solution->getConflictSize();
			int gTimeCost = 1000 * (endG - startG) / CLOCKS_PER_SEC;
			
			//cout<<"Greedy Solution:";
			//solution->print();
			//cout<<"Conflict Size: "<<gConflictSize<<endl;
			
			vector<PointFeature*>* problem = conflictGraph2->getPointFeatures();
			solution->setProblem(problem);
			clock_t startLS = clock();
			LocalSearchOptimization localSearch;
			solution = localSearch.localSearchFirst(solution);
			//solution = localSearch.optimize(solution, false);
			clock_t endLS = clock();
			int lsConflictSize = solution->getConflictSize();
			int lsTimeCost = 1000 * (endLS - startLS) / CLOCKS_PER_SEC;
	
			//cout<<"Local Search Solution:";
			//solution->print();
			//cout<<"Conflict Size: "<<lsConflictSize<<endl;
			
			string filename = i->path().filename().string();
			typedef find_iterator<string::iterator> string_find_iterator;
			regex numRegex("[0-9]+");
    			string_find_iterator sfit=make_find_iterator(filename, regex_finder(numRegex));
			string part1 = copy_range<std::string>(*sfit++);
			string part2 = copy_range<std::string>(*sfit);
			int probSize = atoi(part1.c_str());
			int instanceNo = atoi(part2.c_str());
			
			int* performanceResult = new int[6];
			performanceResults.push_back(performanceResult);
			performanceResult[0]=probSize;
			performanceResult[1]=instanceNo;
			performanceResult[2]=gConflictSize;
			performanceResult[3]=gTimeCost;
			performanceResult[4]=lsConflictSize;
			performanceResult[5]=gTimeCost+lsTimeCost;
			//cout << "S: " << probSize << " #" << instanceNo <<" " << gConflictSize << "\t" << gTimeCost << "\t" << lsConflictSize << "\t" << lsTimeCost << endl;			
			/*vector<int>* lsIterationImprovements = &localSearch.iterationImprovements;
			
			vector<int>*  instanceLsImprovementsReduced = new vector<int>();
			instanceLsImprovementsReduced->push_back(instanceSize);
			int reduceGroupSize = instanceSize / 10;

			for(vector<int>::iterator it = lsIterationImprovements->begin(); it!=lsIterationImprovements->end();)
			{
				int reducedImprovement = 0;
				for(int i=0; i<reduceGroupSize && it!=lsIterationImprovements->end(); i++, it++)
				{
					int improvement = *it;
					reducedImprovement += improvement;
				}
				instanceLsImprovementsReduced->push_back(reducedImprovement);
			}
			lsImprovementsReduced.push_back(instanceLsImprovementsReduced);
			*/
			//std::cout<<"D10"<<std::endl;
			delete solution;
			//std::cout<<"D14"<<std::endl;
			delete conflictGraph2;
		}
	}
	struct {
		bool operator()(int* p1, int* p2) const
		{   
		    if(p1[0]==p2[0])
		    	return p1[1]<p2[1];
		    else
		    	return p1[0]<p2[0];
		}   
	} performanceCompare;

	sort(performanceResults.begin(), performanceResults.end(), performanceCompare);
	for(int i=0; i<performanceResults.size(); i++)
	{
		int* performanceResult = performanceResults[i];
		cout << performanceResult[0] << "\t"  << performanceResult[1] << "\t"  << performanceResult[2] << "\t" << performanceResult[3] << "\t" << performanceResult[4] << "\t" << performanceResult[5] << endl;
		
	}
	cout<<endl;
	
	map<int, int> countBySize;
	map<int, int*> sumsBySize;
	map<int, double*> avgsBySize;
	map<int, double*> stdDevsBySize;
	for(int i=0; i<performanceResults.size(); i++)
	{
		int* performanceResult = performanceResults[i];
		int count = countBySize[performanceResult[0]];
		countBySize[performanceResult[0]]=count+1;
		int* sums = sumsBySize[performanceResult[0]];
		if(sums==NULL)
		{
			sums=new int[4];
			sums[0]=0; sums[1]=0; sums[2]=0; sums[3]=0;
			sumsBySize[performanceResult[0]]=sums;
		}
		for(int i=0; i<4; i++)
			sums[i]+=performanceResult[i+2];
	}
	for(auto mapit = countBySize.begin();  mapit!= countBySize.end(); mapit++)
	{
		int size = mapit->first;
		int count = mapit->second;
		int* sums = sumsBySize[size];
		double* avgs = new double[4];
		for(int i=0; i<4; i++)
			avgs[i]=(double)sums[i]/(double)count;
		avgsBySize[size] = avgs;
	}
	
	for(int i=0; i<performanceResults.size(); i++)
	{
		int* performanceResult = performanceResults[i];
		int size = performanceResult[0];
		double* stdDevs = stdDevsBySize[size];
		if(stdDevs==NULL)
		{
			stdDevs = new double[4];
			stdDevs[0]=0; stdDevs[1]=0; stdDevs[2]=0; stdDevs[3]=0;
			stdDevsBySize[performanceResult[0]]=stdDevs;
		}
		for(int i=0; i<4; i++)
			stdDevs[i]+=pow((double)performanceResult[i+2]-avgsBySize[size][i],2);
	}
	for(auto mapit = countBySize.begin();  mapit!= countBySize.end(); mapit++)
	{
		int size = mapit->first;
		int count = mapit->second;
		double* stdDevs = stdDevsBySize[size];
		for(int i=0; i<4; i++)
			stdDevs[i] = sqrt(stdDevs[i]/count);
	}
	cout<<"SIZE\tG_ACC_AVG\tG_ACC_DEV\tG_TIM_AVG\tG_TIM_STD\tLS_ACC_AVG\tLS_ACC_DEV\tLS_TIM_AVG\tLS_TIM_STD"<<endl;
	for(auto mapit = countBySize.begin();  mapit!= countBySize.end(); mapit++)
	{
		int size = mapit->first;
		double* avgs = avgsBySize[size];
		double* stdDevs = stdDevsBySize[size];
		cout<<size;
		for(int i=0; i<4; i++)
		{
			cout<<"\t"<<avgs[i]<<"\t"<<stdDevs[i];
		}
		cout<<endl;
	}
	/*	
	cout << "LS IMPROVEMENTS\t" << endl;
	int maxLsIterationSize = 0;
	vector<vector<double>*> lsImprovementGroups;
	for(int i=0; i<lsImprovementsReduced.size(); i++)
	{
		vector<int>* instanceLsImprovementsReduced = lsImprovementsReduced.at(i);
		if(instanceLsImprovementsReduced->at(0) == 25)
			continue;
		if(instanceLsImprovementsReduced->size() > maxLsIterationSize)
			maxLsIterationSize = instanceLsImprovementsReduced->size();
	}
	for(int i=0; i<lsImprovementsReduced.size(); i++)
	{
		vector<int>* instanceLsImprovementsReduced = lsImprovementsReduced.at(i);
		if(instanceLsImprovementsReduced->at(0) == 25)
			continue;
		int instanceSize = instanceLsImprovementsReduced->at(0);
		vector<double>* lsImprovementGroup = NULL;
		for(int j=0; j< lsImprovementGroups.size(); j++)
		{
			if(lsImprovementGroups.at(j)->at(0)==instanceSize)
				lsImprovementGroup = lsImprovementGroups.at(j);
		}
		if(lsImprovementGroup == NULL)
		{
			lsImprovementGroup = new vector<double>();
			lsImprovementGroup->assign(maxLsIterationSize+1, 0);
			(*lsImprovementGroup)[0]= instanceSize;
			lsImprovementGroups.push_back(lsImprovementGroup);
		}
		(*lsImprovementGroup)[1]++;
		for(int j=1; j < instanceLsImprovementsReduced->size(); j++)
		{
			(*lsImprovementGroup)[j+1] += instanceLsImprovementsReduced->at(j);
		}
		delete instanceLsImprovementsReduced;
	}
	for(int i=0; i<lsImprovementGroups.size(); i++)
	{
		vector<double>* lsImprovementGroup = lsImprovementGroups.at(i);
		for(int j=0; j<lsImprovementGroup->size(); j++)
		{
			if(j<2)
				cout<<lsImprovementGroup->at(j)<<" ";
			else
				cout<<lsImprovementGroup->at(j) * 1000 / (lsImprovementGroup->at(0)*lsImprovementGroup->at(1))<<" ";
		}
		cout<<endl;
		delete lsImprovementGroup;
	}
	*/
	return 0;
}

void printConflictGraph(vector<PointFeature*>* pointFeatures)
{
	for(vector<PointFeature*>::iterator it=pointFeatures->begin(); it!=pointFeatures->end(); it++)
	{
		PointFeature* pointFeature = *it;
		vector<PositionNode*>* positions = pointFeature->getPositions();
		cout<<"Point Feature Number: "<<pointFeature->getNo()<<endl;
		for(int i=0; i<positions->size(); i++)
		{
			PositionNode* positionNode = (*positions)[i];
			cout<<"\tPosition Number: "<<positionNode->getNo()<<" Conflict Size: "<<positionNode->getConflictingPositions()->size()<<endl;
			cout<<"\tConflicting Positions:";
			for(int j=0; j<positionNode->getConflictingPositions()->size(); j++)
			{
				PositionNode* conflictingPositionNode = positionNode->getConflictingPositions()->at(j);
				cout<<" "<<conflictingPositionNode->getNo();
			}
			cout<<endl;
		}
	}
}
