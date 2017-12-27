#ifndef SYSTEM_H
#define SYSTEM_H
#include "monster.h"
#include "player.h"
#include <string>
using namespace std;

class systemclass
{
	string sysmsg_;
public:
	static void battle_information(player&, monster&);
};
#endif