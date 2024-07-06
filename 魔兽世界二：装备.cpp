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
	//����һ������ÿ���ڵ��Ӧһ����ʿ��volumes�����ʾ������ʿ������ֵ��
	//nums�����ʾ������ʿ�ı�ţ�names�����ʾ������ʿ�����ƣ�size��ʾ����Ĵ�С
	Warriors* head = NULL; //����ͷָ�룬��ʼֵΪ��
	Warriors* prev = NULL; //ǰ���ڵ�ָ�룬��ʼֵΪ��
	Warriors* current = NULL; //��ǰ�ڵ�ָ�룬��ʼֵΪ��
	for (int i = 0; i < size; i++) {
		//�������飬����������ʿ�ڵ�
		current = new Warriors; //����һ���µĽڵ㣬��̬�����ڴ�
		current->vol = volumes[i]; //���ýڵ������ֵΪvolumes�����Ӧλ�õ�ֵ
		current->num = nums[i]; //���ýڵ�ı��Ϊnums�����Ӧλ�õ�ֵ
		current->name = names[i]; //���ýڵ������Ϊnames�����Ӧλ�õ�ֵ
		current->next = NULL; //��ʼ���ڵ�ĺ�̽ڵ�ָ��Ϊ��ָ��
		if (prev == NULL) { //���ǰ���ڵ�Ϊ�գ�˵����ǰ�ڵ��ǵ�һ���ڵ�
			head = current; //������ͷָ��ָ��ǰ�ڵ�
		}
		else { //���򣬽�ǰ���ڵ�ĺ�̽ڵ�ָ��ָ��ǰ�ڵ�
			prev->next = current;
		}
		prev = current; //��ǰ���ڵ�ָ��ָ��ǰ�ڵ�
	}
	prev->next = head; //�����һ���ڵ�ĺ�̽ڵ�ָ��ָ������ͷ���γ�һ����������
	return head; //��������ͷָ��
}

bool if_next(int volume, int x) { //�Ƿ��ܹ����쵱ǰ��ʿ
	return x > volume;
}

int Case, case_n = 1; //��������
//���ڹ���ѭ�����������
int r_nums[5] = { 1,1,1,1,1 };
int b_nums[5] = { 1,1,1,1,1 };
//���ڹ���ѭ�����������
string r_names[5] = { "iceman","lion","wolf","ninja","dragon" };
string b_names[5] = { "lion","dragon","ninja","iceman","wolf" };
int dragon_v, ninja_v, iceman_v, lion_v, wolf_v; //������ʿ����ֵ

unordered_map<int, std::string>  n_to_weapon = { //��������������Ӧ
	{0, "arrow"},
	{1, "sword"},
	{2, "bomb"}
};

void weapon(Warriors* current, int volume, int n) { //�������
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
		int r_t = 0, b_t = 0; //�췢ʱ�䣬����ʱ��
		int volume, r_v, b_v, r_n = 1, b_n = 1; //����ֵ���췽����ֵ����������ֵ,�췽��ʿ��������������ʿ������
		cout << "Case:" << case_n++ << endl;
		cin >> volume;
		r_v = b_v = volume;
		cin >> dragon_v >> ninja_v >> iceman_v >> lion_v >> wolf_v;
		//���ڹ���ѭ�����������
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

		while ((!if_r_stop) || (!if_b_stop)) { //����췽�����������춼��ûֹͣ
			while (!if_r_stop) { //�췽������ʿ
				if (r_attempts >= max_attempts) {  //����췽���Դ����ﵽ���ֵ,�����������ʿ�������������޷�����
					if_r_stop = true; //�췽ֹͣ
					cout << setfill('0') << setw(3) << r_t++ << " red headquarter stops making warriors " << endl;
					break;
				}
				if (!if_next(r_v, r_current->vol)) { //��������쵱ǰ����ʿ
					cout << setfill('0') << setw(3) << r_t++ << " red " << r_current->name << " " << r_n++ << " born with strength "
						<< r_current->vol << "," << r_current->num++ << " " << r_current->name << " in red headquarter " << endl;
					r_v -= r_current->vol; //�췽˾�������Ԫ����
					weapon(r_current, r_v, r_n); //�������
					r_current = r_current->next; //׼��������һ����ʿ
					r_attempts = 0;  //���ú췽���Դ���
					break;
				}
				else { //�����ǰ��ʿ�޷�����
					r_current = r_current->next; //׼��������һ����ʿ
					r_attempts++;  //���Ӻ췽���Դ���
				}
			}

			while (!if_b_stop) { //����������ʿ
				if (b_attempts >= max_attempts) {  //�������Դ����ﵽ���ֵ
					if_b_stop = true; //����ֹͣ
					cout << setfill('0') << setw(3) << b_t++ << " blue headquarter stops making warriors " << endl;
					break;
				}
				if (!if_next(b_v, b_current->vol)) { //���ګګ�����쵱ǰ��ʿ
					cout << setfill('0') << setw(3) << b_t++ << " blue " << b_current->name << " " << b_n++ << " born with strength "
						<< b_current->vol << "," << b_current->num++ << " " << b_current->name << " in blue headquarter " << endl;
					b_v -= b_current->vol; //����˾�������Ԫ����
					weapon(b_current, b_v, b_n); //�������
					b_current = b_current->next; //׼��������һ����ʿ
					b_attempts = 0;  //�����������Դ���
					break;
				}
				else { //�����ǰ��ʿ�޷�����
					b_current = b_current->next; //׼��������һ����ʿ
					b_attempts++;  //�����������Դ���
				}
			}
		}
	}

	return 0;
}