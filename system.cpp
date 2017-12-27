#include "system.h"
#include <iostream>

void systemclass::battle_information(player& player, monster& monster)
{
	cout << player.return_name(); cout << "\t: " << player.return_hp() << "/" << player.return_max_hp() << "\n"; // these 2 should show
	cout << monster.return_name(); cout << "\t: " << monster.return_hp() << "/" << monster.return_max_hp() << "\n"; //  max hp instead of 100
	cout << "-------------------------\n";
}