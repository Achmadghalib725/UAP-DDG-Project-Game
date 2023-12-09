#include <iostream>
#include <windows.h>
#include <stdlib.h>
#include <time.h>
#include <conio.h>

using namespace std;

bool exitGame;
bool gameOver;
bool isMoving;
bool bombExplode;
bool dead;
int height;
int width;
int speed;
int score;
int myCarx;
int myCary;
int highScore;
int highestScore[5];
int enemyX[4];
int enemyY[4]= {-8, -18, -28, -38};
int enemyPositionX;
int enemypositionY;
int enemyNum; 
int life=5;


string bombParticle1[4]={"o   o"," ooo "," ooo ","o   o"};
string bombParticle2[4]={" ooo ","o   o","o   o"," ooo "};
string myCar[4]={" * ",
                 "* *",
				 " * ",
				 "*  *"};
				 
void setConsoleCursorPosition(HANDLE hConsole, COORD coord) {
    SetConsoleCursorPosition(hConsole, coord);
}

void setConsoleCursorInfo(HANDLE hConsole, CONSOLE_CURSOR_INFO* cursorInfo) {
    SetConsoleCursorInfo(hConsole, cursorInfo);
}

				 

void gotoxy(int x,int y){
	COORD coord;
	coord.X=x;
	coord.Y=y;
	setConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}

void hideCursor(){
	CONSOLE_CURSOR_INFO cursor;
	cursor.dwSize=100;
	cursor.bVisible=false;
	setConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &cursor);
}

void startUp(){
	srand(time(NULL));
	exitGame=false;
	isMoving=false;
	gameOver=false;
	dead=false;
	height=20;
	width=19;
	myCarx=8;
	myCary=16;
	speed=1;
	enemyNum=4;
	score=0;
	
	for(int i=0;i< enemyNum; i++){
		enemyPositionX = rand()%3;
		if(enemyPositionX==0) enemyX[i]=2;
		else if(enemyPositionX==1) enemyX[i]=8;	
		else if(enemyPositionX==2) enemyX[i]=14;
	}
	enemyY[0]=-8;
	enemyY[1]=-18;
	enemyY[2]=-28;
    enemyY[3]=-38;
}

void layout(){
	for(int i=0; i<height;i++){
		gotoxy(0,i); cout << "±                  ±";
		if(i % 2 == 0 && isMoving){
			gotoxy(6,i); cout<<"|      |";
		}else if (i % 2 !=0 && !isMoving){
			gotoxy(6,i); cout<<"|      |";
		}
	}
	gotoxy(5,22); cout<< "Score : "<<score;
}

void drawMyCar(){
	if(!dead){
		for (int i=0; i<4; i++){
			gotoxy(myCarx,myCary+i); cout << myCar[i];
		}
	}else{
    	for (int i=0; i<4; i++){
	    gotoxy(myCarx,myCary+1); cout <<"   ";
    	}
    }
}

void drawEnemyCar(){
	for(int i=0; i<enemyNum; i++){
		if (enemyY[i]+3 > 0){
			gotoxy(enemyX[i], enemyY[i]+3);
			cout << " # ";
		}if(enemyY[i]+2 > 0){
			gotoxy(enemyX[i], enemyY[i]+2);
			cout << "# #";
		}if (enemyY[i]+1 > 0){
			gotoxy(enemyX[i], enemyY[i]+1);
			cout << " # ";
    	}if (enemyY[i] > 0){
			gotoxy(enemyX[i], enemyY[i]);
			cout << "# #";
        }if (enemyY[i]+3 >= 20){
			gotoxy(enemyX[i], enemyY[i]+3);
			cout << "   ";
		}if (enemyY[i]+2 >= 20){
			gotoxy(enemyX[i], enemyY[i]+2);
			cout << "   ";
    	}if (enemyY[i]+1 >= 20){
			gotoxy(enemyX[i], enemyY[i]+1);
			cout << "   ";
		}if (enemyY[i] >= 20){
			gotoxy(enemyX[i], enemyY[i]);
			cout << "   ";
        }
    }
}

void input() {
    if (_kbhit()) {
        switch (_getch()) {
            case 72:
                if (myCary > 0 && !dead)
                    myCary = myCary - speed;
                break;
            case 75:
                if (myCarx > 2 && !dead)  
                    myCarx = myCarx - 6;
                break;
            case 77:
                if (myCarx < 14 && !dead)
                    myCarx = myCarx + 6;
                break;
            case 80:
                if (myCary < 16 && !dead)
                    myCary = myCary + speed;
                break;
        }
    }
}

void mecha() {
    srand(time(NULL));

    if (isMoving)
        isMoving = false;
    else
        isMoving = true;

    for (int i = 0; i < enemyNum; i++) {
        enemyY[i]++;
    }

    for (int i = 0; i < enemyNum; i++) {
        if (enemyY[i] > 21) {
            enemyPositionX = rand() % 3;
            if (enemyPositionX == 0)
                enemyX[i] = 2;
            else if (enemyPositionX == 1)
                enemyX[i] = 8;
            else if (enemyPositionX == 2)
                enemyX[i] = 14;
            enemyY[i] = -8;
            score++;
        }
    }

    for (int i = 0; i < enemyNum; i++) {
        if ((myCary <= enemyY[i] + 3 && myCary >= enemyY[i] || enemyY[i] >= myCary && enemyY[i] <= myCary + 3) &&
            myCarx == enemyX[i]) {
            dead = true;
        }
    }
}

void bombExplosion() {
    if (bombExplode) {
        for (int i = 0; i < 4; i++) {
            gotoxy(myCarx - 1, myCary - 1 + i);
            cout << bombParticle1[i] << endl;
        }
        bombExplode = false;
    } else {
        for (int i = 0; i < 4; i++) {
            gotoxy(myCarx - 1, myCary - 1 + i);
            cout << bombParticle2[i] << endl;
        }
        bombExplode = true;
    }
    Sleep(100);
}


void died(){
	if(dead){
		life--;
		int count =0;
		while(count !=10){
			input();
			bombExplosion();
			gotoxy(2,22); cout << "You got "<<score<<endl;
			count++;
		}
		gotoxy(2,22); cout<< "                 ";
		highestScore[life] = score;
		startUp();
	}
}

void transition(){
	for(int i=19; i>=0; i--){
		gotoxy(1,i); cout << "##################";
		Sleep(15);
	}
	for(int i=1; i<20; i++){
		gotoxy(1,i); cout <<"                   ";
		Sleep(15);
	}
}

void game_Over(){
	for(int i=0; i<life; i++){
		if(highestScore[i] >=highestScore[i-1]) highScore=highestScore[i];
		else if(highestScore[i]<= highestScore[i-1]) highScore=highestScore[i-1];
	}
	if (life ==0){
		gameOver=true;
		do{ 
		gotoxy(0,0); cout << "#############################";
		gotoxy(0,1); cout << "#                           #";
		gotoxy(0,2); cout << "#                           #";
		gotoxy(0,3); cout << "#                           #";
		gotoxy(0,4); cout << "#                           #";
		gotoxy(0,5); cout << "#                           #";
		gotoxy(0,6); cout << "#        GAME OVER          #";
		gotoxy(0,7); cout << "#                           #";
		gotoxy(0,8); cout << "#                           #";
		gotoxy(0,9); cout << "#                           #";
		gotoxy(0,10); cout <<"#                           #";
		gotoxy(0,11); cout << "#                           #";
		gotoxy(0,12); cout << "#                           #";
		gotoxy(0,13); cout << "#                           #";
		gotoxy(0,14); cout << "#                           #";
		gotoxy(0,15); cout << "#                           #";
		gotoxy(0,16); cout << "#        GAME OVER          #";
		gotoxy(0,17); cout << "#                           #";
		gotoxy(0,18); cout << "#                           #";
		gotoxy(0,19); cout << "#############################";
		gotoxy(0,20); cout << "                             ";
		gotoxy(0,21); cout <<  "                             ";
		}while(getch() !='x');
		exit(1);
	}
}

void spiralEffect(){
	int row = 1,col =1;
	int last_row=height-2, last_col = width-2;
	
	while(row <= last_row && col<= last_col){
		for(int i=col; i<=last_col;i++){
			gotoxy(i,row);
			cout << "#";
			Sleep(2);
		}row++;
		
		for(int i=row; i<= last_row; i++){
			gotoxy(last_col,i); 
			cout<< "#";
		}last_col--;
		
		if(row <= last_row){
			for(int i=last_col; i >= col;i--){
			gotoxy(i,last_row);
			cout << "#";
			Sleep(1);
		}last_row--;
	}if(col <= last_col){
			for(int i=last_row; i >= row;i--){
			gotoxy(col,i);
			cout << "#";
    	}col++;
    }
}
row =1, col=1;
last_row = height-2,last_col=width-2;

while(row <= last_row && col <= last_col){
	for(int i = col; i<= last_col; i++){
        	gotoxy(i,row);
			cout << " ";
			Sleep(2);	
	}row++;
	
	for(int i = row; i<= last_row; i++){
        	gotoxy(last_col,i);
			cout << " ";
			Sleep(2);	
    }last_col--;
     
    if (row <= last_row){
    	for(int i = last_col; i>= col; i--){
    		gotoxy(i,last_row);
    		cout<<" ";
    		Sleep(1);
    	}last_row--;
    }if(col <= last_col){
    	for(int i = last_row; i>= row; i--){
    		gotoxy(col,i);
    		cout<<" ";
    	}col++;
    }
}
}
void games();
void play() {
    system("cls");
    startUp();
    spiralEffect();
    transition();
    games();

    while (!dead) {
        layout();
        input();
        mecha();
        drawMyCar();
        drawEnemyCar();
        died();
        game_Over();
        Sleep(50);
    }

    system("cls");
    cout << "Thank you for playing";
    exit(0);
}

void instructions() {
    system("cls");
    cout << "===============================================================================" << endl;
    cout << "|                              GAME INSTRUCTIONS                              |" << endl;
    cout << "===============================================================================" << endl;
    cout << "| 1. Use arrow keys to move your car.                                        |" << endl;
    cout << "| 2. Avoid collisions with other cars.                                       |" << endl;
    cout << "| 3. Press 'X' to exit the game at any time.                                 |" << endl;
    cout << "| 4. The game will end when you run out of lives.                            |" << endl;
    cout << "| 5. Your score will be displayed on the top right corner of the screen.    |" << endl;
    cout << "===============================================================================" << endl;
    cout << "Press any key to return to the main menu...";
    _getch();  // Wait for user input
}

void menuScreen() {
    do {
        system("cls");
        gotoxy(25, 5); cout << "------------------------------------------------------------------------------ ";
        gotoxy(25, 6); cout << "|    ######   ##   #####    ###### #      #   ##   ###### # ###### ##    #    | ";
        gotoxy(25, 7); cout << "|    #       #  #  #    #   #       #    #   #  #  #      # #    # # #   #    | ";
        gotoxy(25, 8); cout << "|    #      ###### #####    ######  #    #  ######  ##### # #    # #  #  #    | ";
        gotoxy(25, 9); cout << "|    #      #    # #    #   #        #  #   #    #      # # #    # #   # #    | ";
        gotoxy(25, 10); cout << "|    ###### #    # #    #   ######    ##    #    # ###### # ###### #    ##    | ";
        gotoxy(25, 11); cout << "------------------------------------------------------------------------------ ";
        gotoxy(45, 13); cout << "1. Start Game";
        gotoxy(45, 14); cout << "2. Instructions";
        gotoxy(45, 15); cout << "3. Quit";
        gotoxy(45, 17); cout << "Choose Option: ";
        char op = _getche();

        if (op == '1')
            play();
        else if (op == '2')
            instructions();
        else if (op == '3')
            exit(0);

    } while (1);
}


void games(){
	do{
		gotoxy(0,0); cout <<  "#############################";
		gotoxy(0,1); cout <<  "#                           #";
		gotoxy(0,2); cout <<  "#                           #";
		gotoxy(0,3); cout <<  "#                           #";
		gotoxy(0,4); cout <<  "#                           #";
		gotoxy(0,5); cout <<  "#                           #";
		gotoxy(0,6); cout <<  "# Tekan 'spasi' untuk mulai #";
		gotoxy(0,7); cout <<  "#                           #";
		gotoxy(0,8); cout <<  "#                           #";
		gotoxy(0,9); cout <<  "#                           #";
		gotoxy(0,10); cout << "#                           #";
		gotoxy(0,11); cout << "#                           #";
		gotoxy(0,12); cout << "#                           #";
		gotoxy(0,13); cout << "#                           #";
		gotoxy(0,14); cout << "#                           #";
		gotoxy(0,15); cout << "#                           #";
		gotoxy(0,16); cout << "#                           #";
		gotoxy(0,17); cout << "#                           #";
		gotoxy(0,18); cout << "#                           #";
		gotoxy(0,19); cout << "#############################";
		
		}while(getch() !=32);	
	}


int main(){
	hideCursor();
	startUp();
	menuScreen();
	spiralEffect();
	transition();
	games();
	while(!dead){
		layout();
		input();
		mecha();
		drawMyCar();
		drawEnemyCar();
		died();
		game_Over();
		Sleep(50);
}
system("cls");
cout << "thank you for playing";
}
