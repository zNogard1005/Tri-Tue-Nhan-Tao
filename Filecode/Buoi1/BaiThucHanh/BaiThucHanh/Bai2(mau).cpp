#include <iostream>
#include <stack>
#include <malloc.h>

#define tankcapacity_X 9 // Suc chua binh x
#define tankcapacity_Y 4 // Suc chua binh y
#define empty 0
#define goal 6
#define Maxlength 100

using namespace std;

const char* action[] = { "First State", "Pour Water Full X", "Pour Water Full Y",
						 "Pour Water Empty X", "Pour Water Empty Y",
						 "Pour Water X to Y", "Pour Water Y to X" };
typedef struct {
	int x, y; // Luong nuoc binh x, y
}State;

void Make_Null_State(State* state) {
	state->x = 0;
	state->y = 0;
}

void Print_state(State state) {
	cout << "X = " << state.x << ", Y = " << state.y << endl;
}

int goal_check(State state) {
	return (state.x == goal || state.y == goal);
}

int pour_Water_Full_X(State cur_state, State* result) {
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


typedef struct Node {
	State state;
	struct Node* Parent;
	int no_funtion;
}Node;

int compareStates(State stateA, State stateB) {
	return stateA.x == stateB.x && stateA.y == stateB.y;
}

typedef struct {
	Node* Elements[Maxlength];
	int Top_idx;
}Stack;

void Make_Null_Stack(Stack* stack) {
	stack->Top_idx = Maxlength;
}
int full_stack(Stack stack) {
	return stack.Top_idx == 0;
}

int Empty_Stack(Stack stack) {
	return stack.Top_idx == Maxlength;
}
Node* top(Stack stack) {
	if (!Empty_Stack(stack)) {
		return stack.Elements[stack.Top_idx];
	}
	return NULL;
}

void pop(Stack* stack) {
	if (!Empty_Stack(*stack)) {
		stack->Top_idx += 1;
	}
	else
		cout << "Error!!! Stack is empty" << endl;
}

void push(Node* x, Stack* stack) {
	if (full_stack(*stack))
		cout << "Error! Stack is full";
	else {
		stack->Top_idx -= 1;
		stack->Elements[stack->Top_idx] = x;
	}
}

int find_State(State state, Stack openStack) {
	while (!Empty_Stack(openStack)) {
		if(compareStates(top(openStack)->state, state))
			return 1;
		pop(&openStack);
	}
	return 0;
}

Node* DFS(State state) {
	Stack Open_DFS, Close_DFS;
	Make_Null_Stack(&Open_DFS);
	Make_Null_Stack(&Close_DFS);
	Node* root = (Node*)malloc(sizeof(Node));
	root->state = state;
	root->Parent = NULL;
	root->no_funtion = 0;
	push(root, &Open_DFS);
	while (!Empty_Stack(Open_DFS)) {
		Node* node = top(Open_DFS);
		pop(&Open_DFS);
		push(node, &Close_DFS);
		if (goal_check(node->state))
			return node;
		for (int opt = 1; opt <= 6; opt++) {
			State newstate;
			Make_Null_State(&newstate);
			if (Call_operator(node->state, &newstate, opt)) {
				if (find_State(newstate, Close_DFS) || find_State(newstate, Open_DFS))
					continue;
				Node* newNode = (Node*)malloc(sizeof(Node));
				newNode->state = newstate;
				newNode->Parent = node;
				newNode->no_funtion = opt;
				push(newNode, &Open_DFS);

			}
		}
	}
	return NULL;
}
void print_WaysToGetGoal(Node* node) {
	Stack stackPrint;
	Make_Null_Stack(&stackPrint);
	while (node->Parent != NULL) {
		push(node, &stackPrint);
		node = node->Parent;
	}
	push(node, &stackPrint);
	int no_action = 0;
	while (!Empty_Stack(stackPrint)) {
		cout << "Action " << no_action << ", " << action[top(stackPrint)->no_funtion] << endl;
		Print_state(top(stackPrint)->state);
		pop(&stackPrint);
		no_action++;
	}
}
int main() {
	State cur_state = { 0,0 };
	Node* p = DFS(cur_state);
	print_WaysToGetGoal(p);
	return 0;
}












