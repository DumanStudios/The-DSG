#include "player.h"
#include "monster.h"
#include <string>
#include <Windows.h>
#include <iostream>
using namespace std;

void player::update_battle_hp(monster& monster)
{
	hp_ -= monster.return_dmg();
	cout << "SYSTEM: Damage given by monster to player is: " << monster.return_dmg() << "\n";
}

void player::death(monster& monster)
{
	cout << "\nREST IN PEACE [*]\n\nWhile you were in a battle with " << monster.return_name() << ", you died.\n";
	// add some kind of save system here // hi, I'm you from future. You were right.
}

void player::set_exp(const string& exp)
{
	exp_ = stoi(exp);
}

void player::set_hp(const string& hp)
{
	hp_ = stoi(hp);
}

void player::set_boosts(const string& boosts)
{
	boosts_ = stoi(boosts);
}

void player::set_autohpitem(const string& autohpitem)
{
	auto_hp_item_ = stoi(autohpitem);
}

int player::return_exp() const
{
	return exp_;
}

int player::return_max_hp() const
{
	return max_hp_;
}

int player::return_level() const
{
	return lvl_;
}

int player::return_boosts() const
{
	return boosts_;
}

int player::return_specitem() const
{
	return auto_hp_item_;
}

int player::return_death_count() const
{
	return death_count_;
}

void player::set_death_count(const int deaths)
{
	death_count_ = deaths;
}

void player::set_char_name(const string& name)
{
	name_ = name;
}

void player::set_level(const string& level)
{
	lvl_ = stoi(level);
}

void player::set_max_hp(const string& maxhp)
{
	max_hp_ = stoi(maxhp);
}

void player::should_level_up(player& player)
{
	if (player.exp_ >= 200 && player.exp_ <= 350) {
		lvl_ = 2;
		player.max_hp_ = 120;
	}
	if (player.exp_ >= 351 && player.exp_ <= 750) {
		lvl_ = 3;
		player.max_hp_ = 140;
	}
	if (player.exp_ >= 751 && player.exp_ <= 1500) {
		lvl_ = 4;
		player.max_hp_ = 160;
	}
	if (player.exp_ >= 1501 && player.exp_ <= 4000) {
		lvl_ = 5;
		player.max_hp_ = 180;
	}
	if (player.exp_ >= 4001 && player.exp_ <= 9000) {
		lvl_ = 6;
		player.max_hp_ = 200;
	}
	if (player.exp_ >= 9001 && player.exp_ <= 20000) {
		lvl_ = 7;
		player.max_hp_ = 220;
	}
	if (player.exp_ >= 20001 && player.exp_ <= 50000) {
		lvl_ = 8;
		player.max_hp_ = 240;
	}
	if (player.exp_ >= 50001 && player.exp_ <= 120000) {
		lvl_ = 9;
		player.max_hp_ = 260;
	}
	if (player.exp_ >= 120001 && player.exp_ <= 250000) {
		lvl_ = 10;
		player.max_hp_ = 280;
	}
	if (player.exp_ >= 250001 && player.exp_ <= 1000000) {
		lvl_ = 11;
		player.max_hp_ = 300;
	}
	if (player.exp_ >= 1000001 && player.exp_ <= 10000000) {
		lvl_ = 12;
		player.max_hp_ = 320;
	}
}

void player::set_specitem(const string& item)
{
	auto_hp_item_ = stoi(item);
}

bool player::return_auto_enabled() const
{
	return auto_hp_enabled_;
}

void player::set_auto_enabled(const bool state)
{
	auto_hp_enabled_ = state;
}

void player::initiate_attack(player& player, monster& monster)
{
	const auto user_selection = rand() % 100 + 1;
	const auto syst_selection = rand() % 100 + 1;
	player.set_dmg();
	monster.set_dmg(player);
	monster.update_battle_hp(player);
	if (player.hp_ <= 0)
	{
		alive_ = false;
		death(monster);
	}
	player.update_battle_hp(monster);
	if (monster.return_hp() <= 0)
	{
		if (user_selection == syst_selection && return_specitem() == 0)
		{
			player.set_specitem("1");
			cout << "You got an item: AUTO HP FILLER\n";
			Sleep(1500);
		}
		monsters_killed_++;
	}
}

void player::update_player_exp(monster& monster)
{
	exp_ += monster.return_exp();
}

void player::set_dmg()
{
	auto temp = rand() % ((lvl_ * 10) - (lvl_ - 1) * 10) + ((lvl_ - 1) * 10); // do some better algorithm here
	if (temp == 0)
		temp = 1;
	dmg_ = temp;
}

int player::return_dmg() const
{
	return dmg_;
}

string player::return_name() const
{
	return name_;
}

int player::return_h_pregen() const
{
	return hpregen_;
}

void player::information() const
{
	cout << "Health points   : " << hp_ << "/" << max_hp_ << "\n";
	cout << "Remaining boosts: " << boosts_ << "\n";
	cout << "Current exp     : " << exp_ << "\n";
	cout << "Current level   : " << lvl_ << "\n"; // in the next version also show how much more left, you still didn't do it
	cout << "Monsters killed : " << monsters_killed_ << "\n";
	cout << "Times you died  : " << death_count_ << "\n";
	//cout << "Killing spree   : " << killing_spree_ << "\n"; //add a dynamic system for this. as the spree goes higher make rewards
	// better and also make monsters harder, but increase it little by little don't go full nuts on it. okay? you hear me? thanks.
	cout << "-------------------------\n"; // for the next level
}

void player::set_monsters_killed(const int monsters)
{
	monsters_killed_ = monsters;
}

int player::return_monsters_killed() const
{
	return monsters_killed_;
}

void player::heal_boost()
{
	if (boosts_ <= 0)
	{
		boosts_ = 0;
		return;
	}
	hp_ += 50;
	boosts_ -= 1;
}

int player::return_hp() const
{
	return hp_;
}

void player::heal(const int update)
{
	if (hp_ >= max_hp_)
	{
		hp_ = max_hp_;
		return;
	}
	hp_ += (update*lvl_/2);
}