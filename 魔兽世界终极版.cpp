#include<iostream>
#include<iomanip>
#include<string>
#include<vector>

using namespace std;

//������������Ԫ�ص������У�0��ʾ�췽��1��ʾ����/////////////////////////////////////////////////////
int Case = 0, case_num = 1; //��������
int total_elements = 0, city_total_num = 0; //�涨����ʱ�䡢����������
int arrow_force = 0, loyal_reduce = 0; //arrow�Ĺ��������ҳ϶ȵĽ���
int attach[5] = {}; //��ʿ����������
int life[5] = {}; //��ʿ����ֵ����
int hour = 0, minute = 0, total_time = 0; //Сʱ�����ӣ���ǰ����ʱ��
enum Weapon_name { sword, bomb, arrow };
enum Warrior_name { dragon, ninja, iceman, lion, wolf };
Warrior_name order[2][5] = { {iceman,lion,wolf,ninja,dragon },
	{lion,dragon,ninja,iceman,wolf } };
string color[2] = { "red","blue" };
string names[5] = { "dragon", "ninja", "iceman", "lion", "wolf" };
string weapons[3] = { "sword","bomb","arrow" };

//��ӡʱ��
void print_time() {
	cout << setfill('0') << setw(3) << hour << ":"
		<< setfill('0') << setw(2) << minute << " ";
}

class Weapon;
class Headquarter;
class City;
class Warrior;

//������//////////////////////////////////////////////////////////////////////////////////////////
class Weapon {
private:
	int name_, sword_force_, arrow_usetime_; //�������֣�sword��������arrowʣ��ʹ�ô���
public:
	Weapon(int name, int sword_force) //weapon���캯��
		:name_(name), sword_force_(sword_force), arrow_usetime_(3) {}
	int& get_usetime() { return arrow_usetime_; } //�ӿڣ����arrowʣ��ʹ�ô���
	int get_sword_force() { return sword_force_; } //�ӿڣ����sword������
	bool sword_blunt() { //arrow��ۺ���
		sword_force_ = int(sword_force_ * 0.8);
		return sword_force_;
	}
};

//������//////////////////////////////////////////////////////////////////////////////////////////
class City {
private:
	//��ţ�����(-1��ʾ����)������ʤ������������Ԫ
	int number_, flag_, war_win_time_[2], elements_;
	//�����е���ʿ
	Warrior* warriors[2];
public:
	City(int number);
	int get_num() { return number_; } //�ӿ�
	int get_elements() { return elements_; } //�ӿ�
	bool if_warrior(int i) { return warriors[i]; } //�ӿ�
	void lion_flee(); //warrior�Ľӿڣ�����ʨ������
	void forward(City* last, int i); //����һ��city�ƶ�����
	void move_to_head(Headquarter* headquarter, int i); //�ƶ����з�˾�
	void move_from_head(Warrior* warrior, int i); //��˾��ƶ���city
	void warrior_forward(); //warrior�Ľӿڣ�warriorǰ�����
	void add_elements() { elements_ += 10; }
	void gain_elements(); //���ڵ���warrior��ȡ����Ԫ
	void shot_arrow(City* next[]); //����arrow�¼�
	void bomb_deal(); //����bomb�¼�
	void fight_judge(); //�ж�˭�����������ҵ���fight����
	void set_flag(int i); //���ı任
	void gain_reward(int i); //�ӿڣ���ʿ��˾���ý���
	void warrior_report(int i); //�ӿڣ���ʿ�㱨�������
	void praise_warrior(); //������ʤ��ʿ����Ԫ˳�㴦����������ʿ
};

//˾����//////////////////////////////////////////////////////////////////////////////////////////
class Headquarter {
private:
	//��ʿ����������Ԫ�����˽������������Ӫ
	int warrior_num_, elements_, enemy_num_, color_;
	bool if_enter; //�Ƿ��е��˽���
	Warrior* warrior;
	Warrior* enemys[2];
public:
	Headquarter(int elements, int color);
	int get_color(){ return color_; }; //�ӿ�
	void born(); //��ʿ����
	void lion_flee(); //˾��������ܵ�ʨ��
	void move_to_city(City* city, int i); //city�Ľӿڣ���˾��ƶ���city
	void enemy_enter(Warrior* enemy); //�з�warrior����
	void warrior_forward(); //warrior�Ľӿڣ��з�warrior�������
	bool is_occupied(); //˾��Ƿ�ռ��
	void gain_elements(int add_elements) { //˾�����Ԫ����
		elements_ += add_elements;
	}
	int& get_elements() { return elements_; } //�ӿ�
	void report_elements(); //��������Ԫ
	void report_weapon(); //warrior�Ľӿڣ������enemy��������
	~Headquarter();
};

//��ʿ��//////////////////////////////////////////////////////////////////////////////////////////
class Warrior {
private:
	//����ֵ����ţ����������ҳ϶�(���lion)������(���ninja)������ֵ�ĸ���
	int strength_, number_, force_, loyalty_, step_, l_life;
	double morale_; //ʿ��(���dragon)
	bool if_kill, is_win; //�Ƿ�ɱ�����ˣ��Ƿ��ʤ
	Warrior_name name_;
	Weapon* weapon_[3];
	City* city_;
	Headquarter* headquarter_;
public:
	Warrior(Warrior_name& name, int number, int& strength, Headquarter* headquarter);
	int get_strength() { return strength_; } //�ӿ�
	bool& get_is_win() { return is_win; } //�ӿ�
	bool lion_flee(); //ʨ���������
	void warrior_forward(bool if_arrive); //��ʿǰ�����
	void gain_elements(int add_elements); //��ʿ�ӳ���ȡ������Ԫ���
	void shot_arrow(); //������
	void is_shoted(); //�Ƿ��������
	void change_city(City* next) { city_ = next; } //�ı����
	void morale_change() { is_win ? morale_ += 0.2 : morale_ -= 0.2; } //ʿ���ı�
	bool get_weapon(Weapon_name name) { return weapon_[name]; } //�ӿ�
	void use_bomb(Warrior* enemy); //ʹ��bomb���
	int fight_if_win(Warrior* enemy); //���������Ƿ��ʤ��
	bool if_bomb(Warrior* enemy, int i); //�Ƿ�ʹ��bomb
	void fight(Warrior* enemy); //ս�����
	void after_fight(Warrior* enemy); //ս����
	void dead(); //�������
	void gain_reward(); //սʤ���˾���ý���
	void get_gain_elements(int add_elements) { //headquarter�Ľӿڣ�˾���������Ԫ
		headquarter_->get_elements() += add_elements;
	}
	void loyalty_reduce() { loyalty_ -= loyal_reduce; } //�ҳ϶ȸı�
	void report_weapon(); //��������������
	~Warrior();
};

//��ʼ��Ϸ////////////////////////////////////////////////////////////////////////////////////////
void Game_start() {
	cin >> Case;
	for (int i = 1; i <= Case; ++i) {
		cin >> total_elements >> city_total_num >> arrow_force >> loyal_reduce >> total_time;
		for (int i = 0; i < 5; ++i) { cin >> life[i]; }
		for (int i = 0; i < 5; ++i) { cin >> attach[i]; }
		//�������е�vector
		vector<City> cities;
		for (int i = 0; i < city_total_num; ++i) {
			City temp(i + 1);
			cities.push_back(temp);
		}
		//����˾���0��ʾ�췽��1��ʾ����
		Headquarter head[2] = { Headquarter(total_elements,0),Headquarter(total_elements,1) };
		cout << "Case " << case_num++ << ":\n";
		hour = 0; minute = 0;
		while (hour * 60 + minute <= total_time) {
			head[0].born(); head[1].born(); minute = 5; //��0������ʿ����
			if (hour * 60 + minute > total_time) break; //��������涨ʱ��������ѭ��
			else {
				head[0].lion_flee(); //�жϺ췽˾���lion�Ƿ�����
				for (vector<City>::iterator i = cities.begin(); i != cities.end(); i++) {
					(*i).lion_flee();
				} //�ж�ÿ�����е�lion�Ƿ�����
				head[1].lion_flee(); //�ж�����˾���lion�Ƿ�����
			}
			minute = 10;
			if (hour * 60 + minute > total_time) break; //��10����
			else {
				if (cities[0].if_warrior(1)) cities[0].move_to_head(&head[0], 1); //��ɫ��ʿ�����ɫ˾�
				for (int i = 0; i < city_total_num - 1; ++i)
					cities[i].forward(&cities[i + 1], 1); //��ɫ��ʿ��ǰ�ƶ�
				head[1].move_to_city(&cities[city_total_num - 1], 1); //��ɫ��ʿ��˾��ƶ�������		
				if (cities[city_total_num - 1].if_warrior(0)) cities[city_total_num - 1].move_to_head(&head[1], 0); //��ɫ��ʿ������ɫ˾�
				for (int i = city_total_num - 1; i > 0; --i)
					cities[i].forward(&cities[i - 1], 0); //��ɫ��ʿ��ǰ�ƶ�
				head[0].move_to_city(&cities[0], 0); //��ɫ��ʿ��˾��ƶ�������
				//��ʿ�ƶ������
				head[0].warrior_forward(); //������ʿ����췽˾����
				bool p = head[0].is_occupied(); //�췽�Ƿ�ռ���Լ�������
				for (int i = 0; i < city_total_num; ++i) cities[i].warrior_forward(); //ÿ�����е���ʿǰ�����
				head[1].warrior_forward(); //�췽��ʿ��������˾����
				if (p + head[1].is_occupied()) break; //�����Ƿ�ռ���Լ�������
			}
			minute = 20;
			if (hour * 60 + minute > total_time) break; //��20����
			else { for (int i = 0; i < city_total_num; ++i) cities[i].add_elements(); } //ÿ�����в���10������Ԫ
			minute = 30;
			if (hour * 60 + minute > total_time) break; //��30����
			else { for (int i = 0; i < city_total_num; ++i) cities[i].gain_elements(); } //ÿ�������ڵ�����ʿ��ȡ����Ԫ
			minute = 35;
			if (hour * 60 + minute > total_time) break; //��35����
			else {
				if (city_total_num > 1) { //������������1�Żᷢ��arrow�¼�
					City* next[2] = {};
					//�����ڵ�һ�������е���ʿ������ʿ�����Ŀ��Ϊ�ڶ��������е���ʿ������ʿ�����Ŀ����ΪNULL
					next[0] = NULL; next[1] = &cities[1];
					cities[0].shot_arrow(next); 
					//���ڵ�i�������е���ʿ(1<i<N)������ʿ�����Ŀ��Ϊ��i+1�������е���ʿ������ʿ��Ŀ����Ϊ��i-1�������е���ʿ
					for (int i = 1; i < city_total_num - 1; ++i) {
						next[0] = &cities[i - 1];
						next[1] = &cities[i + 1];
						cities[i].shot_arrow(next);
					}
					//���������һ�������е���ʿ������ʿ�����Ŀ��ΪNULL������ʿ�����Ŀ����Ϊ�����ڶ��������е���ʿ
					next[0] = &cities[city_total_num - 2]; next[1] = NULL;
					cities[city_total_num - 1].shot_arrow(next);
				}
			}
			minute = 38;
			if (hour * 60 + minute > total_time) break; //��38����
			else {
				for (vector<City>::iterator i = cities.begin(); i != cities.end(); ++i) { i->bomb_deal(); }
			} //ÿ�����д���bomb�¼�
			minute = 40;
			if (hour * 60 + minute > total_time) break; //��40����
			else { //����ÿ������
				//�ж��Ƿ�Ҫ��ս
				for (vector<City>::iterator i = cities.begin(); i != cities.end(); ++i) { (*i).fight_judge(); }
				//ÿ�����еĺ���ʿ�Ƿ���˾���ս����
				for (vector<City>::iterator i = cities.begin(); i != cities.end(); ++i) { (*i).gain_reward(0); }
				//ÿ�����е�����ʿ�Ƿ���˾���ս����
				for (vector<City>::reverse_iterator i = cities.rbegin(); i != cities.rend(); ++i) { (*i).gain_reward(1); }
				//ÿ�������л�ʤ����ʿ��øó��е�����Ԫ
				for (vector<City>::iterator i = cities.begin(); i != cities.end(); ++i) { (*i).praise_warrior(); }
			}
			minute = 50;
			if (hour * 60 + minute > total_time) break; //��50����
			else { head[0].report_elements(); head[1].report_elements(); } //˾��㱨����Ԫ
			minute = 55;
			if (hour * 60 + minute > total_time) break; //��55����
			else {
				//���еĺ���ʿ��˳��㱨�������
				for (vector<City>::iterator i = cities.begin(); i != cities.end(); ++i) { (*i).warrior_report(0); }
				head[1].report_weapon(); 
				//���е�����ʿ��˳��㱨�������
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
void City::lion_flee() { //warrior�Ľӿڣ�����ʨ������
	for (int i = 0; i < 2; ++i) {
		if (warriors[i]) {
			if (warriors[i]->lion_flee()) {
				delete warriors[i]; warriors[i] = NULL;
			}
		}
	}
}
void City::forward(City* last, int i) { //����һ��city�ƶ�����
	warriors[i] = last->warriors[i];
	if (warriors[i])warriors[i]->change_city(this); //���б�Ϊthis
}
void City::move_to_head(Headquarter* headquarter, int i) { //�ƶ����з�˾�
	headquarter->enemy_enter(warriors[i]); //�з�˾�����һ����ʿ
	warriors[i] = NULL; //����������ƶ������ʿ
}
void City::move_from_head(Warrior* warrior, int i) { //��˾��ƶ���city
	warriors[i] = warrior;
	if (warriors[i])warriors[i]->change_city(this); //���б�Ϊthis
}
void City::warrior_forward() { //warrior�Ľӿڣ�warriorǰ�����
	if (warriors[0]) warriors[0]->warrior_forward(false);
	if (warriors[1]) warriors[1]->warrior_forward(false);
}
void City::gain_elements() { //���ڵ���warrior��ȡ����Ԫ
	for (int i = 0; i < 2; ++i)
		if ((warriors[i]) && (!warriors[1 - i])) {
			warriors[i]->gain_elements(elements_);
			elements_ = 0; break; //��������Ԫ����
		}
}
void City::shot_arrow(City* next[]) { //����arrow�¼�
	for (int i = 0; i < 2; ++i) {
		if ((warriors[i]) && (warriors[i]->get_weapon(arrow))) {
			if ((next[1 - i]) && (next[1 - i]->warriors[1 - i])) {
				warriors[i]->shot_arrow(); //���
				(next[1 - i]->warriors[1 - i])->is_shoted(); //����
			}
		}
	}
}
void City::bomb_deal() { //����bomb�¼�
	if (warriors[0] && warriors[1]) {
		bool if_use_bomb = false;
		for (int i = 0; i <= 1; i++) {
			if (warriors[i]->get_weapon(bomb)) {
				if (flag_ == -1) if_use_bomb = warriors[i]->if_bomb(warriors[1 - i], number_); //���������
				else if_use_bomb = warriors[i]->if_bomb(warriors[1 - i], flag_ + 1); //���������
				if (if_use_bomb) {
					delete warriors[0]; warriors[1] = NULL;
					delete warriors[1]; warriors[0] = NULL;
					break;
				}
			}
		}
		if (!if_use_bomb && warriors[0]->get_strength() <= 0 && warriors[1]->get_strength() <= 0) { 
			//�����ֻ��ʿ��������
			delete warriors[0]; warriors[0] = NULL;
			delete warriors[1]; warriors[1] = NULL;
		}
	}
}
void City::fight_judge() { //�ж�˭�����������ҵ���fight����
	if (warriors[0] && warriors[1]) {
		if (flag_ == -1) { //���û������
			if (number_ % 2)warriors[0]->fight(warriors[1]); //ż����ų��к췽����
			else warriors[1]->fight(warriors[0]); //������ų�����������
		}
		else warriors[flag_]->fight(warriors[1 - flag_]); //�����ĵ���������
	}
}
void City::set_flag(int i) { //���ı任
	if (i == -1) { war_win_time_[0] = 0; war_win_time_[1] = 0; }
	else {
		++war_win_time_[i]; war_win_time_[1 - i] = 0;
		//�����������
		if (war_win_time_[i] == 2 && flag_ != i) {
			flag_ = i;
			print_time();
			cout << color[i] << " flag raised in city " << number_ << endl;
		}
	}
}
void City::gain_reward(int i) { //�ӿڣ���ʿ��˾���ý���
	if (warriors[i]) warriors[i]->gain_reward();
}
void City::praise_warrior() { //������ʤ��ʿ����Ԫ˳�㴦����������ʿ
	for (int i = 0; i < 2; ++i)
		if (warriors[i]) {
			if (warriors[i]->get_is_win()) {
				warriors[i]->get_gain_elements(elements_); //���ʤ��˾���������Ԫ
				elements_ = 0; //���ó��е�����Ԫ
				warriors[i]->get_is_win() = false; //����ʤ������
			}
			else if ((warriors[i]->get_strength()) <= 0) {
				delete warriors[i];
				warriors[i] = NULL;
			}
		}
}
void City::warrior_report(int i) { //�ӿڣ���ʿ�㱨�������
	if (warriors[i]) warriors[i]->report_weapon(); 
}
Headquarter::Headquarter(int elements, int color)
	:elements_(elements), color_(color) {
	enemys[0] = NULL; enemys[1] = NULL; if_enter = false;
	warrior_num_ = 0; enemy_num_ = 0; warrior = NULL;
}
void Headquarter::born() { //��ʿ����
	if (elements_ >= life[order[color_][warrior_num_ % 5]]) {
		elements_ -= life[order[color_][warrior_num_ % 5]]; //˾�����Ԫ����
		print_time();
		cout << color[color_] << " ";
		//����һ��warrior
		warrior = new Warrior(order[color_][warrior_num_ % 5], warrior_num_ + 1, life[order[color_][warrior_num_ % 5]], this);
		warrior_num_++;
	}
}
void Headquarter::lion_flee() { //˾��������ܵ�ʨ��
	if (warrior && warrior->lion_flee()) {
		delete warrior; warrior = NULL;
	}
}
void Headquarter::move_to_city(City* city, int i) { //city�Ľӿڣ���˾��ƶ���city
	city->move_from_head(warrior, i);
	warrior = NULL;
}
void Headquarter::enemy_enter(Warrior* enemy) { //�з�warrior����
	enemys[enemy_num_] = enemy;
	if_enter = true;
	++enemy_num_;
}
void Headquarter::warrior_forward() { //warrior�Ľӿڣ��з�warrior�������
	if (if_enter) {
		enemys[enemy_num_ - 1]->warrior_forward(true);
		if_enter = false; //���ñ���
	}
}
bool Headquarter::is_occupied() { //˾��Ƿ�ռ��
	if (enemy_num_ == 2) {
		print_time();
		cout << color[color_] << " headquarter was taken" << endl;
		return true;
	}
	return false;
}
void Headquarter::report_elements() { //��������Ԫ
	print_time();
	cout << elements_ << " elements in " << color[color_] << " headquarter" << endl;
}
void Headquarter::report_weapon() { //warrior�Ľӿڣ������enemy��������
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
	switch (name_) { //������ʿ���������벻ͬ������
	case dragon:case iceman:weapon_[number_ % 3] = new Weapon(number_ % 3, int(attach[name_] * 0.2)); break;
	case ninja:weapon_[number_ % 3] = new Weapon(number_ % 3, int(attach[name_] * 0.2));
		weapon_[(number_ + 1) % 3] = new Weapon((number_ + 1) % 3, int(attach[name_] * 0.2)); break;
	default:break;
	}
	morale_ = double(headquarter_->get_elements()) / double(strength_); //��ʼ��ʿ��
	loyalty_ = headquarter_->get_elements(); //��ʼ���ҳ϶�
	city_ = NULL;
	if (weapon_[sword] && !(weapon_[sword]->get_sword_force())) weapon_[sword] = NULL;//����ʼ��������Ϊ0����delete
	cout << names[name_] << " " << number_ << " born" << endl;
	if (name_ == lion) { cout << "Its loyalty is " << loyalty_ << endl; }
	if (name_ == dragon) { cout << fixed << setprecision(2) << "Its morale is " << morale_ << endl; }
}
bool Warrior::lion_flee() { //ʨ���������
	if ((name_ == lion) && (loyalty_ <= 0)) {
		print_time();
		cout << color[headquarter_->get_color()] << " lion " << number_ << " ran away" << endl;
		return true;
	}
	return false;
}
void Warrior::warrior_forward(bool if_arrive) { //��ʿǰ�����
	++step_;
	if ((name_ == iceman) && (step_ % 2 == 0)) { //����ninja�����
		if (strength_ > 9) strength_ -= 9;
		else strength_ = 1;
		force_ += 20;
	}
	print_time();
	cout << color[headquarter_->get_color()] << " " << names[name_] << " " << number_;
	if (if_arrive) { //�������з�˾�
		cout << " reached " << color[1 - headquarter_->get_color()] << " headquarter with "
			<< strength_ << " elements and force " << force_ << endl;
	} //����
	else cout << " marched to city " << city_->get_num() << " with " << strength_ << " elements and force " << force_ << endl;
}
void Warrior::gain_elements(int add_elements) { //��ʿ�ӳ���ȡ������Ԫ���
	print_time();
	cout << color[headquarter_->get_color()] << " " << names[name_] << " " << number_ << " earned "
		<< add_elements << " elements for his headquarter" << endl;
	headquarter_->gain_elements(add_elements); //˾���������Ԫ
}
void Warrior::shot_arrow() { //������
	print_time();
	cout << color[headquarter_->get_color()] << " " << names[name_] << " " << number_ << " shot";
	weapon_[arrow]->get_usetime()--;
	if (!weapon_[arrow]->get_usetime()) { //����������������
		delete weapon_[arrow];
		weapon_[arrow] = NULL;
	}
}
void Warrior::is_shoted() { //�Ƿ��������
	strength_ -= arrow_force;
	if (strength_ <= 0) {
		cout << " and killed " << color[headquarter_->get_color()] << " " << names[name_] << " " << number_;
	}
	cout << endl;
}
void Warrior::use_bomb(Warrior* enemy) { //ʹ��bomb���
	print_time();
	cout << color[headquarter_->get_color()] << " " << names[name_] << " " << number_
		<< " used a bomb and killed " << color[enemy->headquarter_->get_color()]
		<< " " << names[enemy->name_] << " " << enemy->number_ << endl;
}
int Warrior::fight_if_win(Warrior* enemy) { //���������Ƿ��ʤ��
	if (strength_ > 0 && enemy->strength_ > 0) { //���˫����������
		int swf = 0;
		if (weapon_[sword])swf = weapon_[sword]->get_sword_force();
		if (enemy->strength_ - swf - force_ > 0) {
			if (enemy->name_ == ninja) return 0; //ƽ��
			else {
				int e_swf = 0;
				if (enemy->weapon_[sword])e_swf = enemy->weapon_[sword]->get_sword_force();
				if (strength_ - (enemy->force_ / 2) - e_swf <= 0)return -1; //��
				else return 0; //ƽ��
			}
		}
		else return 1; //ʤ��
	}
	return 0; //ƽ��
}
bool Warrior::if_bomb(Warrior* enemy, int i) { //�Ƿ�ʹ��bomb
	if ((i + headquarter_->get_color()) % 2) { //�������������
		if (fight_if_win(enemy) == -1) { use_bomb(enemy); return true; }
	}
	else { //���������������
		if (enemy->fight_if_win(this) == 1) { use_bomb(enemy); return true; }
	}
	return false;
}
void Warrior::fight(Warrior* enemy) { //��������ս�����
	Warrior* winner = NULL; Warrior* loser = NULL; //����ʤ���ߺ�ʧ����
	l_life = strength_; enemy->l_life = enemy->strength_;
	if ((enemy->strength_) > 0 && strength_ > 0) { //���˫��������
		int swf = 0;
		if (weapon_[sword]) swf = weapon_[sword]->get_sword_force();
		print_time();
		cout << color[headquarter_->get_color()] << " " << names[name_] << " " << number_ << " attacked "
			<< color[enemy->headquarter_->get_color()] << " " << names[enemy->name_] << " " << enemy->number_
			<< " in city " << city_->get_num() << " with " << strength_ << " elements and force " << force_ << endl;
		enemy->strength_ = enemy->strength_ - force_ - swf;
		if (swf && !(weapon_[sword]->sword_blunt())) { //���sword����֮�󹥻���Ϊ0��delete
			delete weapon_[sword]; weapon_[sword] = NULL;
		}
		if (enemy->strength_ > 0) { //���������������˻����ţ���ƽ��
			if (enemy->name_ == ninja) {
				city_->set_flag(-1);
				morale_change();
				loyalty_reduce();
			}
			else { //���˷���
				int e_swf = 0;
				if (enemy->weapon_[sword])e_swf = (enemy->weapon_[sword])->get_sword_force();
				print_time();
				cout << color[enemy->headquarter_->get_color()] << " " << names[enemy->name_] << " " << enemy->number_ << " fought back against "
					<< color[headquarter_->get_color()] << " " << names[name_] << " " << number_ << " in city " << city_->get_num() << endl;
				strength_ = strength_ - (enemy->force_ / 2) - e_swf;
				if (e_swf && !(enemy->weapon_[sword]->sword_blunt())) { //�������sword����֮�󹥻���Ϊ0��delete
					delete enemy->weapon_[sword]; enemy->weapon_[sword] = NULL;
				}
				if (strength_ > 0) { //��������ţ���ƽ��
					city_->set_flag(-1); morale_change(); loyalty_reduce();
					enemy->morale_change(); enemy->loyalty_reduce();
				}
				else { enemy->if_kill = true; winner = enemy; loser = this; }
			}
		}
		else { if_kill = true; winner = this; loser = enemy; }
	}
	else { //���δ����ս��ǰ��һ���Ѿ�����
		if (strength_ > 0) { enemy->l_life = 0; winner = this; loser = enemy; }
		else { l_life = 0; winner = enemy; loser = this; }
	}
	if (winner) { //winner��loserս����
		winner->after_fight(loser);
	}
	if (name_ == dragon && strength_ > 0 && morale_ > 0.8) { //dragon����
		print_time();
		cout << color[headquarter_->get_color()] << " " << names[name_] << " " << number_ << " yelled in city " << city_->get_num() << endl;
	}
	if (winner) { //winner��øĳ��е�����Ԫ
		print_time();
		cout << color[winner->headquarter_->get_color()] << " " << names[winner->name_] << " " << winner->number_
			<< " earned " << winner->city_->get_elements() << " elements for his headquarter" << endl;
		(winner->city_)->set_flag((winner->headquarter_)->get_color());
	}
}
void Warrior::after_fight(Warrior* enemy) { //ս����
	is_win = true;
	if (if_kill) { enemy->dead(); if_kill = false; } //���ɱ���˵���
	if (enemy->name_ == lion) strength_ += enemy->l_life; //lionս��������ֵת��
	if (name_ == wolf) //wolf�ɻ�����
		for (int i = 0; i < 3; ++i)
			if (enemy->weapon_[i] && !weapon_[i]) {
				weapon_[i] = enemy->weapon_[i];
				enemy->weapon_[i] = NULL;
			}
	morale_change(); //�ı�ʿ��
}
void Warrior::dead() { //�������
	print_time();
	cout << color[headquarter_->get_color()] << " " << names[name_] << " " << number_
		<< " was killed in city " << city_->get_num() << endl;
}
void Warrior::gain_reward() { //սʤ���˾���ý���
	if (is_win) {
		if (headquarter_->get_elements() > 7) {
			headquarter_->get_elements() -= 8;
			strength_ += 8;
		}
	}
}
void Warrior::report_weapon() { //��������������
	bool if_weapon = false;
	print_time();
	cout << color[headquarter_->get_color()] << " " << names[name_] << " " << number_ << " has ";
	for (int i = 2; i >= 0; --i) {
		if (weapon_[i]) {
			if (if_weapon) cout << ",";
			else if_weapon = true;
			cout << weapons[i];
			if (i == 2)cout << "(" << weapon_[arrow]->get_usetime() << ")"; //�����arrow
			if (i == 0)cout << "(" << weapon_[sword]->get_sword_force() << ")"; //�����sword
		}
	}
	if (!if_weapon)cout << "no weapon"; //���û����
	cout << endl;
}
Warrior::~Warrior() {
	for (int i = 0; i < 3; ++i) {
		if (weapon_[i])delete weapon_[i];
	}
}