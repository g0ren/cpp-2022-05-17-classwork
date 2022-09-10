#include <iostream>
#include <stack>
#include <queue>
#include <fstream>

using namespace std;

void myPrint(const char *c) {
	for (; *c; ++c) {
		cout << *c;
	}
}

template<typename First, typename ... Rest>
void myPrint(const char *c, First first, Rest ... rest) {
	for (; *c && *c != '%'; ++c) {
		cout << *c;
	}
	cout << first;
	myPrint(++c, rest...);
}

class Road {
protected:
	vector<Road*> nexts;
public:
	void initNextRoads(Road *road) {
		nexts.push_back(road);
	}
	virtual Road* forward()=0;
	virtual void start()=0;
};

class SimpleRoad: public Road {
public:
	SimpleRoad() {

	}
	Road* forward() override {
		return nexts[0];
	}
	void start() override{
		cout<<"Simple"<<endl;
	}
};

class DoubleRoad: public Road {
public:
	DoubleRoad() {
	}
	Road* forward() override {
		int choice;
		cout << "1 - Left, 2 - Right" << endl;
		cin >> choice;
		if (choice == 1)
			return nexts[0];
		else
			return nexts[1];
	}
	void start() override{
		cout<<"Double"<<endl;
	}
};

class TripleRoad: public Road {
public:
	TripleRoad() {
	}
	Road* forward() override {
		int choice;
		cout << "1 - Left, 2 - Center, 3 - Right" << endl;
		cin >> choice;
		if (choice == 1)
			return nexts[0];
		else if (choice == 2)
			return nexts[1];
		else
			return nexts[2];
	}
	void start() override{
		cout<<"Triple"<<endl;
	}
};

class SurpriseRoad: public Road {
public:
	SurpriseRoad() {
	}
	Road* forward() override {
		return nexts[0];
	}
	void start() override{
		cout<<"Surprise"<<endl;
	}
};

class RoadTree {
	Road *begin;
public:
	RoadTree() {
		this->begin = new SimpleRoad();
	}
	void create() {
		ifstream fin("game.txt", ios::app);
		if (!fin.is_open()) {
			cout << "Can't open the file..." << endl;
		} else {
			queue<Road*> roots;
			roots.push(begin);
			string str;
			while (!fin.eof()) {
				getline(fin, str);
				for (auto x : str) {
					if (x == '|') {
						Road *top = roots.front();
						Road *tmp = new SimpleRoad();
						top->initNextRoads(tmp);
						roots.pop();
						roots.push(tmp);
						break;
					}
					if (x == '?') {
						Road *top = roots.front();
						Road *tmp = new SurpriseRoad();
						top->initNextRoads(tmp);
						roots.pop();
						roots.push(tmp);
						break;
					}
					if (x == 'L') {
						Road *top = roots.front();
						Road *tmp = new DoubleRoad();
						top->initNextRoads(tmp);
						roots.pop();
						roots.push(tmp);
						roots.push(tmp);
						break;
					}
					if (x == '+') {
						Road *top = roots.front();
						Road *tmp = new TripleRoad();
						top->initNextRoads(tmp);
						roots.pop();
						roots.push(tmp);
						roots.push(tmp);
						roots.push(tmp);
						break;
					}
					if (x == 'x') {
						Road *top = roots.front();
						top->initNextRoads(NULL);
						roots.pop();
						break;
					}
				}
			}
			fin.close();
		}

	}
	Road* getBegin() {
		return this->begin;
	}
};

class ControllerRoads {
	stack<Road*> roots;
	RoadTree tree;
public:
	ControllerRoads() {
		tree.create();
		roots.push(tree.getBegin());
	}
	void start_game() {
		int command { };
		Road *now;
		while (!roots.empty()) {
			now = roots.top();
			if(now==nullptr){
				cout<<"End of the road, boys!"<<endl;
				break;
			}
			now->start();
			cout << "1 - Forward, 2 - Back" << endl;
			cin >> command;
			if (command == 1) {
				roots.push(now->forward());
			} else {
				roots.pop();
			}
		}
	}

};

int main() {
	/*myPrint("A % b % 3 %", 5, 2.5, '3');
	 int a { 1 };
	 float b { 2.5 };
	 char *c { "three" };
	 char d { '4' };
	 myPrint("\nA=%\nB=%\nC=%\nD=%\n", a, b, c, d);*/

	ControllerRoads c;
	c.start_game();

	return 0;
}
