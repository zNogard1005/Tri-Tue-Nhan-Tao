#include <iostream>
#include <set>
#include <vector>
#include <stack>
#include <cmath>

using namespace std;

const int board_rows = 3;
const int board_cols = 3;
const int empty_tile = 0;
const int max_op = 4;

struct State {
	int num[board_rows][board_cols];
	int empty_row; //luu mot dong cua o trong
	int empty_col; //kua mot cot cua o trong
};

struct Node {
	State state;
	Node* parent;
	int g; //luu gia tri duong di cua nut goc den nut hien tai
	int h; //luu ket qua uoc luong cua ham heuristic cua nut hien tai den trang thai dich
	int f; // = g + h
};
//dinh nghia phep toan so sanh trong multiset
struct node_cmp {
	bool operator() (Node* a, Node* b) {
		return a->f < b->f;
	}
};
//kiem tra hai trang thai co giong nhau khong
bool sameState(State s1, State s2) {
	if (s1.empty_col != s2.empty_col || s1.empty_row != s2.empty_row) {
		return false;
	}
	for (int row = 0; row < board_rows; row++) {
		for (int col = 0; col < board_cols; col++) {
			if (s1.num[row][col] != s2.num[row][col]) {
				return false;
			}
		}
	}
	return true;
}
//phep toan di chuyen o trong len tren
bool up(State s, State& out) {
	int er = s.empty_row, ec = s.empty_col;
	if (er > 0) {
		out = s;
		out.empty_col = ec;
		out.empty_row = er - 1;
		out.num[er][ec] = s.num[er - 1][ec];
		out.num[er - 1][ec] = empty_tile;

		return true;
	}
	return false;
}
//phep toan di chuyen o trong xuong duoi
bool down(State s, State& out) {
	int er = s.empty_row, ec = s.empty_col;
	if (er < 2) {
		out = s;
		out.empty_col = ec;
		out.empty_row = er + 1;
		out.num[er][ec] = s.num[er + 1][ec];
		out.num[er + 1][ec] = empty_tile;

		return true;
	}
	return false;
}
//phep toan di chuyen o qua trai
bool left(State s, State& out) {
	int er = s.empty_row, ec = s.empty_col;
	if (ec > 0) {
		out = s;
		out.empty_col = ec - 1;
		out.empty_row = er;
		out.num[er][ec] = s.num[er][ec - 1];
		out.num[er][ec - 1] = empty_tile;

		return true;
	}
	return false;
}
bool right(State s, State& out) {
	int er = s.empty_row, ec = s.empty_col;
	if (ec < 2) {
		out = s;
		out.empty_col = ec + 1;
		out.empty_row = er;
		out.num[er][ec] = s.num[er][ec + 1];
		out.num[er][ec + 1] = empty_tile;

		return true;
	}
	return false;
}
//ham sinh cac phep toan
bool call_operator(State s, State& out, int op_no) {
	switch (op_no) {
	case 1: return up(s, out);
	case 2: return down(s, out);
	case 3: return left(s, out);
	case 4: return right(s, out);
	}
}
//in trang thai s
void print_state(State s) {
	for (int i = 0; i < board_rows; i++) {
		for (int j = 0; j < board_cols; j++) {
			cout << s.num[i][j] << " ";
		}
		cout << "\n";
	}
}
//kiem tra trang thai co phai la goal?
bool is_goal(State s, State goal) {
	return sameState(s, goal);
}
//ham h1 dem so vi tri sai khac
int h1(State s, State s2) {
	int count = 0;
	for (int row = 0; row < board_rows; row++) {
		for (int col = 0; col < board_cols; col++) {
			if (s.num[row][col] != s2.num[row][col]) {
				count++;
			}
		}
	}
	return count;
}
int h2(State s, State s2) {
	int number[9]; // luu cac so sai
	int pos_row_x[9]; //vi tri row sai cua s1
	int pos_col_y[9]; //vi tri col sai cua s1

	int goalpos_row_x[9]; //vi tri row sai cua s2
	int goalpos_col_y[9]; //vi tri col sai cua s2

	int count1 = 0;
	for (int row = 0; row < board_rows; row++) {
		for (int col = 0; col < board_cols; col++) {
			if (s.num[row][col] != s2.num[row][col] && s.num[row][col] != 0) {
				number[count1] = s.num[row][col];
				pos_row_x[count1] = row;
				pos_col_y[count1] = col;
				count1++;
			}
		}
	}
	int count2 = 0;
	for (int i = 0; i < count1; i++) {
		for (int row = 0; row < board_rows; row++) {
			for (int col = 0; col < board_cols; col++) {
				if (s2.num[row][col] == number[i]) {
					goalpos_row_x[count2] = row;
					goalpos_col_y[count2] = col;
					count2++;
				}
			}
		}
	}


	int sum = 0;
	for (int i = 0; i < count1; i++) {
		sum += abs(posx[i] - goalposx[i]) + abs(posy[i] - goalposy[i]);
	}
	return sum;
}
//search phan tu da co trong frontier?
Node* find_node(State s, multiset<Node*, node_cmp> list) {
	for (Node* n : list) {
		if (sameState(s, n->state)) {
			return n;
		}
	}
	return NULL;
}
//search phan tu da co trong explore?
bool find_state(State s, vector <State>* explored) {
	for (State c1 : *explored) {
		if (sameState(s, c1)) {
			return true;
		}
	}
}
//nhap trang thai cho bai toan
State* getState() {
	State* s = new State();
	for (int row = 0; row < board_rows; row++) {
		for (int col = 0; col < board_rows; col++) {
			cin >> s->num[row][col];
			if (s->num[row][col] == 0) {
				s->empty_row = row;
				s->empty_col = col;
			}
		}
	}
	return s;
}
Node* A_star(State init_state, State goal_state, vector<State>* explored) {
	//initial
	Node* root = new Node();
	root->state = init_state;
	root->parent = NULL;
	root->g = 0;
	root->h = h1(init_state, goal_state);
	root->f = root->g + root->h;
	//frontiers la tap Open duoc sap thu tu
	multiset<Node*, node_cmp>frontiers;
	frontiers.insert(root);
	while (!frontiers.empty()) {
		Node* node = *frontiers.begin();
		frontiers.erase(frontiers.begin());
		explored->push_back(node->state);
		if (sameState(node->state, goal_state)) {
			return node;
		}
		//genarate states
		for (int op = 1; op <= 4; op++) {
			State new_state;
			if (call_operator(node->state, new_state, op)) {
				if (find_state(new_state, explored)) {
					continue;
				}
				Node* n = find_node(new_state, frontiers);
				if (n == NULL) {
					n = new Node();
					n->parent = node;
					n->state = new_state;
					n->h = h1(new_state, goal_state);
					print_state(new_state);
					n->g = node->g + 1;
					n->f = n->g + n->h;
					cout << "==== Gia tri g: " << n->g << "==== Gia tri f: " << n->f << endl;
					frontiers.insert(n);
				}
				else {
					//neu nut con moi tim da thuoc duong bien
					//kiem tra gia tri f cua nut can co nho hon cac nut da nam trong frontier ko?
					//new nho hon thi cap nhat lai, neu lon hon thi ko lm j ca
					n->g = node->g + 1;
					n->f = n->g + n->h;
				}
			}
		}
	}
	return NULL;
}
Node* A_star2(State init_state, State goal_state, vector<State>* explored) {
	Node* root = new Node();
	root->state = init_state;
	root->parent = NULL;
	root->g = 0;
	root->h = h2(init_state, goal_state);
	root->f = root->g + root->h;
	multiset<Node*, node_cmp> frontiers;
	frontiers.insert(root);
	while (!frontiers.empty()) {
		Node* node = *frontiers.begin();
		frontiers.erase(frontiers.begin());
		explored->push_back(node->state);
		if (sameState(node->state, goal_state)) return node;

		for (int op = 1; op <= 4; op++) {
			State new_state;
			if (call_operator(node->state, new_state, op)) {
				if (find_state(new_state, explored)) continue;
				Node* n = find_node(new_state, frontiers);
				if (n == NULL) {
					n = new Node();
					n->parent = node;
					n->state = new_state;
					n->h = h2(new_state, goal_state);
					print_state(new_state);
					n->g = node->g + 1;
					n->f = n->g + n->h;
					cout << "==== Gia tri g: " << n->g << "==== Gia tri f: " << n->f << endl;
					frontiers.insert(n);
				}
				else {
					n->g = node->g + 1;
					n->f = n->g + n->h;
				}
			}
		}
	}
	return NULL;
}
void print_path(Node* r) {
	int i = 0;
	stack<State> q;
	cout << "\nDuong di loi giai" << endl;
	while (r->parent != NULL) {
		q.push(r->state);
		r = r->parent;
		//i++;
	}
	q.push(r->state);
	while (!q.empty()) {
		cout << "Trang thai thu " << i++ << endl;
		print_state(q.top());
		cout << endl;
		q.pop();
	}
}
int main() {
	int n;
	cout << "Nhap 1 de thuc hien tim kiem Heuristic. \nNhap 2 de thuc hien tim kiem Mathattan. \n Gia tri nhap: ";
	cin >> n;
	cout << "Nhap trang thai dau: " << endl;
	State* news = getState();
	cout << "Trang thai dau: " << endl;
	print_state(*news);
	cout << "Nhap trang thai goal: " << endl;
	State* newg = getState();
	cout << "Trang thai goal: "; << endl;
	print_state(*newg);
	cout << endl;
	vector<State> ex1;
	if (n == 1) {
		cout << "Tim kiem Heuristic " << endl;
		Node* a = A_star(*news, *newg, &ex1);
		print_path(a);
	}
	if (n == 2) {
		cout << "Tim kiem Manhattan " << endl;
		Node* b = A_star2(*news, *newg, &ex1);
		print_path(b);
	}
	return 0;
}