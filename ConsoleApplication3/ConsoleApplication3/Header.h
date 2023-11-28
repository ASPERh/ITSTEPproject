#include <iostream>
#include <Windows.h>
#include <ctime>
#include <cmath>
#include <vector>
#include <string>
#include <conio.h>
#include <fcntl.h>
#include <io.h>
using namespace std;

enum Colors { BLACK, DARKBLUE, DARKGREEN, DARKCYAN, DARKRED, DARKMAGENTA, DARKYELLOW, GREY, DARKGREY, BLUE, GREEN, CYAN, RED, MAGENTA, YELLOW, WHITE };
HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);
enum Directions { LEFT = 75, RIGHT = 77, DOWN = 80, UP = 72, SHOP = 98 };
enum Objects { HALL = 0, WALL = 1, COIN = 2, ENEMY = 3, HEAL = 4 };