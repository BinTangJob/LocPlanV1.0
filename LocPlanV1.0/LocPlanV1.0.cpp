// LocPlanV1.0.cpp : �������̨Ӧ�ó������ڵ㡣
//

#include "stdafx.h"
#include "locPlan.h"
#include <fstream>
#include <iostream>

//���ݣ����㣩ʢ���������㼰ʢ�߳ߴ�������ռ���С
double getArea(int N, double width, double lenth,double loclenth)
{
	return 0.0;
}
bool getdesighdata(Locplan& myplan)
{
	int entrancenum = 3;//�������
	double entraloc[3] = { 0.0,10.0,20.0 };

	int exitnum = 3;//��������
	double exitloc[3] = { 0.0,30.0,80.0 };

	int rownum = 2;
	double rowloc[2][2] = { {0.0,0.0},{100.0,90.0} };	
	myplan.Getinidata(entraloc, entrancenum, exitloc, exitnum, 2, rowloc);
	return true;
}
int main()
{
	//��ʼ����������,���ļ��ж�ȡ��Ʒ����
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
	//��ʼ���ⷿ����
	getdesighdata(myplan);
	//̰���㷨
	std::vector<int > greedySolution = myplan.GetGreedySolotion();
	std::cout << "greedysolution is "<<myplan.evaluation(greedySolution)<<std::endl;
	//�Ŵ��㷨
	std::vector<int > GASolution = myplan.GA(1000,10000,500,0.02,0.6,true,"test.log");
	std::cout << "final GAsolution is " << myplan.evaluation(GASolution) << std::endl;
    return 0;
}

