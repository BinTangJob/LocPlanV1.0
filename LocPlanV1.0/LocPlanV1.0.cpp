// LocPlanV1.0.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include "locPlan.h"
#include <fstream>
#include <iostream>

//根据（单层）盛具数量计算及盛具尺寸计算零件占库大小
double getArea(int N, double width, double lenth,double loclenth)
{
	return 0.0;
}
bool getdesighdata(Locplan& myplan)
{
	int entrancenum = 3;//入口数量
	double entraloc[3] = { 0.0,10.0,20.0 };

	int exitnum = 3;//出口数量
	double exitloc[3] = { 0.0,30.0,80.0 };

	int rownum = 2;
	double rowloc[2][2] = { {0.0,0.0},{100.0,90.0} };	
	myplan.Getinidata(entraloc, entrancenum, exitloc, exitnum, 2, rowloc);
	return true;
}
int main()
{
	//初始化计算数据,从文件中读取产品数据
	int pronum = 44;
	int exitnum = 3;
	std::ifstream infile;
	infile.open("product.txt");
	string* namelist = new string[pronum];
	double* arealist = new double[pronum];
	double** exitweightlist = new double*[pronum];
	double* entranceweightlist = new double[pronum];
	int* entrancelist = new int[pronum];
	double* weldingdislist = new double[pronum];

	for (int i = 0; i < pronum; i++)
	{
		exitweightlist[i] = new double[3];
		infile >> namelist[i]>>arealist[i];
		for (int j = 0; j < 3; j++)
		{
			infile >> exitweightlist[i][j];
		}
		infile >> entrancelist[i] >> entranceweightlist[i] >> weldingdislist[i];
		entrancelist[i]--;
	}
	Locplan myplan(pronum, exitnum, namelist, arealist, exitweightlist, entranceweightlist, entrancelist, weldingdislist);
	//初始化库房数据
	getdesighdata(myplan);
	//贪心算法
	std::vector<int > greedySolution = myplan.GetGreedySolotion();
	std::cout << "greedysolution is "<<myplan.evaluation(greedySolution)<<std::endl;
	//遗传算法
	std::vector<int > GASolution = myplan.GA(1000,10000,500,0.02,0.6,true,"test.log");
	std::cout << "final GAsolution is " << myplan.evaluation(GASolution) << std::endl;
    return 0;
}

