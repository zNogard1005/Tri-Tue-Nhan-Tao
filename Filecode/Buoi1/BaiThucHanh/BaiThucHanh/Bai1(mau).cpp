
/*	Ho va ten: VO NGOC LONG
*	MSSV: B1812282
*	BAI TAP 1
*/
#include <iostream>

//Bai tap DONG NUOC
#define tankcapacity_X 9 // Suc chua binh x
#define tankcapacity_Y 4 // Suc chua binh y
#define empty 0
#define goal 6
#define Maxlength 100
using namespace std;

const char* action[] = { "First State", "Pour Water Full X", "Pour Water Full Y",
						 "Pour Water Empty X", "Pour Water Empty Y",
						 "Pour Water X to Y", "Pour Water Y to X" };

//Khai bao cau truc trang thai
typedef struct {
	int x, y; // Luong nuoc binh x, y
}State;

//Khoi tao trang thai X=0, Y=0
void Make_Null_State(State* state) {
	state->x = 0;
	state->y = 0;
}

//In trang thai cua binh X, Y
void Print_state(State state) {
	cout << "X = " << state.x << ", Y = " << state.y << endl;
}

//Kiem tra trang thai muc tieu
int goal_check(State state) {
	return (state.x == goal || state.y == goal);
}

//Lam day binh X, Y
int pour_Water_Full_X(State cur_state, State *result) {
	if (cur_state.x < tankcapacity_X) {
		result->x = tankcapacity_X;
		result->y = cur_state.y;
		return 1;
	}
	return 0;
}
int pour_Water_Full_Y(State cur_state, State* result) {
	if (cur_state.y < tankcapacity_Y) {
		result->y = tankcapacity_Y;
		result->x = cur_state.x;
		return 1;
	}
	return 0;
}

//Lam rong binh X, Y
int pour_Water_Empty_X(State cur_state, State* result) {
	if (cur_state.x < tankcapacity_X) {
		result->x = empty;
		result->y = cur_state.y;
		return 1;
	}
	return 0;
}
int pour_Water_Empty_Y(State cur_state, State* result) {
	if (cur_state.y < tankcapacity_X) {
		result->y = empty;
		result->x = cur_state.x;
		return 1;
	}
	return 0;
}
int Max(int x, int y) {
	return x > y ? x : y;
}
int Min(int x, int y) {
	return x < y ? x : y;
}

//Chuyen nuoc tu X->Y
int pour_Water_XY(State cur_state, State* result) {
	if (cur_state.x > 0 && cur_state.y < tankcapacity_Y) {
		result->x = Max(cur_state.x - (tankcapacity_Y - cur_state.y), empty);
		result->y = Min(cur_state.x + cur_state.y, tankcapacity_Y);
		return 1;
	}
	return 0;
}
int pour_Water_YX(State cur_state, State* result) {
	if (cur_state.y > 0 && cur_state.x < tankcapacity_X) {
		result->y = Max(cur_state.y - (tankcapacity_X - cur_state.x), empty);
		result->x = Min(cur_state.y + cur_state.x, tankcapacity_X);
		return 1;
	}
	return 0;
}

//Goi cac phep toan tren trang thai
int Call_operator(State cur_state, State* result, int option) {
	switch (option) 
	{
		case 1: return pour_Water_Full_X(cur_state, result);
		case 2: return pour_Water_Full_Y(cur_state, result);
		case 3: return pour_Water_Empty_X(cur_state, result);
		case 4: return pour_Water_Empty_Y(cur_state, result);
		case 5: return pour_Water_XY(cur_state, result);
		case 6: return pour_Water_YX(cur_state, result);
		default: cout << "Error Calls Operator" << endl;
			return 0;
	}
}



int main() {
	State cur_state = { 5, 4 }, result;
	cout << "Trang thai bat dau" << endl;
	Print_state(cur_state);
	for (int opt = 1; opt <= 6; opt++) {
		int tHien = Call_operator(cur_state, &result, opt);
		if (tHien == 1) {
			cout << action[opt]<<" Thanh cong!!!!" << endl;
			Print_state(result);
		}
		else
			cout << action[opt] <<" Khong thanh cong!!!" << endl;
	}
	return 0;

}
















