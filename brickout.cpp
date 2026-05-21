#include <iostream>
#include <cmath>
#include <iomanip>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <conio.h>
#include <windows.h>
#include <fstream>
#include <string>
#include <cstring>
#include <chrono>
using namespace std;

struct Player
{
    string name = " ";
    int score = 0;
    int HP = 3;
};

Player player;

void set_cursor(int x, int y);
string get_char_at(int x, int y);
void drawAll(string message);
bool cursor(int &paddleX, bool &ballMoving, string &message, bool &pause);
void initialBricks();
void updateBall();
void game();
string livesDisplay();
void HideCursor();
void set_cursor(int x, int y);
bool endOfGame();
void win();
void gameover();
void newrecord();
int menu();
void savePlayer(Player player);
void showHistory();
int highScore();
void setting();
void saveGame();
void savelevel();
void readlevel();
void loadGame();
void exitGame();

const int BRICK_WIDTH = 8;
const int BRICK_HEIGHT = 2;
int level =1;
bool bricks[5][10];
int paddleX = 42;
int stepSize = 4;
string PADDLE = "┏━━━━━━━━━┓";
string Lpaddle = "┏";
string Rpaddle = "┓";
string Mpaddle = "━";
string ball = "●";
int PADDLE_WIDTH = 11;
string color = "color 0F"; // Background
const int PLAYFIELD_WIDTH = 12 * BRICK_WIDTH;

int ballX = paddleX + PADDLE_WIDTH / 2.0; // Start in middle
int ballY = 28;                           // Above paddle
bool ballMoving = false;
bool paused = false;
double angle = M_PI / 4;
double speed = 1.7;
double speedX = speed * cos(angle);
double speedY = speed * sin(angle);
bool sound = true;
int x = 1; // loadgame

int main()
{
    srand(time(0));
    HideCursor();

    while (true)
    {

        system("cls");
        int choice = menu();
        if (choice == 1)
        {
            system(color.c_str());
            player.HP = 3;
            player.name = "player";
            player.score = 0;
            system("cls");
            cout << "Enter your name: ";
            cin >> player.name;
            game();
            savePlayer(player);
            color = "color 0F";
        }
        else if (choice == 2)
        {
            readlevel(); //***
            if (player.HP == 0)
            {
                cout << "No  info found!";
                break;
            }
            system(color.c_str());
            x = 2;
            loadGame();
            game();
            savePlayer(player);
            color = "color 0F";
        }
        else if (choice == 3)
        {
            system("cls");
            char a;
            cout << "╔════════════════════┄help┄═════════════════════╗" << endl;
            cout << "   Try to break all the bricks with the paddle." << endl
                 << "   •Use space to start" << endl
                 << "   •Use ← → arrows to move paddle" << endl
                 << "   •Use ESC or p to pause " << endl
                 << "   •Use p and enter to exit " << endl;
            cout << "╚════════════════════════════════════════════════╝";
            a = getch();
            continue;
        }
        else if (choice == 4)
        {
            char a;
            system("cls");
            cout << "╔═══════┄History┄═══════╗" << endl;
            showHistory();
            cout << endl
                 << " High Score........." << highScore() << endl
                 << "╚═══════════════════════╝";
            a = getch();
            continue;
        }
        else if (choice == 5)
        {
            setting();
            continue;
        }
        else if (choice == 6)
        {
            savelevel();
            exitGame();
            return 0;
        }
    }
    return 0;
}

string get_char_at(int x, int y)
{

    int bx = round(ballX);
    int by = round(ballY);

    // Paddle row
    if (y == 29)
    {

        // Paddle
        if (x >= paddleX && x < paddleX + PADDLE_WIDTH)
        {
            int pos = x - paddleX;

            // If the ball is exactly here, show ball instead of paddle
            if (bx == x && by == y)
            {
                return ball;
            }

            // Otherwise show paddle character
            if (pos == 0)
                return Lpaddle;
            if (pos == PADDLE_WIDTH - 1)
                return Rpaddle;
            return Mpaddle;
        }

        // Outside paddle on this row, print space (unless ball is here)
        if (bx == x && by == y)
        {
            return ball;
        }

        return " ";
    }

    // other rows

    // Print ball across the field
    if (bx == x && by == y)
    {
        return ball;
    }

    // Bricks
    if (y >= 2 && y < 12)
    {
        int brick_row = (y - 2) / 2;
        int line_type = (y - 2) % 2;

        if (x >= BRICK_WIDTH && x < PLAYFIELD_WIDTH - BRICK_WIDTH)
        {
            int brick_col = (x - BRICK_WIDTH) / BRICK_WIDTH;
            if (bricks[brick_row][brick_col])
            {
                int local_x = (x - BRICK_WIDTH) % BRICK_WIDTH;

                if (line_type == 0)
                { // top
                    if (local_x == 0)
                        return "╭";
                    if (local_x == BRICK_WIDTH - 1)
                        return "╮";
                    return "─";
                }
                else
                { // bottom
                    if (local_x == 0)
                        return "╰";
                    if (local_x == BRICK_WIDTH - 1)
                        return "╯";
                    return "─";
                }
            }
        }
    }

    return " ";
}

void drawAll(string message = "")
{
    set_cursor(0, 0);

    // Top border
    cout << "╔";
    for (int i = 0; i < PLAYFIELD_WIDTH; i++)
        cout << "═";
    cout << "╗" << endl;

    // All content rows
    for (int y = 0; y < 30; y++)
    {
        cout << "║";
        for (int x = 0; x < PLAYFIELD_WIDTH; x++)
        {
            cout << get_char_at(x, y);
        }
        cout << "║" << endl;
    }

    // Bottom border
    cout << "╚";
    for (int i = 0; i < PLAYFIELD_WIDTH; i++)
        cout << "═";
    cout << "╝" << endl;

    // Details
    cout << " Lives: " << livesDisplay();
    for (int i = 0; i < 75; i++)
    {
        cout << " ";
    }
    int hScore=highScore();
    cout << "Score: " << player.score <<"    High Score:"<<hScore<< "\n\n";

    set_cursor(0, 34);
    cout << string(80, ' ') << "\r";

    if (!message.empty())
    {
        cout << " " << message << "\n\n";
    }
    else
    {
        cout << "\n\n";
    }

    cout << " level " << level;
}

bool cursor(int &paddleX, bool &ballMoving, string &message, bool &pause)
{
    if (!_kbhit())
    {
        return true;
    }

    int ch = _getch();
    if (ch == 0 || ch == 224)
    {
        ch = _getch();
        if (!paused)
        {
            if (ch == 75)
            { // Moving left
                if (paddleX > 0)
                {
                    paddleX -= (paddleX < 4 ? paddleX : stepSize);
                }
            }
            if (ch == 77)
            { // Moving right
                if (paddleX < PLAYFIELD_WIDTH - PADDLE_WIDTH)
                {
                    int maxMove = PLAYFIELD_WIDTH - PADDLE_WIDTH - paddleX;
                    paddleX += (maxMove < 4 ? maxMove : stepSize);
                }
            }
        }
    }

    else if (ch == 32)
    { // Space key
        if (!ballMoving && !paused)
        {
            angle = (rand() % 2 == 0 ? 0.5 : -0.5);
            speedX = speed * cos(angle);
            speedY = -speed * sin(angle);
            ballMoving = true;
            message = "";
        }
    }

    else if (ch == 27 || ch == 'p')
    { // ESC (or 'p')
        paused = !paused;

        if (paused)
        {
            message = "PAUSED. Press ESC to resume or ENTER to return to menu. ";
            ballMoving = false;
        }
        else
        {
            message = "";
            ballMoving = true;
        }
    }

    if (ch == 13 && paused)
    { // Enter when paused --> exit
        saveGame();
        return false;
    }
    return true;
}

void initialBricks()
{
    for (int i = 0; i < 5; i++)
    {
        for (int j = 0; j < 10; j++)
        {
            bricks[i][j] = false;
        }
    }
}

bool endOfGame()
{
    for (int i = 0; i < 5; i++)
    {
        for (int j = 0; j < 10; j++)
        {
            if (bricks[i][j] == true)
                return false;
        }
    }
    return true;
}

void updateBall()
{
    int prevX = ballX;
    int prevY = ballY;

    double nextX = ballX + speedX;
    double nextY = ballY + speedY;

    if (nextX <= 0.0)
    { // Hiting left wall
        if (sound)
        {
            Beep(800, 50);
            Sleep(10);
            Beep(1200, 40);
        }
        nextX *= (-1);
        speedX *= (-1);
    }
    if (nextX >= PLAYFIELD_WIDTH)
    { // Hiting right wall
        if (sound)
        {
            Beep(800, 50);
            Sleep(10);
            Beep(1200, 40);
        }
        nextX = 2 * PLAYFIELD_WIDTH - nextX;
        speedX *= (-1);
    }

    // Hiting top wall or bricks

    if (nextY < 12.0)
    { // When inside bricks area

        // Moving upward (hitting bottom of bricks)
        if (speedY < 0.0)
        {
            double penetration = 12.0 - nextY;
            int brick_line = (int)floor(penetration);
            int brickRow = 4 - (brick_line / 2);

            if (brickRow >= 0 && brickRow < 5)
            {
                int brickCol = (int)floor((nextX - BRICK_WIDTH) / (double)BRICK_WIDTH);
                if (brickCol >= 0 && brickCol < 10 && bricks[brickRow][brickCol])
                {
                    // Brick is hit
                    bricks[brickRow][brickCol] = false;
                    if (sound)
                    {
                        Beep(500, 15);
                        Beep(300, 10);
                    }
                    player.score += 100;

                    // Coordinates of side of a brick
                    double brick_left = BRICK_WIDTH + brickCol * BRICK_WIDTH;
                    double brick_right = brick_left + BRICK_WIDTH;

                    // Ball's center position after move
                    double ball_x = nextX;

                    // Distance from each side
                    double dist_left = ball_x - brick_left;
                    double dist_right = brick_right - ball_x;

                    // Is ball near left/right edge?
                    bool near_left_edge = dist_left <= 1.5 && dist_left >= -1.5;
                    bool near_right_edge = dist_right <= 1.5 && dist_right >= -1.5;

                    bool side_hit = false;

                    // For LEFT edge: only bounce if ball is moving RIGHT (ballDX > 0)
                    if (near_left_edge && speedX > 0)
                    {
                        side_hit = true;
                        speedX = -speedX;
                        nextX = ballX + speedX * 0.4;
                    }
                    // For RIGHT edge: only bounce if ball is moving LEFT (ballDX < 0)
                    else if (near_right_edge && speedX < 0)
                    {
                        side_hit = true;
                        speedX = -speedX;
                        nextX = ballX + speedX * 0.4;
                    }

                    // If not a side hit --> vertical bounce
                    if (!side_hit)
                    {
                        speedY = -speedY;
                        nextY = ballY + speedY * 0.4;
                    }
                }
            }
        }

        // Moving downward (hitting top of bricks)
        else if (speedY > 0.0)
        {
            double distance_from_top = nextY - 2.0;
            if (distance_from_top >= 0)
            {
                int brick_line = (int)floor(distance_from_top);
                int brickRow = brick_line / 2;

                if (brickRow >= 0 && brickRow < 5)
                {
                    int brickCol = (int)floor((nextX - BRICK_WIDTH) / (double)BRICK_WIDTH);
                    if (brickCol >= 0 && brickCol < 10 && bricks[brickRow][brickCol])
                    {
                        // Brick is hit
                        bricks[brickRow][brickCol] = false;
                        if (sound)
                        {
                            Beep(500, 15);
                            Beep(300, 10);
                        }
                        player.score += 100;

                        // Coordinates of side of a brick
                        double brick_left = BRICK_WIDTH + brickCol * BRICK_WIDTH;
                        double brick_right = brick_left + BRICK_WIDTH;

                        // Ball's center position after move
                        double ball_x = nextX;

                        // Distance from each side
                        double dist_left = ball_x - brick_left;
                        double dist_right = brick_right - ball_x;

                        // Is ball near left/right edge?
                        bool near_left_edge = dist_left <= 1.5 && dist_left >= -1.5;
                        bool near_right_edge = dist_right <= 1.5 && dist_right >= -1.5;

                        bool side_hit = false;

                        // For LEFT edge: only bounce if ball is moving RIGHT
                        if (near_left_edge && speedX > 0)
                        {
                            side_hit = true;
                            speedX = -speedX;
                            nextX = ballX + speedX * 0.4;
                        }
                        // For RIGHT edge: only bounce if ball is moving LEFT
                        else if (near_right_edge && speedX < 0)
                        {
                            side_hit = true;
                            speedX = -speedX;
                            nextX = ballX + speedX * 0.4;
                        }

                        // If not a side hit --> vertical bounce
                        if (!side_hit)
                        {
                            speedY = -speedY;
                            nextY = ballY + speedY * 0.4;
                        }
                    }
                }
            }
        }

        // Hitting top wall
        if (nextY < 0.0)
        {
            if (sound)
            {
                Beep(800, 50);
                Sleep(10);
                Beep(1200, 40);
            }
            nextY *= (-1);
            speedY *= (-1);
        }
    }

    if (nextY > 28.0 && speedY > 0.0)
    { // Hitting paddle line
        if (sound)
        {
            Beep(600, 60);
        }
        if (nextX >= paddleX && nextX <= paddleX + PADDLE_WIDTH)
        { // Ball top of paddle

            int paddle_middle = paddleX + PADDLE_WIDTH / 2.0;
            int distance = round(nextX) - paddle_middle;

            int n_distance;

            if (distance == 0)
            {
                n_distance = 0;
            }
            else if (distance == 1 || distance == 2)
            {
                n_distance = 1;
            }
            else if (distance == -1 || distance == -2)
            {
                n_distance = -1;
            }
            else if (distance == 3 || distance == 4)
            {
                n_distance = 2;
            }
            else if (distance == -3 || distance == -4)
            {
                n_distance = -2;
            }
            else if (distance == 5)
            {
                n_distance = 3;
            }
            else if (distance == -5)
            {
                n_distance = -3;
            }

            double angle_degrees;

            int abs_n = abs(n_distance);

            if (abs_n == 0)
            {
                angle_degrees = 90.0;
            }
            else if (abs_n == 1)
            {
                angle_degrees = 60.0;
            }
            else if (abs_n == 2)
            {
                angle_degrees = 45.0;
            }
            else
            {
                angle_degrees = 30.0;
            }

            // Mirror for left side
            if (n_distance < 0)
            {
                angle_degrees = 180.0 - angle_degrees;
            }

            double angle_rad = angle_degrees * M_PI / 180.0;

            nextY = 56.0 - nextY;

            speedX = speed * cos(angle_rad);
            speedY = -speed * sin(angle_rad);
        }
        else
        { // Ball hitting the ground
            player.HP -= 1;
            if (sound)
            {
                Beep(900, 50);
            }
            Sleep(50); //***
            ballX = paddleX + PADDLE_WIDTH / 2;
            ballY = 28;
            speedX = (rand() % 2 == 0 ? 0.8 : -0.8);
            speedY = -0.8;
            ballMoving = false;
            return;
        }
    }

    ballX = round(nextX);
    ballY = round(nextY);
}

void game()
{
    // If x==2, the game has entered through load game
    if (x == 1)
    {
        initialBricks();

        // Brick pattern in each level
        if (level == 1)
        {
            for (int i = 1; i < 3; i++)
            {
                for (int j = 2; j < 8; j++)
                {
                    bricks[i][j] = true;
                }
            }
        }
        else if (level == 2)
        {
            bricks[0][3] = true;
            bricks[0][6] = true;
            bricks[2][2] = true;
            bricks[2][7] = true;
            bricks[3][2] = true;
            bricks[3][3] = true;
            bricks[3][6] = true;
            bricks[3][7] = true;
            bricks[4][3] = true;
            bricks[4][4] = true;
            bricks[4][5] = true;
            bricks[4][6] = true;
        }
        else if (level == 3)
        {
            bricks[0][7] = true;
            bricks[0][6] = true;
            bricks[0][3] = true;
            bricks[0][2] = true;
            for(int i=1; i<5; i++)
            {
                for (int j=1; j<9; j++)
                    bricks[i][j]=true;
            }
        }
        else if (level == 4)
        {
            for (int i = 2; i < 8; i++)
                bricks[0][i];
            for (int i = 3; i < 7; i++)
                bricks[1][i];
            for (int i = 4; i < 6; i++)
                bricks[2][i];
            for (int i = 2; i < 8; i++)
                bricks[4][i];
            for (int i = 3; i < 7; i++)
                bricks[3][i];
        }
        else if (level == 5)
        {
            bricks[0][2] = true;
            bricks[0][3] = true;
            bricks[0][6] = true;
            bricks[0][7] = true;
            for (int i = 1; i < 9; i++)
                bricks[1][i] = true;
            for (int i = 2; i < 8; i++)
                bricks[2][i] = true;
            for (int i = 3; i < 7; i++)
                bricks[3][i] = true;
            bricks[4][4] = true;
            bricks[4][5] = true;
        }
        else if (level == 6)
        {
            for (int i = 0; i < 5; i++)
            {
                for (int j = 0; j < 10; j++)
                {
                    bricks[i][j] = true;
                }
            }
        }
        else
        {
            system("cls");
            cout << "You found easter egg!!";
            player.name += "✰";
            Sleep(3000);
            return;
        }
    }
    x = 1;

    ballX = paddleX + PADDLE_WIDTH / 2;
    ballY = 28;
    speedX = speed / (sqrt(2));
    speedY = -(speed / (sqrt(2)));
    ballMoving = false;
    paused = false;

    string message = " ";

    bool running = true;

    while (running)
    {
        running = cursor(paddleX, ballMoving, message, paused);

        if (!ballMoving && !paused)
        {
            ballX = paddleX + PADDLE_WIDTH / 2;
        }

        if (ballMoving && !paused)
        {
            updateBall();
        }

        drawAll(message);

        Sleep(40);

        if (player.HP == 0)
        // Losing
        {
            system("cls");
            if (sound)
            {
                Beep(200, 300);
                Beep(165, 300);
                Beep(130, 400);
            }
            gameover();
            Sleep(300);
            system("cls");
            Sleep(200);
            gameover();
            Sleep(3000);
            system("cls");
            level=1;
            //اینو اخیرا اضافه کردم
            return;
            //اینجا جای ریترن میشه رانینگ رو هم برعکس کرد ولی فعلا نمیتونم ران کنم ببینم کدوم بهتره
        }

        if (endOfGame())
        // Going to next level
        {
            level++;
            player.HP = 3;
            speed += 0.5;
            system("cls");
            if (sound)
            {
                Beep(523, 150);
                Beep(659, 150);
                Beep(784, 150);
                Beep(1046, 300);
            }
            win();
            Sleep(300);
            system("cls");
            Sleep(200);
            win();
            Sleep(3000);
            if (player.score > highScore())
            {
                system("cls");
                newrecord();
            }
            system("cls");
            game();
        }
    }
}

string livesDisplay()
{
    if (player.HP == 3)
    {
        return "♥ ♥ ♥";
    }
    if (player.HP == 2)
    {
        return "♥ ♥ ♡";
    }
    if (player.HP == 1)
    {
        return "♥ ♡ ♡";
    }
    return "♡ ♡ ♡";
}

void HideCursor()
{
    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_CURSOR_INFO ci;
    ci.dwSize = 100;
    ci.bVisible = false;
    SetConsoleCursorInfo(hOut, &ci);
}

void set_cursor(int x = 0, int y = 0)
{

    HANDLE handle;
    COORD coordinates;
    handle = GetStdHandle(STD_OUTPUT_HANDLE);
    coordinates.X = x;
    coordinates.Y = y;
    SetConsoleCursorPosition(handle, coordinates);
}

void exitGame()
{
    system("cls");
    cout << "Thanks for playing!\n";
    Sleep(800);
}

int menu()
{
    char a = ' ';
    string newGame = "NewGame";
    string loadGame = "Load Game";
    string help = "Help";
    string history = "History";
    string setting = "Setting";
    string Exit = "Exit";
    int x = 2;

    // Menu arrangement
    char arr[24][21];
    for (int i = 0; i < 24; i++)
    {
        for (int j = 0; j < 21; j++)
        {
            if ((j > 4 && j < 18 && i % 4 == 3) || (j > 4 && j < 18 && i % 4 == 0))
                arr[i][j] = '_';
            else
                arr[i][j] = ' ';
        }
    }
    arr[2][2] = '>';
    arr[2][3] = '>';
    arr[2][19] = '<';
    arr[2][20] = '<';

    for (int i = 0; i < 7; i++)
        arr[2][8 + i] = newGame[i];
    for (int i = 0; i < 9; i++)
        arr[6][7 + i] = loadGame[i];
    for (int i = 0; i < 4; i++)
        arr[10][9 + i] = help[i];
    for (int i = 0; i < 7; i++)
        arr[14][8 + i] = history[i];
    for (int i = 0; i < 7; i++)
        arr[18][8 + i] = setting[i];
    for (int i = 0; i < 4; i++)
        arr[22][9 + i] = Exit[i];

    // Menu displayment
    while (a != 13)
    {
        cout << "╔═══════┄Breakout┄═══════╗" << endl;
        for (int i = 0; i < 24; i++)
        {
            for (int j = 0; j < 21; j++)
            {
                if ((i == x - 1 || i == x || i == x + 1 || i == x - 2) && (j < 18 && j > 4))
                    cout << "\033[37;95m" << arr[i][j] << "\033[0m";
                else if ((j < 18 && j > 4))
                    cout << "\033[37;90m" << arr[i][j] << "\033[0m";
                else
                    cout << arr[i][j];
            }
            cout << endl;
        }
        cout << endl
             << "╚═══════════════════════╝";
        a = _getch();
        system("cls");

        arr[x][2] = ' ';
        arr[x][3] = ' ';
        arr[x][20] = ' ';
        arr[x][19] = ' ';

        if (a == 80)
            x += 4;
        else if (a == 72)
            x -= 4;
        else
            continue;

        if (x < 2)
            x = 22;
        if (x > 22)
            x = 2;

        arr[x][2] = '>';
        arr[x][3] = '>';
        arr[x][20] = '<';
        arr[x][19] = '<';
    }

    switch (x)
    {
    case 2:
        return 1; // New Game
    case 6:
        return 2; // loadgame
    case 10:
        return 3; // help
    case 14:
        return 4; // history
    case 18:
        return 5; // setting
    case 22:
        return 6; // Exit
    default:
        return 7;
    }
}

void savePlayer(Player player)
{
    ofstream file("info.txt", ios::app);
    if (!file)
    {
        cout << "No info found!";
        return;
    }

    file << player.name << ": " << player.score << endl;
}
void showHistory()
{
    ifstream infofile("info.txt");
    if (!infofile)
    {
        cout << "No info found!";
        return;
    }

    string s;
    while (getline(infofile, s))
    {
        cout << "  " << s << endl;
    }
    infofile.close();
}

int highScore()
{
    int max = 0;
    ifstream infofile("info.txt");
    if (!infofile)
    {
        cout << "No info found!";
        return 0;
    }

    string s;
    while (getline(infofile, s))
    {
        int pos = s.find_last_of(": ");
        string scoreStr = s.substr(pos + 1);
        int score = stoi(scoreStr);
        if (score > max)
            max = score;
    }
    infofile.close();
    return max;
}

void savelevel()
{
    ofstream file("level.txt");
    if (!file)
    {
        cout << "No info found!";
        return;
    }

    file << level << " " << player.HP;
    file.close();
}

void readlevel()
{
    ifstream file("level.txt");
    if (!file)
    {
        return;
    }
    file >> level >> player.HP;
    file.close();
}

void saveGame()
{
    ofstream file("game.txt");
    if (!file)
    {
        cout << "No info found!";
        return;
    }
    // Saving remaining bricks
    for (int i = 0; i < 5; i++)
    {
        for (int j = 0; j < 10; j++)
        {
            if (bricks[i][j] == true)
                file << i << " " << j << endl;
        }
    }
}

void loadGame()
{
    int i, j;
    ifstream file("game.txt");
    if (!file)
    {
        cout << "No info found!";
        return;
    }

    string s;
    while (file >> i >> j)
    {
        bricks[i][j] = true;
    }

    file.close();
}

void setting()
{
    char choise;
    while (true)
    {
        cout << "╔═══════┄setting┄═══════╗" << endl
             << " 1.ball" << endl
             << " 2.paddle" << endl
             << " 3.backgerand" << endl
             << " 4.language" << endl
             << " 5.difficulty" << endl
             << " 6.sound" << endl
             << " 7.back to menu" << endl
             << "╚═══════════════════════╝";
        choise = getch();
        system("cls");
        if (choise == '2')
        {
            cout << "1.╔═════════╗" << endl
                 << "2.∿∿∿∿∿∿∿∿∿" << endl
                 << "3.▛▞▞▞▞▞▞▍";
            char p = getch();
            switch (p)
            {
            case '1':
                Lpaddle = "╔";
                Rpaddle = "╗";
                Mpaddle = "═";
                break;
            case '2':
                Lpaddle = "∿";
                Rpaddle = "∿";
                Mpaddle = "∿";
                break;
            case '3':
                Lpaddle = "▛";
                Rpaddle = "▍";
                Mpaddle = "▞";
                break;
            default:
                Lpaddle = "┏";
                Rpaddle = "┓";
                Mpaddle = "━";
            }
        }
        else if (choise == '1')
        {
            cout << "1.◯" << endl
                 << "2.ꕥ" << endl
                 << "3.⭗" << endl
                 << "4.✿";
            char b = getch();
            switch (b)
            {
            case '1':
                ball = "◯";
                break;
            case '2':
                ball = "ꕥ";
                break;
            case '3':
                ball = "⭗";
                break;
            case '4':
                ball = "✿";
                break;
            default:
                ball = "●";
            }
        }
        else if (choise == '3')
        {
            char bg;
            color = "color ";
            cout << "1.black" << endl
                 << "2.blue" << endl
                 << "3.green" << endl
                 << "4.red" << endl
                 << "5.purple";
            char c = getch();
            switch (c)
            {
            case '2':
                bg = '3';
                break;
            case '3':
                bg = '3';
                break;
            case '4':
                bg = '4';
                break;
            case '5':
                bg = '5';
                break;
            default:
                bg = '0';
            }
            color += bg;
            color += 'F';
        }
        else if (choise == '4')
        {
            cout << "This was a decorative option. OwO";
            Sleep(500);
        }
        else if (choise == '5')
        {
            cout << "1.easy" << endl
                 << "2.normal" << endl
                 << "3.hard";
            char b = getch();
            switch (b)
            {
            case '1':
                speed = 1.2;
                break;
            case '2':
                speed = 2;
                break;
            case '3':
                speed = 3;
                break;
            default:
                speed = 1.7; // 2.65
            }
        }
        else if (choise == '6')
        {
            cout << "1.ON" << endl
                 << "2.OFF";
            char b = getch();
            sound = (b == '1' ? true : false);
        }
        else if (choise == '7')
            break;
        system("cls");
    }
}
void win()
{
    cout << endl
         << "██╗   ██╗ ██████╗ ██╗   ██╗    ██╗    ██╗██╗███╗   ██╗" << endl
         << "╚██╗ ██╔╝██╔═══██╗██║   ██║    ██║    ██║██║████╗  ██║" << endl
         << " ╚████╔╝ ██║   ██║██║   ██║    ██║ █╗ ██║██║██╔██╗ ██║" << endl
         << "  ╚██╔╝  ██║   ██║██║   ██║    ██║███╗██║██║██║╚██╗██║" << endl
         << "   ██║   ╚██████╔╝╚██████╔╝    ╚███╔███╔╝██║██║ ╚████║" << endl
         << "   ╚═╝    ╚═════╝  ╚═════╝      ╚══╝╚══╝ ╚═╝╚═╝  ╚═══╝";
}
void gameover()
{
    cout << endl
         << "██████╗   █████╗ ███╗   ███╗███████╗     ██████╗ ██╗   ██╗███████╗██████╗ " << endl
         << "██╔════╝ ██╔══██╗████╗ ████║██╔════╝    ██╔═══██╗██║   ██║██╔════╝██╔══██╗" << endl
         << "██║  ███╗███████║██╔████╔██║█████╗      ██║   ██║██║   ██║█████╗  ██████╔╝" << endl
         << "██║   ██║██╔══██║██║╚██╔╝██║██╔══╝      ██║   ██║╚██╗ ██╔╝██╔══╝  ██╔══██╗" << endl
         << "╚██████╔╝██║  ██║██║ ╚═╝ ██║███████╗    ╚██████╔╝ ╚████╔╝ ███████╗██║  ██║" << endl
         << " ╚═════╝ ╚═╝  ╚═╝╚═╝     ╚═╝╚══════╝     ╚═════╝   ╚═══╝  ╚══════╝╚═╝  ╚═╝";
         
}
void newrecord()
{
    if (sound)
    {
        Beep(784, 250);
        Beep(880, 250);
        Beep(988, 250);
        Beep(1047, 500);
    }
    cout << endl
         << "███╗   ██╗███████╗██╗    ██╗    ██████╗ ███████╗ ██████╗ ██████╗ ██████╗ ██████╗  " << endl
         << "████╗  ██║██╔════╝██║    ██║    ██╔══██╗██╔════╝██╔════╝██╔═══██╗██╔══██╗██╔══██╗" << endl
         << "██╔██╗ ██║█████╗  ██║ █╗ ██║    ██████╔╝█████╗  ██║     ██║   ██║██████╔╝██║  ██║" << endl
         << "██║╚██╗██║██╔══╝  ██║███╗██║    ██╔══██╗██╔══╝  ██║     ██║   ██║██╔══██╗██║  ██║" << endl
         << "██║ ╚████║███████╗╚███╔███╔╝    ██║  ██║███████╗╚██████╗╚██████╔╝██║  ██║██████╔╝" << endl
         << "╚═╝  ╚═══╝╚══════╝ ╚══╝╚══╝     ╚═╝  ╚═╝╚══════╝ ╚═════╝ ╚═════╝ ╚═╝  ╚═╝╚═════╝ ";
}