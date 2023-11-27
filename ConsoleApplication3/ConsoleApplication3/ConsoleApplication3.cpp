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

int maze[25][60] = {};

void SetCursor(int x, int y, Colors color, string text)
{
    HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);
    COORD pos;
    pos.X = x;
    pos.Y = y;
    SetConsoleCursorPosition(h, pos);
    SetConsoleTextAttribute(h, color);
    cout << text;
    cout << "\n";
}

void initializeConsole()
{
    SetConsoleOutputCP(CP_UTF8);
    system("title Maze");
    HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);
    srand(time(0));

    CONSOLE_CURSOR_INFO info;
    info.bVisible = false;
    info.bVisible = 1;
    SetConsoleCursorInfo(h, &info);
}

void generateMaze(int maze[][60], const int height, const int width) // Генерация лабиринта
{
    for (int y = 0; y < height; y++)
    {
        for (int x = 0; x < width; x++)
        {
            // GENERATION
            maze[y][x] = rand() % 5;

            if (x == 0 || y == 0 || x == width - 1 || y == height - 1)
                maze[y][x] = WALL;

            if (x == 0 && y == 2 || x == 1 && y == 2 || x == 2 && y == 2 ||
                x == width - 1 && y == height - 3)
                maze[y][x] = 0;

            if (maze[y][x] == 3) // если элемент который создается это 3 
            {
                int probability = rand() % 10; // то шанс то что он останется будет 10%
                if (probability != 0)
                {
                    maze[y][x] = 0;
                }
            }
            if (maze[y][x] == 4) // если элемент который создается это 4
            {
                int probability = rand() % 10; // то шанс то что он останется будет 10%
                if (probability != 0)
                {
                    maze[y][x] = 0;
                }
            }
        }
    }
}

void printMaze(int maze[][60], const int height, const int width) // вывод массива
{
    for (int y = 0; y < height; y++)
    {
        for (int x = 0; x < width; x++)
        {
            if (maze[y][x] == HALL)
            {
                SetCursor(x, y, BLACK, " ");
            }
            else if (maze[y][x] == WALL)
            {
                SetCursor(x, y, DARKBLUE, "X");
            }
            else if (maze[y][x] == COIN)
            {
                SetCursor(x, y, YELLOW, ".");
            }
            else if (maze[y][x] == ENEMY)
            {
                SetCursor(x, y, RED, "*");
            }
            else if (maze[y][x] == HEAL)
            {
                SetCursor(x, y, CYAN, "I");
            }
            else
            {
                cout << maze[y][x];
            }
        }
        cout << "\n";
    }
}

void gameLoop(int maze[][60], const int height, const int width, int& coins, int& enemyDAMAGE, int& medkitHEAL) // Логика игры
{
    COORD position;
    position.X = 0;
    position.Y = 2;

    SetCursor(position.X, position.Y, MAGENTA, "0");

    int HealthPoints = 100;

    const int COIN_COST = 5;
    const int ENEMY_DAMAGE_UPGRADE_COST = 10; // стоимость улучшения 
    const int HEALTH_KIT_HEAL_UPGRADE_COST = 8; // стоимость улучшения 

    while (true)
    {
        int code = _getch();
        if (code == 224) // позволяет кнопке "B" работать
        {
            code = _getch();
        }

        if (code == SHOP) // Если игрок нажал "B"
        {
            SetCursor(width + 2, 5, YELLOW, "Добро пожаловать в магазин!");
            SetCursor(width + 2, 6, YELLOW, "1. Увеличить урон противников(усложнение) (Cost: " + to_string(ENEMY_DAMAGE_UPGRADE_COST) + " coins)");
            SetCursor(width + 2, 7, YELLOW, "2. Улучшить исцеление от медикоментов(I) (Cost: " + to_string(HEALTH_KIT_HEAL_UPGRADE_COST) + " coins)");
            SetCursor(width + 2, 8, YELLOW, "3. Выйти из магазина");
            SetConsoleCursorPosition(h, { static_cast<SHORT>(width + 2), 10 }); // Перемещение текстового курсора в позицию
            int choice;
            cin >> choice;

            switch (choice)
            {
            case 1:
                if (coins >= ENEMY_DAMAGE_UPGRADE_COST) // если игрок выбрал 1
                {
                    enemyDAMAGE += 5; // то урон противника +5
                    coins -= ENEMY_DAMAGE_UPGRADE_COST;
                    COORD coins_info;
                    SetCursor(width + 2, 1, YELLOW, "COINS:");
                    SetCursor(width + 8, 1, YELLOW, to_string(coins));
                }
                else
                {
                    SetCursor(width + 2, 9, YELLOW, "Не хватает монет"); // или говорит то что не хватает монет
                }
                break;
            case 2:
                if (coins >= HEALTH_KIT_HEAL_UPGRADE_COST) // если игрок выбрал 2
                {
                    medkitHEAL += 5; // то исцеление от (I) +5
                    coins -= HEALTH_KIT_HEAL_UPGRADE_COST;
                    COORD coins_info;
                    SetCursor(width + 2, 1, YELLOW, "COINS:");
                    SetCursor(width + 8, 1, YELLOW, to_string(coins) + " ");
                }
                else
                {
                    SetCursor(width + 2, 9, YELLOW, "Не хватает монет"); // или говорит то что не хватает монет
                }
                break;
            case 3:
                // Выход из магазина
                break;
            default:
                cout << "Некорректный выбор\n";
                break;
            }
        }

        SetConsoleCursorPosition(h, position);
        cout << " ";
        if (code == RIGHT and maze[position.Y][position.X + 1] != 1) // если нажата стрелка вправо то
            position.X++; // позиция гг меняется на X++ то есть направо
        if (code == LEFT and maze[position.Y][position.X - 1] != 1) // если нажата стрелка влево то
            position.X--; // позиция гг меняется на X-- то есть налево
        if (code == UP and maze[position.Y - 1][position.X] != 1) // если нажата стрелка вверх то
            position.Y--; // позиция гг меняется на Y-- то есть вверх
        if (code == DOWN and maze[position.Y + 1][position.X] != 1) // если нажата стрелка вниз то
            position.Y++; // позиция гг меняется на Y++ то есть вниз
        SetCursor(position.X, position.Y, MAGENTA, "0"); // перемещение гг

        if (position.X == width - 1 and position.Y == height - 3) // если позиция гг равна позиции финиша то
        {
            MessageBoxA(0, "Вы прошли лабиринт", "победа", MB_OK); // выводится этот текст
            break;
        }
        if (maze[position.Y][position.X] == COIN) // если гг наступает на монету то
        {
            coins++; // к переменной записывается +1
            maze[position.Y][position.X] = HALL; // и место где была монета меняется на обьект HALL
            COORD coins_info;
            SetCursor(width + 2, 1, YELLOW, "COINS: "); 
            SetCursor(width + 8, 1, YELLOW, to_string(coins) + " "); // Вывод монет
        }
        if (maze[position.Y][position.X] == ENEMY or maze[position.Y][position.X] == HEAL) // если гг наступает на ENEMY или HEAL то выполняется два разных случая
        {
            if (maze[position.Y][position.X] == ENEMY) // если это ENEMY то
            {
                HealthPoints = HealthPoints - enemyDAMAGE; // отнимается HealthPoints = равный enemyDamage
                maze[position.Y][position.X] = HALL; // и место где была монета меняется на обьект HALL
            }
            if (maze[position.Y][position.X] == HEAL) // если это HEAL то
            {
                HealthPoints = HealthPoints + medkitHEAL; // прибавляется к HealthPoints число которе записано в переменной medkitHeal которое равняется 25
                maze[position.Y][position.X] = HALL; // и место где была монета меняется на обьект HALL
                if (HealthPoints > 100) // также если при поднятии HEAL но если у гг HealthPoints > 100
                {
                    HealthPoints = 100; // то HealthPoints будет равен 100
                }
            }
            COORD HP_info;
            SetCursor(width + 2, 2, RED, "HEALTH: ");
            SetCursor(width + 9, 2, RED, to_string(HealthPoints) + " "); // вывод HealthPoints
            if (HealthPoints < 5) // если HealthPoints < 5 то 
            {
                system("cls"); // вся консоль стирается командой CLS
                cout << "проигрыш\n\n"; // и выводится надпись проигрыш
                Sleep(INFINITE);
            }

        }
        else // нажатия не было, двигаем врагов
        { 
            Sleep(15);
            COORD enemy_positions[100];
            int enemy_count = 0;
            for (int y = 0; y < height; y++) // перебор строк
            {
                for (int x = 0; x < width; x++) // перебор столбцов
                {
                    // если очередная ячейка - это враг
                    if (maze[y][x] == Objects::ENEMY)
                    {
                        enemy_positions[enemy_count].X = x;
                        enemy_positions[enemy_count].Y = y;
                        enemy_count++;
                    }
                }
            }
            // перебор всех врагов
            for (int i = 0; i < enemy_count; i++)
            {
                int r = rand() % 100; // 0-left, 1-right, 2-up, 3-down
                if (r == LEFT &&
                    maze[enemy_positions[i].Y][enemy_positions[i].X - 1] != Objects::WALL &&
                    maze[enemy_positions[i].Y][enemy_positions[i].X - 1] != Objects::ENEMY)
                {
                    // стирание врага в старой позиции
                    COORD temp = enemy_positions[i];
                    SetConsoleCursorPosition(h, temp);
                    cout << " ";
                    maze[enemy_positions[i].Y][enemy_positions[i].X] = Objects::HALL;

                    // перемещаем врага в новую позицию
                    temp.X = enemy_positions[i].X - 1;
                    temp.Y = enemy_positions[i].Y;
                    SetConsoleCursorPosition(h, temp);
                    SetConsoleTextAttribute(h, Colors::RED);
                    cout << "*";
                    maze[enemy_positions[i].Y][enemy_positions[i].X - 1] = Objects::ENEMY;
                }
                else if (r == RIGHT &&
                    maze[enemy_positions[i].Y][enemy_positions[i].X + 1] != Objects::WALL &&
                    maze[enemy_positions[i].Y][enemy_positions[i].X + 1] != Objects::ENEMY)
                {
                    // стирание врага в старой позиции
                    COORD temp = enemy_positions[i];
                    SetConsoleCursorPosition(h, temp);
                    cout << " ";
                    maze[enemy_positions[i].Y][enemy_positions[i].X] = Objects::HALL;

                    // перемещаем врага в новую позицию
                    temp.X = enemy_positions[i].X + 1;
                    temp.Y = enemy_positions[i].Y;
                    SetConsoleCursorPosition(h, temp);
                    SetConsoleTextAttribute(h, Colors::RED);
                    cout << "*";
                    maze[enemy_positions[i].Y][enemy_positions[i].X + 1] = Objects::ENEMY;
                }

                else if (r == UP &&
                    maze[enemy_positions[i].Y - 1][enemy_positions[i].X] != Objects::WALL &&
                    maze[enemy_positions[i].Y - 1][enemy_positions[i].X] != Objects::ENEMY)
                {
                    // стирание врага в старой позиции
                    COORD temp = enemy_positions[i];
                    SetConsoleCursorPosition(h, temp);
                    cout << " ";
                    maze[enemy_positions[i].Y][enemy_positions[i].X] = Objects::HALL;

                    // перемещаем врага в новую позицию
                    temp.X = enemy_positions[i].X;
                    temp.Y = enemy_positions[i].Y - 1;
                    SetConsoleCursorPosition(h, temp);
                    SetConsoleTextAttribute(h, Colors::RED);
                    cout << "*";
                    maze[enemy_positions[i].Y - 1][enemy_positions[i].X] = Objects::ENEMY;
                }
                else if (r == DOWN &&
                    maze[enemy_positions[i].Y + 1][enemy_positions[i].X] != Objects::WALL &&
                    maze[enemy_positions[i].Y + 1][enemy_positions[i].X] != Objects::ENEMY)
                {
                    // стирание врага в старой позиции
                    COORD temp = enemy_positions[i];
                    SetConsoleCursorPosition(h, temp);
                    cout << " ";
                    maze[enemy_positions[i].Y][enemy_positions[i].X] = Objects::HALL;

                    // перемещаем врага в новую позицию
                    temp.X = enemy_positions[i].X;
                    temp.Y = enemy_positions[i].Y + 1;
                    SetConsoleCursorPosition(h, temp);
                    SetConsoleTextAttribute(h, Colors::RED);
                    cout << "*";
                    maze[enemy_positions[i].Y + 1][enemy_positions[i].X] = Objects::ENEMY;
                }
            }
        }
    }

    system("cls");
    cout << "победа\n\n";
    Sleep(INFINITE);
}

void mainMenu(const int height, const int width, int& coins, int& enemyDAMAGE, int& medkitHEAL) // главное меню
{
    int choice;
    cout << "=== Main Menu ===\n";
    cout << "1. Start Game\n";
    cout << "2. Exit\n";
    cout << "Enter your choice: ";
    cin >> choice;

    switch (choice)
    {
    case 1: // если выбор = 1 то запускаются функции ниже
        generateMaze(maze, height, width);
        printMaze(maze, height, width);
        gameLoop(maze, height, width, coins, enemyDAMAGE, medkitHEAL);
        break;

    case 2: // если выбор = 2 то программа заканчивается
        exit(0);
        break;

    default: // если выбор не 1 и не 2 то
        cout << "Неверный выбор выберите 1 или 2\n"; // выдается это сообщение
        break;
    }
}

int main()
{
    initializeConsole();

    const int width = 60;
    const int height = 25;
    int coins = 0;
    int enemyDAMAGE = 25;
    int medkitHEAL = 25;

    while (true)
    {
        mainMenu(height, width, coins, enemyDAMAGE, medkitHEAL);
    }

    return 0;
}
