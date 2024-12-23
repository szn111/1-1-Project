#include<iostream>
using namespace std;
# include "iGraphics.h"
#include<windows.h>
#include<mmsystem.h>
#include<string.h>
#pragma comment(lib,"winmm.lib")
#pragma comment(lib,"libwinmm.a")

# define screen_width  1000 
# define screen_height 600
# define asteroid_number 5
# define bullet_number 200

# define spaceship_height 90
# define spaceship_width 102

#define rows 4
#define cols 4
#define alien1_number (rows * cols) 
#define alien1_width 50

#define alien2_number 5
# define alien2_missile_number 3
# define alien2_missile_width 30
# define alien2_height 70
 
# define boss_number 1
# define boss1_height 256
# define boss1_bullet_number 200
# define boss1_bullet_width 30
# define boss1_bullet_height 63

# define bg_slices 15
# define bg_slice_height 40

# define asteroid_width 46
# define asteroid_height 46
# define bullet_width 10
# define bullet_height 35
# define bullet_asteroid_height 80

# define number_of_menu_buttons 3
# define number_of_game_mode 3

int game_state = -1;
int game_mode_state = -1;

bool music_on = true;

int dx = 0; // spaceship x
int dy = 0; // spaceship y

int score = 0;
int health = 100;
char score_string[1000];
char health_string[1000];
int health_boss1 = 200;
char health_boss1_string[1000];

bool collision = false;
double collision_x = 0;
double collision_y = 0;
int collision_time = 0;

void intialize_asteroid();
void initialize_bullets();
void initialize_alien1();
void intialize_alien2();
void intialize_alien2_missile();
void initialize_boss1();
void intialize_boss1_bullet();

char home_menu_bg[30] = "Images\\home6.bmp"; 
char menu_button[number_of_menu_buttons][40] = {"Images\\1-start.bmp", "Images\\2-score.bmp", "Images\\3-exit.bmp"};
char game_mode_button[number_of_game_mode][40]={"Images\\1-easy.bmp", "Images\\2-medium.bmp", "Images\\3-hard.bmp"};

typedef struct
{
	int button_x;
	int button_y;
} button_coordinates;

button_coordinates button_coordinate[number_of_menu_buttons];

void intialize_menu_buttons(void)
{
	int y = 185;
	for (int i = 0; i < number_of_menu_buttons; i++)
	{
		button_coordinate[i].button_x = 430;
		button_coordinate[i].button_y = y;
		y -= 60;
	}
}

button_coordinates game_mode_coordinate[number_of_game_mode];

void initialize_game_mode_buttons(void)
{
	int y = 280;

	for (int i = 0; i < number_of_menu_buttons; i++)
	{
		game_mode_coordinate[i].button_x = 100;
		game_mode_coordinate[i].button_y = y;
		y -= 60;
	}

}

char bgimages[bg_slices][40] = {"Images\\renderBG\\tile014.bmp", "Images\\renderBG\\tile013.bmp", "Images\\renderBG\\tile012.bmp","Images\\renderBG\\tile011.bmp", "Images\\renderBG\\tile010.bmp", "Images\\renderBG\\tile009.bmp", "Images\\renderBG\\tile008.bmp",	"Images\\renderBG\\tile007.bmp",	"Images\\renderBG\\tile006.bmp",	"Images\\renderBG\\tile005.bmp",	"Images\\renderBG\\tile004.bmp",	"Images\\renderBG\\tile003.bmp",	"Images\\renderBG\\tile002.bmp",	"Images\\renderBG\\tile001.bmp",	"Images\\renderBG\\tile014.bmp"}; 
int bg_y[bg_slices];

void initialize_bg_y(void)
{
	int sum = 0;
	for (int i = 0; i < bg_slices; i++)
	{
		bg_y[i] = sum;
		sum += bg_slice_height;
	}
}

typedef struct 
{
	double bullet_x;
	double bullet_y;
	bool bullet_active;

} bullet_struct;

bullet_struct bullet_index[bullet_number];

typedef struct 
{
	int asteroid_x_coordinate;
	int asteroid_y_coordinate;
	int asteroid_speed;
	int asteroid_direction;
	int asteroid_picture;
	
} asteroid_struct;

asteroid_struct asteroid_index[asteroid_number];

typedef struct 
{
	int enemy1_x;
	int enemy1_y;
	int enemy1_index;
	bool enemy1_show;
	int direction;

} enemy1;

enemy1 alien1[alien1_number]; // alien1

asteroid_struct alien2[alien2_number]; // alien2

bullet_struct alien2_missile[alien2_missile_number];

typedef struct // boss1
{
	int boss_x;
	int boss_y;
	int boss_direction_x;
	int boss_direction_y;
	bool boss_active;
} boss_struct;

boss_struct boss1[boss_number];
bullet_struct boss1_bullet[boss1_bullet_number];

//for leaderboard
void newHighscore();
void readScore();
int len = 0;
char str1[100];
bool newscore = true;

typedef struct 
{
	char name[30];
	int player_score = 0;
} hscore;

hscore high_score[5];

void readScore()
{
    FILE *fp;
    fp = fopen("Score.txt", "r");
    if (fp == NULL) 
    {
        printf("Error opening file\n");
        return;
    }

    char showName[5][30], showScore[5][5];

    for (int i = 0; i < 5; i++)
    {
        fscanf(fp, "%s %d\n", high_score[i].name, &high_score[i].player_score);
        sprintf(showName[i], "%s", high_score[i].name);
        sprintf(showScore[i], "%d", high_score[i].player_score);
    }

    for (int i = 0; i < 5; i++)
    {
        iSetColor(207, 230, 10);
        iText(425, 350 - 50 * i, showName[i], GLUT_BITMAP_TIMES_ROMAN_24);
        iText(525, 350 - 50 * i, showScore[i], GLUT_BITMAP_TIMES_ROMAN_24);
    }

    fclose(fp);
}

void newHighscore()
{
    FILE *fp;
    fp = fopen("Score.txt", "r");
    if (fp == NULL) 
    {
        printf("Error opening file\n");
        return;
    }

    // read current high scores
    for (int i = 0; i < 5; i++) 
    {
        fscanf(fp, "%s %d\n", high_score[i].name, &high_score[i].player_score);
    }
    fclose(fp);

    if (newscore) 
    {
        // if new score a high score
        for (int i = 0; i < 5; i++) 
        {
            if (high_score[i].player_score < score) 
            {
                high_score[4].player_score = score;
                strcpy(high_score[4].name, str1);

                // descending order
                for (int j = 0; j < 5 - 1; j++) 
                {
                    for (int k = 0; k < 5 - j - 1; k++) 
                    {
                        if (high_score[k].player_score < high_score[k + 1].player_score) 
                        {

                            int tempScore = high_score[k].player_score;
                            high_score[k].player_score = high_score[k + 1].player_score;
                            high_score[k + 1].player_score = tempScore;

                            char tempName[30];
                            strcpy(tempName, high_score[k].name);
                            strcpy(high_score[k].name, high_score[k + 1].name);
                            strcpy(high_score[k + 1].name, tempName);
                        }
                    }
                }

                // write updated high scores
                fp = fopen("Score.txt", "w");
                if (fp == NULL) 
                {
                    printf("Error opeing file\n");
                    return;
                }

                for (int i = 0; i < 5; i++) 
                {
                    fprintf(fp, "%s %d\n", high_score[i].name, high_score[i].player_score);
                }
                fclose(fp);

                newscore = false; // new score handled
                break;
            }
        }
    }
}

void showChar(void)
{
	iSetColor(255, 255, 255);
	iText(330, 255, "Your name: ", GLUT_BITMAP_TIMES_ROMAN_24);
	iRectangle(455, 245, 160, 30); 
	iText(470, 255, str1, GLUT_BITMAP_TIMES_ROMAN_24); 
}

void takeinput(unsigned key)
{
		if (key == '\r' || key == '\n')
		{
			
			game_state = -1;
			health = 100;
			game_mode_state = -1;
			dx = 0;
			dy = 0;
			newHighscore();
			newHighscore();
            intialize_asteroid();
	        initialize_bullets();
	        initialize_alien1();
	        intialize_alien2();
	        intialize_alien2_missile();
	        initialize_boss1();
	        intialize_boss1_bullet();
			health_boss1 = 200;
			newscore = true;
	
		}

        else if(key=='\b')
        {
            if(len<=0)
			{
				len=0;
			}

            else
			{
				len--;
			}

            str1[len]='\0';
        }
        else
        {
            str1[len]=key;
            len++;

            if(len>15)len=15;

            str1[len]='\0';
        }

}

void show_home_menu(void)
{
    iShowBMP(0, 0, home_menu_bg);
    
    if(music_on)
    {
        iShowBMP2(10, 500, "Images\\music_button.bmp", 0);
    }
    
    else
    {
        iShowBMP2(10, 500, "Images\\music_off.bmp", 0);
    }

	for (int i = 0; i < number_of_menu_buttons; i++)
	{
		iShowBMP2(button_coordinate[i].button_x, button_coordinate[i].button_y, menu_button[i], 0);
	}
}

void show_leaderboard(void)
{
    iShowBMP(0, 0, "Images\\score.bmp");
	iShowBMP2(10, 500, "Images\\backbutton1.bmp", 0);
	readScore();
}

void show_mode_selection(void)
{
    iShowBMP(0, 0, "Images\\modepage2.bmp"); // background
	iShowBMP2(10, 500, "Images\\backbutton1.bmp", 0);
	initialize_game_mode_buttons();
    score = 0;

	for (int i = 0; i < number_of_game_mode; i++) // show game mode buttons
	{
		iShowBMP2(game_mode_coordinate[i].button_x, game_mode_coordinate[i].button_y, game_mode_button[i], 0);
	}
}

void show_bg_spaceship_bullet(void)
{
	for (int i = 0; i < bg_slices; i++) //background rendering
	{
		iShowBMP(0, bg_y[i], bgimages[i]);
	}

	iShowBMP2(dx, dy+2, "Images\\spaceship82.bmp", 0); // showing the spaceship

	for (int i = 0; i < bullet_number; i++) //bullets
	{
		if(bullet_index[i].bullet_active)
		{
			iShowBMP2(bullet_index[i].bullet_x, bullet_index[i].bullet_y, "Images\\bullet3.bmp", 0);
		}
	}

	iShowBMP2(10, 500, "Images\\homebutton2.bmp", 0); // showing the homebutton

}

void show_score_health(void)
{
	iSetColor(255, 255, 255);
	iText(825, 550, "SCORE   : ", GLUT_BITMAP_TIMES_ROMAN_24);

	sprintf(score_string, "%d", score);
	iText(940, 550, score_string, GLUT_BITMAP_TIMES_ROMAN_24);

	iText(825, 520, "HEALTH: ", GLUT_BITMAP_TIMES_ROMAN_24);
	sprintf(health_string, "%d", health);
	iText(940, 520, health_string, GLUT_BITMAP_TIMES_ROMAN_24);

}

void game_over(void)
{
	iShowBMP(0, 0, "Images\\gameover3.bmp");
	showChar();

	iSetColor(255, 255, 255);
	iText(420, 320, "SCORE ", GLUT_BITMAP_TIMES_ROMAN_24);

	sprintf(score_string, "%d", score);
	iText(510, 320, score_string, GLUT_BITMAP_TIMES_ROMAN_24);
}

void show_asteroid(void)
{
    for (int i = 0; i < asteroid_number; i++)
    {
        if (asteroid_index[i].asteroid_picture == 1)
        {
            iShowBMP2(asteroid_index[i].asteroid_x_coordinate, asteroid_index[i].asteroid_y_coordinate, "Images\\meteor2.bmp", 0);
        }
        else
        {
            iShowBMP2(asteroid_index[i].asteroid_x_coordinate, asteroid_index[i].asteroid_y_coordinate, "Images\\enemy6.bmp", 0);
        }
    }

    if (collision)
    {
        iShowBMP2(collision_x - 10, collision_y, "Images\\explosion.bmp", 0); // collision picture
    }
}

void show_alien1(void)
{
    for (int i = 0; i < alien1_number; i++)
    {
        if (alien1[i].enemy1_show == true)
        {
            iShowBMP2(alien1[i].enemy1_x, alien1[i].enemy1_y, "Images\\alien.bmp", 0);
        }
    }
}

void show_alien2(void)
{
    for (int i = 0; i < alien2_number; i++)
    {
        if (game_mode_state == 1)
        {
            iShowBMP2(alien2[i].asteroid_x_coordinate, alien2[i].asteroid_y_coordinate, "Images\\alien3.bmp", 0);
        }
        else
        {
            iShowBMP2(alien2[i].asteroid_x_coordinate, alien2[i].asteroid_y_coordinate, "Images\\alien2.bmp", 0);
        }
    }

    for (int i = 0; i < alien2_missile_number; i++)
    {
        if (alien2_missile[i].bullet_active)
        {
            if (game_mode_state == 1)
            {
                iShowBMP2(alien2_missile[i].bullet_x, alien2_missile[i].bullet_y, "Images\\missile2.bmp", 0);
            }
            else
            {
                iShowBMP2(alien2_missile[i].bullet_x, alien2_missile[i].bullet_y, "Images\\missile3.bmp", 0);
            }
        }
    }

    if (collision)
    {
        iShowBMP2(collision_x - 10, collision_y, "Images\\explosion.bmp", 0); // collision picture
    }
}

void show_boss1(void)
{
    for (int i = 0; i < boss_number; i++)
    {
        if (boss1[i].boss_active)
        {
            iShowBMP2(boss1[i].boss_x, boss1[i].boss_y, "Images\\enemyship2.bmp", 0);
        }
    }

    for (int k = 0; k < boss_number; k++)
    {
        if (boss1[k].boss_active)
        {
            for (int i = 0; i < boss1_bullet_number; i++)
            {
                if (boss1_bullet[i].bullet_active)
                {
                    iShowBMP2(boss1_bullet[i].bullet_x, boss1_bullet[i].bullet_y, "Images\\blue1.bmp", 0);
                }
            }
        }
    }

    int count = 0;
    for (int k = 0; k < boss_number; k++)
    {
        if (!boss1[k].boss_active)
        {
            count++;
        }
    }

    if (count == boss_number) // if all bosses have been eliminated
    {
        iSetColor(52, 235, 85);
        iText(400, 550, "BOSS DEFEATED!", GLUT_BITMAP_TIMES_ROMAN_24);
    }
    else
    {
        iSetColor(52, 235, 85);
        iText(400, 550, "BOSS HEALTH: ", GLUT_BITMAP_TIMES_ROMAN_24);
        sprintf(health_boss1_string, "%d", health_boss1);
        iText(580, 550, health_boss1_string, GLUT_BITMAP_TIMES_ROMAN_24);
    }

    if (collision)
    {
        iShowBMP2(collision_x - 10, collision_y, "Images\\explosion.bmp", 0); // collision picture
    }
}

void iDraw() {
	//place your drawing codes here
	iClear();

	if (game_state == -1) // shows home menu
	{
		show_home_menu();
	}

	else if (game_state == 2) // exit button
	{
		exit(0);
	}

	else if (game_state == 1) // leaderboard button
	{
        show_leaderboard();
	}

	else if (game_state == 0) // play button
	{
		if (game_mode_state == -1) // mode selection
		{
            show_mode_selection();			
		}

		else //for every game mode state
		{
            show_bg_spaceship_bullet();
            show_score_health();
		}

		if (game_mode_state == 0) // easy mode
		{
            show_asteroid();
		}

		else if (game_mode_state == 1)  // if medium mode
		{          
            show_alien2();
            show_alien1();
		}

		else if (game_mode_state == 2) // if hard mode
		{
            show_alien2();
            show_boss1();
		}

	}

	else if (game_state == 3) // game over
	{
        game_over();
	}


    if (game_mode_state != -1) // so that the menu button shows above every picture
	{
		iShowBMP2(10, 500, "Images\\homebutton2.bmp", 0);
	}
}

void iMouseMove(int mx, int my) { printf("x = %d, y= %d\n",mx,my); } /*function iMouseMove() is called when the user presses and drags the mouse. (mx, my) is the position where the mouse pointer is. */


int music_count = 0; /*function iMouse() is called when the user presses/releases the mouse. (mx, my) is the position where the mouse pointer is.*/
void iMouse(int button, int state, int mx, int my) {

	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {

        if (game_state == - 1)
		{
		for (int i = 0; i < number_of_menu_buttons; i++)
		{
			if ((mx >= button_coordinate[i].button_x && mx <= button_coordinate[i].button_x + 200) && (my >= button_coordinate[i].button_y && my <= button_coordinate[i].button_y + 73) )
			{
				game_state = i;
			}
		}

        if ((mx >= 25 && mx <= 93) && (my >= 510 && my <= 590)) // music button press
		{
            music_count++;

            if(music_count % 2 != 0)
            {
            music_on = false;
			PlaySound(0, 0, 0);
            }

            else
            {
                music_on = true;
                PlaySound("music\\music4.wav",NULL,SND_LOOP|SND_ASYNC);
            }
		}

		}

		if (game_state == 0)
		{
			for (int i = 0; i < number_of_game_mode; i++)
			{
				if ((mx >= game_mode_coordinate[i].button_x && mx <= game_mode_coordinate[i].button_x + 200) && (my >= game_mode_coordinate[i].button_y && my <= game_mode_coordinate[i].button_y + 73) )
				{
					game_mode_state = i;
				}
			}
		}

		if (game_state == 1 || game_mode_state == -1) // if back button pressed
		{
		if ((mx >= 25 && mx <= 93) && (my >= 510 && my <= 583))
		{
			game_state = -1;
		}

		}

		if(game_mode_state != -1) // if home button pressed
		{
			if ((mx >= 25 && mx <= 93) && (my >= 510 && my <= 583))
			{
			game_state = -1;
			game_mode_state = -1;
			health = 100;
			dx = 0;
			dy = 0;
            newscore = false;
			newHighscore();
            intialize_asteroid();
	        initialize_bullets();
	        initialize_alien1();
	        intialize_alien2();
	        intialize_alien2_missile();
	        initialize_boss1();
	        intialize_boss1_bullet();
			health_boss1 = 200;
			newscore = true;
			}
		}
	}

}
/*
	function iKeyboard() is called whenever the user hits a key in keyboard.
	key- holds the ASCII value of the key pressed.
	*/
void iKeyboard(unsigned char key) {
	if (key == 'q' || key == 'Q') {
		exit(0);
	}


	if (key == ' ')
	{
		for (int i = 0; i < bullet_number; i++)
		{
			if(!bullet_index[i].bullet_active)
			{
				bullet_index[i].bullet_active = true;
				bullet_index[i].bullet_x = dx + spaceship_width/2 - 18;
				bullet_index[i].bullet_y = dy + spaceship_height - 15;
				break;
			}
		}
	}

	if (game_state == 3)
	{
		takeinput(key);
	}

	if (game_mode_state != -1)
	{
		if (key == 'p' || key == 'P')
		{
			iPauseTimer(0);
			iPauseTimer(1);
			iPauseTimer(2);
		}
	}

		if (game_mode_state != -1)
	{
		if (key == 'r' || key == 'R')
		{
			iResumeTimer(0);
			iResumeTimer(1);
			iResumeTimer(2);
		}
	}

	//place your codes for other keys here
}

/*
	function iSpecialKeyboard() is called whenver user hits special keys like-
	function keys, home, end, pg up, pg down, arraows etc. you have to use
	appropriate constants to detect them. A list is:
	GLUT_KEY_F1, GLUT_KEY_F2, GLUT_KEY_F3, GLUT_KEY_F4, GLUT_KEY_F5, GLUT_KEY_F6,
	GLUT_KEY_F7, GLUT_KEY_F8, GLUT_KEY_F9, GLUT_KEY_F10, GLUT_KEY_F11, GLUT_KEY_F12,
	GLUT_KEY_LEFT, GLUT_KEY_UP, GLUT_KEY_RIGHT, GLUT_KEY_DOWN, GLUT_KEY_PAGE UP,
	GLUT_KEY_PAGE DOWN, GLUT_KEY_HOME, GLUT_KEY_END, GLUT_KEY_INSERT
	*/
void iSpecialKeyboard(unsigned char key) {

	//if (key == GLUT_KEY_END) {
		//exit(0);
	//}

	if (key == GLUT_KEY_RIGHT)
	{
		if (dx <= screen_width - spaceship_width + 5)
		{
		dx += 5;
		}		
	}

	if (key == GLUT_KEY_LEFT)
	{
		if (dx >= 5)
		{
			dx -= 5;
		}
	}

	if (key == GLUT_KEY_UP)
	{
		if (dy <= screen_height - spaceship_height - 20)
		{
			dy += 5;
		}
	}

		if (key == GLUT_KEY_DOWN)
	{
		if (dy >= 5)
		{
			dy -= 5;
		}
	}

	//place your codes for other keys here
}


// collision between different sprites requires different changes to be made in the if part
// could've been handled in a better way 

void collision_check(void)
{
    if (game_mode_state == 0) // easy mode
    {
        // bullet and asteroid collision
        for (int i = 0; i < bullet_number; i++)
        {
            if (bullet_index[i].bullet_active)
            {
                for (int j = 0; j < asteroid_number; j++)
                {
                    if (bullet_index[i].bullet_x < asteroid_index[j].asteroid_x_coordinate + asteroid_width &&
                        bullet_index[i].bullet_x + bullet_width > asteroid_index[j].asteroid_x_coordinate &&
                        bullet_index[i].bullet_y < asteroid_index[j].asteroid_y_coordinate + asteroid_height &&
                        bullet_index[i].bullet_y + bullet_height > asteroid_index[j].asteroid_y_coordinate)
                    {
                        collision = true;
                        score += 10;
                        collision_x = asteroid_index[j].asteroid_x_coordinate;
                        collision_y = asteroid_index[j].asteroid_y_coordinate;
                        collision_time = 10;

                        bullet_index[i].bullet_active = false; // bullet disappears

                        // Initializes the asteroid
                        asteroid_index[j].asteroid_x_coordinate = rand() % screen_width;
                        asteroid_index[j].asteroid_y_coordinate = screen_height - 70;
                    }
                }
            }
        }

        // collison between asteroid and spaceship
        for (int i = 0; i < asteroid_number; i++)
        {
            if (dx < asteroid_index[i].asteroid_x_coordinate + asteroid_width &&
                dx + spaceship_width > asteroid_index[i].asteroid_x_coordinate &&
                dy < asteroid_index[i].asteroid_y_coordinate + asteroid_height &&
                dy + spaceship_height > asteroid_index[i].asteroid_y_coordinate)
            {
                health -= 5;
                collision = true;

                collision_x = dx + 25;
                collision_y = dy + 50;
                collision_time = 10;

                // Initializes the asteroid
                asteroid_index[i].asteroid_x_coordinate = rand() % screen_width;
                asteroid_index[i].asteroid_y_coordinate = screen_height - 70;
            }
        }
    }

    // spaceship and alien1 collision
    if (game_mode_state == 1) // medium
    {
        for (int i = 0; i < bullet_number; i++)
        {
            if (bullet_index[i].bullet_active)
            {
                for (int j = 0; j < alien1_number; j++)
                {
                    if (alien1[j].enemy1_show)
                    {
                        if (bullet_index[i].bullet_x < alien1[j].enemy1_x + alien1_width - 20 &&
    bullet_index[i].bullet_x + bullet_width > alien1[j].enemy1_x + 20 &&
    bullet_index[i].bullet_y < alien1[j].enemy1_y + alien1_width &&
    bullet_index[i].bullet_y + bullet_height > alien1[j].enemy1_y)
                        {
                            collision = true;
                            score += 10;
                            collision_x = alien1[j].enemy1_x;
                            collision_y = alien1[j].enemy1_y;
                            collision_time = 10;

                            bullet_index[i].bullet_active = false; // bullet disappears
                            alien1[j].enemy1_show = false;
                        }
                    }
                }
            }
        }
    }

    // bullet and alien2 collision
    if (game_mode_state == 1 || game_mode_state == 2)
    {
        for (int i = 0; i < bullet_number; i++)
        {
            if (bullet_index[i].bullet_active)
            {
                for (int j = 0; j < alien2_number; j++)
                {
                    if (bullet_index[i].bullet_x < alien2[j].asteroid_x_coordinate + alien2_height &&
                        bullet_index[i].bullet_x + bullet_width > alien2[j].asteroid_x_coordinate &&
                        bullet_index[i].bullet_y < alien2[j].asteroid_y_coordinate + alien2_height &&
                        bullet_index[i].bullet_y + bullet_height > alien2[j].asteroid_y_coordinate)
                    {
                        collision = true;
                        score += 10;
                        collision_x = alien2[j].asteroid_x_coordinate;
                        collision_y = alien2[j].asteroid_y_coordinate;
                        collision_time = 10;

                        bullet_index[i].bullet_active = false; // bullet disappears

                        // Initializes alien2
                        alien2[j].asteroid_x_coordinate = (rand() % 2 == 0) ? 0 : screen_width - alien2_height;
                        if (alien2[j].asteroid_x_coordinate == 0)
                        {
                            alien2[j].asteroid_direction = -1;
                        }
                        else
                        {
                            alien2[j].asteroid_direction = 1;
                        }
                        alien2[j].asteroid_y_coordinate = rand() % (screen_height - alien2_height) + 1;
                        alien2[j].asteroid_speed = rand() % 5 + 2;
                    }
                }
            }
        }

        // spaceship and missile2 collsion
        for (int i = 0; i < alien2_missile_number; i++)
        {
            if ((alien2_missile[i].bullet_x + alien2_missile_width > dx) && 
                (alien2_missile[i].bullet_x < dx + spaceship_width - alien2_missile_width + 10) && 
                (alien2_missile[i].bullet_y + alien2_missile_width > dy) && 
                (alien2_missile[i].bullet_y < dy + spaceship_height))  
            {
                health -= 5;
                collision = true;

                collision_x = dx + 25;
                collision_y = dy + 50;
                collision_time = 10;

                // Initializes the missile
                int j = rand() % alien2_number;

                alien2_missile[i].bullet_x = alien2[j].asteroid_x_coordinate;
                alien2_missile[i].bullet_y = alien2[j].asteroid_y_coordinate;
                alien2_missile[i].bullet_active = true;
            }
        }
    }

    // spaceship's bullet and boss collision
    if (game_mode_state == 2)
    {
        for (int i = 0; i < boss_number; i++)
        {
            if (bullet_index[i].bullet_active)
            {
                for (int j = 0; j < boss_number; j++)
                {
                    if (boss1[j].boss_active)
                    {
                        if (bullet_index[i].bullet_x < boss1[j].boss_x + boss1_height - 30 && 
                        bullet_index[i].bullet_x + bullet_width > boss1[j].boss_x - 25 &&
    bullet_index[i].bullet_y < boss1[j].boss_y + boss1_height &&
    bullet_index[i].bullet_y + bullet_height > boss1[j].boss_y)
                        {
                            collision = true;
                            score += 10;
                            health_boss1 -= 10;
                            collision_x = bullet_index[i].bullet_x + bullet_width;
                            collision_y = bullet_index[i].bullet_y + bullet_width;
                            collision_time = 10;

                            bullet_index[i].bullet_active = false; // bullet disappears

                            if (health_boss1 <= 0)
                            {
                                boss1[j].boss_active = false;
                            }
                        }
                    }
                }
            }
        }

        // spaceship and boss bullet collsion
        for (int i = 0; i < boss1_bullet_number; i++)
        {
            if (boss1_bullet[i].bullet_active)
            {
                if ((boss1_bullet[i].bullet_x + boss1_bullet_width > dx) && 
                    (boss1_bullet[i].bullet_x < dx + spaceship_width - boss1_bullet_width + 10) &&
                    (boss1_bullet[i].bullet_y + boss1_bullet_height > dy) &&
                    (boss1_bullet[i].bullet_y < dy + spaceship_height)) 
                {
                    health -= 5;
                    collision = true;

                    collision_x = dx + 25;
                    collision_y = dy + 50;
                    collision_time = 10;
                    boss1_bullet[i].bullet_active = false;
                }
            }
        }
    }

    if (health <= 0)
    {
        game_state = 3;
    }
}

void change(void)
{
    if (game_mode_state == 0)
    {
        for (int i = 0; i < asteroid_number; i++)
        {
            if (asteroid_index[i].asteroid_y_coordinate >= 0)
            {
                asteroid_index[i].asteroid_y_coordinate -= asteroid_index[i].asteroid_speed;
            }
            else
            {
                asteroid_index[i].asteroid_picture *= -1;
                asteroid_index[i].asteroid_x_coordinate = rand() % screen_width;
                asteroid_index[i].asteroid_y_coordinate = screen_height - 70;
                asteroid_index[i].asteroid_speed = rand() % 4 + 3;
            }
        }
    }

    for (int i = 0; i < bullet_number; i++)
    {
        if (bullet_index[i].bullet_active)
        {
            if (bullet_index[i].bullet_y <= screen_height)
            {
                bullet_index[i].bullet_y += 10;
            }
            else
            {
                bullet_index[i].bullet_active = false;
            }
        }
    }

    if (game_mode_state == 0 || game_mode_state == 1 || game_mode_state == 2)
    {
        collision_check();
    }

    if (collision)
    {
        collision_time--;
        if (collision_time <= 0)
        {
            collision = false;
        }
    }

    if (game_mode_state == 1)
    {
        // alien1 movement
        bool reverseDirection = false;
        for (int j = 0; j < rows; j++) 
        {
            int firstIndexInRow = j * cols;
            int lastIndexInRow = firstIndexInRow + cols - 1;

            if (alien1[firstIndexInRow].enemy1_x <= 10 ||
                alien1[lastIndexInRow].enemy1_x >= screen_width - 50) 
            {
                reverseDirection = true;
                break;
            }
        }

        // reverse direction 
        if (reverseDirection) 
        {
            for (int i = 0; i < alien1_number; i++) 
            {
                alien1[i].direction *= -1;
            }
        }

        // updating positions 
        for (int i = 0; i < alien1_number; i++) 
        {
            alien1[i].enemy1_x += 10 * alien1[i].direction;

            alien1[i].enemy1_index++;

            if (alien1[i].enemy1_index >= alien1_number)
            {
                alien1[i].enemy1_index = 0;            
            }
        }
    }

    if (game_mode_state == 1 || game_mode_state == 2)
    {
        for (int i = 0; i < alien2_number; i++)
        {
            if (alien2[i].asteroid_x_coordinate >= -300 && alien2[i].asteroid_x_coordinate <= screen_width)
            {
                alien2[i].asteroid_x_coordinate -= (alien2[i].asteroid_speed) * (alien2[i].asteroid_direction);
            }
            else
            {
                alien2[i].asteroid_x_coordinate = (rand() % 2 == 0) ? 0 : screen_width - alien2_height;
                if (alien2[i].asteroid_x_coordinate == 0)
                {
                    alien2[i].asteroid_direction = -1;
                }
                else
                {
                    alien2[i].asteroid_direction = 1;
                }
                alien2[i].asteroid_y_coordinate = rand() % (screen_height - alien2_height) + 1;
                alien2[i].asteroid_speed = rand() % 5 + 2;
            }
        }

        // alien2 missile
        for (int i = 0; i < alien2_missile_number; i++)
        {
            if (alien2_missile[i].bullet_active)
            {
                if (alien2_missile[i].bullet_y >= -50)
                {
                    alien2_missile[i].bullet_y -= 5;
                }
                else
                {
                    alien2_missile[i].bullet_active = false;
                }
            }
            else // if false, initialize again
            {
                int j = rand() % alien2_number;

                alien2_missile[i].bullet_x = alien2[j].asteroid_x_coordinate;
                alien2_missile[i].bullet_y = alien2[j].asteroid_y_coordinate;
                alien2_missile[i].bullet_active = true;
            }
        }

        static int shoot_timer = 0; 

        // Boss shooting bullets at regular intervals
        shoot_timer++;
        if (shoot_timer >= 10) // adjusting interval every 10 frames
        {
            for (int i = 0; i < boss1_bullet_number; i++)
            {
                if (boss1[0].boss_active)
                {
                    if (!boss1_bullet[i].bullet_active) 
                    {
                        boss1_bullet[i].bullet_x = boss1[0].boss_x + boss1_height / 2 - 7; // spawn from alien
                        boss1_bullet[i].bullet_y = boss1[0].boss_y;    // spawn point
                        boss1_bullet[i].bullet_active = true;
                        break; // exiting loop after one bullet
                    }
                }
            }
            shoot_timer = 0; 
        }

        // update bullets
        for (int i = 0; i < boss1_bullet_number; i++)
        {
            if (boss1_bullet[i].bullet_active)
            {
                boss1_bullet[i].bullet_y -= 10;

                // if bullet hits the ground
                if (boss1_bullet[i].bullet_y < 0)
                {
                    boss1_bullet[i].bullet_active = false;
                }
            }
        }
    }
}

void change2(void)
{
    for (int i = 0; i < bg_slices; i++)
    {
        if (bg_y[i] < bg_slice_height)
        {
            bg_y[i] = screen_height - bg_slice_height;
        }
        else
        {
            bg_y[i] -= bg_slice_height;
        }
    }

    // movement of boss1 handled by change2
    if (game_mode_state == 2)
    {
        for (int i = 0; i < boss_number; i++)
        {
            if (boss1[i].boss_active)
            {
                int l = rand() % 2;

                if (l == 0)
                {
                    boss1[i].boss_x += 5 * boss1[i].boss_direction_x;

                    if (boss1[i].boss_x + 256 >= screen_width || boss1[i].boss_x <= 0)
                    {
                        boss1[i].boss_direction_x *= -1;
                    }
                }
                else
                {
                    boss1[i].boss_y -= 5 * boss1[i].boss_direction_y;

                    if (boss1[i].boss_y >= 450 || boss1[i].boss_y <= 200)
                    {
                        boss1[i].boss_direction_y *= -1;
                    }
                }
            }
        }
    }
}

void change3(void)
{
    // spaceship and alien1 collision
    if (game_mode_state == 0 || game_mode_state == 1 || game_mode_state == 2)
    {
        if (game_mode_state == 1)
        {
            for (int i = 0; i < alien1_number; i++)
            {
                if (alien1[i].enemy1_show)
                {
                    if (abs(dy - alien1[i].enemy1_y) < spaceship_height && abs(dx - alien1[i].enemy1_x) <= spaceship_width)
                    {
                        health -= 5;
                        collision = true;

                        collision_x = dx + 25;
                        collision_y = dy + 50;
                        collision_time = 10;
                    }
                }
            }
        }
    }

    // alien2 and spaceship collision
    if (game_mode_state == 1 || game_mode_state == 2)
    {
        for (int i = 0; i < alien2_number; i++)
        {
            if (dx < alien2[i].asteroid_x_coordinate + alien2_height &&
                dx + spaceship_width > alien2[i].asteroid_x_coordinate &&
                dy < alien2[i].asteroid_y_coordinate + alien2_height - 10 &&
                dy + spaceship_height > alien2[i].asteroid_y_coordinate)
            {
                health -= 5;
                collision = true;

                collision_x = dx + 25;
                collision_y = dy + 50;
                collision_time = 10;
            }
        }
    }
}

void initialize_alien1(void)
{
	int common_x = rand() % (screen_width - (cols * (alien1_width + 10))); // ensures the matrix fits horizontally.
	int common_y = 450 + rand()%5;
	int index = 0;

	for (int i = 0; i < rows; i++)
	{
		for (int j = 0; j < cols; j++)
		{
			if (index >= alien1_number)
			{
				return;
			}

			alien1[index].enemy1_x = common_x + j * (alien1_width + 5); 
			alien1[index].enemy1_y = common_y;
			
			alien1[index].enemy1_index = rand()%10;
	        alien1[index].enemy1_show = true;                              
			alien1[index].direction = 1;
			index++;		
		}

		// Initializing common_y for the next row
		common_y -= alien1_width + 10 ; // alien1's height and width same

		
	}
}

void intialize_alien2(void)
{
    // intializing all alien2
	for (int i = 0; i < alien2_number; i++)
	{ 
		alien2[i].asteroid_x_coordinate = (rand()%2 == 0)? 0: screen_width - alien2_height;
		if  (alien2[i].asteroid_x_coordinate == 0)
		{
			alien2[i].asteroid_direction = -1;
		}
		else
		{
			alien2[i].asteroid_direction = 1;
		}

		alien2[i].asteroid_y_coordinate = rand() % 351 + 180;
		alien2[i].asteroid_speed = rand()% 5 + 2;

	}
}

void intialize_alien2_missile(void)
{
	for (int i = 0; i < alien2_missile_number; i++)
	{
		int j = rand() % alien2_number;

		alien2_missile[i].bullet_x = alien2[j].asteroid_x_coordinate;
		alien2_missile[i].bullet_y = alien2[j].asteroid_y_coordinate;
		alien2_missile[i].bullet_active = true;

	}
}

void initialize_boss1(void)
{
	for (int i = 0; i < boss_number; i++)
	{
		boss1[i].boss_x = 0;
		boss1[i].boss_y = 450;
		boss1[i].boss_direction_x = 1;
		boss1[i].boss_direction_y = 1;
		boss1[i].boss_active = true;
	}
}

void intialize_boss1_bullet(void)
{

    for (int i = 0; i < boss1_bullet_number; i++)
	{
		boss1_bullet[i].bullet_active = false;
	}

}

void intialize_asteroid(void)
{
    // intializing all aesteroid
	for (int i = 0; i < asteroid_number; i++)
	{ 
		asteroid_index[i].asteroid_x_coordinate = rand()%screen_width;
		asteroid_index[i].asteroid_y_coordinate = screen_height - 70;
		asteroid_index[i].asteroid_speed = rand()% 5 + 3;
		asteroid_index[i].asteroid_picture = (rand()%2 == 0)? 1: -1;

	}
}

void initialize_bullets(void)
{
	for (int i = 0; i < bullet_number; i++)
	{
		bullet_index[i].bullet_active = false;
	}
}

int main() {
	//place your own initialization codes here.
	srand(NULL);
	
	if (music_on)
	{
		PlaySound("music\\music4.wav",NULL,SND_LOOP|SND_ASYNC);
	}

	intialize_menu_buttons();
	initialize_bg_y();
    intialize_asteroid();
	initialize_bullets();
	initialize_alien1();
	intialize_alien2();
	intialize_alien2_missile();
	initialize_boss1();
	intialize_boss1_bullet();
	iSetTimer(25, change);
	iSetTimer(80, change2); 
	iSetTimer(700, change3);
	iInitialize(screen_width, screen_height, "Spooky Invaders");
	return 0;

}