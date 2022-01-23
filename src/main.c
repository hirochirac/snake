/*
 * main.c
 *
 *  Created on: 7 janv. 2022
 *      Author: b.Le Grin
 */

#include <stdlib.h>
#include <stdio.h>
#include <malloc.h>
#include <math.h>
#include <SDL2/SDL_main.h>
#include <SDL2/SDL.h>

const int HEIGHT = 800;
const int WIDTH  = 800;
const int SCL    = 20;
const int GRID   = 1600;

typedef struct Snake_t {
	int w;
	int h;
	int x;
	int y;
	int speedX;
	int speedY;
	int dirX;
	int dirY;
	struct Snake_t * next;
} Snake;


typedef struct {
	int w;
	int h;
	int x;
	int y;
} Apple;


/**
 *
 */
Apple * init_apple (int w, int h) {
	Apple * apple = (Apple * ) malloc(sizeof(Apple));
	int cols = floor(w/SCL);
	int rows = floor(h/SCL);
	if (apple != NULL) {
		apple->h = SCL;
		apple->w = SCL;
		apple->x = floor(rand() % cols) * SCL;
	    apple->y = floor(rand() % rows) * SCL;
	}
	return apple;
}


/**
 *
 */
Snake init_snake (int x, int y, int spdX, int spdY) {
	Snake s ;
	s.w = SCL;
	s.h = SCL;
	s.x = x;
	s.y = y;
	s.speedX = spdX;
	s.speedY = spdY;
	s.dirX = 0;
	s.dirY = 1;
	s.next = NULL;
	return s;
}


/**
 *
 */
void draw_snake (SDL_Renderer * renderer, Snake tail[], int hit) {
	SDL_Rect rect;

	for(int i=0; i<hit+1; i++) {
		SDL_SetRenderDrawColor(renderer,0, 255, 0, 255);
		rect.h = tail[i].h;
		rect.w = tail[i].w;
		rect.x = tail[i].x;
		rect.y = tail[i].y;
		SDL_RenderFillRect(renderer, &rect);
		SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
	}
}


/**
 *
 */
void draw_apple (SDL_Renderer * renderer, Apple * apple) {
	SDL_Rect rect;
	rect.h = apple->h;
	rect.w = apple->w;
	rect.x = apple->x;
	rect.y = apple->y;
	SDL_SetRenderDrawColor(renderer,255, 0, 0, 255);
	SDL_RenderFillRect(renderer, &rect);
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
}


/**
 *
 */
int snake_eats (Apple * apple, Snake * snake) {
	double snake_mX = (snake->x + snake->x + SCL)/2.0;
	double snake_mY = (snake->y + snake->y + SCL)/2.0;

	double apple_mX = (apple->x + apple->x + SCL)/2.0;
	double apple_mY = (apple->y + apple->y + SCL)/2.0;

	double dist = (abs((snake_mX * snake_mX) - (apple_mX * apple_mX)) + abs((snake_mY * snake_mY) - (apple_mY * apple_mY)));

	return sqrt(dist) == 0.0;
}


int snake_eats_snake (Snake * tete, Snake  * tail) {
	double dist = 0.0;
	double tete_mX = (tete->x + tete->x + SCL)/2;
    double tete_mY = (tete->y + tete->y + SCL)/2;
    double tail_mX = (tail->x + tail->x + SCL)/2;
    double tail_mY = (tail->y + tail->y + SCL)/2;
    dist = (abs((tete_mX * tete_mX) - (tail_mX * tail_mX)) + abs((tete_mY * tete_mY) - (tail_mY * tail_mY)));
	return sqrt(dist);
}



/**
 *
 */
int main (int argc, char** argv) {

	Snake tail[GRID+1];
	Snake tmp[GRID+1];
	Apple * apple           = NULL;
	SDL_Window   * window  	= NULL;
	SDL_Renderer * renderer = NULL;
	int close               = 0;
	int hit                 = 0;
	int lost                = 0;

	if (SDL_Init(SDL_INIT_EVERYTHING)) {
		SDL_Log("INIT SDL failed... %s", SDL_GetError());
		return EXIT_FAILURE;
	}

	window = SDL_CreateWindow("Snake", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WIDTH, HEIGHT, 0);
	if ( window == NULL) {
		SDL_Log("INIT SDL failed... %s", SDL_GetError());
		return EXIT_FAILURE;
	}

	renderer = SDL_CreateRenderer(window,-1,SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	if ( window == NULL) {
			SDL_Log("INIT SDL failed... %s", SDL_GetError());
			return EXIT_FAILURE;
	}

	SDL_Event event;
	time_t t;
	srand((unsigned) time(&t));
	apple =  init_apple(WIDTH,HEIGHT);
	tail[0] = init_snake(floor(((WIDTH/2)/SCL)*SCL),floor(((HEIGHT/2)/SCL)*SCL),SCL,SCL);

	while (!close && hit < GRID+1) {

		Uint64 start = SDL_GetPerformanceCounter();

		while (SDL_PollEvent(&event)) {
			switch (event.type) {
				case SDL_QUIT: close = 1; break;
				case SDL_KEYDOWN: {
					if (event.key.keysym.scancode == SDL_SCANCODE_ESCAPE){
						close = 1;
					} else if (event.key.keysym.scancode == SDL_SCANCODE_UP) {
						tail[0].dirX = 0;
						tail[0].dirY = -1;
					} else if (event.key.keysym.scancode == SDL_SCANCODE_DOWN) {
						tail[0].dirX = 0;
						tail[0].dirY = 1;
					} else if (event.key.keysym.scancode == SDL_SCANCODE_RIGHT) {
						tail[0].dirX = 1;
						tail[0].dirY = 0;
					} else if (event.key.keysym.scancode == SDL_SCANCODE_LEFT) {
						tail[0].dirX = -1;
						tail[0].dirY = 0;
					}
					break;
				}
			}
		}

		if (snake_eats(apple,&tail[0])) {
			apple = init_apple(WIDTH,HEIGHT);
			hit++;
		}

		for (int i=0; i<hit+1; i++) {
			tmp[i] = tail[i];
		}
		for (int k=1; k<hit+1; k++) {
			tail[k] = tmp[k-1];
		}

		tail[0].x += tail[0].speedX * tail[0].dirX;
		tail[0].y += tail[0].speedY * tail[0].dirY;

		if (tail[0].x <= 0) {
			tail[0].x = 0;
		}

		if (tail[0].x >= WIDTH-SCL) {
			tail[0].x =  WIDTH-SCL;
		}

		if (tail[0].y <= 0) {
			tail[0].y = 0;
		}

		if (tail[0].y >= HEIGHT-SCL) {
			tail[0].y = HEIGHT-SCL;
		}

		for (int i=2; i<hit+1; i++) {
			if (snake_eats_snake(&tail[0],&tail[i]) == 0) {
				lost = 1;
				break;
			}
		}

		SDL_RenderClear(renderer);

		if (!lost) {
			draw_snake(renderer,tail,hit);
			draw_apple(renderer, apple);
		} else {

		}

		SDL_RenderPresent(renderer);


		Uint64 end = SDL_GetPerformanceCounter();

		float elapsedMS = (end - start) / (float)SDL_GetPerformanceFrequency() * 1000.0f;

		if (close == 0) {
			SDL_Delay(20*(int)(floor(abs(16.000f-elapsedMS))));
		}

	}


	free(apple);
	SDL_DestroyWindow(window);
	SDL_DestroyRenderer(renderer);
	SDL_Quit();
	return EXIT_SUCCESS;

}
