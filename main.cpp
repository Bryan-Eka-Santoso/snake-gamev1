#include <iostream>
#include <windows.h>
#include <conio.h>
#include <time.h>
#include <stdlib.h>
#include <vector>
#include <string>
#include <fstream>

using namespace std;

struct badan
{
    int x, y;
    char ch;
};

void gotoxy(int x, int y)
{
    COORD coordinate;
    coordinate.X = x;
    coordinate.Y = y;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coordinate);
}

void spawn(string &username, int &n, int &dx, int &dy, int &score, badan &food, badan snake[])
{
    fstream save("savegame.txt", ios::in);
    if (save.is_open())
    {
        save >> username;
        save >> n;
        save >> dx;
        save >> dy;
        save >> score;
        save >> food.x;
        save >> food.y;
        for (int i = 0; i < n; i++)
        {
            save >> snake[i].x >> snake[i].y >> snake[i].ch;
        }
        save.close();
    }
}

void saveLeaderboardHistory(const vector<string> &leaderboardUsername, const vector<int> &leaderboardScore, const vector<string> &historyUsername, const vector<int> &historyScore)
{
    fstream leaderboardFile("leaderboard.txt", ios::out);
    if (leaderboardFile.is_open())
    {
        for (int i = 0; i < leaderboardUsername.size(); i++)
        {
            leaderboardFile << leaderboardUsername[i] << " " << leaderboardScore[i] << endl;
        }
        leaderboardFile.close();
    }

    fstream historyFile("history.txt", ios::out);
    if (historyFile.is_open())
    {
        for (int i = 0; i < historyUsername.size(); i++)
        {
            historyFile << historyUsername[i] << " " << historyScore[i] << endl;
        }
        historyFile.close();
    }
}

void loadLeaderboardHistory(vector<string> &leaderboardUsername, vector<int> &leaderboardScore, vector<string> &historyUsername, vector<int> &historyScore)
{
    fstream leaderboardFile("leaderboard.txt", ios::in);
    if (leaderboardFile.is_open())
    {
        string username;
        int score;
        while (leaderboardFile >> username >> score)
        {
            leaderboardUsername.push_back(username);
            leaderboardScore.push_back(score);
        }
        leaderboardFile.close();
    }

    fstream historyFile("history.txt", ios::in);
    if (historyFile.is_open())
    {
        string username;
        int score;
        while (historyFile >> username >> score)
        {
            historyUsername.push_back(username);
            historyScore.push_back(score);
        }
        historyFile.close();
    }
}

int main()
{
    srand(time(0));
    badan snake[100];
    badan food;
    int dx = -1;
    int dy = 0;
    int n = 5;
    int score = 0;
    int menu, historyMenu, leaderboardMenu;
    string username;
    string fileSave = "savegame.txt";

    vector<string> historyUsername;
    vector<int> historyScore;

    vector<string> leaderboardUsername;
    vector<int> leaderboardScore;

    loadLeaderboardHistory(leaderboardUsername, leaderboardScore, historyUsername, historyScore);

    do
    {
        cout << "== Snake Game ==" << endl;
        cout << "1. Play Game" << endl;
        cout << "2. Leaderboard" << endl;
        cout << "3. History" << endl;
        cout << "0. Exit" << endl;

        do
        {
            cout << ">> ";
            cin >> menu;
        } while (menu < 0 || menu > 3);

        system("cls");

        switch (menu)
        {
        case 1:
        {
            bool duplicate;
            bool simpan;
            if (simpan != true)
            {
                do
                {
                    duplicate = false;
                    cout << "Enter Username: ";
                    cin >> username;
                    for (int i = 0; i < historyUsername.size(); i++)
                    {
                        if (username == historyUsername[i])
                        {
                            duplicate = true;
                        }
                    }
                    if (duplicate == true)
                    {
                        cout << "Username already taken." << endl;
                        Sleep(1000);
                        system("cls");
                    }
                } while (duplicate == true);
            }

            system("cls");

            for (int i = 0; i < n; i++)
            {
                snake[i].x = 20 + i;
                snake[i].y = 10;
                snake[i].ch = '#';
            }
            snake[0].ch = '@';

            if (simpan != true)
            {
                food.x = rand() % 70 + 5;
                food.y = rand() % 20 + 3;
            }

            spawn(username, n, dx, dy, score, food, snake);

            char key;
            bool gameOver = false;
            simpan = false;
            while (key != 27 && gameOver == false && simpan == false)
            {
                //cetak
                for (int i = 0; i < n; i++)
                {
                    gotoxy(snake[i].x, snake[i].y);
                    cout << snake[i].ch;
                }

                //food
                gotoxy(food.x, food.y);
                cout << '+';

                gotoxy(0, 0);
                cout << "Score: " << score;

                if (score >= 1000)
                {
                    Sleep(50);
                }
                else if (score >= 500)
                {
                    Sleep(100);
                }
                else
                {
                    Sleep(200);
                }

                //hapus
                for (int i = 0; i < n; i++)
                {
                    gotoxy(snake[i].x, snake[i].y);
                    cout << " ";
                }

                //pindah
                for (int i = n - 1; i > 0; i--)
                {
                    snake[i].x = snake[i - 1].x;
                    snake[i].y = snake[i - 1].y;
                }
                snake[0].y += dy;
                snake[0].x += dx;
                if (snake[0].x >= 80)
                {
                    snake[0].x = 1;
                }
                if (snake[0].x <= 0)
                {
                    snake[0].x = 80;
                }
                if (snake[0].y >= 25)
                {
                    snake[0].y = 1;
                }
                if (snake[0].y <= 0)
                {
                    snake[0].y = 25;
                }
                if (snake[0].y == food.y && snake[0].x == food.x)
                {
                    food.x = rand() % 70 + 5;
                    food.y = rand() % 20 + 3;
                    n++;
                    score += 100;
                    snake[n - 1].x = snake[n - 2].x;
                    snake[n - 1].y = snake[n - 2].y;
                    snake[n - 1].ch = '#';
                }

                for (int i = 1; i < n; i++)
                {
                    if (snake[0].x == snake[i].x && snake[0].y == snake[i].y)
                    {
                        gameOver = true;
                    }
                }

                if (kbhit())
                {
                    key = getch();
                    switch (key)
                    {
                    case 'w':
                        if (dy != 1)
                        {
                            dx = 0;
                            dy = -1;
                        }
                        break;
                    case 'a':
                        if (dx != 1)
                        {
                            dx = -1;
                            dy = 0;
                        }
                        break;
                    case 's':
                        if (dy != -1)
                        {
                            dx = 0;
                            dy = 1;
                        }
                        break;
                    case 'd':
                        if (dx != -1)
                        {
                            dx = 1;
                            dy = 0;
                        }
                        break;
                    case 'x':
                        fstream save;
                        save.open(fileSave, ios::out);
                        if (save.is_open())
                        {
                            save << username << endl;
                            save << n << " " << dx << " " << dy << " " << score << endl;
                            save << food.x << " " << food.y << endl;
                            for (int i = 0; i < n; i++)
                            {
                                save << snake[i].x << " " << snake[i].y << " " << snake[i].ch << endl;
                            }

                            save.close();
                        }
                        simpan = true;
                        break;
                    }
                }
                gotoxy(0, 25);
                cout << "Press w/a/s/d to move around" << endl;
                cout << "Press x to exit (autosave)" << endl;
            }
            if (gameOver == true)
            {
                gotoxy(0, 0);
                cout << "Game Over !!!" << endl;
                cout << "Your Score: " << score << endl;

                historyUsername.push_back(username);
                historyScore.push_back(score);

                bool inserted = false;
                for (int i = 0; i < leaderboardScore.size(); i++)
                {
                    if (score > leaderboardScore[i])
                    {
                        leaderboardScore.insert(leaderboardScore.begin() + i, score);
                        leaderboardUsername.insert(leaderboardUsername.begin() + i, username);
                        inserted = true;
                        break;
                    }
                }

                if (!inserted && leaderboardScore.size() < 5)
                {
                    leaderboardScore.push_back(score);
                    leaderboardUsername.push_back(username);
                }

                if (leaderboardScore.size() > 5)
                {
                    leaderboardScore.pop_back();
                    leaderboardUsername.pop_back();
                }

                score = 0;
                n = 5;

                saveLeaderboardHistory(leaderboardUsername, leaderboardScore, historyUsername, historyScore);

                remove("savegame.txt");
            }

            Sleep(1000);
            system("cls");
            break;
        }
        case 2:
        {
            do
            {
                if (leaderboardUsername.size() == 0 || leaderboardScore.size() == 0)
                {
                    cout << "Leaderboard is empty." << endl;
                }
                else
                {
                    cout << "== Leaderboard Snake Game ==" << endl
                         << endl;
                    cout << "Username || Score" << endl;
                    cout << "-----------------" << endl;
                    for (int i = 0; i < leaderboardUsername.size(); i++)
                    {
                        cout << i + 1 << ". " << leaderboardUsername[i] << " || " << leaderboardScore[i] << endl;
                    }
                }
                cout << "0. Back" << endl;
                do
                {
                    cout << ">> ";
                    cin >> leaderboardMenu;
                } while (leaderboardMenu != 0);
            } while (leaderboardMenu != 0);
            system("cls");
            break;
        }
        case 3:
        {
            do
            {
                if (historyUsername.size() == 0 || historyScore.size() == 0)
                {
                    cout << "History is empty." << endl;
                }
                else
                {
                    cout << "== History Game ==" << endl
                         << endl;
                    cout << "Username || Score" << endl;
                    cout << "-----------------" << endl;
                    for (int i = 0; i < historyUsername.size(); i++)
                    {
                        cout << i + 1 << ". " << historyUsername[i] << " || " << historyScore[i] << endl;
                    }
                }
                cout << "0. Back" << endl;
                do
                    {
                        cout << ">> ";
                        cin >> historyMenu;
                    } while (historyMenu != 0);
                } while (historyMenu != 0);
                system("cls");
                break;
            }
        }

        saveLeaderboardHistory(leaderboardUsername, leaderboardScore, historyUsername, historyScore);

    } while (menu != 0);

    return 0;
}
