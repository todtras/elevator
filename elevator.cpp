#include <iostream>
#include <vector>
#include <thread>
#include <chrono>
#include <windows.h>

using namespace std;

void moveCursor(int x, int y) {
	COORD coord;
	coord.X = x; coord.Y = y;
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}

void clearLine(int y) {
	moveCursor(0, y);
	cout << string(80, ' ');
	moveCursor(0, y);
}

enum Direction{UP = 1, DOWN = (-1), IDLE = 0 };

class Elevator 
{
private:
	int currentFloor;
	Direction direction;
	vector<int> requests;

public:
	Elevator() :
		currentFloor(1), direction(IDLE) {};

	void requestFloor(int floor)
	{
		requests.push_back(floor);
	}

	void setDirection()
	{
		if (requests.empty()) return;

		int floorDiff = (currentFloor - requests[0]);

		if (floorDiff == 0) direction = IDLE;
		if (floorDiff < 0) direction = UP;
		if (floorDiff > 0) direction = DOWN;
	}

	void proceed()
	{
		

		if (requests.empty())
		{
			print();
			return;
		}

		currentFloor += direction;
		print();
		moveCursor(0, 3);
		cout << "Moving..." << endl;
		moveCursor(13, 0);

		if (currentFloor == requests[0])
		{
			moveCursor(0, 4);
			cout << currentFloor << " floor arived!" << endl;
			moveCursor(13, 0);
			requests.erase(requests.begin());
			direction = IDLE;
			this_thread::sleep_for(chrono::seconds(3));
		}

	}

	void print() const
	{
		clearLine(2);
		moveCursor(0, 2);

		cout << "Current Floor : " << currentFloor << " | ";

		if (direction == UP) cout << "ก่";
		else if (direction == DOWN) cout << "ก้";
		else cout << " ";

		cout << " | Requests: ";
		for (int f : requests) cout << f << " ";
		cout << endl;
		cout << "                                    " << endl;
		cout << "                                    " << endl;
		moveCursor(13, 0);
	}
};

int main()
{
	Elevator elevator;

	thread inputThread([&]()
		{
			while (true)
			{
				moveCursor(0, 0);
				int floor;
				cout << "Input floor:      ";
				moveCursor(13, 0);
				cin >> floor;
				elevator.requestFloor(floor);
			}
		});

	while (true)
	{
		elevator.setDirection();

		elevator.proceed();
		this_thread::sleep_for(chrono::seconds(1));
	}
}