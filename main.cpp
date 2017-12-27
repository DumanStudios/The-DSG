#include <string>
#include <iostream>
#include <conio.h>
#include <Windows.h>
#undef  MOUSE_MOVED
#include <curses.h>
#include <ctime>
#include <cstdlib>
#include <string>
#include <duman.h>
#include "player.h"
#include "monster.h"
#include "system.h"
#include "mysql_connection.h"
#include <cppconn/driver.h>
#include <cppconn/exception.h>
#include <cppconn/prepared_statement.h>
#include <chrono>
#include <curl/curl.h>
using namespace std;

player p1(100, 100, "NULL");
string username, password, mail, time_setting;
int auto_save_time_interval_seconds = 20;
__int64 total = 0;
int total_kills = 0;
int total_max_hp = 0;
bool auto_save_enabled = false; // in settings give ability to set time. don't allow below 15 seconds. self note. do it.
bool first_save = true;
int times_you_died = 0;
bool auto_option = false;
bool auto_option_enabled = true;
char choice = '?';
char ch;

const auto sql_ip = "tcp://sql_ip:port";
const auto sql_username = "sql_username";
const auto sql_password = "sql_password";

string monster_name_array[50] = {
"Ivana Lessley",
"Federico Meese",
"Delana Hayner",
"Noella Schultheis",
"Raeann Winner",
"Santana Darley",
"Lashay Poorman",
"Ryan Womble",
"Tonya Maddocks",
"Jake Moffet",
"Somer Brazeau",
"Aretha Mullinax",
"Dwayne Avant",
"Olevia Birt",
"Elisabeth Gioia",
"Ivette Jimenes",
"Lacy Fraga",
"Vaughn Collingsworth",
"Rochelle Philhower",
"Dennise Fishel",
"Lesa Boer",
"Esteban Worrall",
"Sherman Teller",
"Stacee Banning",
"Katharyn Descoteaux",
"Elroy Litwin",
"Maura Kollar",
"Virgil Gettys",
"Laurence Frisbee",
"Rosanne Ney",
"Fred Rego",
"Shenita Watchman",
"Sandie Narcisse",
"Kali Fassett",
"Christen Points",
"Kari Lehrman",
"Evie Dancer",
"Zelma Bader",
"Codi Gingras",
"Kayleigh Gains",
"Rosette Renken",
"Deb Elston",
"Gretchen Corchado",
"Emmaline Waldeck",
"Vennie Perrigo",
"Kristy Goosby",
"Charleen Padilla",
"Anastasia Profitt",
"Bob Carls",
"Alona Basye"
};

void online_save();
string get_password(bool show_asterisk);

int main()
{
	//libcurl internet availablity check below
	// do it here
	//libcurl internet availablity check above
	
	// MYSQL STUFF BELOW
	sql::Driver *driver;
	sql::Connection *con;
	sql::Statement *stmt;
	sql::PreparedStatement *pstmt;
	sql::ResultSet *result;
	auto selection = '?';
	auto selection2 = '?';
	string char_selection;
	string execution_command;
	string selection3;
	
	try {
	driver = get_driver_instance();
	//for demonstration only. never save password in the code!
	con = driver->connect(sql_ip, sql_username, sql_password);

	con->setSchema("duman");
	stmt = con->createStatement();
	stmt->execute("CREATE TABLE IF NOT EXISTS `duman`.`members`(`id` int(11) NOT NULL AUTO_INCREMENT,`username` varchar(30) NOT NULL, `charName` varchar(50) NOT NULL, `hp` varchar(50) NOT NULL,`email` varchar(50) NOT NULL,`password` varchar(128) NOT NULL, `exp` varchar(50) NOT NULL, `level` varchar(50) NOT NULL, `boosts` varchar(50) NOT NULL, `autoHPItem` varchar(50) NOT NULL, `autoSaveTime` varchar(50) NOT NULL, `playTime` varchar(50) NOT NULL, `monstersKilled` varchar(50) NOT NULL, `autoSaveEnabled` varchar(50) NOT NULL, `maxHP` varchar(50) NOT NULL, `youDied` varchar(50) NOT NULL, `autoEnabled` varchar(50) NOT NULL, PRIMARY KEY(`id`), UNIQUE KEY `username` (`username`)) ENGINE = MYISAM DEFAULT CHARSET = utf8; ");
	
	selection5:
	cout << "Are you a registered user? (y/n): ";
	cin >> selection3;
	cin.ignore();
	if (selection3 != "y" && selection3 != "n")
	{
		cout << "Invalid selection!\n";
		cout << "Do you want to try again? (y/n): ";
		cin >> selection2;
		if (selection2 == 'y')
		{
			clear();
			refresh();
			cout << "\n";
			char_selection.clear();
			goto selection5;
		}
		cout << "To quit, press any key...\n";
		_getch();
		exit(1);
	}
	if (selection3 == "y")
	{
		//login here and load stuff from previous instance
		selection2:
		cout << "Username: ";
		cin >> username;
		cin.ignore();
		// checking if username exists or not as the first step
		execution_command = "SELECT username FROM members WHERE username='" + username + "'";
		pstmt = con->prepareStatement(execution_command.c_str());
		result = pstmt->executeQuery();
		if (result->next() == 0)
		{
			cout << "\nUser " << username << " doesn't exists in the database.\n";
			cout << "Do you want to try again? (y/n): ";
			cin >> selection;
			cin.ignore();
			if (selection == 'y')
			{
				clear();
				refresh();
				cout << "\n";
				goto selection2;
			}
			cout << "To quit, press any key...\n";
			_getch();
			exit(1);
		}
		// end of check
		selection3:
		cout << "Password: ";
		//new password masking trial
		password = get_password(true);
		//between here
		execution_command = "SELECT password FROM members WHERE username='"
			+ username + "'";
		pstmt = con->prepareStatement(execution_command.c_str());
		result = pstmt->executeQuery();
		while (result->next()) {
			const string databasepulledpw = result->getString("password").c_str();
			cout << "\n";
			if (databasepulledpw != password)
			{
				cout << "You have entered incorrect password for user " + username + "!\n";
				cout << "Do you want to try again? (y/n): ";
				cin >> selection;
				if (selection == 'y')
				{
					clear();
					refresh();
					cout << "\n";
					password.clear();
					goto selection3;
				}
				cout << "To quit, press any key...\n";
				_getch();
				exit(1);
			} // add an else if to check if username exists
			// LOAD ALL DATA TO THE ITEMS HERE
			// SINCE MYSQL IS IN STRING FORM CONVERT IT TO INT BACK
			// STORING IS NO PROBLEM I GOT IT ALREADY
			execution_command = "SELECT charName FROM members WHERE username='" + username + "'";
			pstmt = con->prepareStatement(execution_command.c_str());
			result = pstmt->executeQuery();
			while (result->next()) {
				p1.set_char_name(result->getString("charName").c_str());
			} // charName
			execution_command = "SELECT exp FROM members WHERE username='" + username + "'";
			pstmt = con->prepareStatement(execution_command.c_str());
			result = pstmt->executeQuery();
			while (result->next()) {
				p1.set_exp(result->getString("exp").c_str());
			} // exp
			execution_command = "SELECT boosts FROM members WHERE username='" + username + "'";
			pstmt = con->prepareStatement(execution_command.c_str());
			result = pstmt->executeQuery();
			while (result->next()) {
				p1.set_boosts(result->getString("boosts").c_str());
			} // boosts
			execution_command = "SELECT hp FROM members WHERE username='" + username + "'";
			pstmt = con->prepareStatement(execution_command.c_str());
			result = pstmt->executeQuery();
			while (result->next()) {
				p1.set_hp(result->getString("hp").c_str());
				if (p1.return_hp() <= 0)
				{
					p1.set_hp("1");
					break;
				}
			} // hp
			execution_command = "SELECT autoSaveEnabled FROM members WHERE username='" + username + "'";
			pstmt = con->prepareStatement(execution_command.c_str());
			result = pstmt->executeQuery();
			while (result->next()) {
				auto_save_enabled = stoi(result->getString("autoSaveEnabled").c_str());
			} // autoSaveEnabled
			execution_command = "SELECT autoSaveTime FROM members WHERE username='" + username + "'";
			pstmt = con->prepareStatement(execution_command.c_str());
			result = pstmt->executeQuery();
			while (result->next()) {
				auto_save_time_interval_seconds = stoi(result->getString("autoSaveTime").c_str());
			} // autoSaveTimeInterval
			execution_command = "SELECT playTime FROM members WHERE username='" + username + "'";
			pstmt = con->prepareStatement(execution_command.c_str());
			result = pstmt->executeQuery();
			while (result->next()) {
				total = stoi(result->getString("playTime").c_str());
			} // playTime
			execution_command = "SELECT monstersKilled FROM members WHERE username='" + username + "'";
			pstmt = con->prepareStatement(execution_command.c_str());
			result = pstmt->executeQuery();
			while (result->next()) {
				total_kills = stoi(result->getString("monstersKilled").c_str());
			} // monstersKilled
			execution_command = "SELECT maxHP FROM members WHERE username='" + username + "'";
			pstmt = con->prepareStatement(execution_command.c_str());
			result = pstmt->executeQuery();
			while (result->next()) {
				p1.set_max_hp(result->getString("maxHP").c_str());
			} // maxHP
			execution_command = "SELECT youDied FROM members WHERE username='" + username + "'";
			pstmt = con->prepareStatement(execution_command.c_str());
			result = pstmt->executeQuery();
			while (result->next()) {
				times_you_died = stoi(result->getString("youDied").c_str());
			} // you died
			execution_command = "SELECT autoHPItem FROM members WHERE username='" + username + "'";
			pstmt = con->prepareStatement(execution_command.c_str());
			result = pstmt->executeQuery();
			while (result->next()) {
				p1.set_autohpitem(result->getString("autoHPItem").c_str());
			} // autoHPItem
			execution_command = "SELECT autoEnabled FROM members WHERE username='" + username + "'";
			pstmt = con->prepareStatement(execution_command.c_str());
			result = pstmt->executeQuery();
			while (result->next()) {
				p1.set_auto_enabled(stoi(result->getString("autoEnabled").c_str()));
			} // autoEnabled
			delete stmt;
			delete con;
			delete pstmt;
		}
	}
	if (selection3 == "n")
	{
		selection6:
		cout << "Enter your username: ";
		cin >> username;
		cin.ignore();
		// check if user exists to prevent highjacking
		execution_command = "SELECT username FROM members WHERE username='" + username + "'";
		pstmt = con->prepareStatement(execution_command.c_str());
		result = pstmt->executeQuery();
		if (result->next() != 0) {
			cout << "User " << username << " is already registered! Try another username!\n";
			goto selection6;
		} // autoHPItem
		//
		cout << "Enter your mail address: ";
		cin >> mail;
		cin.ignore();
		cout << "Enter your password: ";
		cin >> password;
		cin.ignore();
		execution_command = "INSERT INTO `members`(`username`, `email`, `password`) VALUES ('"
						 + username + "','" + mail + "','" + password + "')";
		stmt->execute(execution_command.c_str());
		delete pstmt;
		delete con;
		delete stmt;
	}
	}
	catch (sql::SQLException &e) {
		if(e.getErrorCode() == 2003)
		{
			cout << "Secure server connection to DumanSTUDIOS.com is failed!\n";
			key_to_exit(1000, 'q', 0, true);
		}
		cout << "# ERR: SQLException in " << __FILE__;
		cout << "(" << __FUNCTION__ << ") on line " << __LINE__ << endl;
		cout << "# ERR: " << e.what();
		cout << " (MySQL error code: " << e.getErrorCode();
		cout << ", SQLState: " << e.getSQLState() << " )" << endl;
		cout << "Press 'q' to quit.\n";
		key_to_exit(1000, 'q', 0, true);
	}
	// MYSQL STUFF ABOVE
	// MY STUFF BELOW
	p1.set_char_name(username);
	p1.set_monsters_killed(total_kills);
	srand(static_cast<unsigned int>(time(nullptr)));
	auto random_access = rand() % 49 + 1;
	auto exit = false;
	systemclass sys;
	monster m1(100, 230, monster_name_array[random_access]);
	auto key = 'z';
	// Constructs the new thread and runs it. Does not block execution.
	p1.set_death_count(times_you_died);
	if (p1.return_specitem() == 1)
	{
		auto_option = true;
	}
	initscr();
	while (!exit)
	{
		const auto start = chrono::system_clock::now();
		if (_kbhit())
			key = _getch();
		
		switch (key)
		{
		case 27:
			exit = true;
			online_save();
			cout << "\nSynced all the variables and settings with the server!\n";
			cout << "Exiting!\n";
			Sleep(2500);
			break;
		case 'a':
		case 'A':
			random_access = rand() % 49 + 1;
			m1.seed(p1, monster_name_array[random_access]);
		REATTK:
			p1.initiate_attack(p1,m1);
			clear();
			Sleep(800);
			refresh();
			systemclass::battle_information(p1,m1);
			if (p1.return_hp() <= 0)
			{
				exit = true;
				times_you_died++;
				online_save();
				cout << "\nREST IN PEACE [*]\n\nWhile you were in a battle with " << m1.return_name() << ", you died.\n";
				cout << "Press any key to close the program...\n";		
				_getch();
				break;
			}
			if (m1.return_hp() > 0)
				goto REATTK;
			total_kills++;
			p1.set_monsters_killed(total_kills);
			p1.update_player_exp(m1);
			online_save();
			break;
		case 'h':
		case 'H':
			p1.heal(p1.return_h_pregen());
			break;
		case 's':
		case 'S':
			online_save();
			cout << "\nCloud save complete!\n";
			Sleep(750);
			break;
		case 'o':
		case 'O':
			cout << "OPTIONS:\n";
			cout << "1. Enable/Disable auto save";
			cout << " (" << boolalpha << auto_save_enabled << noboolalpha << ")\n";
			cout << "2. Set auto save time interval";
			cout << " (" << auto_save_time_interval_seconds << " seconds)\n";
			if (auto_option)
			{
				cout << "3. Enable/Disable auto hp item";
				cout << " (" << boolalpha << p1.return_auto_enabled() << noboolalpha << ")\n";
			}
			choice = _getch();

			switch (choice)
			{
			case '1':
				if (!auto_save_enabled)
				{
					auto_save_enabled = true;
					cout << "Auto save is now enabled!\n";
					driver = get_driver_instance();
					con = driver->connect(sql_ip, sql_username, sql_password);

					con->setSchema("duman");
					stmt = con->createStatement();
					execution_command = "UPDATE `members` SET `autoSaveEnabled`='" + to_string(auto_save_enabled) + "' WHERE `username`='" + username + "';";
					stmt->execute(execution_command.c_str());
					cout << "\nYour settings are synced online.\n";
					delete con;
					delete stmt;
					Sleep(1500);
					break;
				}
				if (auto_save_enabled)
				{
					auto_save_enabled = false;
					cout << "Auto save is now disabled!\n";
					driver = get_driver_instance();
					con = driver->connect(sql_ip, sql_username, sql_password);

					con->setSchema("duman");
					stmt = con->createStatement();
					execution_command = "UPDATE `members` SET `autoSaveEnabled`='" + to_string(auto_save_enabled) + "' WHERE `username`='" + username + "';";
					stmt->execute(execution_command.c_str());
					cout << "\nYour settings are synced online.\n";
					delete con;
					delete stmt;
					Sleep(1500);
					break;
				}
			case '2':
				selection4:
				cout << "Per XX seconds sync with the servers: ";
				time_setting = get_password(false);
				auto_save_time_interval_seconds = stoi(time_setting);
				
				if (auto_save_time_interval_seconds < 15)
				{
					cout << "\n\nTime interval cannot be smaller than 15 seconds!\n";
					cout << "Provide a time interval which is greater than 15 seconds.\n";
					auto_save_time_interval_seconds = 20;
					Sleep(1500);
					time_setting.clear();
					goto selection4;
				}
				driver = get_driver_instance();
				con = driver->connect(sql_ip, sql_username, sql_password);

				con->setSchema("duman");
				stmt = con->createStatement();
				execution_command = "UPDATE `members` SET `autoSaveTime`='" + to_string(auto_save_time_interval_seconds) + "' WHERE `username`='" + username + "';";
				stmt->execute(execution_command.c_str());

				cout << "\n\nTime interval to auto save is now " << auto_save_time_interval_seconds << " seconds!\n";
				cout << "Your settings are synced online.\n";
				delete stmt;
				delete con;
				time_setting.clear();
				Sleep(1500);
				break;
			case '3':
				{
					if (p1.return_specitem() == 0)
					{
						cout << "\nYou don't have access to this menu yet!\n";
						Sleep(1500);
						break;
					}
					if (p1.return_auto_enabled())
					{
						p1.set_auto_enabled(false);
						cout << "Auto healing item is disabled!\n";
						online_save();
						Sleep(1500);
						break;
					}
					if (!p1.return_auto_enabled())
					{
						p1.set_auto_enabled(true);
						cout << "Auto healing item is enabled!\n";
						online_save();
						Sleep(1500);
						break;
					}
				}
			case 27:
			case 'o':
			case 'O':
				break;
			default:
				cout << "Invalid choice.\n";
				Sleep(1500);
			}
			break;
		case 'b':
		case 'B':
			p1.heal_boost();
			break;
		case 't':
		case 'T':
			cout << "This is a debug text.\n";
			Sleep(1000); // change this to make the text remain longer for att later
			break;
		default:
			break;
		} key = 'z';

		clear();
		Sleep(75);
		refresh();
		p1.should_level_up(p1);
		p1.information();
		if (p1.return_specitem() == 1 && p1.return_auto_enabled())
		{
			p1.heal(p1.return_h_pregen());
		}
		const auto end = chrono::system_clock::now();
		auto elapsed = std::chrono::duration_cast<chrono::milliseconds>(end - start);
		total += elapsed.count();
		if (total > (auto_save_time_interval_seconds * 1000) &&
			total % (auto_save_time_interval_seconds * 1000) < 100 &&
			total % (auto_save_time_interval_seconds * 1000) > 1 &&
			auto_save_enabled)
		{
			online_save();
		}
		if (first_save)
		{
			online_save();
			first_save = false;
		}
	}
	endwin();
}

void online_save()
{
	auto driver = get_driver_instance();
	auto con = driver->connect(sql_ip, sql_username, sql_password);

	con->setSchema("duman");
	auto stmt = con->createStatement();
	auto execution_command = "UPDATE `members` SET `charName`='" + p1.return_name() + "' WHERE `username`='" + username + "';";
	stmt->execute(execution_command.c_str());
	execution_command = "UPDATE `members` SET `hp`='" + to_string(p1.return_hp()) + "' WHERE `username`='" + username + "';";
	stmt->execute(execution_command.c_str());
	execution_command = "UPDATE `members` SET `exp`='" + to_string(p1.return_exp()) + "' WHERE `username`='" + username + "';";
	stmt->execute(execution_command.c_str());
	execution_command = "UPDATE `members` SET `level`='" + to_string(p1.return_level()) + "' WHERE `username`='" + username + "';";
	stmt->execute(execution_command.c_str());
	execution_command = "UPDATE `members` SET `boosts`='" + to_string(p1.return_boosts()) + "' WHERE `username`='" + username + "';";
	stmt->execute(execution_command.c_str());
	execution_command = "UPDATE `members` SET `autoHPItem`='" + to_string(p1.return_specitem()) + "' WHERE `username`='" + username + "';";
	stmt->execute(execution_command.c_str());
	execution_command = "UPDATE `members` SET `playTime`='" + to_string(total) + "' WHERE `username`='" + username + "';";
	stmt->execute(execution_command.c_str());
	execution_command = "UPDATE `members` SET `autoSaveEnabled`='" + to_string(auto_save_enabled) + "' WHERE `username`='" + username + "';";
	stmt->execute(execution_command.c_str());
	execution_command = "UPDATE `members` SET `autoSaveTime`='" + to_string(auto_save_time_interval_seconds) + "' WHERE `username`='" + username + "';";
	stmt->execute(execution_command.c_str());
	execution_command = "UPDATE `members` SET `monstersKilled`='" + to_string(total_kills) + "' WHERE `username`='" + username + "';";
	stmt->execute(execution_command.c_str());
	execution_command = "UPDATE `members` SET `maxHP`='" + to_string(p1.return_max_hp()) + "' WHERE `username`='" + username + "';";
	stmt->execute(execution_command.c_str());
	execution_command = "UPDATE `members` SET `youDied`='" + to_string(times_you_died) + "' WHERE `username`='" + username + "';";
	stmt->execute(execution_command.c_str());
	execution_command = "UPDATE `members` SET `autoHPItem`='" + to_string(p1.return_specitem()) + "' WHERE `username`='" + username + "';";
	stmt->execute(execution_command.c_str());
	execution_command = "UPDATE `members` SET `autoEnabled`='" + to_string(p1.return_auto_enabled()) + "' WHERE `username`='" + username + "';";
	stmt->execute(execution_command.c_str());
	delete stmt;
	delete con;
}

string get_password(const bool show_asterisk = true)
{
	const char backspace_key = 8;
	const char enter_key = 13;

	string password;
	unsigned char ch = 0;

	DWORD con_mode;
	DWORD dw_read;

	const auto h_in = GetStdHandle(STD_INPUT_HANDLE);

	GetConsoleMode(h_in, &con_mode);
	SetConsoleMode(h_in, con_mode & ~(ENABLE_ECHO_INPUT | ENABLE_LINE_INPUT));

	while (ReadConsoleA(h_in, &ch, 1, &dw_read, nullptr) && ch != enter_key)
	{
		if (ch == backspace_key)
		{
			if (password.length() != 0)
			{
				cout << "\b \b";
				password.resize(password.length() - 1);
			}
		}
		else
		{
			password += ch;
			if (show_asterisk)
				cout << '*';
			else
				cout << ch;
		}
	}
	cout << endl;
	return password;
}