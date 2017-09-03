#include "stdafx.h"
#include <math.h>
#include <Eigen/Dense>
#include <iostream>
#include <string>
#include <SDL.h>
using namespace std;
using namespace Eigen;

//constant declarations
const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 600;
const int MAP_WIDTH = 24;
const int MAP_HEIGHT = 24;

const int map[MAP_WIDTH][MAP_HEIGHT] = {
	{1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
	{1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
	{1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
	{1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
	{1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
	{1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
	{1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
	{1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
	{1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
	{1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
	{1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 2, 0, 0, 0, 0, 0, 0, 0, 0, 1},
	{1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 2, 0, 0, 0, 0, 0, 0, 0, 0, 1},
	{1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
	{1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
	{1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
	{1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
	{1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
	{1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
	{1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
	{1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
	{1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
	{1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
	{1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
	{1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1}
};
//sdl variables
SDL_Window* window = NULL;
SDL_Renderer* renderer = NULL;
SDL_Texture* texture = NULL;

Uint32* buffer = new Uint32[SCREEN_WIDTH*SCREEN_HEIGHT];//reserves memory for the pixel colours, with each pixel on the screen requiring 32 bits of information.

//structures

struct player {
	Vector2d position;
	Vector2d direction;
	Vector2d cameraPlane;
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

Vector2d intersection(Vector2d v1, Vector2d v2, Vector2d v3, Vector2d v4) {
	//v1 is the mob position, v2 is a point on the viewline facing the wall, v3 and v4 are wall verticies
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

void showMap(player player1) {
	
	SDL_Rect playerRect = { player1.position(0) - 5, player1.position(1) - 5, 10, 10 };
	//SDL_Rect interceptionRect = { (int)interceptVector(0) - 5 , (int)interceptVector(1) - 5 , 10, 10};

	//drawing the walls

	SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0, SDL_ALPHA_OPAQUE);
	SDL_RenderDrawRect(renderer, &playerRect);//drawing the player rectangle

	//SDL_SetRenderDrawColor(renderer, 0xFF, 0, 0, SDL_ALPHA_OPAQUE);
	//SDL_RenderDrawRect(renderer, &interceptionRect);//drawing the interception rectangle
	
	drawLine(0xFF, 0xFF, 0xFF,//drawing the direction vector
							(player1.position),
							(player1.position + player1.direction*50));

	drawLine(0xFF, 0, 0xFF,//drawing camera vector, not to scale
		(player1.position + player1.direction * 50),
		((player1.position + player1.direction * 50 + player1.cameraPlane * 50)));

	drawLine(0xFF, 0, 0xFF,//drawing camera vector, not to scale
		(player1.position + player1.direction * 50),
		((player1.position + player1.direction * 50 - player1.cameraPlane * 50)));

	drawLine(0xFF, 0xAA, 0,//drawing view angle, not to scale
		player1.position,
		((player1.position + player1.direction * 200 - player1.cameraPlane * 200)));

	drawLine(0xFF, 0xAA, 0,//drawing view angle, not to scale
		player1.position,
		((player1.position + player1.direction * 200 + player1.cameraPlane * 200)));
		


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
	player1.position = { 22, 12 };
	player1.direction = { -1 , 0 };
	player1.cameraPlane = {0, 0.66};

	bool quitLoop = false;
	bool mapVisible = false;
	SDL_Event event;

	//main loop
	while (!quitLoop) {

		//raycasting loop
		for (int i = 0; i < SCREEN_WIDTH; i++) {
			double cameraX = 2 * i / (double)SCREEN_WIDTH - 1;//this creates the camera X plane, which goes from -1 to 1, with 0 in the middle
			Vector2d rayPosition = player1.position;//the ray starts at the player coordinates
			Vector2d rayDirection = player1.direction + player1.cameraPlane*cameraX;//calculate the direction of the ray, as a sum of the direction and plane vectors
		
			Vector2i mapPosition = {(int)rayPosition(0), (int)rayPosition(1)};//what box of the map we're in
			//the distance to the next tile, being either the horizontal wall or the vertical
			double sideDistanceX;
			double sideDistanceY;
			double wallDist;//the distance to the wall in question;

			//the distance from one tile wall to the next, being either horizontal or vertical
			//MAGIC NUMBER ALERT
			double deltaDistX = sqrt(1 + pow(rayDirection(1), 2) / pow(rayDirection(0), 2));
			double deltaDistY = sqrt(1 + pow(rayDirection(0), 2) / pow(rayDirection(1), 2));

			int stepX;//whether to go in a negative or positive direction
			int stepY;

			int hit = 0;
			int side;

			if (rayDirection(0) < 0) {//what quadrant the x value of the ray direction is in
				stepX = -1;
				sideDistanceX = (rayPosition(0) - mapPosition(0))*deltaDistX;//the oblique distance to the next xside, more mystery math
			}
			else {
				stepX = 1;
				sideDistanceX = (mapPosition(0) + 1 - rayPosition(0))*deltaDistX;//the +1 is to ensure that the distance is never negative
			}
			if (rayDirection(1) < 0) {//what quadrant the y value of the ray direction is in
				stepY = -1;
				sideDistanceY = (rayPosition(1) - mapPosition(1))*deltaDistY;
			}
			else {
				stepY = 1;
				sideDistanceY = (mapPosition(1) + 1 - rayPosition(1))*deltaDistY;
			}

			int iteration = 0;
			//DDA casting start: travels along until it hits a wall
			while (hit == 0 && iteration < 100) {
				if (sideDistanceX < sideDistanceY) {
					sideDistanceX += deltaDistX;
					mapPosition(0) += stepX;
					side = 0;
				}
				else {
					sideDistanceY += deltaDistY;
					mapPosition(1) += stepY;
					side = 1;
				}
				if (map[mapPosition(0)][mapPosition(1)] > 0) {
					hit = 1;
				}
				else {
					iteration++;
				}
			}

			//calculate distance to intersection
			if (side == 0) {//x intercept distance
				wallDist = (mapPosition(0) - rayPosition(0) + (1 - stepX) / 2) / rayDirection(0);
			}
			else {//y intercept distance
				wallDist = (mapPosition(1) - rayPosition(1) + (1 - stepY) / 2) / rayDirection(1);
			}

			int lineHeight = (int)((SCREEN_HEIGHT*2)/wallDist);

			Uint8 green = (side == 1) ? 0xFF / 2 : 0xFF;
			int drawStart = -lineHeight / 2 + SCREEN_HEIGHT / 2;
			if (drawStart < 0) {
				drawStart = 0;
			}
			int drawEnd = lineHeight / 2 + SCREEN_HEIGHT / 2;
			if (drawEnd >= SCREEN_HEIGHT) {
				drawEnd = SCREEN_HEIGHT - 1;
			}

			drawLine(0, green, 0,
								{i, drawStart},
								{i, drawEnd});

		}

		//render screen objects
		SDL_RenderPresent(renderer);
		SDL_RenderClear(renderer);
		SDL_RenderCopy(renderer, texture, NULL, NULL);

		//controls
		const Uint8 *state = SDL_GetKeyboardState(NULL);
		if (state[SDL_SCANCODE_W]) {
			player1.position += player1.direction;
		}
		if (state[SDL_SCANCODE_S]) {
			player1.position -= player1.direction;
		}

		if (state[SDL_SCANCODE_A]) {
			player1.direction = rotate(player1.direction, M_PI / -48.0);
			player1.cameraPlane = rotate(player1.cameraPlane, M_PI / -48.0);
		}
		if (state[SDL_SCANCODE_D]) {
			player1.direction = rotate(player1.direction, M_PI / 48.0);
			player1.cameraPlane = rotate(player1.cameraPlane, M_PI / 48.0);
		}
		
		cout << player1.position << endl;

		while (SDL_PollEvent(&event)) {	//checks to see if the event queue has any more events, stops if it doesn't
										//also, it changes the event variable to be the next event in the queue

			if (event.type == SDL_QUIT) {
				quitLoop = true;
			}
			/*
			if (event.type == SDL_KEYDOWN) {
				switch (event.key.keysym.sym) {
					case SDLK_TAB: 
						mapVisible = !mapVisible;
						break;
				}
			}*/
		}
	}

	quit();

	delete[] buffer;

	return 0;
}