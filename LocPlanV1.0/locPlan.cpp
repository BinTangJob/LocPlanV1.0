#include "stdafx.h"
#include "locPlan.h"

const static double ZERO=1.0E-8;
const static int MAX = 100;
//类的初始化

Locplan::Locplan(int num, int exitnum, string* namelist, double * arealist,
	double** exitweightlist,double* entranceweightlist,int* entrancelist, 
	double* weldingdislist)
{
	
	for (int i = 0; i < num; i++)
	{
		Product temp;
		temp.id = i;
		temp.name = namelist[i];
		temp.area = arealist[i];
		temp.entranceid = entrancelist[i];
		temp.entranceweight = entranceweightlist[i];
		temp.exitweight.assign(exitweightlist[i], exitweightlist[i]+exitnum);
		temp.averageWeldingdistance = weldingdislist[i];
		products.push_back(temp);
		/*
		for (int j = 0; j < doornum; j++)
		{
			products[i].doorWeight.push_back(doorweightlist[i][j]);
		}
		*/
	}
}

bool Locplan::Getinidata(double* entrancelist,int entrancenum ,double* exitlist,int exitnum,
	int rownum, double rowloc[][2])
{
	//初始化入口数据
	entrance.num = entrancenum;
	entrance.doorloc.assign(entrancelist, entrancelist + entrancenum);
	//初始化出口数据
	exit.num = exitnum;
	exit.doorloc.assign(exitlist, exitlist + exitnum);
	//初始化库房排布数据
	rows.num = rownum;
	rows.Rstart.assign(rowloc[0], rowloc[0] + rownum);
	rows.Rend.assign(rowloc[1], rowloc[1] + rownum);
	//初始化每个产品的关键点数据
	for (std::vector<Product>::iterator it = products.begin(); it != products.end(); it++)
	{
		GetProductKeyPointS(*it);
	}
	return true;
}

double Locplan::Caldistance(double locx,Product calProduct)
{
	double res = 0.0;
	//入库距离
	res += calProduct.entranceweight*(abs(locx - entrance.doorloc[calProduct.entranceid] + calProduct.area / 2.0) + doorlength);
	//出库距离
	for (unsigned int i=0;i< calProduct.exitweight.size();++i)
	{
		res += calProduct.exitweight[i] * (abs(locx + calProduct.area / 2.0 - exit.doorloc[i]) + calProduct.averageWeldingdistance + doorlength);
	}
	return res;
}

double Locplan::Calsolutiondistance(std::vector<int> solution)
{
	double res = 0.0;
	double xloc = 0.0;
	for (unsigned int i = 0; i < solution.size(); i++)
	{
		res += Caldistance(xloc, products[solution[i]]);
		xloc += products[solution[i]].area;
	}
	return res;
}


void Locplan::GetProductKeyPointS(Product& calProduct)
{
	//加入起点端值
	calProduct.keyPointS.push_back(Caldistance(rows.Rstart[0], calProduct));
	for (std::vector<double>::iterator it = exit.doorloc.begin(); it != exit.doorloc.end(); it++)
	{
		calProduct.keyPointS.push_back(Caldistance(*it, calProduct));
	}
	//加入终点端值
	calProduct.keyPointS.push_back(Caldistance(rows.Rend[0], calProduct));
}





bool Locplan::InstorageAt(int storageNo, seartchflag & mysearchpoint,std::priority_queue<sortorder>& abspque,
	int &kickout, std::vector<double> &storageVolume, std::vector<double> &curstorage)
{
	if (curstorage[storageNo]+ZERO > storageVolume[storageNo])
	{
		if (abspque.empty())
			return false;
	    if (abspque.top().value > mysearchpoint.keyweight[storageNo])
			return false;
		sortorder pqnode;
		pqnode.code = mysearchpoint.id;
		pqnode.value = abs(mysearchpoint.keyweight[storageNo]);
		kickout = abspque.top().code;
		abspque.pop();
		curstorage[storageNo] -= products[kickout].area;
		curstorage[storageNo] += products[pqnode.code].area;
		abspque.push(pqnode);
		return true;
	}
	else
	{
		sortorder pqnode;
		pqnode.code = mysearchpoint.id;
		pqnode.value = abs(mysearchpoint.keyweight[storageNo]);
		kickout = -1;
		abspque.push(pqnode);
		curstorage[storageNo] += products[pqnode.code].area;
		return true;

	}
	
}

int Locplan::Instorage(seartchflag & mysearchpoint, std::vector<std::priority_queue<sortorder> >& abspque,
	std::vector<double> &storageVolume, std::vector<double> &curstorage)
{
	int res = -1;
	for (size_t i = mysearchpoint.num; i < mysearchpoint.seartchorder.size(); i++)
	{
		if (mysearchpoint.seartchorder[i] == 0|| mysearchpoint.seartchorder[i] == 4)
		{
			int storagenum;
			if (mysearchpoint.seartchorder[i] == 0)
				storagenum = mysearchpoint.seartchorder[i];
			else
				storagenum = mysearchpoint.seartchorder[i] - 1;
			if (mysearchpoint.doneflag[storagenum])
				continue;
			else
			{
				mysearchpoint.doneflag[storagenum] = true;
			}
			if (InstorageAt(storagenum, mysearchpoint, abspque[storagenum], res, storageVolume, curstorage))
			{
				mysearchpoint.num = i;
				mysearchpoint.seletedstorage = storagenum;
				return res;
			}
			else
				continue;
		}
		else
		{
			int storagenumleft = mysearchpoint.seartchorder[i]-1;
			int storagenumright = mysearchpoint.seartchorder[i];
			if (mysearchpoint.doneflag[storagenumleft])
			{
				if (mysearchpoint.doneflag[storagenumright])
					continue;
				else
				{
					mysearchpoint.doneflag[storagenumright] = true;
					if (InstorageAt(storagenumright, mysearchpoint, abspque[storagenumright], res, storageVolume, curstorage))
					{
						mysearchpoint.num = i;
						mysearchpoint.seletedstorage = storagenumright;
						return res;
					}
					else
						continue;
				}					
			}
			else
			{
				if (mysearchpoint.doneflag[storagenumright])
				{
					mysearchpoint.doneflag[storagenumleft] = true;
					if (InstorageAt(storagenumleft, mysearchpoint, abspque[storagenumleft], res, storageVolume, curstorage))
					{
						mysearchpoint.num = i;
						mysearchpoint.seletedstorage = storagenumleft;
						return res;
					}
					else
						continue;
				}
				else
				{
					if (mysearchpoint.keyweight[storagenumleft] < mysearchpoint.keyweight[storagenumright])
					{
						mysearchpoint.doneflag[storagenumleft] = true;
						if (InstorageAt(storagenumleft, mysearchpoint, abspque[storagenumleft], res, storageVolume, curstorage))
						{
							mysearchpoint.num = i;
							mysearchpoint.seletedstorage = storagenumleft;
							return res;
						}
						else
							continue;
					}
					else
					{
						mysearchpoint.doneflag[storagenumright] = true;
						if (InstorageAt(storagenumright, mysearchpoint, abspque[storagenumright], res, storageVolume, curstorage))
						{
							mysearchpoint.num = i;
							mysearchpoint.seletedstorage = storagenumright;
							return res;
						}
						else
							continue;
					}
				}
			}
		}
	}
	return res;
}


bool comp(sortorder A, sortorder B)
{
	return(A.value < B.value);
}
bool operator<(sortorder A, sortorder B)
{
	return(A.value > B.value);
}

//排布贪心算法
std::vector<int> Locplan::GetGreedySolotion()
{
	std::vector<int> res;
	//std::vector<std::vector<double>> keyvalue;
	//搜索变量
	std::vector<seartchflag> searchProducts;
	std::vector<double> keyloc;//关键点变量（出口坐标）
	keyloc.push_back(rows.Rstart[0]);
	keyloc.insert(keyloc.begin() + 1, exit.doorloc.begin(), exit.doorloc.end());
	keyloc.push_back(rows.Rend[0]);
	//库区变量
	/***************************************************
	三个门可将一个库区分成4个小区域，因此需要生成对应的优先队列和容量记录器
	abs队列用于排列优先级，绝对值越大离门越近;非abs队列代表方向，正负号表示方向
	***************************************************/
	std::vector<std::priority_queue<sortorder> > storageArea_abs(exit.num + 1);
	std::vector<std::priority_queue<sortorder> > storageArea(exit.num + 1);
	std::vector<double> storageVolume(exit.num + 1);
	std::vector<double> curstoragevolume(exit.num + 1);
	//变量初始化
	//初始化库位数据
	for (int i = 1; i < exit.num + 2;i++)
	{
		storageVolume[i - 1] = keyloc[i] - keyloc[i - 1];
		curstoragevolume[i - 1] = 0.0;
	}
	for (std::vector<Product>::iterator it = products.begin(); it != products.end(); it++)
	{
		std::vector<sortorder> sortKPS;//用于记录布局为每个门时的距离值
		seartchflag searchitem;
		searchitem.num = 0;
		searchitem.id = it->id;
		searchitem.seletedstorage= -1;
		for (size_t i = 0; i < it->keyPointS.size(); i++)
		{
			
			sortorder temp;
			temp.code = i;
			temp.value = it->keyPointS[i];
			sortKPS.push_back(temp);
			if (i != 0)
			{
				searchitem.doneflag.push_back(false);
				if (abs(keyloc[i] - keyloc[i - 1]) < ZERO)
					searchitem.keyweight.push_back(0.0);
				else
					searchitem.keyweight.push_back((it->keyPointS[i]- it->keyPointS[i-1])/(keyloc[i]-keyloc[i-1]));
			}
		}
		std::stable_sort(sortKPS.begin(), sortKPS.end(),comp);
		for (size_t i = 0; i < sortKPS.size(); i++)
		{
			searchitem.seartchorder.push_back(sortKPS[i].code);
		}
		searchProducts.push_back(searchitem);		
	}
	//搜索算法
	for (std::vector<seartchflag>::iterator it = searchProducts.begin(); it != searchProducts.end(); it++)
	{
		int restorage = it->id;
		while (restorage != -1)
		{
			//入库
			restorage=Instorage(searchProducts[restorage], storageArea_abs, storageVolume, curstoragevolume);
			if (restorage != -1)
				continue;
			for (size_t i = 0; i < curstoragevolume.size(); i++)
			{
				if (storageVolume[i] < ZERO|| storageArea_abs[i].size()==0)
					continue;
				if (curstoragevolume[i] - products[storageArea_abs[i].top().code].area - ZERO > storageVolume[i])
				{
					restorage = storageArea_abs[i].top().code;
					storageArea_abs[i].pop();
					curstoragevolume[i] -= products[storageArea_abs[i].top().code].area;
					break;
				}
			}
		}
	}
	//整合输出
	for (size_t i = 0; i < storageArea_abs.size(); i++)
	{
		while (!storageArea_abs[i].empty())
		{
			int id = storageArea_abs[i].top().code;
			sortorder node;
			node.code = id;
			node.value = searchProducts[id].keyweight[searchProducts[id].seletedstorage];
			storageArea[i].push(node);
			storageArea_abs[i].pop();
		}	
	}
	for (size_t i = 0; i < storageArea.size(); i++)
	{
		std::vector<int> temp;
		while (!storageArea[i].empty())
		{
			temp.push_back(storageArea[i].top().code);
			storageArea[i].pop();
		}
		for (std::vector<int>::reverse_iterator rit = temp.rbegin(); rit != temp.rend(); rit++)
		{
			res.push_back(*rit);
		}
	}
	return res;
}

double Locplan::evaluation(std::vector<int> solution)
{
	return Calsolutiondistance(solution);
}

std::vector<int> Locplan::GetRandomSolotion()
{
	std::vector<int> res;
	std::list <int> baseSeedlist;
	int productsnum = products.size();
	for (int i = 0; i < productsnum; i++)
	{
		baseSeedlist.push_back(i);
	}
	//srand(seed);
	//cout << (unsigned)time(NULL) << endl;
	int curnum = productsnum;
	for (int i = 0; i < productsnum; i++)
	{
		int step = rand() % curnum;
		std::list<int>::iterator it=baseSeedlist.begin();
		for (int j = 0; j < step; j++)
			it++;
		res.push_back(*it);
		baseSeedlist.erase(it);
		curnum--;
	}
	return res;
}
int genecount(int gene,std::vector<int> &pos, std::vector<int> solution)
{
	pos.clear();
	int res = 0;
	for (size_t i = 0; i < solution.size(); i++)
	{
		if (solution[i] == gene)
		{
			res++;
			pos.push_back(i);
		}
	}
	return res;
}
std::vector<int> genefixpos(int p1, int p2, std::vector<int> solution)
{
	std::vector<int> res1;
	for (int i = p1; i < p2 + 1; i++)
	{
		std::vector<int> pos1;
		if (genecount(solution[i], pos1, solution) != 2)
			continue;
		if (pos1[0] >= p1&&pos1[0] <= p2)
			res1.push_back(pos1[1]);
		else
			res1.push_back(pos1[0]);
	}
	std::sort(res1.begin(),res1.end());
	return res1;
}
std::vector<std::vector<int> > Locplan::GAcross(std::vector<int> parent1, std::vector<int> parent2)
{
	int num = parent1.size();
	//srand((unsigned)time(NULL));
	int point1 = rand() % num;
	int point2 = rand() % num;
	if (point1 > point2)
	{
		int temp = point1;
		point1 = point2;
		point2 = temp;
	}
	std::vector<int>::iterator it1 = parent1.begin();
	std::vector<int>::iterator it2 = parent2.begin();
	std::vector<int> child1;
	std::vector<int> child2;
	child1.assign(it1, it1 + point1);
	child1.insert(child1.end(), it2 + point1, it2 + point2+1);
	child2.assign(it2, it2 + point1);
	child2.insert(child1.end(), it1 + point1, it1 + point2+1);
	if (point2 < num - 1)
	{
		child2.insert(child2.end(), it2 + point2 + 1, parent2.end());
		child1.insert(child1.end(), it1 + point2 + 1, parent1.end());
	}
	//修复子代重复基因
	std::vector<int> fix1 = genefixpos(point1, point2, child1);
	std::vector<int> fix2 = genefixpos(point1, point2, child2);
	for (size_t i = 0; i < fix1.size(); i++)
	{
		child1[fix1[i]] = parent2[fix2[i]];
		child2[fix2[i]] = parent1[fix1[i]];
	}
	std::vector<std::vector<int> > res{ child1,child2 };
	return res;
}

std::vector<int> Locplan::GAmutation(std::vector<int> solution)
{
	std::vector<int> res;
	int num = solution.size();
	res.assign(solution.begin(), solution.end());
	//srand((unsigned)time(NULL));
	int point1 = rand() % num;
	int point2 = rand() % num;
	while (point2==point1)
	{
		point2 = rand() % num;
	}
	int temp = res[point1];
	res[point1] = res[point2];
	res[point2] = temp;
	return res;
}

double mypartialsum(std::vector<double> fitness, std::vector<double>& roulette)
{
	double sum = 0.0;
	for (size_t i=0;i<fitness.size();i++)
	{
		sum += fitness[i];
		roulette.push_back(sum);
	}
	return sum;
}

std::vector<std::vector<int>> Locplan::GAselection(int groupnum,std::vector<std::vector<int>> parents, bool isElitistStrategy,int &best,double &bestval)
{
	std::vector<std::vector<int>> res;
	//轮盘赌算法
	bool bestinnext = false;
	int bestsolution;
	std::vector<double> fitness = GAfitness(1.0, parents, bestsolution, bestval);
	std::vector<double> roulette;
	double sum = mypartialsum(fitness, roulette);
	for (size_t i = 0; i < groupnum; i++)
	{
		//srand((unsigned)time(NULL));
		double bid = double(rand()) / RAND_MAX;
		for (size_t j = 0; j < roulette.size(); j++)
		{
			if (bid <= roulette[j]/sum)
			{
				res.push_back(parents[j]);
				if (j == bestsolution)
				{
					best = res.size() - 1;
					bestinnext = true;
				}	
				break;
			}
		}
	}
	if (isElitistStrategy && !bestinnext)
	{
		//srand((unsigned)time(NULL));
		int point = rand() % groupnum;
		res[point] = parents[bestsolution];
		best = point;
	}
	return res;
}

std::vector<double> Locplan::GAfitness(double fitnessindex, std::vector<std::vector<int> > solutions,int& best,double &bestval)
{
	double val= 0.0;
	std::vector<double> res;
	for (size_t i=0;i<solutions.size();i++)
	{
		double fitness = std::pow(Calsolutiondistance(solutions[i])*0.001, -fitnessindex);
		if (fitness > val)
		{
			val = fitness;
			best = i;
		}
		res.push_back(fitness);
	}
	bestval = val;
	return res;
}
//遗传算法
std::vector<int> Locplan::GA(int groupnum, int maxiters,int endflag,double emutationrate, double crossrate, bool isElitistStrategy, string logfile)
{
	std::ofstream outfile;
	outfile.open(logfile);
	//生成初始种群
	std::vector<int> historybest;
	double historybestval=0.0;
	int bestsol;
	int stablenum = 0;
	double prebestval=1.0e8;
	std::vector<double> bestlog;
	std::vector<std::vector<int> >  group;
	srand((unsigned)time(NULL));
	//srand(1);
	for (int i = 0; i < groupnum; i++)
	{
		group.push_back(GetRandomSolotion());
	}
	//
	int iters = 0;
	while (stablenum < endflag&&iters < maxiters)
	{
		/*
		if (iters >= 48)
		{
			int best;
			double bestval1;
			GAfitness(1.0, group, best, bestval1);
			cout << iters << "before selection is " << bestval1 << endl;
			cout << std::pow(Calsolutiondistance(group[best]), -1.0) << endl;
		}*/
		//选择
		double bestval;
		group = GAselection(groupnum,group, isElitistStrategy,bestsol,bestval);
		std::vector<std::vector<int> > groupNextGenerateion(group);
		int couplecount = 0;
		int parent1 = -1;
		int parent2 = -1;
		//交叉
		
		/*
		if (iters >= 48)
		{
			int best;
			double bestval1;
			std::vector<double> fitness=GAfitness(1.0, group, best, bestval1);
			cout << iters << "before cross is " << bestval1 << endl;
			cout << std::pow(Calsolutiondistance(group[bestsol]), -1.0) << endl;
			cout << std::pow(Calsolutiondistance(group[best]), -1.0)<<endl;
		}
		*/
		for (int i = 0; i < groupnum; i++)
		{
			
			//srand((unsigned)time(NULL));
			double crossover = double(rand()) / RAND_MAX;
			if (crossover < crossrate)
			{
				if (couplecount % 2 == 1)
				{
					if (isElitistStrategy&&bestsol == i)
						continue;
					parent1 = i;
					couplecount++;
				}					
				if (couplecount % 2 == 0 && couplecount > 0)
				{
					if (isElitistStrategy&&bestsol == i)
						continue;
					parent2 = i;
					std::vector<std::vector<int> > children = GAcross(group[parent1], group[parent2]);
					//groupNextGenerateion.push_back(children[0]);
					//groupNextGenerateion.push_back(children[1]);
					groupNextGenerateion[parent1] = children[0];
					groupNextGenerateion[parent2] = children[1];
					couplecount++;
				}
			}
		}
		/*
		if (iters >= 52)
		{
			int best;
			double bestval1;
			GAfitness(1.0, groupNextGenerateion, best, bestval1);
			cout << iters << "before emutation is " << bestval1 << endl;
			cout << std::pow(Calsolutiondistance(group[best]), -1.0) << endl;
		}*/
		//变异
		for (int i = 0; i < groupnum; i++)
		{
			//srand((unsigned)time(NULL));
			double mutation = double(rand()) / RAND_MAX;
			if (mutation < emutationrate)
			{
				if (isElitistStrategy&&bestsol == i)
					continue;
				std::vector<int> newsol = GAmutation(groupNextGenerateion[i]);
				//groupNextGenerateion.push_back(newsol);
				groupNextGenerateion[i] = newsol;
			}
		}
		bestlog.push_back(bestval);
		if (abs(bestval - prebestval) < ZERO)
			stablenum++;
		else
		{
			outfile << "iter " << iters << " is " << bestval << " ";
			outfile << "sumdistance  is " << evaluation(historybest) << endl;
			stablenum = 0;
		}
			
		//std::cout << "iter " << iters << " is " << bestval << endl;
		//cout << "stable num is " << stablenum << endl;
		//outfile << "stable num is " << stablenum << endl;
		prebestval = bestval;
		iters++;
		group = groupNextGenerateion;
		if (historybestval < bestval)
		{
			historybestval = bestval;
			historybest = group[bestsol];
		}
	}
	outfile.close();
	return historybest;
}


