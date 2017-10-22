#pragma once
#ifndef LOCPLAN
#define LOCPLAN
#include "stdafx.h"
#include "time.h"
#include <string>
#include <vector>
#include <cmath>
#include <queue>   
#include<algorithm>
#include<list>
#include<iostream>

using namespace std;

struct Door
{
public:
	int num;
	std::vector<double> doorloc;
}; 
struct Product
{
public:
	std::string name;
	int id;
	double area;
	double averageWeldingdistance;
	int entranceid;
	double entranceweight;
	std::vector<double> exitweight;
	std::vector<double> keyPointS;
};
struct Row
{
	int num;
	std::vector<double> Rstart;
	std::vector<double> Rend;
};
//̰���㷨ʹ�ýṹ��
struct seartchflag
{
	int num;//��¼Seartchorder����Ѱ˳��
	int id;//��¼��ƷID
	int seletedstorage;//��¼ѡ��Ŀ�λ����
	std::vector<bool> doneflag;//��¼����λ�����������
	std::vector<int> seartchorder;//����������˳��
	std::vector<double> keyweight;//��¼б�ʣ������Ϊ��Ч������
};

struct sortorder
{
	int code;
	double value;
};

class Locplan
{
public:
	//�������
	Door entrance;
	Door exit;
	Row rows;
	double** keypointSMatrix;
	std::vector<Product> products;
	//���庯��
	Locplan(int num, int exitnum, string* namelist, double * arealist, double ** exitweightlist,
		double * entranceweightlist, int * entrancelist, double * weldingdislist);
	bool Getinidata(double * entrancelist, int entrancenum, double * exitlist, int exitnum, int rownum, double rowloc[][2]);
	//test github
	//�Ŵ��㷨
	std::vector<int> GA(int groupnum, int maxiters, int endflag, double emutationrate, double crossrate, bool isElitistStrategy);
	//̰���㷨
	std::vector<int> GetGreedySolotion();
	//���۽��
	double evaluation(std::vector<int> solution);
private:
	double doorlength = 10.0;
	double Caldistance(double locx, Product calProduct);
	double Calsolutiondistance(std::vector<int> solution);
	void GetProductKeyPointS(Product& calProduct);
	//̰���㷨�Ӻ���
	bool InstorageAt(int storageNo, seartchflag & mysearchpoint, std::priority_queue<sortorder>& abspque,
		int &kickout, std::vector<double> &storageVolume, std::vector<double> &curstorage);
	int Instorage(seartchflag & mysearchpoint, std::vector<std::priority_queue<sortorder> >& abspque,
		std::vector<double> &storageVolume, std::vector<double> &curstorage);
	//�Ŵ��㷨�����Ⱥ��ʼ��
	std::vector<int> GetRandomSolotion();
	//��������
	std::vector<std::vector<int> >  GAcross(std::vector<int> parent1, std::vector<int> parent2);
	//ͻ������
	std::vector<int> GAmutation(std::vector<int> solution);
	//ѡ������
	std::vector<std::vector<int> >  GAselection(int groupnum,std::vector<std::vector<int> > parents,bool isElitistStrategy,int &best,double &bestval);
	//��Ӧ�ȼ���
	std::vector<double> GAfitness(double fitnessindex, std::vector<std::vector<int> > solutions,int &best,double &bestval);

};


#endif // LOCPLAN