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

typedef struct {
	int w;
	int h;
	int x;
	int y;
	int speedX;
	int speedY;
	int dirX;
	int dirY;
} Snake;


typedef struct {
	int w;
	int h;
	int x;
	int y;
} Apple;


Apple * init_apple (int scl, int w, int h) {
	Apple * apple = malloc(sizeof(Apple));
	int cols = floor(w/scl);
	int rows = floor(h/scl);
	if (apple != NULL) {
		apple->h = scl;
		apple->w = scl;
		apple->x = floor(rand() % cols) * scl;
	    apple->y = floor(rand() % rows) * scl;
	}
	return apple;
}


/**
 *
 */
Snake * init_snake (int scl, int x, int y, int spdX, int spdY) {
	Snake * s = malloc(sizeof(Snake));
	if (s != NULL) {
		s->w = scl;
		s->h = scl;
		s->x = x;
		s->y = y;
		s->speedX = spdX;
		s->speedY = spdY;
		s->dirX = 0;
		s->dirY = 1;
		return s;
	} else {
		return NULL;
	}
}

/**
 *
 */

void draw_snake (SDL_Renderer * renderer, Snake * snake) {
	SDL_Rect rect;
	rect.h = snake->h;
	rect.w = snake->w;
	rect.x = snake->x;
	rect.y = snake->y;
	SDL_SetRenderDrawColor(renderer,0, 255, 0, 255);
	SDL_RenderFillRect(renderer, &rect);
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
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
int snake_eat (Apple * apple, Snake * snake) {

}

/**
 *
 */
int main (int argc, char** argv) {

	const int HEIGHT = 800;
	const int WIDTH  = 800;
	const int SCL    = 20;

	Apple * apple           = NULL;
	Snake * head            = NULL;
	SDL_Window   * window  	= NULL;
	SDL_Renderer * renderer = NULL;
	int close               = 0;

    head = init_snake(SCL, WIDTH/2, HEIGHT/2, 1, 1);
    if ( head == NULL) {
    	SDL_Log("INIT SNAKE HEAD failed");
    	return EXIT_FAILURE;
    }

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
	apple =  init_apple(SCL,WIDTH,HEIGHT);
	while (!close) {

		Uint64 start = SDL_GetPerformanceCounter();

		while (SDL_PollEvent(&event)) {
			switch (event.type) {
				case SDL_QUIT: close = 1; break;
				case SDL_KEYDOWN: {
					if (event.key.keysym.scancode == SDL_SCANCODE_ESCAPE){
						close = 1;
					} else if (event.key.keysym.scancode == SDL_SCANCODE_UP) {
						head->dirX = 0;
						head->dirY = -1;
					} else if (event.key.keysym.scancode == SDL_SCANCODE_DOWN) {
						head->dirX = 0;
						head->dirY = 1;
					} else if (event.key.keysym.scancode == SDL_SCANCODE_RIGHT) {
						head->dirX = 1;
					    head->dirY = 0;
					} else if (event.key.keysym.scancode == SDL_SCANCODE_LEFT) {
						head->dirX = -1;
						head->dirY = 0;
					}
					break;
				}
			}
		}

		head->x += head->speedX * head->dirX;
        head->y += head->speedY * head->dirY;

        if (head->x < 0)
        	head->x = 0;

        if (head->x > WIDTH-SCL)
        	head->x = WIDTH-SCL;

        if (head->y < 0)
        	head->y = 0;

        if (head->y > HEIGHT-SCL)
            head->y = WIDTH-SCL;

        printf("%d\t%d\n",head->x, head->y);

		SDL_RenderClear(renderer);

		draw_snake(renderer,head);
		draw_apple(renderer, apple);

		SDL_RenderPresent(renderer);


		Uint64 end = SDL_GetPerformanceCounter();

		float elapsedMS = (end - start) / (float)SDL_GetPerformanceFrequency() * 1000.0f;
		//SDL_Delay(floor(16.000f-elapsedMS));

	}

	free(apple);
	SDL_DestroyWindow(window);
	SDL_DestroyRenderer(renderer);
	SDL_Quit();
	return EXIT_SUCCESS;

}
