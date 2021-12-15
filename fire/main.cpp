#include<bits/stdc++.h>
#include <conio.h>
#include<graphics.h>
#pragma comment(lib,"Winmm.lib")
using namespace std;

const int map_x = 500;
const int map_y = 700;
const int unit_len = 32;
const int tank_len = unit_len * 2;
const int bullet_len = unit_len / 2;
const int edge = 10;
IMAGE img[2][4];
IMAGE img_hide[2][4];
IMAGE background;
//int Map[map_x][map_y];
typedef struct {
	int x, y;
}P_XY;

enum class Direction {
	UP=0, DOWN,LEFT,RIGHT,NO
};
/*强枚举类型
强作用域：强类型枚举成员的名称不会被输出到其父作用域空间。
转换限制: 强类型枚举成员的值不可以与整型隐式的互相转换。*/

void inimg() {
	loadimage(&background, _T("others\\background.png"));//_T:若使用unicode字符集则自动加L
	loadimage(&img[0][0], _T("tank1\\player1_0_up1.png"));//L:将ANSI字符串转换成unicode的字符串
	loadimage(&img[0][1], _T("tank1\\player1_0_down1.png"));
	loadimage(&img[0][2], _T("tank1\\player1_0_left1.png"));
	loadimage(&img[0][3], _T("tank1\\player1_0_right1.png"));
	loadimage(&img[1][0], _T("bullet\\bullet_up.png"));
	loadimage(&img[1][1], _T("bullet\\bullet_down.png"));
	loadimage(&img[1][2], _T("bullet\\bullet_left.png"));
	loadimage(&img[1][3], _T("bullet\\bullet_right.png"));
	loadimage(&img_hide[0][0], _T("tank1\\player_hide0_up.png"));
	loadimage(&img_hide[0][1], _T("tank1\\player_hide0_down.png"));
	loadimage(&img_hide[0][2], _T("tank1\\player_hide0_left.png"));
	loadimage(&img_hide[0][3], _T("tank1\\player_hide0_right.png"));
	loadimage(&img_hide[1][0], _T("bullet\\bullet_hide_up.png"));
	loadimage(&img_hide[1][1], _T("bullet\\bullet_hide_down.png"));
	loadimage(&img_hide[1][2], _T("bullet\\bullet_hide_left.png"));
	loadimage(&img_hide[1][3], _T("bullet\\bullet_hide_right.png"));
}

class Thing {
private://主函数无法调用，只可在类中调用，子类无法调用
	P_XY pos;
	int Hp;
	Direction dir;
	int speed;
protected:
	//主函数无法调用，子类可以调用
public:
	virtual void show() {};//虚函数，功能由子类决定。实现多态
	void SetPos(P_XY a)
	{
		pos = a;
	}
	void SetHp(int a) {
		Hp = a;
	}
	void SetDir(Direction a) {
		dir = a;
	}
	Direction GetDir() const{//const 防止修改
		return dir;
	}
	void SetSp(int a) {
		speed = a;
	}
	P_XY GetPos() const{
		return pos;
	}
	int GetHp() const{
		return Hp;
	}
	int GetSp() const {
		return speed;
	}
};

class wall :public Thing {
public:
	~wall() {}
};
class tank :public Thing {
public:
	virtual void key(){};//虚函数，功能由子类决定。实现多态
	void show() {
		if (int(GetDir()) == 0) {//位运算得到透明图片
			putimage(GetPos().x - tank_len / 2, GetPos().y - tank_len / 2, &img_hide[0][0], SRCAND);
			putimage(GetPos().x - tank_len / 2, GetPos().y - tank_len / 2, &img[0][0], SRCPAINT);
		}
		if (int(GetDir()) == 1) {
			putimage(GetPos().x - tank_len / 2, GetPos().y - tank_len / 2, &img_hide[0][1], SRCAND);
			putimage(GetPos().x - tank_len / 2, GetPos().y - tank_len / 2, &img[0][1], SRCPAINT);
		}
		if (int(GetDir()) == 2) {
			putimage(GetPos().x - tank_len / 2, GetPos().y - tank_len / 2, &img_hide[0][2],SRCAND);
			putimage(GetPos().x - tank_len / 2, GetPos().y - tank_len / 2, &img[0][2],SRCPAINT);
		}
		if (int(GetDir()) == 3) {
			putimage(GetPos().x - tank_len / 2, GetPos().y - tank_len / 2, &img_hide[0][3], SRCAND);
			putimage(GetPos().x - tank_len / 2, GetPos().y - tank_len / 2, &img[0][3], SRCPAINT);
		}
		circle(GetPos().x, GetPos().y, tank_len/4);
	}
};

class player :public tank {
private:
	int Deathtimes;
	int score;

public://父类无构造函数，带参构造子类
	player(int sp, int die) {
		SetSp(sp), SetDie(die);
	}
	void SetDie(int a) {
		Deathtimes = a;
	}
	int GetDie()const {
		return Deathtimes;
	}
	void SetSco(int a) {
		score = a;
	}
	int GetSco() const{
		return score;
	}
	void key() {
		if ((GetAsyncKeyState('W') & 0x8000)) {
			if (GetPos().y >= edge)//边界框
				SetPos({ GetPos().x, GetPos().y - GetSp() });
			SetDir(Direction::UP);
		}
		if ((GetAsyncKeyState('S') & 0x8000)) {
			if (GetPos().y < map_y - edge)
				SetPos({ GetPos().x, GetPos().y + GetSp() });
			SetDir(Direction::DOWN);
		}
		if ((GetAsyncKeyState('A') & 0x8000)) {
			if (GetPos().x >= edge)
				SetPos({ GetPos().x - GetSp(), GetPos().y });
			SetDir(Direction::LEFT);
		}
		if ((GetAsyncKeyState('D') & 0x8000)) {
			if (GetPos().x < map_x - edge)
				SetPos({ GetPos().x + GetSp(), GetPos().y });
			SetDir(Direction::RIGHT);
		}
	}
	
	void check() {
		if (GetHp() <= 0) {
			SetPos({ map_x / 2,map_y / 2 });
			SetDie(GetDie() + 1);
			SetHp(3);
		}
	}
	~player(){}
};
class player2 :public player {
public://父类带参构造，子类显式调用
	player2(int sp, int die) :player(sp, die) {}
	void key() {
		if ((GetAsyncKeyState(VK_UP) & 0x8000)) {
			if (GetPos().y >= edge)//边界框
				SetPos({ GetPos().x, GetPos().y - GetSp() });
			SetDir(Direction::UP);
		}
		if ((GetAsyncKeyState(VK_DOWN) & 0x8000)) {
			if (GetPos().y < map_y - edge)
				SetPos({ GetPos().x, GetPos().y + GetSp() });
			SetDir(Direction::DOWN);
		}
		if ((GetAsyncKeyState(VK_LEFT) & 0x8000)) {
			if (GetPos().x >= edge)
				SetPos({ GetPos().x - GetSp(), GetPos().y });
			SetDir(Direction::LEFT);
		}
		if ((GetAsyncKeyState(VK_RIGHT) & 0x8000)) {
			if (GetPos().x < map_x - edge)
				SetPos({ GetPos().x + GetSp(), GetPos().y });
			SetDir(Direction::RIGHT);
		}
	}
	~player2(){}
};
class enemy :public tank {
private:
	int deathtimes;
public:
	enemy(int die, int sp, int hp, Direction dir, P_XY xy) {
		SetDie(die), SetSp(sp), SetHp(hp), SetDir(dir), SetPos(xy);
	}
	void SetDie(int a){
		deathtimes = a;
	}
	int GetDie() const{
		return deathtimes;
	}
	~enemy(){}
};

class bullet :public Thing {
public:
	virtual void key() {};
	void show() {
		if (int(GetDir()) == 0) {//类运算得到透明图片
			putimage(GetPos().x - bullet_len / 2, GetPos().y - bullet_len / 2, &img_hide[1][0], SRCAND);
			putimage(GetPos().x - bullet_len / 2, GetPos().y - bullet_len / 2, &img[1][0], SRCPAINT);
		}
		if (int(GetDir()) == 1) {
			putimage(GetPos().x - bullet_len / 2, GetPos().y - bullet_len / 2, &img_hide[1][1], SRCAND);
			putimage(GetPos().x - bullet_len / 2, GetPos().y - bullet_len / 2, &img[1][1], SRCPAINT);
		}
		if (int(GetDir()) == 2) {
			putimage(GetPos().x - bullet_len / 2, GetPos().y - bullet_len / 2, &img_hide[1][2], SRCAND);
			putimage(GetPos().x - bullet_len / 2, GetPos().y - bullet_len / 2, &img[1][2], SRCPAINT);
		}
		if (int(GetDir()) == 3) {
			putimage(GetPos().x - bullet_len / 2, GetPos().y - bullet_len / 2, &img_hide[1][3],SRCAND);
			putimage(GetPos().x - bullet_len / 2, GetPos().y - bullet_len / 2, &img[1][3],SRCPAINT);
		}
		circle(GetPos().x, GetPos().y, tank_len/4);
	}
	void run() {
		if (GetPos().x < 0 || GetPos().x >= map_x)return;
		if (GetPos().y < 0 || GetPos().y >= map_y)return;
		SetSp(7);
		if (int(GetDir()) == 0) {
				SetPos({ GetPos().x, GetPos().y - GetSp()});
				show();
		}
		if (int(GetDir()) == 1) {
			SetPos({ GetPos().x, GetPos().y + GetSp()});
			show();
		}
		if (int(GetDir()) == 2) {
			SetPos({ GetPos().x-GetSp(), GetPos().y});
			show();
		}
		if (int(GetDir()) == 3) {
			SetPos({ GetPos().x+GetSp(), GetPos().y});
			show();
		}
	}

	
	void check(tank&a) {
		if ((GetPos().x - a.GetPos().x) * (GetPos().x - a.GetPos().x) + (GetPos().y - a.GetPos().y) * (GetPos().y - a.GetPos().y) < tank_len*tank_len/4) {
			a.SetHp(a.GetHp() - 1);
			SetHp(0);
		}
		if (GetHp() <= 0) {
			SetPos({ -tank_len, -tank_len });
			SetHp(1);
		}
	}
	~bullet() {}
};
/*
void mapcheck(tank& a) {未完成的地图系统片段
	if (a.GetHp() <= 0)return;
	if (a.GetPos().x < 0 || a.GetPos().x >= map_x)return;
	if (a.GetPos().y < 0 || a.GetPos().y >= map_y)return;
	if (Map[a.GetPos().x][a.GetPos().y] == -1) {
		a.SetHp(a.GetHp() - 1);

	}
	if (a.GetHp() > 0) {
		for (int i = 0; i < tank_len; i++) {//9*9
			for (int j = 0; j < tank_len; j++) {
				Map[a.GetPos().x - tank_len/2 + i][a.GetPos().y - tank_len/2 + j] == 1;
			}
		}
	}
}
void mapcheck(bullet& a) {
	if (a.GetHp() <= 0)return ;
	if (a.GetPos().x < 0 || a.GetPos().x >= map_x)return;
	if (a.GetPos().y < 0 || a.GetPos().y >= map_y)return;
	if (Map[a.GetPos().x][a.GetPos().y] == 1) {
		a.SetHp(a.GetHp() - 1);
		for (int i = 0; i < tank_len*2; i++) {//9*9
			for (int j = 0; j < tank_len*2; j++) {
				Map[a.GetPos().x - tank_len + i][a.GetPos().y - tank_len + j] == -1;
			}
		}
	}
}
*/

class bullet_player :public bullet {
public:
	void key(player a) {
		if ((GetAsyncKeyState(' ') & 0x8000)) {
			Sleep(100);//防止按一次发射多个子弹，引入冷却时间
			SetDir(a.GetDir());
			SetHp(1);
			SetPos({ a.GetPos().x,a.GetPos().y });
			run();
		}
	}
};

class bullet_player2 :public bullet_player {
public:
	void key(player a) {
		if ((GetAsyncKeyState(VK_SHIFT) & 0x8000)) {
			Sleep(100);//防止按一次发射多个子弹，引入冷却时间
			SetDir(a.GetDir());
			SetHp(1);
			SetPos({ a.GetPos().x ,a.GetPos().y });
			run();
		}
	}
};
class bullet_enemy :public bullet {
public:
	void key(tank a) {
		Sleep(100);//防止按一次发射多个子弹，引入冷却时间
		SetDir(a.GetDir());
		SetHp(1);
		SetPos({ a.GetPos().x ,a.GetPos().y });
		run();
	}
};

void game() {
	player p1(5,0);
	player2 p2(5,0);
	enemy boss(0, 1, 5, Direction::DOWN, { map_x / 2,11 });

	mciSendString(_T("play others\\GameBegin.mp3 from 0"), NULL, 0, NULL);//播放音乐
	
	bullet_player* temp1 = new bullet_player[100];
	bullet_player2* temp2 = new bullet_player2[100];
	bullet_enemy *temp=new bullet_enemy[100];//最大同时存在子弹数
	int i = 0, j = 0, k = 0;
	bool tag = 1;
	int time=0;
	TCHAR s1[100], s2[100], s3[100];
	while (1) {
		putimage(0, 0, &background);

		//move
		p1.key();
		temp1[i].key(p1);
		i++;
		i = i % 100;

		p2.key();
		temp2[j].key(p2);
		j++;
		j = j % 100;

		if (tag == 1)  boss.SetPos({ boss.GetPos().x - boss.GetSp(),boss.GetPos().y });
		if(tag==0)  boss.SetPos({ boss.GetPos().x + boss.GetSp(),boss.GetPos().y });
		if (boss.GetPos().x < edge * 2)
			tag = 0;
		if (boss.GetPos().x > map_x - edge * 2)
			tag = 1;
		Sleep(10);
		if(k==1)
		temp[k].key(boss);
		if (k == 50)
			temp[k].key(boss);
		k++;
		k = k % 100;

		//check
		for (int t = 0; t < 100; t++) {
			//mapcheck(temp[i]);
			temp2[t].check(p1);
			temp1[t].check(p2);
			if (t == 1) {
				temp[t].check(p1);
				temp[t].check(p2);
				temp[t].run();
			}
			if (t == 50) {
				temp[t].check(p1);
				temp[t].check(p2);
				temp[t].run();
			}
			temp1[t].check(boss);
			temp2[t].check(boss);
			temp1[t].run();
			temp2[t].run();
		
		}
		
		//draw
		//mapcheck(p1);
		p1.check();
		p2.check();
		if (boss.GetHp() <= 0||boss.GetDie()>=1) {
			outtextxy(11, map_y-15, _T("YOU ARE WINNING,BUT YOU CAN CONTINUE，或者按ESC退出"));
			if (boss.GetHp() <= 0) {
				boss.SetHp(5);
				boss.SetDie(boss.GetDie() + 1);
			}
			if ((GetAsyncKeyState(VK_ESCAPE) & 0x8000)) {
				EndBatchDraw();
				cleardevice();
				putimage(0, 0, &background);
				settextcolor(RGB(57, 197, 187));//#39C5BB
				settextstyle(100, 0, L"Consolas");//L
				outtextxy(35, map_y/2, _T("GOOG GAME"));//_T
				Sleep(1000);
				delete []temp1;
				delete []temp2;
				delete []temp;
				/*delete s1;//已被delete，再delete会重复
				delete s2;
				delete s3;*/
				p1.~player();
				p2.~player2();
				boss.~enemy();
				break;
			}
		}
	
		swprintf_s(s1,80, _T("%d"), p1.GetDie()-1); //转换
		swprintf_s(s2,80,_T("%d"),p2.GetDie()-1 );
		swprintf_s(s3,80, _T("%d"), boss.GetHp() );
		outtextxy(10, map_y - 65, _T("enemy_HP"));
		outtextxy(100, map_y - 65, s3);
		outtextxy(10, map_y - 50, _T("p1死亡次数"));
		outtextxy(100, map_y - 50, s1);
		outtextxy(10, map_y - 35, _T("p2死亡次数"));
		outtextxy(100, map_y - 35, s2);
		outtextxy(170, map_y - 35, _T("请和您的朋友愉快的游戏吧~"));
		outtextxy(170, map_y - 65, _T("您的血量为3，地图中央为重生点"));
		outtextxy(170, map_y - 50, _T("长按发射按钮使用技能 THE WORLD!!!"));
		p1.show();
		p2.show();
		boss.show();
		FlushBatchDraw();//双缓冲
		cleardevice();
	}
}

void welcome() {
	MOUSEMSG m;
	setbkcolor(RGB(57, 197, 187));//背景色//#39C5BB
	cleardevice();
	setfillcolor(RGB(102,204,255));//填充色//66CCFF
	fillrectangle(map_x/2-50, map_y/2-20,map_x/2+50 ,map_y/2+20);
	settextstyle(15, 0,L"微软黑体");
	setbkmode(TRANSPARENT);//字体颜色等于背景色
	outtextxy(map_x / 2 - 30, map_y / 2 - 10,_T ("开始游戏!!!"));
	while (1) {
		m = GetMouseMsg();
		if (m.x >= map_x / 2 - 50 && m.x <= map_x / 2 + 50 && m.y >= map_y / 2 - 20 && m.y<+map_y / 2 + 20) {
			setlinecolor(BLUE);//线条颜色
			rectangle(map_x / 2 - 70, map_y / 2 - 40, map_x / 2 + 70, map_y / 2 + 40);
			if (m.uMsg == WM_LBUTTONDOWN) {		//如果按下鼠标左键则开始游戏
				setlinecolor(WHITE);
				settextcolor(RGB(255,226, 17));//#FFE211
				break;
			}
		}
		else {	//当鼠标不在目标位置就覆盖之前的蓝色边框
			setlinecolor(WHITE);
			rectangle(map_x / 2 - 70, map_y / 2 - 40, map_x / 2 + 70, map_y / 2 + 40);
		}
	}
}
int  main() {
	initgraph(map_x, map_y);
	inimg();
	welcome();
	BeginBatchDraw();
	//outtextxy(100, 100, _T("Hello World!"));
	//putimage(100, 100, &img[0][0]);
	game();
	
	closegraph();
	return 0;
}
