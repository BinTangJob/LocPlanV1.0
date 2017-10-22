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
//贪心算法使用结构体
struct seartchflag
{
	int num;//记录Seartchorder的搜寻顺序
	int id;//记录产品ID
	int seletedstorage;//记录选择的库位区域
	std::vector<bool> doneflag;//记录各库位区域搜索结果
	std::vector<int> seartchorder;//排序后的搜索顺序
	std::vector<double> keyweight;//记录斜率，可理解为等效物流量
};

struct sortorder
{
	int code;
	double value;
};

class Locplan
{
public:
	//定义参数
	Door entrance;
	Door exit;
	Row rows;
	double** keypointSMatrix;
	std::vector<Product> products;
	//定义函数
	Locplan(int num, int exitnum, string* namelist, double * arealist, double ** exitweightlist,
		double * entranceweightlist, int * entrancelist, double * weldingdislist);
	bool Getinidata(double * entrancelist, int entrancenum, double * exitlist, int exitnum, int rownum, double rowloc[][2]);
	//test github
	//遗传算法
	std::vector<int> GA(int groupnum, int maxiters, int endflag, double emutationrate, double crossrate, bool isElitistStrategy);
	//贪心算法
	std::vector<int> GetGreedySolotion();
	//评价结果
	double evaluation(std::vector<int> solution);
private:
	double doorlength = 10.0;
	double Caldistance(double locx, Product calProduct);
	double Calsolutiondistance(std::vector<int> solution);
	void GetProductKeyPointS(Product& calProduct);
	//贪心算法子函数
	bool InstorageAt(int storageNo, seartchflag & mysearchpoint, std::priority_queue<sortorder>& abspque,
		int &kickout, std::vector<double> &storageVolume, std::vector<double> &curstorage);
	int Instorage(seartchflag & mysearchpoint, std::vector<std::priority_queue<sortorder> >& abspque,
		std::vector<double> &storageVolume, std::vector<double> &curstorage);
	//遗传算法随机种群初始化
	std::vector<int> GetRandomSolotion();
	//交叉算子
	std::vector<std::vector<int> >  GAcross(std::vector<int> parent1, std::vector<int> parent2);
	//突变算子
	std::vector<int> GAmutation(std::vector<int> solution);
	//选择算子
	std::vector<std::vector<int> >  GAselection(int groupnum,std::vector<std::vector<int> > parents,bool isElitistStrategy,int &best,double &bestval);
	//适应度计算
	std::vector<double> GAfitness(double fitnessindex, std::vector<std::vector<int> > solutions,int &best,double &bestval);

};


#endif // LOCPLAN