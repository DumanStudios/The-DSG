#ifndef MONSTER_H
#define MONSTER_H
#include <string>
class player;
#include "player.h"
using namespace std;

class monster
{
	string name_;
	int max_hp_;
	int hpregen_ = 2; //hpregen
	int hp_; //hp
	int dmg_;
	int exp_ = 0;
public:
	monster(const int health, const int experience, const string& nm): max_hp_(0), dmg_(0)
	{
		hp_ = health;
		exp_ = experience;
		name_ = nm;
	}

	void heal(int);
	int return_hp() const;
	int return_h_pregen() const;
	void information() const;
	void set_dmg(player&);
	int return_dmg() const;
	int return_exp() const;
	int return_max_hp() const;
	string return_name() const;
	void seed(player&, const string&);
	void update_battle_hp(player&);
	void set_max_hp(int);
};
#endif