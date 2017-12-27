#ifndef PLAYER_H
#define PLAYER_H
#include <string>
class monster;
#include "monster.h"
using namespace std;

class player
{
	string name_;
	bool alive_ = true;
	bool auto_hp_enabled_ = false;
	int hpregen_ = 2; //hpregen
	int hp_; //hp
	int boosts_ = 5; //hppot
	int dmg_;
	int lvl_ = 1;
	int exp_ = 0;
	int auto_hp_item_ = 0;
	int monsters_killed_ = 0;
	int max_hp_;
	int death_count_;
	int killing_spree_;
public:
	player(const int a, const int b, const string nm): dmg_(0), death_count_(0), killing_spree_(0)
	{
		hp_ = a;
		max_hp_ = b;
		name_ = nm;
	}

	void heal(int);
	int return_hp() const;
	int return_h_pregen() const;
	void heal_boost();
	void information() const;
	void set_dmg();
	int return_dmg() const;
	int return_exp() const;
	int return_level() const;
	int return_boosts() const;
	int return_specitem() const;
	int return_monsters_killed() const;
	int return_max_hp() const;
	int return_death_count() const;
	bool return_auto_enabled() const;
	string return_name() const;
	void set_death_count(int);
	void initiate_attack(player&, monster&);
	void update_battle_hp(monster&);
	void update_player_exp(monster&);
	static void death(monster&);
	void should_level_up(player&);
	void set_char_name(string);
	void set_exp(string);
	void set_boosts(string);
	void set_hp(string);
	void set_autohpitem(string);
	void set_level(string);
	void set_specitem(string);
	void set_monsters_killed(int);
	void set_max_hp(string);
	void set_auto_enabled(bool);
};
#endif
