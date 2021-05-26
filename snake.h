#pragma once
#include "map.h"
#include <vector>
#include <set>

class snake_t
{
private:
	maps* map;
	struct node
	{
		int i, j, prevI, prevJ;
		node() : i(0), j(0), prevI(0), prevJ(0) { }
		node(int i, int j, int prevI, int prevJ)
		{
			this->i     = i;
			this->j     = j;
			this->prevI = prevI;
			this->prevJ = prevJ;
		}
	};
	std::vector <node> teil; // head = teil[0]
	std::pair<node, node> foods;
	void initFoods();
	int size;
	bool RecAImove(std::vector<std::vector<int>>&, std::set<std::pair<int, int>>, int);
	std::vector<std::pair<int, int>> searchWay(std::vector<std::vector<int>>, int, int);
public:
	snake_t(maps*);
	bool move(char);
	char AImove();
};