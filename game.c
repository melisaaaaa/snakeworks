/*Written by Fredrik Lundström and Michell Dib 2021 (c)*/

#include <stdint.h>
#include <pic32mx.h>
#include <stdlib.h> /* Includes e.g. rand-function and srand-function */
#include "snake.h" //Project header file

int btns;
int speed = 10000; //Higher is slower
int randoty;
int randotx;
/*
//We made premade apples since the time.h library would not work on the Uno32
int dot_x[] = {
    56,
    39,
    28,
    95,
    46,
    107,
    13,
    69,
    65,
    11,
    28,
    82
};

int dot_y[] = {
    10,
    29,
    20,
    9,
    15,
    3,
    21,
    27,
    11,
    17,
    8,
    12
    };
*/
void collision(){
    char score2[] = "X";
    score2[0] = score + '0'; //Here we make an integer to a string
    

    while(btns != 0b1000){ //While BTN4 is not pressed we will display the score and a game over string.
        clear_display();
        display_string(0, "Your snake died! Score:");
        display_string(1,score2);
	display_string(2, "Press for high");
	display_string(3,"score list!");
	
        display_update();
        translateToImage();
	    display_image(0, oled_display);
        btns = getbtns();
 
    }
    main(); //If BTN4 is pressed we jump back to main and the game will then restart.
}



void create_snake(){ //Here we will print out the snake
    int i,j,k,x,y;
    for(k = 0; k < snake.size; k++){
        i=1;
	j=1;
        x = snake.x[k] + i;
        y = snake.y[k] + j;
        display[y][x] = 1;
            }
        }

void snake_move(){ //MOVEMENT FOR SNAKE

    btns = getbtns(); //Get buttons

    int i;
    for(i = snake.size - 1; i > 0; i--){ //This for loop will change the coordinates of every part of the snake so that the body will follow the head
        snake.x[i] = snake.x[i - 1];
        snake.y[i] = snake.y[i - 1];
    }

    if(snake.dir == 'R'){ //RIGHT
        snake.x[0] += 1;
    }
    if(snake.dir == 'D'){ //DOWN
        snake.y[0] += 1;
    }
    if(snake.dir == 'U'){ //UP
        snake.y[0] -= 1;
    }
    if(snake.dir == 'L'){ //LEFT
        snake.x[0] -= 1;
    }

    
    //  SO THE UNO32 REMEMBER THE DIRECTION EVEN AFTER THE BUTTON IS RELEASED
    if(snake.dir != 'L'){
        if(btns == 0b0001){ //RIGHT
            snake.dir = 'R';
        }
    }
    if(snake.dir != 'U'){
        if(btns == 0b0010){ //DOWN
            snake.dir = 'D';
        }
    }
    if(snake.dir != 'D'){
        if(btns == 0b0100){ //UP
            snake.dir = 'U';
        }
    }
    if(snake.dir != 'R'){
        if(btns == 0b1000){ //LEFT
            snake.dir = 'L';
        }
    }
}
void dot(){
  int minx=2;
  int maxx=127;
  int miny=2;
  int maxy=31;
  randotx=minx + rand()%(maxx+1-minx);
 randoty=miny+rand()%(maxy+1-miny);
  display[randoty][randotx]=1;
  }
/*void dot(){
    //RANDOMIZE POSITION does not work :(
    display[dot_y[score]][dot_x[score]] = 1; //Display apple
    }*/

void eat_dots(){
    snake.size++; //Increase length
    score++; //Increase score which will also give the apple new coordinates
}
int getsw( void ){

	int SW = (PORTD >> 8) & 0x000F;

	return SW;
}


int getbtns(void){ //Get btns
    int btn = (PORTD >> 5) & 0x0007;
    btn = (btn << 1);
                                    //Since the buttons was set to bits 11 through 5 we will now
                                    //shift the bits 5 bits the right and check them with and operation
                                   //and then see if buttons 4,3 or 2 are active.
                                   // & them with seven since it is 0b0111 and button 1 will then always be 0
    btn = ((PORTF >> 1) & 0x1) | btn;  //For some reason the first button is in the portf register
    return btn; //Return the active buttons as integer (active bits will be 1)
}
void game(){
    snake_move(); //First we will call for the snake movements
    if(snake.y[0] == randoty && snake.x[0] == randotx){ //If we collide with apple
        eat_dots(); //Call eat_apple function
    }
    if((snake.y[0] == 32 || snake.y[0] == 0) || (snake.x[0] == 128 || snake.x[0] == 0)){ //If we collide with wall
        collision(); //Call collision function
    }
    create_snake(); //After movements are made we create the snake

    int k;
    for(k = snake.size - 1; k > 0; k--){ // This for loop check if the head will collide with any coordinate of the body
        if(snake.x[0] == snake.x[k] && snake.y[0] == snake.y[k])
            collision();
    }

    dot(); //Every frame we create an apple based on the score that changes the apples coordinates to any of the premade values.
    delay(speed);
}


