/* mipslabmain.c

   This file written 2015 by Axel Isaksson,
   modified 2015, 2017 by F Lundevall

	 modified 2021 by Philip Salqvist

   Latest update 2017-04-21 by F Lundevall

   For copyright and licensing, see file COPYING */

#include <stdint.h>   /* Declarations of uint_32 and the like */
#include <pic32mx.h>  /* Declarations of system-specific addresses etc */
#include <string.h>
#include <stdio.h>
#include "mipslab.h"  /* Declatations for these labs */
#include "structs.h"

/* a lower number of SPEED makes the game run faster */
#define SPEED 1
#define HEIGHT 8
#define TRUE 1
#define FALSE 0
#define MIDDLE_MIN (HEIGHT/2)-1
#define MIDDLE_MAX (HEIGHT/2)+1
#define INNER_MIN (HEIGHT/2)-2
#define INNER_MAX (HEIGHT/2)+2
#define EDGE_MIN (HEIGHT/2)-3
#define EDGE_MAX (HEIGHT/2)+3

Player player1 = {0, (32-HEIGHT)/2, HEIGHT, 0};
Player player2 = {127, (32-HEIGHT)/2, HEIGHT, 0};
Ball ball = {127/2, 32/2, 2, 1};
int play = FALSE;
int end = FALSE;
int start = TRUE;

/*
Functions to manipulate ball and players
*/

void move_ball(Ball *b) {
	// move on the x-axis
	(*b).x += (*b).directionX;
	// move on the y-axis, check if bounce occur on upper or lower wall
	if((*b).y <= 0 || (*b).y >= 31) {
		// if so, change direction on y-axis
		(*b).directionY *= -1;
	}
	(*b).y += (*b).directionY;
}

void move_down(Player *p) {
	if(((*p).y + HEIGHT) < 32) {
		(*p).y += 1;
	}
}

void move_up(Player *p) {
	if(((*p).y) > 0) {
		(*p).y -= 1;
	}
}

int main(void) {
	main_init();
	display_init();

	init_registers(); /* Do any lab-specific initialization */



	while(1){}
	return 0;
}

void check_players() {
	if(getbtns() & 8) {
		move_up(&player1);
	}
	if(getbtns() & 4) {
		move_down(&player1);
	}
	if(getbtns() & 2) {
		move_up(&player2);
	}
	if(getbtns() & 1) {
		move_down(&player2);
	}
}

/*
check if the players are colliding with the ball, if so, change balls x-direction
*/
void check_collisions(Player *player1, Player *player2, Ball *b) {
	// check where it hits player 1 on racket
	if((*b).x == ((*player1).x + 1) && (*b).y >= ((*player1).y + MIDDLE_MIN) && (*b).y <= ((*player1).y + MIDDLE_MAX)) {
		(*b).directionX = 3;
	} else if((*b).x == ((*player1).x + 1) && (*b).y >= ((*player1).y + INNER_MIN) && (*b).y <= ((*player1).y + INNER_MAX)) {
		(*b).directionX = 2;
	} else if((*b).x == ((*player1).x + 1) && (*b).y >= ((*player1).y + EDGE_MIN) && (*b).y <= ((*player1).y + EDGE_MAX)) {
		(*b).directionX = 1;
	}

	if((*b).x == ((*player2).x) && (*b).y >= ((*player2).y + MIDDLE_MIN) && (*b).y <= ((*player2).y + MIDDLE_MAX)) {
		(*b).directionX = 3;
		(*b).directionX *= -1;
	} else if((*b).x == ((*player2).x) && (*b).y >= ((*player2).y + INNER_MIN) && (*b).y <= ((*player2).y + INNER_MAX)) {
		(*b).directionX = 2;
		(*b).directionX *= -1;
	} else if((*b).x == ((*player2).x) && (*b).y >= ((*player2).y + EDGE_MIN) && (*b).y <= ((*player2).y + EDGE_MAX)) {
		(*b).directionX = 2;
		(*b).directionX *= -1;
	}
}

int score1 = 0, score2 = 0, score3 = 0, score4 = 0;
int score5 = 0, score6 = 0, score7 = 0, score8 = 0;
void change_score(Player *player1, Player *player2) {
	if((*player1).points == 1 && score1 == 0) { PORTE += 128; score1 = 1; }
	if((*player1).points == 2 && score2 == 0) { PORTE += 64; score2 = 1; }
	if((*player1).points == 3 && score3 == 0) { PORTE += 32; score3 = 1; }
	if((*player1).points == 4 && score4 == 0) { PORTE += 16; score4 = 1; }

	if((*player2).points == 1 && score5 == 0) { PORTE += 1; score5 = 1; }
	if((*player2).points == 2 && score6 == 0) { PORTE += 2; score6 = 1; }
	if((*player2).points == 3 && score7 == 0) { PORTE += 4; score7 = 1; }
	if((*player2).points == 4 && score8 == 0) { PORTE += 8; score8 = 1; }
}


void reset_positions(Player *player1, Player *player2, Ball *ball) {
	(*player1).x = 0;
	(*player1).y = (32-HEIGHT)/2;
	(*player2).x = 127;
	(*player2).y = (32-HEIGHT)/2;

	(*ball).x = 127/2;
	(*ball).y = 32/2;

	if((*player1).points > (*player2).points) {
		(*ball).directionX = 2;
	} else {
		(*ball).directionX = -2;
	}
}

void reset_points(Player *player1, Player *player2) {
	(*player1).points = 0;
	(*player2).points = 0;

	score1 = 0, score2 = 0, score3 = 0, score4 = 0;
	score5 = 0, score6 = 0, score7 = 0, score8 = 0;

	PORTE = 0;
}

void check_if_score(Player *player1, Player *player2, Ball *b) {
	if((*b).x < (*player1).x) {
		(*player2).points += 1;
		reset_positions(player1, player2, b);
		change_score(player1, player2);
	}
	if((*b).x > (*player2).x) {
		(*player1).points += 1;
		reset_positions(player1, player2, b);
		change_score(player1, player2);
	}
}

int winner;
void if_win(Player *player1, Player *player2) {
	if((*player1).points == 4) {
		winner = 1;
		end = TRUE;
		play = FALSE;
	}
	if((*player2).points == 4) {
		winner = 2;
		end = TRUE;
		play = FALSE;
	}
}

void start_game() {
	if((getbtns() & 1) || (getbtns() & 2) || (getbtns() & 4) || (getbtns() & 8)) {
		reset_positions(&player1, &player2, &ball);
		reset_points(&player1, &player2);

		start = FALSE;
		end = FALSE;
		play = TRUE;
	}
}


/* Interrupt Service Routine */
int timeoutcount = 0;
void user_isr( void )
{
  if(IFS(0) & 0x100) {
    timeoutcount++;
    IFSCLR(0) = 0x100;
    if(timeoutcount == SPEED) {
			if(start == TRUE) {
				display_string(1, "Welcome to PONG!");
				display_string(2, "Press any button");
				display_string(3, "to start to play");
				start_game();
				display_update();
				timeoutcount = 0;
			}
			if(play == TRUE) {
				check_collisions(&player1, &player2, &ball);
				check_players();
				check_if_score(&player1, &player2, &ball);
				if_win(&player1, &player2);
				move_ball(&ball);
				draw_game(&player1, &player2, &ball);
	      timeoutcount = 0;
			}
			if(end == TRUE) {
				reset_display();
				if(winner == 1) {
					display_string(1, "Congrats Player1");
				} else {
					display_string(1, "Congrats Player2");
				}

				display_string(2, "Press button to");
				display_string(3, "restart the game ");
				display_update();
				start_game();
				timeoutcount = 0;
			}
    }
  }
}
