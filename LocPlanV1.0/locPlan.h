#pragma once
#ifndef LOCPLAN
#define LOCPLAN
#include "stdafx.h"
#include <list>


struct Door
{
public:
	int num;
	std::list<double> doorloc;
}; 
struct Product
{
public:
	std::string name;
	int id;
	std::list<double> keyPointS;
};
class Locplan
{
public:
	//�������
	Door entrance;
	Door exit;
	double** keypointSMatrix;
	Product* products;
	//���庯��
	Locplan(int num,char** namelist,double* arealist);
	bool Getinidata(double &enlist, double &exlist, double start, double end,double uplength,double downlength);
	//test github
};


#endif // LOCPLAN