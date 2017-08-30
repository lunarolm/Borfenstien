// sdltest.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <math.h>
#include <Eigen/Dense>
//#include <Eigen/Geometry>
#include <iostream>
#include <string>
#include <SDL.h>
using namespace std;
using namespace Eigen;

//constant declarations
const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 600;

//errorcode variable
int errorCode = 0;

//sdl variables
SDL_Window* window = NULL;
SDL_Renderer* renderer = NULL;
SDL_Texture* texture = NULL;

Uint32* buffer = new Uint32[SCREEN_WIDTH*SCREEN_HEIGHT];//reserves memory for the pixel colours, with each pixel on the screen requiring 32 bits of information.

//structures

struct player {
	Vector2d position;
	Vector2d direction;
}player1;

//functions

//mathy functions
Vector2d rotate(Vector2d inputVector, double angle) {
	MatrixXd stdMatrix(2, 2);
	stdMatrix(0, 0) = cos(angle);
	stdMatrix(0, 1) = sin(angle);
	stdMatrix(1, 0) = -sin(angle);
	stdMatrix(1, 1) = cos(angle);

	return stdMatrix * inputVector;
}

Vector2d intersection(Vector2d v1, Vector2d v2, Vector2d v3, Vector2d v4) {//v1 is the mob position, v2 is a point on the viewline facing the wall, v3 and v4 are wall verticies
	//pass in two points on the player line, and two points on the line you want to intersect, and it will output the coordinates of the intersect point.
	//If that point is equal to the hitbox of the player which is to say, the centre coordinate of the player in this case, stop movement along that vector

	Matrix2d m[8];
	//x intercept value calculation
	m[0] << v1(0), v1(1),
			v2(0), v2(1);
	m[1] << v1(0), 1,
			v2(0), 1;
	m[2] << v3(0), v3(1),
			v4(0), v4(1);
	m[3] << v3(0), 1,
			v4(0), 1;
	m[4] << v1(0), 1,
			v2(0), 1;
	m[5] << v1(1), 1,
			v2(1), 1;
	m[6] << v3(0), 1,
			v4(0), 1;
	m[7] << v3(1), 1,
			v4(1), 1;

	Matrix2d M1;
	M1 << m[0].determinant(), m[1].determinant(),
		  m[2].determinant(), m[3].determinant();

	Matrix2d M2;
	M2 << m[4].determinant(), m[5].determinant(),
		  m[6].determinant(), m[7].determinant();

	double x = M1.determinant() / M2.determinant();//x coordinate value return


	//y intercept value calculation
	m[1] << v1(1), 1,
			v2(1), 1;
	m[3] << v3(1), 1,
			v4(1), 1;

	M1 << m[0].determinant(), m[1].determinant(),
		  m[2].determinant(), m[3].determinant();

	M2 << m[4].determinant(), m[5].determinant(),
		  m[6].determinant(), m[7].determinant();

	double y = M1.determinant() / M2.determinant();//y coordinate value return
	return { x, y };

}

//sdl functions
bool init() {
	bool success = true;

	//initialize SDL
	if (SDL_Init(SDL_INIT_VIDEO) < 0) {
		printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
		return false;
	}
	else {
		//window init
		window = SDL_CreateWindow("Borfenstien", 50, 50, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);

		if (window == NULL) {
			cout << "WINDOW FAILED TO INITIALIZE" << SDL_GetError() << endl;

			success = false;
		}
	}

	return success;
}

bool initTexture() {
	bool success = true;

	//initialize the renderer
	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_PRESENTVSYNC);

	if (renderer == NULL) {
		cout << "RENDERER FAILED TO INITIALIZE: " << SDL_GetError() << endl;
		success = false;
	}

	//initalise the texture
	texture = SDL_CreateTexture(
		renderer,
		SDL_PIXELFORMAT_RGBA8888,//Represents each pixel as 4 bytes, with one byte for each colour and one for alpha. That is to say, each colour can have 255 shades
		SDL_TEXTUREACCESS_STATIC,//writes numeric values directly to the texture every iteration
		SCREEN_WIDTH,
		SCREEN_WIDTH
	);

	if (texture == NULL) {
		cout << "TEXTURE FAILED TO INITIALIZE" << SDL_GetError() << endl;
		success = false;
	}

	return success;

}

void quit() {
	if (renderer != NULL) {
		SDL_DestroyRenderer(renderer);
		printf("Destroyed Renderer\n");
	}
	if (texture != NULL) {
		SDL_DestroyTexture(texture);
		printf("Destroyed Texture\n");
	}
	if (window != NULL) {
		SDL_DestroyWindow(window);
		printf("Destroyed Window\n");
	}
	SDL_Quit();
}

//updates the screen
void update(Uint32* buffer) {
	SDL_UpdateTexture(texture, NULL, buffer, SCREEN_HEIGHT * sizeof(Uint32));
	SDL_RenderClear(renderer);
	SDL_RenderCopy(renderer, texture, NULL, NULL);
	SDL_RenderPresent(renderer);
}

void setPixel(int x, int y, Uint8 red, Uint8 green, Uint8 blue) {
	Uint32 color = 0;

	color += red;
	color <<= 8;
	color += green;
	color <<= 8;
	color += blue;
	color <<= 8;
	color += 0xFF;

	buffer[(y * SCREEN_WIDTH) + x] = color;
}

void drawLine(Uint8 red, Uint8 green, Uint8 blue, Vector2d start, Vector2d end) {
	SDL_SetRenderDrawColor(renderer, red, green, blue, SDL_ALPHA_OPAQUE);
	SDL_RenderDrawLine(renderer, start(0), start(1), end(0), end(1));
}

/******************************************************************************MAIN*********************************************************************************/
/*******************************************************************************************************************************************************************/
int main(int argc, char* argv[]) {

	//initialising window and SDL
	if (!init()) {
		return 1;
	}

	//initialize renderer and texture
	if (!initTexture()) {
		quit();
		return 2;
	}
	//PLAYER INITIALISATION
	player1.position = { 400, 300 };
	player1.direction = {0, 1};

	//Uint32* buffer = new Uint32[SCREEN_WIDTH*SCREEN_HEIGHT];//reserves memory for the pixel colours, with each pixel on the screen requiring 32 bits of information.
	//memset(buffer, 0x00, SCREEN_WIDTH*SCREEN_HEIGHT * 4);

	bool quitLoop = false;
	SDL_Event event;

	//main loop
	while (!quitLoop) {

		/*
		int elapsed = SDL_GetTicks();
		double red = (1 + sin(elapsed * 0.001)) * 128;
		double green = (1 + sin(elapsed * 0.002)) * 128;
		double blue = (1 + sin(elapsed * 0.003)) * 128;

		for (int y = 0; y < SCREEN_HEIGHT; y++) {
			for (int x = 0; x < SCREEN_WIDTH; x++) {
				setPixel(x, y, (Uint8)floor(red), (Uint8)floor(green), (Uint8)floor(blue));
			}
		}*/

		//updates the screen
		//update(buffer);

		const Uint8 *state = SDL_GetKeyboardState(NULL);
		if (state[SDL_SCANCODE_W]) {
			//printf("<W> is pressed.\n");
			player1.position += player1.direction;
		}
		if (state[SDL_SCANCODE_S]) {
			//printf("<S> is pressed.\n");
			player1.position -= player1.direction;
		}
		
		if (state[SDL_SCANCODE_A]) {
			//printf("<A> is pressed.\n");
			player1.direction = rotate(player1.direction, M_PI/24.0);
		}
		if (state[SDL_SCANCODE_D]) {
			//printf("<D> is pressed.\n");
			player1.direction = rotate(player1.direction, M_PI / -24.0);
		}

		//drawing direction vector
		drawLine(0xFF, 0xFF, 0xFF, { player1.position(0), player1.position(1) }, { player1.position(0) + player1.direction(0) * 50, player1.position(1) + player1.direction(1) *50});

		drawLine(0xFF, 0, 0, { 400, 300 }, { 800, 600 });
		drawLine(0xFF, 0, 0xFF, { 400, 300 }, { 800, 0 });
		drawLine(0, 0, 0xFF, { 400, 300 }, { 0, 600 });
		drawLine(0xFF, 0xFF, 0, { 400, 300 }, { 0, 0 });

		drawLine(0xFF, 0xFF, 0, { 700, 200 }, { 700, 400 });

		SDL_Rect playerRect;
		playerRect.x = player1.position(0) - 5;
		playerRect.y = player1.position(1) - 5;
		playerRect.w = 10;
		playerRect.h = 10;

		SDL_RenderDrawRect(renderer, &playerRect);

		Vector2d interceptVector = intersection(player1.position, player1.position + player1.direction  *50, { 700, 200 }, {700, 400});

		SDL_Rect interceptionRect;
		interceptionRect.x = (int)interceptVector(0) - 5;
		interceptionRect.y = (int)interceptVector(1) - 5;
		interceptionRect.h = 10;
		interceptionRect.w = 10;

		SDL_SetRenderDrawColor(renderer, 0xFF, 0, 0, SDL_ALPHA_OPAQUE);
		SDL_RenderDrawRect(renderer, &interceptionRect);


		//render screen objects
		SDL_RenderPresent(renderer);
		SDL_RenderClear(renderer);
		SDL_RenderCopy(renderer, texture, NULL, NULL);

		while (SDL_PollEvent(&event)) {	//checks to see if the event queue has any more events, stops if it doesn't
										//also, it changes the event variable to be the next event in the queue

			if (event.type == SDL_QUIT) {
				quitLoop = true;
			}
		}
	}

	quit();

	delete [] buffer;

	return 0;
}
