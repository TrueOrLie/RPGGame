#include "stdafx.h"
#include <string>
#include <iostream>
#include <fstream>
#include <thread>
#include <mutex>
#include <Windows.h>
#include <conio.h>
#include <random>
#include <ctime>
#include <chrono>

#define WIDTH 3
#define HEIGHT 10
using namespace std;
int SelectCheck;
ofstream out;
ifstream in;

struct Status
{
public:
	string ID;
	string name;
	int Level;
	int MaxExp;
	int Exp;
	int MaxHp;
	int Hp;
	int Str;
	int Dex;
	int Int;
	int Luk;
	int MinAtk;
	int MaxAtk;
	int Def;
};
class player {
public:
	Status playerstatus;
	void inputdata() {
		in.open("player.txt");
		if (!in.is_open()) {
			cout << "플레이어의 정보를 받을수 없습니다." << endl;
			in.open("player.txt");
		}
		if (in.good()) {
			in >> playerstatus.ID >> playerstatus.name >> playerstatus.Level >> playerstatus.MaxExp >> playerstatus.Exp >> playerstatus.MaxHp >>
				playerstatus.Hp >> playerstatus.Str >> playerstatus.Dex >> playerstatus.Int >> playerstatus.Luk >> playerstatus.MinAtk >> playerstatus.MaxAtk >> playerstatus.Def;
			in.close();
		}

	}
	void outputdata() {
		in.open("player.txt");
		in.ignore();
		in.close();
		out.open("player.txt");
		if (!out.is_open()) {
			out.open("player.txt");
		}
		if (in.good()) {
			out << playerstatus.ID << endl << playerstatus.name << endl << playerstatus.Level << endl << playerstatus.MaxExp << endl << playerstatus.Exp << endl << playerstatus.MaxHp << endl <<
				playerstatus.Hp << endl << playerstatus.Str << endl << playerstatus.Dex << endl << playerstatus.Int << endl << playerstatus.Luk << endl << playerstatus.MinAtk << endl << playerstatus.MaxAtk << endl << playerstatus.Def;
			out.close();
		}
	}
};
int getRandomNumber(int min, int max) {
	random_device rn;
	mt19937_64 rnd(rn());

	uniform_int_distribution<int> range(min, max);

	return range(rnd);
}
void StatusOutput(Status player) {
	cout << "이름 : " << player.name << endl;
	cout << "Level : " << player.Level << endl;
	cout << "Exp : " << player.Exp << " / " << player.MaxExp << endl;
	cout << "HP : " << player.Hp << " / " << player.MaxHp << endl;
	cout << "공격력 : " << player.MinAtk << " ~ " << player.MaxAtk << endl;
	cout << "방어력 : " << player.Def << endl << endl;
}
void Battle(Status &player, Status monster) {
	std::mutex mtx_lock;
	thread t1([&] {
		chrono::system_clock::time_point pretime = chrono::system_clock::now();
		while (true)
		{
			if (monster.Hp <= 0) {
				cout << "몬스터가 디졌습니다" << endl << endl;
				player.Exp += monster.Exp;
				if (player.Exp >= player.MaxExp) {
					cout << "레벨 업!" << endl << endl;
					player.Level += 1;
					player.MaxHp += 100;
					player.Exp = player.Exp - player.MaxExp;
					player.Hp = player.MaxHp;
					player.Str += 1;
					player.Dex += 1;
					player.Luk += 1;
					player.Int += 1;
				}
				break;
			}
			if (player.Hp <= 0) {
				break;
			}
			mtx_lock.lock();
			if (chrono::system_clock::now() - pretime >= chrono::milliseconds(1000)) {
				pretime = chrono::system_clock::now();

				int playeratk = getRandomNumber(player.MinAtk, player.MaxAtk);
				monster.Hp -= playeratk;

				cout << player.name << " : " << endl;
				cout << playeratk << "만큼의 데미지를 " << monster.name << "에게 주셨습니다." << endl;
				cout << monster.name << "의 체력이 " << monster.Hp << " 남았습니다." << endl << endl << endl;
			}
			mtx_lock.unlock();
			Sleep(1);
		}
	});

	thread t2([&] {
		chrono::system_clock::time_point pretime = chrono::system_clock::now();
		while (true)
		{
			if (player.Hp <= 0) {
				cout << "플레이어가 디졌습니다" << endl << endl;
				player.Hp = player.MaxHp;
				break;
			}
			if (monster.Hp <= 0) {
				break;
			}
			mtx_lock.lock();
			if (chrono::system_clock::now() - pretime >= chrono::milliseconds(1800)) {
				pretime = chrono::system_clock::now();

				int monsteratk = getRandomNumber(monster.MinAtk, monster.MaxAtk);
				player.Hp -= monsteratk;
				cout << monster.name << " : " << endl;
				cout << monsteratk << "만큼의 데미지를 맞으셨습니다." << endl;
				cout << player.name << "의 체력이 " << player.Hp << " 남았습니다." << endl << endl << endl;
			}
			mtx_lock.unlock();
			Sleep(1);
		}
	});
	t1.join();
	t2.join();
}

void map(Status &player, vector<Status> monster) {

	chrono::system_clock::time_point pretime = chrono::system_clock::now();
	int map[WIDTH][HEIGHT] =
	{
		{ 9,0,0,0,0,0,0,0,0,9 },
	{ 9,1,0,0,0,0,0,0,0,9 },
	{ 9,9,9,9,9,9,9,9,9,9 },
	};
	int pos[2];
	pos[0] = 1;
	pos[1] = 8;
	while (true)
	{
		if (chrono::system_clock::now() - pretime >= chrono::milliseconds(500)) {
			pretime = chrono::system_clock::now();
			system("cls");
			map[pos[0]][pos[1]] = 0;
			map[pos[0]][pos[1] - 1] = 2;
			for (int x = 0; x < WIDTH; x++) {
				cout << endl;
				for (int y = 0; y < HEIGHT; y++) {

					if (map[x][y] == 0)
						cout << "  ";
					else if (map[x][y] == 1) {
						cout << "☆";
					}
					else if (map[x][y] == 2) {
						pos[1] = y;
						cout << "★";
					}
					else if (map[x][y] == 9)
						cout << "□";
				}
			}
			if (map[pos[0]][pos[1] - 1] == 1 && map[pos[0]][pos[1]] == 2) {

				Status MonsterSelect = monster[getRandomNumber(0, monster.size() - 1)];
				cout << endl << endl << MonsterSelect.name << "과 대치하였습니다." << endl << endl;
				cout << "도망가시겠습니까? 싸운다[1] 도망간다[2]" << endl;
				cin >> SelectCheck;
				if (SelectCheck == 1) {
					Battle(player, MonsterSelect);
				}
				break;
			}
		}
		Sleep(1);
	}
}
void heal(Status &player) {
	chrono::system_clock::time_point time = chrono::system_clock::now();
	chrono::system_clock::time_point healtime = chrono::system_clock::now();
	while (true)
	{
		if (chrono::system_clock::now() - healtime >= chrono::milliseconds(400)) {
			healtime = chrono::system_clock::now();
			player.Hp += 10;
			cout << player.Hp << endl;
		}

		if (player.Hp >= player.MaxHp) {
			cout << "체력이 모두 회복되었습니다. 마을로 이동합니다." << endl;
			player.Hp = player.MaxHp;
			break;
		}

		Sleep(1);
	}
}
int main()
{
	player player;

	in.open("player.txt");
	if (!in.is_open()) {
		string name;
		out.open("player.txt");
		out << "TrueOrLie" << endl;
		cout << "최초생성입니다. 이름을 입력해주세요." << endl;
		cin >> name;
		out << name << endl << 1 << endl << 100 << endl << 0 << endl << 200 << endl << 200 << endl << 10 << endl << 10 << endl << 10 << endl << 10 << endl << 20 << endl << 50 << endl << 5;
		cout << "생성이 완료되었습니다." << endl;
		out.close();
	}
	in.close();
	player.inputdata();

	Status slime;
	Status satan;
	Status orc;
	Status troll;
	slime.ID = "A001";
	slime.name = "슬라임";
	slime.Hp = 200;
	slime.MinAtk = 5;
	slime.MaxAtk = 12;
	slime.Def = 3;
	slime.Exp = 50;

	satan.ID = "A002";
	satan.name = "사탄";
	satan.Hp = 200;
	satan.MinAtk = 10;
	satan.MaxAtk = 20;
	satan.Def = 5;
	satan.Exp = 70;

	orc.ID = "A003";
	orc.name = "오크";
	orc.Hp = 500;
	orc.MinAtk = 20;
	orc.MaxAtk = 30;
	orc.Def = 5;
	orc.Exp = 120;

	troll.ID = "A004";
	troll.name = "트롤";
	troll.Hp = 230;
	troll.MinAtk = 15;
	troll.MaxAtk = 18;
	troll.Def = 10;
	troll.Exp = 90;
	vector<Status> Monster = { slime, satan, orc, troll };

	while (true)
	{
		StatusOutput(player.playerstatus);
		cout << "무엇을 하시겠습니까. 사냥[1] 휴식(체력회복)[2]" << endl;
		cin >> SelectCheck;
		switch (SelectCheck)
		{
		case 1:
			map(player.playerstatus, Monster);
			player.outputdata();
			break;
		case 2:
			heal(player.playerstatus);
			break;
		}

	}
	return 0;
}



