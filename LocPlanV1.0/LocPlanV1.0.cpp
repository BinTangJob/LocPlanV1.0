// LocPlanV1.0.cpp : �������̨Ӧ�ó������ڵ㡣
//

#include "stdafx.h"
#include "locPlan.h"
#include <sstream>
#include <fstream>
#include <iostream>
#include <unordered_map>

struct desighnode
{
	int entrancenum;
	vector<double> entraloc;
	int exitnum;
	vector<double> exitloc;
	double rowloc[2][2];
};

struct systemnode
{
	int line;
	int entrancepackage;
	int exitpackage;
	double area;

};
struct inpnode 
{
	int partnum;//�ܳ�����;
	int recnum;//�����¼��
	double area;//��λ���
	double iflux;//�������
	double oflux1;//����1��������
	double oflux2;//����2��������
	double oflux3;//����3��������
	int entranceNo;//����ű��;
	double odistance;//���⸺�ؾ���
	
};
typedef unordered_map<string, systemnode> sysmap;
typedef unordered_map<string, inpnode> inpmap;
//���ݣ����㣩ʢ���������㼰ʢ�߳ߴ�������ռ���С
double getArea(int N, double width, double lenth,double loclenth)
{
	return 0.0;
}
//����
bool getdesighdatafromfile(string desfile, desighnode &dnode)
{
	std::ifstream infile;
	infile.open(desfile);
	if (infile.fail())
	{
		cout << "�򿪿��������ļ�" << desfile << "ʧ�ܣ������ļ���" << endl;
		return false;
	}
	char title[256];
	infile.getline(title, 256);
	// ��ȡ������ļ�
	char strtemp[128];
	infile.getline(strtemp, 128, ',');
	infile.getline(strtemp, 128, '\n');
	int entrancenum = atoi(strtemp);
	if (entrancenum > 0)
	{
		dnode.entrancenum = entrancenum;
	}
	else
	{
		cout << "���������ļ������������ĿΪ" << entrancenum << "ʧ�ܣ������ļ���" << endl;
		return false;
	}
	infile.getline(strtemp, 128, ',');
	for (int i = 0; i < entrancenum - 1; i++)
	{
		infile.getline(strtemp, 128, ',');
		double enloc = atof(strtemp);
		dnode.entraloc.push_back(enloc);
	}
	infile.getline(strtemp, 128, '\n');
	double enloc = atof(strtemp);
	dnode.entraloc.push_back(enloc);
	//��ȡ�������ļ�
	infile.getline(strtemp, 128, ',');
	infile.getline(strtemp, 128, '\n');
	int exitnum = atoi(strtemp);
	if (exitnum > 0)
	{
		dnode.exitnum = exitnum;
	}
	else
	{
		cout << "���������ļ����󣬳�������ĿΪ" << exitnum << "ʧ�ܣ������ļ���" << endl;
		return false;
	}
	infile.getline(strtemp, 128, ',');
	for (int i = 0; i < exitnum - 1; i++)
	{
		infile.getline(strtemp, 128, ',');
		double exloc = atof(strtemp);
		dnode.entraloc.push_back(enloc);
	}
	infile.getline(strtemp, 128, '\n');
    double exloc = atof(strtemp);
	dnode.entraloc.push_back(enloc);
	//��ȡ��λ����
	infile.getline(strtemp, 128, ',');
	double loc1 = atof(strtemp);
	infile.getline(strtemp, 128, ',');
	double loc2 = atof(strtemp);
	infile.getline(strtemp, 128, ',');
	double loc3 = atof(strtemp);
	infile.getline(strtemp, 128, '\n');
	double loc4 = atof(strtemp);
	dnode.rowloc[0][0] = loc1;
	dnode.rowloc[1][0] = loc2;
	dnode.rowloc[0][1] = loc3;
	dnode.rowloc[1][1] = loc4;
	return true;
}
bool getflexsimmodedata(desighnode &dnode)
{
	dnode.entrancenum = 3;//�������
	dnode.entraloc = { 0.0,10.0,20.0 };
	dnode.exitnum = 3;//��������
	dnode.exitloc = { 0.0,30.0,80.0 };
	dnode.rowloc[0][0] =0.0;
	dnode.rowloc[0][1] = 0.0;
	dnode.rowloc[1][0] = 90.0;
	dnode.rowloc[1][1] = 100.0;
	//myplan.Getinidata(entraloc, entrancenum, exitloc, exitnum, 2, rowloc);
	return true;
}
bool getsystemdata(string filename,sysmap &systempars)
{
	std::ifstream infile;
	infile.open(filename);
	if (infile.fail())
	{
		cout << "��ϵͳ�����ļ�"<<filename<< "ʧ�ܣ������ļ���" << endl;
		return false;
	}
	char title[256];
	infile.getline(title, 256);
	while (true)
	{
		char strtemp[128];
		string partcode, line;
		int ipackage, opackage;
		double area;
		infile.getline(strtemp, 128, ',');
		if (infile.eof())
		{
			break;
		}
		partcode = strtemp;
		infile.getline(strtemp, 128, ',');
		line = strtemp;
		infile.getline(strtemp, 128, ',');
		ipackage = atoi(strtemp);
		infile.getline(strtemp, 128, ',');
		opackage = atoi(strtemp);
		infile.getline(strtemp, 128, '\n');
		area = atof(strtemp);
		//cout<< partcode << "," << line << "," << ipackage <<","<< opackage<<"," << area<<endl;
		systemnode tempnode;
		tempnode.entrancepackage = ipackage;
		tempnode.exitpackage = opackage;
		tempnode.area = area;
		if (line=="A")
		{
			tempnode.line = 2;
		}
		else if (line == "B")
		{
			tempnode.line = 3;
		}
		if (systempars.find(partcode)!=systempars.end())
		{
			cout << partcode << "�����ظ�����������;" << endl;
			return false;
		}
		systempars[partcode]=tempnode;
	}
	return true;	
}
bool getinpdata(inpmap &myinp, desighnode &stockdesigh,string flexsim,
	string sysfile,string designfile,int mode)
{
	if (mode == 0)
	{
		getflexsimmodedata(stockdesigh);
	}
	else
		getdesighdatafromfile(designfile, stockdesigh);
	double stockwidth=10.0;
	sysmap mysysmap;
	getsystemdata(sysfile,mysysmap);
	ifstream infile;
	infile.open(flexsim);
	if (infile.fail())
	{
		cout << "�򿪳���·����¼�ļ�" << flexsim << "ʧ�ܣ������ļ���" << endl;
		return false;
	}
	char title[256];
	infile.getline(title, 256);
	infile.getline(title, 256);
	while (true)
	{
		char strtemp[128];
		string partcode;
		infile.getline(strtemp, 128, ',');
		if (infile.eof())
		{
			break;
		}
		partcode = strtemp;
		if (myinp.find(partcode) == myinp.end())
		{
			inpnode tempnode;
			tempnode.recnum = 0;
			tempnode.partnum = 0;
			tempnode.area = mysysmap[partcode].area / stockwidth;
			tempnode.entranceNo = mysysmap[partcode].line;
			tempnode.oflux1 = 0.0;
			tempnode.oflux2 = 0.0;
			tempnode.oflux3 = 0.0;
			tempnode.odistance = 0.0;
			myinp[partcode] = tempnode;		
		}
		myinp[partcode].recnum++;
		myinp[partcode].partnum += mysysmap[partcode].exitpackage;
		infile.getline(strtemp, 128, ',');
		int exitNo= atoi(strtemp);
		switch (exitNo)
		{
		case 1:
			myinp[partcode].oflux1 += 1;
			break;
		case 2:
			myinp[partcode].oflux2 += 1;
			break;
		case 3:
			myinp[partcode].oflux3 += 1;
			break;
		default:
			cout << partcode << "�����źŴ����������ݡ�" << endl;
			return false;
		}
		infile.getline(strtemp, 128, '\n');
		double curodistance = atof(strtemp);
		if (curodistance<=0.0)
		{
			cout << partcode << "�����¼�ļ���������" << endl;
			return false;
		}
		myinp[partcode].odistance += curodistance;
	}
	for (inpmap::iterator it=myinp.begin();it!=myinp.end();it++)
	{
		it->second.odistance /= it->second.partnum;
		it->second.iflux = it->second.partnum % mysysmap[it->first].entrancepackage>0?1:0+ it->second.partnum / mysysmap[it->first].entrancepackage;
	}
	return true;
}

int main()
{
	//��ʼ����������,���ļ��ж�ȡ��Ʒ����
	int mode = 0;
	string sysfile="system.csv";
	string recfile="out.csv";
	string designfile = "designdata.csv";
	inpmap myinpmap;
	desighnode stockdesigh;
	getinpdata(myinpmap, stockdesigh,recfile, sysfile,designfile,mode);
	int pronum = myinpmap.size();
	int exitnum = stockdesigh.exitnum;
	//std::ifstream infile;
	//infile.open("product.txt");
	string* namelist = new string[pronum];
	double* arealist = new double[pronum];
	double** exitweightlist = new double*[pronum];
	double* entranceweightlist = new double[pronum];
	int* entrancelist = new int[pronum];
	double* weldingdislist = new double[pronum];
	int idx = 0;
	for (inpmap::iterator it=myinpmap.begin();it!=myinpmap.end();it++)
	{
		namelist[idx] = it->first;
		arealist[idx] = it->second.area;
		exitweightlist[idx] = new double[3];
		exitweightlist[idx][0]=it->second.oflux1;
		exitweightlist[idx][1] = it->second.oflux2;
		exitweightlist[idx][2] = it->second.oflux3;
		entrancelist[idx] = it->second.entranceNo - 1;
		entranceweightlist[idx] = it->second.iflux;
		weldingdislist[idx] = it->second.odistance;
		idx++;
	}
	Locplan myplan(pronum, exitnum, namelist, arealist, exitweightlist, entranceweightlist, entrancelist, weldingdislist);
	//��ʼ���ⷿ����
	myplan.Getinidata(stockdesigh.entraloc.data(), stockdesigh.entrancenum, stockdesigh.exitloc.data(), exitnum, 2, stockdesigh.rowloc);
	//getdesighdata(myplan);
	//̰���㷨
	std::vector<int > greedySolution = myplan.GetGreedySolotion();
	std::cout << "greedysolution is "<<myplan.evaluation(greedySolution)<<std::endl;
	//�Ŵ��㷨
	std::vector<int > GASolution = myplan.GA(1000,10000,500,0.02,0.6,true,"test.log");
	std::cout << "final GAsolution is " << myplan.evaluation(GASolution) << std::endl;
    return 0;
}

