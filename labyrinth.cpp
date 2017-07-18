#include <iostream>
#include <windows.h>
#include <cmath>
#include <stdlib.h>
using namespace std;

class Labyrinth {
public:
	int _width, _height;
	int **_map;
	void Free();
	Labyrinth(int width, int height);
	void Recreate(int width, int height);
	void Substitute(int current, int next);
	bool IsFinish();
	void NewWall(int x, int y);
	void Print(int depth);
};

char Texture(int value);

const int generation_factor = 50;

Labyrinth::Labyrinth(int width, int height) {
	_map = NULL;
	Recreate(width, height);
}

void Labyrinth::Free() {
	for (int i = 0; i < _height; i++) {
        if (_map && (_map[i] != nullptr) )
        {
            delete _map[i];
            _map[i] = nullptr;
        }
	}
	delete _map;
	_width = 0;
	_height = 0;
	_map = NULL;
}

void Labyrinth::Substitute(int current, int next) {
	if (current < next) {
		Substitute(next, current);
		return;
	}
	for (int i = 0; i < _height; i += 2) {
		for (int j = 0; j < _width; j += 2) {
			if (_map[i][j] == current)
				_map[i][j] = next;
		}
	}
}

void Labyrinth::Recreate(int width, int height) {
	Free();
	_width = 2 * width + 1;
	_height = 2 * height + 1;
	_map = new int*[_height];
	for (int i = 0; i < _height; i++) {
		_map[i] = new int[_width];
		for (int j = 0; j < _width; j++)
			_map[i][j] = 0;
	}
	for (int i = 0; i < _height; i++) {
		_map[i][0] = 1;
		_map[i][_width - 1] = 1;
	}
	for (int i = 0; i < _width; i++) {
		_map[0][i] = 1;
		_map[_height - 1][i] = 1;
	}
	int counter = 2;
	for (int i = 2; i < _height - 2; i += 2) {
		for (int j = 2; j < _width - 2; j += 2) {
			_map[i][j] = counter++;
		}
	}
	while (!IsFinish()) {
		for (int i = 0; i < generation_factor; i++) {
			NewWall(rand() % _width + 1, rand() % _height + 1);
		}
	}
	_map[0][1] = 0;
	_map[_height - 1][_width - 2] = 0;
}

void Labyrinth::NewWall(int x, int y) {
	if ((x < 1) || (y < 1) || (x >= _width) || (y >= _height)
			|| ((x + y) % 2 == 0))
		return;
	if (x % 2 == 1) {
		if (_map[y][x + 1] == _map[y][x - 1])
			return;
		Substitute(_map[y][x + 1], _map[y][x - 1]);
	} else {
		if (_map[y + 1][x] == _map[y - 1][x])
			return;
		Substitute(_map[y + 1][x], _map[y - 1][x]);
	}
	_map[y][x] = 1;
}

bool Labyrinth::IsFinish() {
	for (int i = 0; i < _height; i += 2) {
		for (int j = 0; j < _width; j += 2) {
			if (_map[i][j] > 1)
				return false;
		}
	}
	return true;
}

char Texture(int value) {
	if (value == 0)
		return ' ';
	return '#';
}

void Labyrinth::Print(int depth) {
	if (_map == NULL)
		cout << "Empty labyrinth" << endl;
	for (int i = 0; i < _height; i++) {
		for (int j = 0; j < _width; j++) {
			cout << Texture((_map[i][j]));
			if (j % 2 == 1) {
				for (int ij = 0; ij < depth - 1; ij++) {
					cout << Texture(_map[i][j]);
				}
			}
		}
		cout << endl;
		if (i % 2 == 1) {
			for (int ij = 0; ij < depth - 1; ij++) {
				for (int j = 0; j < _width; j++) {
					cout << Texture((_map[i][j]));
					if (j % 2 == 1) {
						for (int ji = 0; ji < depth - 1; ji++) {
							cout << Texture(_map[i][j]);
						}
					}
				}
				cout << endl;
			}
		}
	}
}

int main() {
	int width, height, depth;
	cout << "Enter width  of labyrinth: ";
	cin >> width;
	cout << "Enter height of labyrinth: ";
	cin >> height;
	cout << "Enter depth  of labyrinth: ";
	cin >> depth;
	Labyrinth labyrinth = Labyrinth(width, height);
	labyrinth.Print(depth);
	system("pause");
	return 0;
}
