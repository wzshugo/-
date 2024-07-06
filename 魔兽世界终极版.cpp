#include<iostream>
#include<iomanip>
#include<string>
#include<vector>

using namespace std;

//以下所有两个元素的数组中，0表示红方，1表示蓝方/////////////////////////////////////////////////////
int Case = 0, case_num = 1; //例子数量
int total_elements = 0, city_total_num = 0; //规定的总时间、城市总数量
int arrow_force = 0, loyal_reduce = 0; //arrow的攻击力、忠诚度的降低
int attach[5] = {}; //武士攻击力数组
int life[5] = {}; //武士生命值数组
int hour = 0, minute = 0, total_time = 0; //小时，分钟，当前的总时间
enum Weapon_name { sword, bomb, arrow };
enum Warrior_name { dragon, ninja, iceman, lion, wolf };
Warrior_name order[2][5] = { {iceman,lion,wolf,ninja,dragon },
	{lion,dragon,ninja,iceman,wolf } };
string color[2] = { "red","blue" };
string names[5] = { "dragon", "ninja", "iceman", "lion", "wolf" };
string weapons[3] = { "sword","bomb","arrow" };

//打印时间
void print_time() {
	cout << setfill('0') << setw(3) << hour << ":"
		<< setfill('0') << setw(2) << minute << " ";
}

class Weapon;
class Headquarter;
class City;
class Warrior;

//武器类//////////////////////////////////////////////////////////////////////////////////////////
class Weapon {
private:
	int name_, sword_force_, arrow_usetime_; //武器名字，sword攻击力，arrow剩余使用次数
public:
	Weapon(int name, int sword_force) //weapon构造函数
		:name_(name), sword_force_(sword_force), arrow_usetime_(3) {}
	int& get_usetime() { return arrow_usetime_; } //接口：获得arrow剩余使用次数
	int get_sword_force() { return sword_force_; } //接口：获得sword攻击力
	bool sword_blunt() { //arrow变钝函数
		sword_force_ = int(sword_force_ * 0.8);
		return sword_force_;
	}
};

//城市类//////////////////////////////////////////////////////////////////////////////////////////
class City {
private:
	//编号，旗帜(-1表示无旗)，连续胜利次数，生命元
	int number_, flag_, war_win_time_[2], elements_;
	//城市中的武士
	Warrior* warriors[2];
public:
	City(int number);
	int get_num() { return number_; } //接口
	int get_elements() { return elements_; } //接口
	bool if_warrior(int i) { return warriors[i]; } //接口
	void lion_flee(); //warrior的接口：处理狮子逃跑
	void forward(City* last, int i); //从上一个city移动过来
	void move_to_head(Headquarter* headquarter, int i); //移动到敌方司令部
	void move_from_head(Warrior* warrior, int i); //从司令部移动到city
	void warrior_forward(); //warrior的接口：warrior前进输出
	void add_elements() { elements_ += 10; }
	void gain_elements(); //城内单独warrior获取生命元
	void shot_arrow(City* next[]); //处理arrow事件
	void bomb_deal(); //处理bomb事件
	void fight_judge(); //判断谁主动进攻并且调用fight函数
	void set_flag(int i); //旗帜变换
	void gain_reward(int i); //接口：武士从司令部获得奖励
	void warrior_report(int i); //接口：武士汇报武器情况
	void praise_warrior(); //奖励获胜武士生命元顺便处理死亡的武士
};

//司令类//////////////////////////////////////////////////////////////////////////////////////////
class Headquarter {
private:
	//武士数量，生命元，敌人进入的数量，阵营
	int warrior_num_, elements_, enemy_num_, color_;
	bool if_enter; //是否有敌人进入
	Warrior* warrior;
	Warrior* enemys[2];
public:
	Headquarter(int elements, int color);
	int get_color(){ return color_; }; //接口
	void born(); //武士降生
	void lion_flee(); //司令部处理逃跑的狮子
	void move_to_city(City* city, int i); //city的接口：从司令部移动到city
	void enemy_enter(Warrior* enemy); //敌方warrior进入
	void warrior_forward(); //warrior的接口：敌方warrior进入输出
	bool is_occupied(); //司令部是否被占领
	void gain_elements(int add_elements) { //司令部生命元增加
		elements_ += add_elements;
	}
	int& get_elements() { return elements_; } //接口
	void report_elements(); //报告生命元
	void report_weapon(); //warrior的接口：进入的enemy报告武器
	~Headquarter();
};

//武士类//////////////////////////////////////////////////////////////////////////////////////////
class Warrior {
private:
	//生命值，编号，攻击力，忠诚度(针对lion)，步数(针对ninja)，生命值的副本
	int strength_, number_, force_, loyalty_, step_, l_life;
	double morale_; //士气(针对dragon)
	bool if_kill, is_win; //是否杀死敌人，是否获胜
	Warrior_name name_;
	Weapon* weapon_[3];
	City* city_;
	Headquarter* headquarter_;
public:
	Warrior(Warrior_name& name, int number, int& strength, Headquarter* headquarter);
	int get_strength() { return strength_; } //接口
	bool& get_is_win() { return is_win; } //接口
	bool lion_flee(); //狮子逃跑输出
	void warrior_forward(bool if_arrive); //武士前进输出
	void gain_elements(int add_elements); //武士从城市取走生命元输出
	void shot_arrow(); //射箭输出
	void is_shoted(); //是否被射死输出
	void change_city(City* next) { city_ = next; } //改变城市
	void morale_change() { is_win ? morale_ += 0.2 : morale_ -= 0.2; } //士气改变
	bool get_weapon(Weapon_name name) { return weapon_[name]; } //接口
	void use_bomb(Warrior* enemy); //使用bomb输出
	int fight_if_win(Warrior* enemy); //主动进攻是否会胜利
	bool if_bomb(Warrior* enemy, int i); //是否使用bomb
	void fight(Warrior* enemy); //战斗输出
	void after_fight(Warrior* enemy); //战后处理
	void dead(); //死亡输出
	void gain_reward(); //战胜后从司令部获得奖励
	void get_gain_elements(int add_elements) { //headquarter的接口：司令部增加生命元
		headquarter_->get_elements() += add_elements;
	}
	void loyalty_reduce() { loyalty_ -= loyal_reduce; } //忠诚度改变
	void report_weapon(); //报告武器情况输出
	~Warrior();
};

//开始游戏////////////////////////////////////////////////////////////////////////////////////////
void Game_start() {
	cin >> Case;
	for (int i = 1; i <= Case; ++i) {
		cin >> total_elements >> city_total_num >> arrow_force >> loyal_reduce >> total_time;
		for (int i = 0; i < 5; ++i) { cin >> life[i]; }
		for (int i = 0; i < 5; ++i) { cin >> attach[i]; }
		//构建城市的vector
		vector<City> cities;
		for (int i = 0; i < city_total_num; ++i) {
			City temp(i + 1);
			cities.push_back(temp);
		}
		//构建司令部：0表示红方，1表示蓝方
		Headquarter head[2] = { Headquarter(total_elements,0),Headquarter(total_elements,1) };
		cout << "Case " << case_num++ << ":\n";
		hour = 0; minute = 0;
		while (hour * 60 + minute <= total_time) {
			head[0].born(); head[1].born(); minute = 5; //第0分钟武士降生
			if (hour * 60 + minute > total_time) break; //如果超过规定时间则跳出循环
			else {
				head[0].lion_flee(); //判断红方司令部的lion是否逃跑
				for (vector<City>::iterator i = cities.begin(); i != cities.end(); i++) {
					(*i).lion_flee();
				} //判断每个城市的lion是否逃跑
				head[1].lion_flee(); //判断蓝方司令部的lion是否逃跑
			}
			minute = 10;
			if (hour * 60 + minute > total_time) break; //第10分钟
			else {
				if (cities[0].if_warrior(1)) cities[0].move_to_head(&head[0], 1); //蓝色武士进入红色司令部
				for (int i = 0; i < city_total_num - 1; ++i)
					cities[i].forward(&cities[i + 1], 1); //蓝色武士向前移动
				head[1].move_to_city(&cities[city_total_num - 1], 1); //蓝色武士从司令部移动到城市		
				if (cities[city_total_num - 1].if_warrior(0)) cities[city_total_num - 1].move_to_head(&head[1], 0); //红色武士进入蓝色司令部
				for (int i = city_total_num - 1; i > 0; --i)
					cities[i].forward(&cities[i - 1], 0); //红色武士向前移动
				head[0].move_to_city(&cities[0], 0); //红色武士从司令部移动到城市
				//武士移动输出：
				head[0].warrior_forward(); //蓝方武士进入红方司令部输出
				bool p = head[0].is_occupied(); //红方是否被占领以及相关输出
				for (int i = 0; i < city_total_num; ++i) cities[i].warrior_forward(); //每个城市的武士前进输出
				head[1].warrior_forward(); //红方武士进入蓝方司令部输出
				if (p + head[1].is_occupied()) break; //蓝方是否被占领以及相关输出
			}
			minute = 20;
			if (hour * 60 + minute > total_time) break; //第20分钟
			else { for (int i = 0; i < city_total_num; ++i) cities[i].add_elements(); } //每个城市产出10个生命元
			minute = 30;
			if (hour * 60 + minute > total_time) break; //第30分钟
			else { for (int i = 0; i < city_total_num; ++i) cities[i].gain_elements(); } //每个城市内单独武士获取生命元
			minute = 35;
			if (hour * 60 + minute > total_time) break; //第35分钟
			else {
				if (city_total_num > 1) { //城市数量大于1才会发生arrow事件
					City* next[2] = {};
					//对于在第一个城市中的武士，红武士的射箭目标为第二个城市中的武士，蓝武士的射箭目标则为NULL
					next[0] = NULL; next[1] = &cities[1];
					cities[0].shot_arrow(next); 
					//对于第i个城市中的武士(1<i<N)，红武士的射箭目标为第i+1个城市中的武士，蓝武士的目标则为第i-1个城市中的武士
					for (int i = 1; i < city_total_num - 1; ++i) {
						next[0] = &cities[i - 1];
						next[1] = &cities[i + 1];
						cities[i].shot_arrow(next);
					}
					//对于在最后一个城市中的武士，红武士的射箭目标为NULL，蓝武士的射箭目标则为导数第二个城市中的武士
					next[0] = &cities[city_total_num - 2]; next[1] = NULL;
					cities[city_total_num - 1].shot_arrow(next);
				}
			}
			minute = 38;
			if (hour * 60 + minute > total_time) break; //第38分钟
			else {
				for (vector<City>::iterator i = cities.begin(); i != cities.end(); ++i) { i->bomb_deal(); }
			} //每个城市处理bomb事件
			minute = 40;
			if (hour * 60 + minute > total_time) break; //第40分钟
			else { //对于每个城市
				//判断是否要对战
				for (vector<City>::iterator i = cities.begin(); i != cities.end(); ++i) { (*i).fight_judge(); }
				//每个城市的红武士是否获得司令部的战后奖励
				for (vector<City>::iterator i = cities.begin(); i != cities.end(); ++i) { (*i).gain_reward(0); }
				//每个城市的蓝武士是否获得司令部的战后奖励
				for (vector<City>::reverse_iterator i = cities.rbegin(); i != cities.rend(); ++i) { (*i).gain_reward(1); }
				//每个城市中获胜的武士获得该城市的生命元
				for (vector<City>::iterator i = cities.begin(); i != cities.end(); ++i) { (*i).praise_warrior(); }
			}
			minute = 50;
			if (hour * 60 + minute > total_time) break; //第50分钟
			else { head[0].report_elements(); head[1].report_elements(); } //司令部汇报生命元
			minute = 55;
			if (hour * 60 + minute > total_time) break; //第55分钟
			else {
				//所有的红武士按顺序汇报武器情况
				for (vector<City>::iterator i = cities.begin(); i != cities.end(); ++i) { (*i).warrior_report(0); }
				head[1].report_weapon(); 
				//所有的蓝武士按顺序汇报武器情况
				head[0].report_weapon();
				for (vector<City>::iterator i = cities.begin(); i != cities.end(); ++i) { (*i).warrior_report(1); }
			}
			minute = 0;
			hour++;
		}
	}
}

int main()
{
	Game_start();

	return 0;
}

City::City(int number) :number_(number) {
	war_win_time_[0] = 0; war_win_time_[1] = 0;
	elements_ = 0; flag_ = -1;
	warriors[0] = NULL;
	warriors[1] = NULL;
}
void City::lion_flee() { //warrior的接口：处理狮子逃跑
	for (int i = 0; i < 2; ++i) {
		if (warriors[i]) {
			if (warriors[i]->lion_flee()) {
				delete warriors[i]; warriors[i] = NULL;
			}
		}
	}
}
void City::forward(City* last, int i) { //从上一个city移动过来
	warriors[i] = last->warriors[i];
	if (warriors[i])warriors[i]->change_city(this); //城市变为this
}
void City::move_to_head(Headquarter* headquarter, int i) { //移动到敌方司令部
	headquarter->enemy_enter(warriors[i]); //敌方司令部增加一名武士
	warriors[i] = NULL; //清除城市中移动后的武士
}
void City::move_from_head(Warrior* warrior, int i) { //从司令部移动到city
	warriors[i] = warrior;
	if (warriors[i])warriors[i]->change_city(this); //城市变为this
}
void City::warrior_forward() { //warrior的接口：warrior前进输出
	if (warriors[0]) warriors[0]->warrior_forward(false);
	if (warriors[1]) warriors[1]->warrior_forward(false);
}
void City::gain_elements() { //城内单独warrior获取生命元
	for (int i = 0; i < 2; ++i)
		if ((warriors[i]) && (!warriors[1 - i])) {
			warriors[i]->gain_elements(elements_);
			elements_ = 0; break; //城市生命元清零
		}
}
void City::shot_arrow(City* next[]) { //处理arrow事件
	for (int i = 0; i < 2; ++i) {
		if ((warriors[i]) && (warriors[i]->get_weapon(arrow))) {
			if ((next[1 - i]) && (next[1 - i]->warriors[1 - i])) {
				warriors[i]->shot_arrow(); //射箭
				(next[1 - i]->warriors[1 - i])->is_shoted(); //被射
			}
		}
	}
}
void City::bomb_deal() { //处理bomb事件
	if (warriors[0] && warriors[1]) {
		bool if_use_bomb = false;
		for (int i = 0; i <= 1; i++) {
			if (warriors[i]->get_weapon(bomb)) {
				if (flag_ == -1) if_use_bomb = warriors[i]->if_bomb(warriors[1 - i], number_); //如果无旗帜
				else if_use_bomb = warriors[i]->if_bomb(warriors[1 - i], flag_ + 1); //如果有旗帜
				if (if_use_bomb) {
					delete warriors[0]; warriors[1] = NULL;
					delete warriors[1]; warriors[0] = NULL;
					break;
				}
			}
		}
		if (!if_use_bomb && warriors[0]->get_strength() <= 0 && warriors[1]->get_strength() <= 0) { 
			//如果两只武士都被射死
			delete warriors[0]; warriors[0] = NULL;
			delete warriors[1]; warriors[1] = NULL;
		}
	}
}
void City::fight_judge() { //判断谁主动进攻并且调用fight函数
	if (warriors[0] && warriors[1]) {
		if (flag_ == -1) { //如果没有旗帜
			if (number_ % 2)warriors[0]->fight(warriors[1]); //偶数编号城市红方进攻
			else warriors[1]->fight(warriors[0]); //奇数编号城市蓝方进攻
		}
		else warriors[flag_]->fight(warriors[1 - flag_]); //有旗帜的主动进攻
	}
}
void City::set_flag(int i) { //旗帜变换
	if (i == -1) { war_win_time_[0] = 0; war_win_time_[1] = 0; }
	else {
		++war_win_time_[i]; war_win_time_[1 - i] = 0;
		//旗帜升起输出
		if (war_win_time_[i] == 2 && flag_ != i) {
			flag_ = i;
			print_time();
			cout << color[i] << " flag raised in city " << number_ << endl;
		}
	}
}
void City::gain_reward(int i) { //接口：武士从司令部获得奖励
	if (warriors[i]) warriors[i]->gain_reward();
}
void City::praise_warrior() { //奖励获胜武士生命元顺便处理死亡的武士
	for (int i = 0; i < 2; ++i)
		if (warriors[i]) {
			if (warriors[i]->get_is_win()) {
				warriors[i]->get_gain_elements(elements_); //如果胜利司令部增加生命元
				elements_ = 0; //重置城市的生命元
				warriors[i]->get_is_win() = false; //重置胜利变量
			}
			else if ((warriors[i]->get_strength()) <= 0) {
				delete warriors[i];
				warriors[i] = NULL;
			}
		}
}
void City::warrior_report(int i) { //接口：武士汇报武器情况
	if (warriors[i]) warriors[i]->report_weapon(); 
}
Headquarter::Headquarter(int elements, int color)
	:elements_(elements), color_(color) {
	enemys[0] = NULL; enemys[1] = NULL; if_enter = false;
	warrior_num_ = 0; enemy_num_ = 0; warrior = NULL;
}
void Headquarter::born() { //武士降生
	if (elements_ >= life[order[color_][warrior_num_ % 5]]) {
		elements_ -= life[order[color_][warrior_num_ % 5]]; //司令部生命元减少
		print_time();
		cout << color[color_] << " ";
		//申请一个warrior
		warrior = new Warrior(order[color_][warrior_num_ % 5], warrior_num_ + 1, life[order[color_][warrior_num_ % 5]], this);
		warrior_num_++;
	}
}
void Headquarter::lion_flee() { //司令部处理逃跑的狮子
	if (warrior && warrior->lion_flee()) {
		delete warrior; warrior = NULL;
	}
}
void Headquarter::move_to_city(City* city, int i) { //city的接口：从司令部移动到city
	city->move_from_head(warrior, i);
	warrior = NULL;
}
void Headquarter::enemy_enter(Warrior* enemy) { //敌方warrior进入
	enemys[enemy_num_] = enemy;
	if_enter = true;
	++enemy_num_;
}
void Headquarter::warrior_forward() { //warrior的接口：敌方warrior进入输出
	if (if_enter) {
		enemys[enemy_num_ - 1]->warrior_forward(true);
		if_enter = false; //重置变量
	}
}
bool Headquarter::is_occupied() { //司令部是否被占领
	if (enemy_num_ == 2) {
		print_time();
		cout << color[color_] << " headquarter was taken" << endl;
		return true;
	}
	return false;
}
void Headquarter::report_elements() { //报告生命元
	print_time();
	cout << elements_ << " elements in " << color[color_] << " headquarter" << endl;
}
void Headquarter::report_weapon() { //warrior的接口：进入的enemy报告武器
	if (enemy_num_)enemys[enemy_num_ - 1]->report_weapon();
}
Headquarter::~Headquarter() {
	if (warrior) delete warrior;
	if (enemys[0])delete enemys[0];
	if (enemys[1])delete enemys[1];
}
Warrior::Warrior(Warrior_name& name, int number, int& strength, Headquarter* headquarter)
	:name_(name), number_(number), strength_(strength), headquarter_(headquarter) {
	weapon_[0] = NULL; weapon_[1] = NULL; weapon_[2] = NULL;
	step_ = 0;
	force_ = attach[name_];
	if_kill = false, is_win = false;
	l_life = 0;
	switch (name_) { //按照武士的名字申请不同的武器
	case dragon:case iceman:weapon_[number_ % 3] = new Weapon(number_ % 3, int(attach[name_] * 0.2)); break;
	case ninja:weapon_[number_ % 3] = new Weapon(number_ % 3, int(attach[name_] * 0.2));
		weapon_[(number_ + 1) % 3] = new Weapon((number_ + 1) % 3, int(attach[name_] * 0.2)); break;
	default:break;
	}
	morale_ = double(headquarter_->get_elements()) / double(strength_); //初始化士气
	loyalty_ = headquarter_->get_elements(); //初始化忠诚度
	city_ = NULL;
	if (weapon_[sword] && !(weapon_[sword]->get_sword_force())) weapon_[sword] = NULL;//若初始武器攻击为0，则delete
	cout << names[name_] << " " << number_ << " born" << endl;
	if (name_ == lion) { cout << "Its loyalty is " << loyalty_ << endl; }
	if (name_ == dragon) { cout << fixed << setprecision(2) << "Its morale is " << morale_ << endl; }
}
bool Warrior::lion_flee() { //狮子逃跑输出
	if ((name_ == lion) && (loyalty_ <= 0)) {
		print_time();
		cout << color[headquarter_->get_color()] << " lion " << number_ << " ran away" << endl;
		return true;
	}
	return false;
}
void Warrior::warrior_forward(bool if_arrive) { //武士前进输出
	++step_;
	if ((name_ == iceman) && (step_ % 2 == 0)) { //处理ninja的情况
		if (strength_ > 9) strength_ -= 9;
		else strength_ = 1;
		force_ += 20;
	}
	print_time();
	cout << color[headquarter_->get_color()] << " " << names[name_] << " " << number_;
	if (if_arrive) { //如果到达敌方司令部
		cout << " reached " << color[1 - headquarter_->get_color()] << " headquarter with "
			<< strength_ << " elements and force " << force_ << endl;
	} //否则
	else cout << " marched to city " << city_->get_num() << " with " << strength_ << " elements and force " << force_ << endl;
}
void Warrior::gain_elements(int add_elements) { //武士从城市取走生命元输出
	print_time();
	cout << color[headquarter_->get_color()] << " " << names[name_] << " " << number_ << " earned "
		<< add_elements << " elements for his headquarter" << endl;
	headquarter_->gain_elements(add_elements); //司令部增加生命元
}
void Warrior::shot_arrow() { //射箭输出
	print_time();
	cout << color[headquarter_->get_color()] << " " << names[name_] << " " << number_ << " shot";
	weapon_[arrow]->get_usetime()--;
	if (!weapon_[arrow]->get_usetime()) { //如果射箭次数用完了
		delete weapon_[arrow];
		weapon_[arrow] = NULL;
	}
}
void Warrior::is_shoted() { //是否被射死输出
	strength_ -= arrow_force;
	if (strength_ <= 0) {
		cout << " and killed " << color[headquarter_->get_color()] << " " << names[name_] << " " << number_;
	}
	cout << endl;
}
void Warrior::use_bomb(Warrior* enemy) { //使用bomb输出
	print_time();
	cout << color[headquarter_->get_color()] << " " << names[name_] << " " << number_
		<< " used a bomb and killed " << color[enemy->headquarter_->get_color()]
		<< " " << names[enemy->name_] << " " << enemy->number_ << endl;
}
int Warrior::fight_if_win(Warrior* enemy) { //主动进攻是否会胜利
	if (strength_ > 0 && enemy->strength_ > 0) { //如果双方都还活着
		int swf = 0;
		if (weapon_[sword])swf = weapon_[sword]->get_sword_force();
		if (enemy->strength_ - swf - force_ > 0) {
			if (enemy->name_ == ninja) return 0; //平局
			else {
				int e_swf = 0;
				if (enemy->weapon_[sword])e_swf = enemy->weapon_[sword]->get_sword_force();
				if (strength_ - (enemy->force_ / 2) - e_swf <= 0)return -1; //输
				else return 0; //平局
			}
		}
		else return 1; //胜利
	}
	return 0; //平局
}
bool Warrior::if_bomb(Warrior* enemy, int i) { //是否使用bomb
	if ((i + headquarter_->get_color()) % 2) { //如果是主动进攻
		if (fight_if_win(enemy) == -1) { use_bomb(enemy); return true; }
	}
	else { //如果不是主动进攻
		if (enemy->fight_if_win(this) == 1) { use_bomb(enemy); return true; }
	}
	return false;
}
void Warrior::fight(Warrior* enemy) { //主动进攻战斗输出
	Warrior* winner = NULL; Warrior* loser = NULL; //定义胜利者和失败者
	l_life = strength_; enemy->l_life = enemy->strength_;
	if ((enemy->strength_) > 0 && strength_ > 0) { //如果双方都活着
		int swf = 0;
		if (weapon_[sword]) swf = weapon_[sword]->get_sword_force();
		print_time();
		cout << color[headquarter_->get_color()] << " " << names[name_] << " " << number_ << " attacked "
			<< color[enemy->headquarter_->get_color()] << " " << names[enemy->name_] << " " << enemy->number_
			<< " in city " << city_->get_num() << " with " << strength_ << " elements and force " << force_ << endl;
		enemy->strength_ = enemy->strength_ - force_ - swf;
		if (swf && !(weapon_[sword]->sword_blunt())) { //如果sword用完之后攻击力为0则delete
			delete weapon_[sword]; weapon_[sword] = NULL;
		}
		if (enemy->strength_ > 0) { //如果主动进攻后敌人还活着，即平局
			if (enemy->name_ == ninja) {
				city_->set_flag(-1);
				morale_change();
				loyalty_reduce();
			}
			else { //敌人反击
				int e_swf = 0;
				if (enemy->weapon_[sword])e_swf = (enemy->weapon_[sword])->get_sword_force();
				print_time();
				cout << color[enemy->headquarter_->get_color()] << " " << names[enemy->name_] << " " << enemy->number_ << " fought back against "
					<< color[headquarter_->get_color()] << " " << names[name_] << " " << number_ << " in city " << city_->get_num() << endl;
				strength_ = strength_ - (enemy->force_ / 2) - e_swf;
				if (e_swf && !(enemy->weapon_[sword]->sword_blunt())) { //如果敌人sword用完之后攻击力为0则delete
					delete enemy->weapon_[sword]; enemy->weapon_[sword] = NULL;
				}
				if (strength_ > 0) { //如果还活着，即平局
					city_->set_flag(-1); morale_change(); loyalty_reduce();
					enemy->morale_change(); enemy->loyalty_reduce();
				}
				else { enemy->if_kill = true; winner = enemy; loser = this; }
			}
		}
		else { if_kill = true; winner = this; loser = enemy; }
	}
	else { //如果未发生战斗前有一方已经死亡
		if (strength_ > 0) { enemy->l_life = 0; winner = this; loser = enemy; }
		else { l_life = 0; winner = enemy; loser = this; }
	}
	if (winner) { //winner对loser战后处理
		winner->after_fight(loser);
	}
	if (name_ == dragon && strength_ > 0 && morale_ > 0.8) { //dragon欢呼
		print_time();
		cout << color[headquarter_->get_color()] << " " << names[name_] << " " << number_ << " yelled in city " << city_->get_num() << endl;
	}
	if (winner) { //winner获得改城市的生命元
		print_time();
		cout << color[winner->headquarter_->get_color()] << " " << names[winner->name_] << " " << winner->number_
			<< " earned " << winner->city_->get_elements() << " elements for his headquarter" << endl;
		(winner->city_)->set_flag((winner->headquarter_)->get_color());
	}
}
void Warrior::after_fight(Warrior* enemy) { //战后处理
	is_win = true;
	if (if_kill) { enemy->dead(); if_kill = false; } //如果杀死了敌人
	if (enemy->name_ == lion) strength_ += enemy->l_life; //lion战死后生命值转移
	if (name_ == wolf) //wolf缴获武器
		for (int i = 0; i < 3; ++i)
			if (enemy->weapon_[i] && !weapon_[i]) {
				weapon_[i] = enemy->weapon_[i];
				enemy->weapon_[i] = NULL;
			}
	morale_change(); //改变士气
}
void Warrior::dead() { //死亡输出
	print_time();
	cout << color[headquarter_->get_color()] << " " << names[name_] << " " << number_
		<< " was killed in city " << city_->get_num() << endl;
}
void Warrior::gain_reward() { //战胜后从司令部获得奖励
	if (is_win) {
		if (headquarter_->get_elements() > 7) {
			headquarter_->get_elements() -= 8;
			strength_ += 8;
		}
	}
}
void Warrior::report_weapon() { //报告武器情况输出
	bool if_weapon = false;
	print_time();
	cout << color[headquarter_->get_color()] << " " << names[name_] << " " << number_ << " has ";
	for (int i = 2; i >= 0; --i) {
		if (weapon_[i]) {
			if (if_weapon) cout << ",";
			else if_weapon = true;
			cout << weapons[i];
			if (i == 2)cout << "(" << weapon_[arrow]->get_usetime() << ")"; //如果是arrow
			if (i == 0)cout << "(" << weapon_[sword]->get_sword_force() << ")"; //如果是sword
		}
	}
	if (!if_weapon)cout << "no weapon"; //如果没武器
	cout << endl;
}
Warrior::~Warrior() {
	for (int i = 0; i < 3; ++i) {
		if (weapon_[i])delete weapon_[i];
	}
}