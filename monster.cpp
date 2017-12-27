#include "monster.h"
#include "player.h"
#include <iostream>

void monster::update_battle_hp(player& player)
{
	hp_ -= player.return_dmg();
	cout << "SYSTEM: Damage given by player to monster is: " << player.return_dmg() << "\n";
}

void monster::set_dmg(player& pl)
{
	const auto temp = rand() % ((pl.return_level() * 10) - (pl.return_level() - 1) * 10) + ((pl.return_level() - 1) * 10);
	if (pl.return_death_count() >= 0 && pl.return_death_count() < 14)
		dmg_ = temp*0.75;
	if (pl.return_death_count() >= 14 && pl.return_death_count() < 20)
		dmg_ = temp*0.66;
	if (pl.return_death_count() >= 20 && pl.return_death_count() < 27)
		dmg_ = temp*0.42;
	if (pl.return_death_count() >= 27)
		dmg_ = temp*0.30;
}

void monster::set_max_hp(const int maxhp)
{
	max_hp_ = maxhp;
}

int monster::return_max_hp() const
{
	return max_hp_;
}

int monster::return_h_pregen() const
{
	return hpregen_;
}

int monster::return_exp() const
{
	return exp_;
}

void monster::seed(player& player, const string& name)
{
	name_ = name;
	hp_ = rand() % ((player.return_level() * 100) - (player.return_level()) * 90) + ((player.return_level()) * 90);
	exp_ = player.return_level() * 100;
	set_dmg(player);
	set_max_hp(hp_);
}

void monster::information() const
{
	cout << "Points          : " << hp_ << "/100\n";
	cout << "Total dmg       : " << dmg_ << "\n";
	cout << "-------------------------\n";
}

int monster::return_hp() const
{
	return hp_;
}

int monster::return_dmg() const
{
	return dmg_;
}

string monster::return_name() const
{
	return name_;
}

void monster::heal(const int update)
{
	if (hp_ >= 100)
	{
		hp_ = 100;
		return;
	}
	hp_ += update;
}