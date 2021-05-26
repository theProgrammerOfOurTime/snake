#include "snake.h"
#include "map.h"
#include <vector>
#include <iostream>

void snake_t::initFoods()
{
	if (map->field[foods.first.i][foods.first.j] != '<')
	{
		do 
		{
			foods.first.i = (rand() % (this->map->HEIGTH - 2)) + 1;
			foods.first.j = (rand() % (this->map->LENGTH - 5)) + 1;
		} while (map->field[foods.first.i][foods.first.j] != ' ');
		map->field[foods.first.i][foods.first.j] = '<';
	}
	if (map->field[foods.second.i][foods.second.j] != '>')
	{
		do
		{
			foods.second.i = (rand() % (this->map->HEIGTH - 2)) + 1;
			foods.second.j = (rand() % (this->map->LENGTH - 5)) + 1;
		} while (map->field[foods.second.i][foods.second.j] != ' ');
		map->field[foods.second.i][foods.second.j] = '>';
	}
	return;
}

char snake_t::AImove()
{
	std::vector<std::vector<int>> numsField(map->HEIGTH, std::vector<int>(map->LENGTH));
	for (int i = 0; i < map->HEIGTH; i++)
	{
		for (int j = 0; j < map->LENGTH; j++)
		{
			if (map->field[i][j] != ' ' && map->field[i][j] != 'p') { numsField[i][j] = -1; }
			if (map->field[i][j] == 'p') { map->field[i][j] = ' '; }
		}
	}
	numsField[tail[0].i][tail[0].j] = 1;
	numsField[foods.first.i][foods.first.j] = -2;
	numsField[foods.second.i][foods.second.j] = -2;
	std::set<std::pair<int, int>> cells;
	cells.insert(std::make_pair(tail[0].i, tail[0].j));
	std::vector<std::pair<int, int>> way;
	if (RecAImove(numsField, cells, 1))
	{
		if (numsField[foods.first.i][foods.first.j] != -2)
		{
			way = searchWay(numsField, foods.first.i, foods.first.j);
		}
		if (numsField[foods.second.i][foods.second.j] != -2)
		{
			way = searchWay(numsField, foods.second.i, foods.second.j);
		}
		for (int i = 1; i < way.size() - 1; i++)
		{
			map->field[way[i].first][way[i].second] = 'p';
		}
	}
	else
	{
		int max = -1, maxI = 0, maxJ = 0;
		for (int i = 0; i < map->HEIGTH; i++)
		{
			for (int j = 0; j < map->LENGTH; j++)
			{
				if (numsField[i][j] > max)
				{
					max = numsField[i][j];
					maxI = i;
					maxJ = j;
				}
			}
		}
		way = searchWay(numsField, maxI, maxJ);
	}
	if (way.size() < 2) { return 'e'; }
	if ((way[way.size() - 1].second - way[way.size() - 2].second) == 1) { return 'w'; }
	if ((way[way.size() - 1].second - way[way.size() - 2].second) == -1) { return 's'; }
	if ((way[way.size() - 1].first - way[way.size() - 2].first) == 1) { return 'a'; }
	if ((way[way.size() - 1].first - way[way.size() - 2].first) == -1) { return 'd'; }
}

std::vector<std::pair<int, int>> snake_t::searchWay(std::vector<std::vector<int>> numsField, int startI, int startJ)
{
	std::vector<std::pair<int, int>> way;
	way.push_back(std::make_pair(startI, startJ));
	while (numsField[startI][startJ] != 1)
	{
		if (numsField[startI][startJ - 1] + 1 == numsField[startI][startJ]) { startJ--; }
		else if (numsField[startI][startJ + 1] + 1 == numsField[startI][startJ]) { startJ++; }
		else if (numsField[startI + 1][startJ] + 1 == numsField[startI][startJ]) { startI++; }
		else if (numsField[startI - 1][startJ] + 1 == numsField[startI][startJ]) { startI--; }
		way.push_back(std::make_pair(startI, startJ));
	}
	return way;
}

bool snake_t::RecAImove(std::vector<std::vector<int>>& numsField, std::set<std::pair<int, int>> cells, int tact)
{
	std::set<std::pair<int, int>> newCells;
	for (auto it : cells)
	{
		if (numsField[it.first][it.second] == -2)
		{
			numsField[it.first][it.second] = tact;
			return true;
		}
		numsField[it.first][it.second] = tact;
		if (numsField[it.first + 1][it.second] == 0 || numsField[it.first + 1][it.second] == -2) { newCells.insert(std::make_pair(it.first + 1, it.second)); }
		if (numsField[it.first - 1][it.second] == 0 || numsField[it.first - 1][it.second] == -2) { newCells.insert(std::make_pair(it.first - 1, it.second)); }
		if (numsField[it.first][it.second + 1] == 0 || numsField[it.first][it.second + 1] == -2) { newCells.insert(std::make_pair(it.first, it.second + 1)); }
		if (numsField[it.first][it.second - 1] == 0 || numsField[it.first][it.second - 1] == -2) { newCells.insert(std::make_pair(it.first, it.second - 1)); }
	}
	if (newCells.empty()) { return false; }
	return RecAImove(numsField, newCells, tact + 1);
}

snake_t::snake_t(maps* map)
{
	size = 4;
	srand(time(NULL));
	this->map = map;
	int i = (rand() % (this->map->HEIGTH - 2)) + 1;
	int j = (rand() % (this->map->LENGTH - 5)) + 1;
	tail.push_back(node(i, j, i, j));
	this->map->field[tail[0].i][tail[0].j] = 'h';
	for (int i = 1; i < size; i++)
	{
		tail.push_back(node(tail[0].i, tail[0].j + i, tail[0].i, tail[0].j + i));
		this->map->field[tail[i].i][tail[i].j] = 'b';
	}
	initFoods();
}

bool snake_t::move(char dir)
{
	if (dir == 'e') { return false; }
	tail[0].prevJ = tail[0].j;
	tail[0].prevI = tail[0].i;
	if (dir == 'w')
	{
		tail[0].i = tail[0].i;
		tail[0].j = tail[0].j - 1;
		
	}
	if (dir == 'a')
	{
		tail[0].i = tail[0].i - 1;
		tail[0].j = tail[0].j;
	}
	if (dir == 'd')
	{
		tail[0].i = tail[0].i + 1;
		tail[0].j = tail[0].j;
	}
	if (dir == 's')
	{
		tail[0].i = tail[0].i;
		tail[0].j = tail[0].j + 1;
	}
	if (map->field[tail[0].i][tail[0].j] == '#' || map->field[tail[0].i][tail[0].j] == 'b') { return false; }
	bool food = false;
	if (map->field[tail[0].i][tail[0].j] == '>' || map->field[tail[0].i][tail[0].j] == '<')
	{
		tail.push_back(node(tail[size - 1].prevI, tail[size - 1].prevJ, tail[size - 1].prevI, tail[size - 1].prevJ));
		size++;
		food = true;
	}
	map->field[tail[0].i][tail[0].j] = 'h';
	initFoods();
	for (int i = 1; i < size; i++)
	{
		tail[i].prevI = tail[i].i;
		tail[i].prevJ = tail[i].j;
		tail[i].i     = tail[i - 1].prevI;
		tail[i].j     = tail[i - 1].prevJ;
		map->field[tail[i].i][tail[i].j] = 'b';
	}
	if (food == false) { map->field[tail[size - 1].prevI][tail[size - 1].prevJ] = ' '; }
	return true;
}
