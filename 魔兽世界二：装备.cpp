#include<iostream>
#include<iomanip>
#include<string>
#include<unordered_map>

using namespace std;

struct Warriors {
	int num;
	int vol;
	string name;
	Warriors* next;
};

struct Warriors* createList(int volumes[], int nums[], string names[], int size) {
	//创建一个链表，每个节点对应一个武士，volumes数组表示各种武士的生命值，
	//nums数组表示各种武士的编号，names数组表示各种武士的名称，size表示数组的大小
	Warriors* head = NULL; //链表头指针，初始值为空
	Warriors* prev = NULL; //前驱节点指针，初始值为空
	Warriors* current = NULL; //当前节点指针，初始值为空
	for (int i = 0; i < size; i++) {
		//遍历数组，创建各个武士节点
		current = new Warriors; //创建一个新的节点，动态分配内存
		current->vol = volumes[i]; //设置节点的生命值为volumes数组对应位置的值
		current->num = nums[i]; //设置节点的编号为nums数组对应位置的值
		current->name = names[i]; //设置节点的名称为names数组对应位置的值
		current->next = NULL; //初始化节点的后继节点指针为空指针
		if (prev == NULL) { //如果前驱节点为空，说明当前节点是第一个节点
			head = current; //将链表头指针指向当前节点
		}
		else { //否则，将前驱节点的后继节点指针指向当前节点
			prev->next = current;
		}
		prev = current; //将前驱节点指针指向当前节点
	}
	prev->next = head; //将最后一个节点的后继节点指针指向链表头，形成一个环形链表
	return head; //返回链表头指针
}

bool if_next(int volume, int x) { //是否能够制造当前武士
	return x > volume;
}

int Case, case_n = 1; //例子数量
//用于构造循环链表的数组
int r_nums[5] = { 1,1,1,1,1 };
int b_nums[5] = { 1,1,1,1,1 };
//用于构造循环链表的数组
string r_names[5] = { "iceman","lion","wolf","ninja","dragon" };
string b_names[5] = { "lion","dragon","ninja","iceman","wolf" };
int dragon_v, ninja_v, iceman_v, lion_v, wolf_v; //各个武士生命值

unordered_map<int, std::string>  n_to_weapon = { //将武器和序号相对应
	{0, "arrow"},
	{1, "sword"},
	{2, "bomb"}
};

void weapon(Warriors* current, int volume, int n) { //武器输出
	if (current->name == "dragon") {
		cout << "It has a " << n_to_weapon[n % 3] << ",and it's morale is "
			<< fixed << setprecision(2) << double(volume) / double(dragon_v) << endl;
	}
	if (current->name == "ninja") {
		cout << "It has a " << n_to_weapon[n % 3] << " and a " << n_to_weapon[(n + 1) % 3] << endl;
	}
	if (current->name == "iceman") {
		cout << "It has a " << n_to_weapon[n % 3] << endl;
	}
	if (current->name == "lion") {
		cout << "It's loyalty is " << volume << endl;
	}
	return;
}

int main() {
	cin >> Case;
	for (int i = 1; i <= Case; i++) {
		int r_t = 0, b_t = 0; //红发时间，蓝方时间
		int volume, r_v, b_v, r_n = 1, b_n = 1; //生命值，红方生命值，蓝方生命值,红方武士总数量，蓝方武士总数量
		cout << "Case:" << case_n++ << endl;
		cin >> volume;
		r_v = b_v = volume;
		cin >> dragon_v >> ninja_v >> iceman_v >> lion_v >> wolf_v;
		//用于构造循环链表的数组
		int r_volumes[5] = { iceman_v, lion_v, wolf_v, ninja_v, dragon_v };
		int b_volumes[5] = { lion_v, dragon_v, ninja_v, iceman_v, wolf_v };
		bool if_r_stop = false;
		bool if_b_stop = false;

		Warriors* r_warriors = createList(r_volumes, r_nums, r_names, 5);
		Warriors* r_current = r_warriors;
		Warriors* b_warriors = createList(b_volumes, b_nums, b_names, 5);
		Warriors* b_current = b_warriors;

		int max_attempts = 5;
		int r_attempts = 0;
		int b_attempts = 0;

		while ((!if_r_stop) || (!if_b_stop)) { //如果红方和蓝方的制造都还没停止
			while (!if_r_stop) { //红方制造武士
				if (r_attempts >= max_attempts) {  //如果红方尝试次数达到最大值,则表明所有武士都尝试制造且无法制造
					if_r_stop = true; //红方停止
					cout << setfill('0') << setw(3) << r_t++ << " red headquarter stops making warriors " << endl;
					break;
				}
				if (!if_next(r_v, r_current->vol)) { //如果能制造当前的武士
					cout << setfill('0') << setw(3) << r_t++ << " red " << r_current->name << " " << r_n++ << " born with strength "
						<< r_current->vol << "," << r_current->num++ << " " << r_current->name << " in red headquarter " << endl;
					r_v -= r_current->vol; //红方司令部总生命元减少
					weapon(r_current, r_v, r_n); //武器输出
					r_current = r_current->next; //准备制造下一个武士
					r_attempts = 0;  //重置红方尝试次数
					break;
				}
				else { //如果当前武士无法制造
					r_current = r_current->next; //准备制造下一个武士
					r_attempts++;  //增加红方尝试次数
				}
			}

			while (!if_b_stop) { //蓝方制造武士
				if (b_attempts >= max_attempts) {  //蓝方尝试次数达到最大值
					if_b_stop = true; //蓝方停止
					cout << setfill('0') << setw(3) << b_t++ << " blue headquarter stops making warriors " << endl;
					break;
				}
				if (!if_next(b_v, b_current->vol)) { //如果讷讷感制造当前武士
					cout << setfill('0') << setw(3) << b_t++ << " blue " << b_current->name << " " << b_n++ << " born with strength "
						<< b_current->vol << "," << b_current->num++ << " " << b_current->name << " in blue headquarter " << endl;
					b_v -= b_current->vol; //蓝方司令部总生命元减少
					weapon(b_current, b_v, b_n); //武器输出
					b_current = b_current->next; //准备制造下一个武士
					b_attempts = 0;  //重置蓝方尝试次数
					break;
				}
				else { //如果当前武士无法制造
					b_current = b_current->next; //准备制造下一个武士
					b_attempts++;  //增加蓝方尝试次数
				}
			}
		}
	}

	return 0;
}