#include "stdafx.h"
#include <glew.h>
#include <stdio.h>
#include <stdlib.h>
#include <SDL.h>

#define true 1
#define false 0
#define length 100

typedef int bool;

// Define a single frame used in an animation
typedef struct AnimFrameDef{
	// combined with the AnimDef's name to make
	// the actual texture name
	int frameNum;
	float frameTime;
} AnimFrameDef;

// Defines a single animation used by a character
typedef struct AnimDef{
	const char* name;
	AnimFrameDef frames[20];
	int numFrames;
} AnimDef;

//Runtime state for an animation
typedef struct AnimData{
	AnimDef* def;
	int curFrame;
	float timeToNextFrame;
	bool isPlaying;
} AnimData;

typedef struct Player{
	AnimData anim;
	float posX;
	float posY;
	bool draw;
} Player;

typedef struct Enemy{
	AnimData anim;
	float posX;
	float posY;
	bool draw;
	bool walkLeft;
	bool coll;
} Enemy;

typedef struct Camera{
	float posX;
	float posY;
} Camera;

typedef struct AABB {
	int x, y, h, w;
} AABB;

typedef struct Tile {
	int image;
	bool coll;
	int value;
	AABB box;
} Tile;

//function declarations
void animTick(AnimData* data, float dt);
void animSet(AnimData* anim, AnimDef* toPlay);
void animReset(AnimData* anim);
void animDraw(AnimData* anim, int x, int y, int w, int h);
void cameraUpdate(Camera cam, float deltaTime);
void frankWalk();
void medusaWalk();
void dusa1_trayectory();
void dusa2_trayectory();
void scythe1_trayectory();
void scythe3_trayectory();
void scythe2_trayectory();
int generate_number(int min, int max);

/* Art to draw */
GLuint bgTex;
GLuint candleTex;
GLuint groundTex;
GLuint simon_left_Tex_1;
GLuint simon_left_Tex_2;
GLuint simon_left_Tex_3;
GLuint simon_right_Tex_1;
GLuint simon_right_Tex_2;
GLuint simon_right_Tex_3;
GLuint bat1Tex;
GLuint bat2Tex;
GLuint medusa1Tex;
GLuint medusa2Tex;
GLuint sk1RightTex;
GLuint sk2RightTex;
GLuint sk1LeftTex;
GLuint sk2LeftTex;
GLuint pillarTex;
GLuint crouch_leftTex;
GLuint crouch_rightTex;
GLuint bone1_Tex;
GLuint bone2_Tex;
GLuint bone3_Tex;
GLuint bone4_Tex;
GLuint bone5_Tex;
GLuint bone6_Tex;
GLuint bone7_Tex;
GLuint bone8_Tex;
GLuint smallBat1_Tex;
GLuint smallBat2_Tex;
GLuint smallBat3_Tex;
GLuint smallBat4_Tex;
GLuint hub_Tex;
GLuint logo_Tex;
GLuint enter_Tex;
GLuint whipLeft1_Tex;
GLuint whipLeft2_Tex;
GLuint whipLeft3_Tex;
GLuint whipRight1_Tex;
GLuint whipRight2_Tex;
GLuint whipRight3_Tex;
GLuint frank1_Tex;
GLuint frank2_Tex;
GLuint frank3_Tex;
GLuint frame_Tex;
GLuint castle_Tex;
GLuint lifeEmpty_Tex;
GLuint lifeFull_Tex;
GLuint playerText_Tex;
GLuint gameOver_Tex;
GLuint heart_Tex;
GLuint lvl1_Tex;
GLuint lvl2_Tex;
GLuint lvl3_Tex;
GLuint ending_Tex;

GLuint dusa_Tex;
GLuint deathLeft_Tex;
GLuint deathRight_Tex;
GLuint scythe1_Tex;
GLuint scythe2_Tex;

GLuint textures[80];

Uint32 lastFrameMs;
Uint32 currentFrameMs;

Tile level[30][length];
int size = 30, i = 0, j = 0, currentTile, x = 0, y = 329, simonSize = 60;
int playerX = 290, playerY = 300, batX = 100, batY = 400, medusaX = 700, medusaY = 300, medusaRange = 0, boneTime = 50, boneRange = 30, dusaSize = 40;
int skX = 800, skY = 635, frankRange = 0, pillarX = 229, pillarY = 0, pillarW = 60, pillarH = 750, frameTime = 300, simonFrame = 150, life = 5, cont = 258;
int boss = 50, frankSize = 150, frankX = 600, frankY = 270, limit, dusa1Range = 0, dusa2Range = 0;
int dusa1X = 700, dusa1Y = 400, dusa1RangeY, dusa2X = 700, dusa2Y = 400, dusa2RangeY, deathX = 650, deathY = 300, deathSize = 120, scythe1X = 700, scythe1Y = 325, scythe2X = 700, scythe2Y = 375, scythe3X = 700, scythe3Y = 325;
int scythe1Range = 0, scythe2Range = 0, scythe3Range = 0, scytheVelocity = 6;
float xcam = 4, ycam = 4, deltaTime, prevX = 0, prevY = 0;
bool title = true, gameOver = false, jump = false, descend = false, hub = false, level1 = false, level2 = false, level3 = false, dusa1Up, dusa2Up;
bool scythe1Left = true, scythe2Left = true, scythe3Left = true, ending = false;
double yVelocity = 8.0, height = 0.0;

char shouldExit = 0;

/* The previous frame's keyboard state */
unsigned char kbPrevState[SDL_NUM_SCANCODES] = { 0 };
/* The current frame's keyboard state */
const unsigned char* kbState = NULL;

Player player;
Player whip;
Enemy bat;
Enemy medusa;
Enemy skeleton;
Enemy bone;
Enemy smallBat;
Enemy frank;
Enemy dusa1;
Enemy dusa2;
Enemy dusa3;
Enemy dusa4;
Enemy death;
Enemy scythe1;
Enemy scythe2;
Enemy scythe3;
Camera camera;
AABB playerBox;


AnimFrameDef left1;
AnimFrameDef left2;
AnimFrameDef left3;
AnimFrameDef left4;
AnimFrameDef right1;
AnimFrameDef right2;
AnimFrameDef right3;
AnimFrameDef right4;
AnimFrameDef whipLeft1;
AnimFrameDef whipLeft2;
AnimFrameDef whipLeft3;
AnimFrameDef whipRight1;
AnimFrameDef whipRight2;
AnimFrameDef whipRight3;

AnimDef walkingLeft;
AnimDef walkingRight;
AnimDef idleLeft;
AnimDef idleRight;
AnimDef crouchLeft;
AnimDef crouchRight;
AnimDef whipLeft;
AnimDef whipRight;

// Bat
AnimFrameDef batOpen;
AnimFrameDef batClose;

AnimDef batIdle;

AABB batBox;

// Medusa
AnimFrameDef medusaBlue;
AnimFrameDef medusaWhite;

AnimDef medusaIdle;

AABB medusaBox;

// Skeleton
AnimFrameDef sk1Right;
AnimFrameDef sk2Right;
AnimFrameDef sk1Left;
AnimFrameDef sk2Left;

AnimDef skRight;
AnimDef skLeft;

AABB skBox;

//bone
AnimFrameDef bone1;
AnimFrameDef bone2;
AnimFrameDef bone3;
AnimFrameDef bone4;
AnimFrameDef bone5;
AnimFrameDef bone6;
AnimFrameDef bone7;
AnimFrameDef bone8;

AnimDef boneDef;

AABB boneBox;

// small bat
AnimFrameDef smallBat1;
AnimFrameDef smallBat2;
AnimFrameDef smallBat3;
AnimFrameDef smallBat4;

AnimDef smallBatDef;
AnimDef smallBatIdle;

AABB smallBatBox;

//frankenstein
AnimFrameDef frank1;
AnimFrameDef frank2;
AnimFrameDef frank3;

AnimDef frankDef;

AABB frankBox;


//small medusa
AABB dusaBox1;
AABB dusaBox2;
AABB dusaBox3;
AABB dusaBox4;

//scythe
AnimFrameDef scythe_1;
AnimFrameDef scythe_2;

AnimDef scythe;

AABB scythe1Box;
AABB scythe2Box;
AABB scythe3Box;

//ground box
AABB groundBox;
AABB pillarBox1;
AABB pillarBox2;

AABB whipBox;

AABB deathBox;

int _tmain(int argc, _TCHAR* argv[])
{

	unsigned char* keys = SDL_GetKeyboardState(NULL);

	whip.draw = false;

	

	//Put data in structs
	left1.frameNum = 1;
	left2.frameNum = 2;
	left3.frameNum = 3;
	left4.frameNum = 11;
	left1.frameTime = simonFrame;
	left2.frameTime = simonFrame;
	left3.frameTime = simonFrame;
	left4.frameTime = simonFrame;

	walkingLeft.numFrames = 4;
	walkingLeft.frames[0] = left1;
	walkingLeft.frames[1] = left2;
	walkingLeft.frames[2] = left3;
	walkingLeft.frames[3] = left2;

	right1.frameNum = 0;
	right2.frameNum = 5;
	right3.frameNum = 6;
	right4.frameNum = 12;
	right1.frameTime = simonFrame;
	right2.frameTime = simonFrame;
	right3.frameTime = simonFrame;
	right4.frameTime = simonFrame;

	walkingRight.numFrames = 4;
	walkingRight.frames[0] = right1;
	walkingRight.frames[1] = right2;
	walkingRight.frames[2] = right3;
	walkingRight.frames[3] = right2;

	whipLeft1.frameNum = 29;
	whipLeft2.frameNum = 30;
	whipLeft3.frameNum = 31;
	whipLeft1.frameTime = simonFrame - 50;
	whipLeft2.frameTime = simonFrame - 50;
	whipLeft3.frameTime = simonFrame - 50;

	whipLeft.numFrames = 3;
	whipLeft.frames[0] = whipLeft1;
	whipLeft.frames[1] = whipLeft2;
	whipLeft.frames[2] = whipLeft3;

	whipRight1.frameNum = 32;
	whipRight2.frameNum = 33;
	whipRight3.frameNum = 34;
	whipRight1.frameTime = simonFrame - 50;
	whipRight2.frameTime = simonFrame - 50;
	whipRight3.frameTime = simonFrame - 50;

	whipRight.numFrames = 3;
	whipRight.frames[0] = whipRight1;
	whipRight.frames[1] = whipRight2;
	whipRight.frames[2] = whipRight3;

	idleLeft.numFrames = 1;
	idleLeft.frames[0] = left1;

	idleRight.numFrames = 1;
	idleRight.frames[0] = right1;

	crouchLeft.numFrames = 1;
	crouchLeft.frames[0] = left4;


	crouchRight.numFrames = 1;
	crouchRight.frames[0] = right4;

	batOpen.frameNum = 7;
	batClose.frameNum = 8;
	batOpen.frameTime = frameTime;
	batClose.frameTime = frameTime;

	batIdle.numFrames = 2;
	batIdle.frames[0] = batOpen;
	batIdle.frames[1] = batClose;

	medusaBlue.frameNum = 9;
	medusaWhite.frameNum = 10;
	medusaBlue.frameTime = frameTime;
	medusaWhite.frameTime = frameTime;

	medusaIdle.numFrames = 2;
	medusaIdle.frames[0] = medusaBlue;
	medusaIdle.frames[1] = medusaWhite;

	medusaBox.h = size * 4;
	medusaBox.w = size * 4;
	medusaBox.x = medusaX;
	medusaBox.y = medusaY;

	sk1Right.frameNum = 13;
	sk2Right.frameNum = 14;
	sk1Right.frameTime = frameTime;
	sk2Right.frameTime = frameTime;

	skRight.numFrames = 2;
	skRight.frames[0] = sk1Right;
	skRight.frames[1] = sk2Right;

	sk1Left.frameNum = 15;
	sk2Left.frameNum = 16;
	sk1Left.frameTime = frameTime;
	sk2Left.frameTime = frameTime;

	skLeft.numFrames = 2;
	skLeft.frames[0] = sk1Left;
	skLeft.frames[1] = sk2Left;

	bone1.frameNum = 17;
	bone2.frameNum = 18;
	bone3.frameNum = 19;
	bone4.frameNum = 20;
	bone5.frameNum = 21;
	bone6.frameNum = 22;
	bone7.frameNum = 23;
	bone8.frameNum = 24;

	bone1.frameTime = boneTime;
	bone2.frameTime = boneTime;
	bone3.frameTime = boneTime;
	bone4.frameTime = boneTime;
	bone5.frameTime = boneTime;
	bone6.frameTime = boneTime;
	bone7.frameTime = boneTime;
	bone8.frameTime = boneTime;

	boneDef.numFrames = 8;
	boneDef.frames[0] = bone1;
	boneDef.frames[1] = bone2;
	boneDef.frames[2] = bone3;
	boneDef.frames[3] = bone4;
	boneDef.frames[4] = bone5;
	boneDef.frames[5] = bone6;
	boneDef.frames[6] = bone7;
	boneDef.frames[7] = bone8;


	// small bat
	smallBat1.frameNum = 25;
	smallBat2.frameNum = 26;
	smallBat3.frameNum = 27;
	smallBat4.frameNum = 28;

	smallBat1.frameTime = frameTime;
	smallBat2.frameTime = frameTime;
	smallBat3.frameTime = frameTime;
	smallBat4.frameTime = frameTime;

	smallBatIdle.numFrames = 1;
	smallBatIdle.frames[0] = smallBat1;

	smallBatDef.numFrames = 3;
	smallBatDef.frames[0] = smallBat2;
	smallBatDef.frames[1] = smallBat3;
	smallBatDef.frames[2] = smallBat4;

	//frankenstein
	frank1.frameNum = 35;
	frank2.frameNum = 36;
	frank3.frameNum = 37;
	frank1.frameTime = frameTime;
	frank2.frameTime = frameTime;
	frank3.frameTime = frameTime;

	frankDef.numFrames = 4;
	frankDef.frames[0] = frank1;
	frankDef.frames[1] = frank2;
	frankDef.frames[2] = frank3;
	frankDef.frames[3] = frank2;

	dusaBox1.h = dusaSize;
	dusaBox1.w = dusaSize;
	dusaBox2.h = size;
	dusaBox2.w = size;
	dusaBox3.h = size;
	dusaBox3.w = size;
	dusaBox4.h = size;
	dusaBox4.w = size;

	deathBox.h = deathSize;
	deathBox.w = deathSize;
	deathBox.x = deathX;
	deathBox.y = deathY;

	scythe_1.frameNum = 38;
	scythe_2.frameNum = 39;
	scythe_1.frameTime = 150;
	scythe_2.frameTime = 150;

	scythe.numFrames = 2;
	scythe.frames[0] = scythe_1;
	scythe.frames[1] = scythe_2;

	scythe1Box.h = dusaSize;
	scythe1Box.w = dusaSize;
	scythe2Box.h = dusaSize;
	scythe2Box.w = dusaSize;
	scythe3Box.h = dusaSize;
	scythe3Box.w = dusaSize;

	// Populate the array with the values for each tile
	for (i = 0; i < 30; i++)
	{
		for (j = 0; j < length; j++)
		{
			// 2 is for the candle
			if (i == 18 && j % 8 == 0 && j != 0 && j != 8)
			{
				level[i][j].value = 2;
				level[i][j].coll = true;
				level[i][j + 1].value = 2;
				level[i][j + 1].coll = true;
				level[i][j].box.x = (size * j) - x;
				level[i][j].box.y = (size * i) - y;
				level[i][j].box.w = size;
				level[i][j].box.h = size;
				level[i][j + 1].box.x = (size * j) - x + size;
				level[i][j + 1].box.y = (size * i) - y;
				level[i][j + 1].box.w = size;
				level[i][j + 1].box.h = size;
			}

			// 1 is for the ground tile
			else if (i > 24)
			{
				level[i][j].value = 1;
				level[i][j].coll = true;
				level[i][j].box.x = (size * j) - x;
				level[i][j].box.y = (size * i) - y;
				level[i][j].box.w = size;
				level[i][j].box.h = size;

			}

			// 0 is for the background color
			else if (level[i][j].value == NULL)
			{
				level[i][j].value = 0;
			}
		}
	}

	/* Initialize SDL */
	if (SDL_Init(SDL_INIT_VIDEO) < 0) {
		return 1;
	}

	SDL_GL_SetAttribute(SDL_GL_BUFFER_SIZE, 32);
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	SDL_Window* window = SDL_CreateWindow(
		"Game",
		SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
		800, 480,
		SDL_WINDOW_OPENGL);
	if (!window) {
		fprintf(stderr, "Could not create window.ErrorCode = %s\n", SDL_GetError());
		SDL_Quit();
		return 1;
	}

	SDL_GL_CreateContext(window);

	/* Make sure we have a recent version of OpenGL */
	GLenum glewError = glewInit();
	if (glewError != GLEW_OK) {
		fprintf(stderr, "Colud not initialize glew.ErrorCode = %s\n", glewGetErrorString(glewError));
		SDL_Quit();
		return 1;
	}
	if (!GLEW_VERSION_3_0) {
		fprintf(stderr, "OpenGL max supported version is too low.\n");
		SDL_Quit();
		return 1;
	}
	/* Setup OpenGL state */
	glViewport(0, 0, 800, 480);
	glMatrixMode(GL_PROJECTION);
	glOrtho(0, 800, 480, 0, 0, 100);
	glEnable(GL_TEXTURE_2D);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);


	/* load the textures */
	bgTex = glTexImageTGAFile("bg_color.tga", NULL, NULL);
	candleTex = glTexImageTGAFile("candle.tga", NULL, NULL);
	groundTex = glTexImageTGAFile("ground.tga", NULL, NULL);
	bat1Tex = glTexImageTGAFile("bat1.tga", NULL, NULL);
	bat2Tex = glTexImageTGAFile("bat2.tga", NULL, NULL);
	simon_left_Tex_1 = glTexImageTGAFile("simon_left_1.tga", NULL, NULL);
	simon_left_Tex_2 = glTexImageTGAFile("simon_left_2.tga", NULL, NULL);
	simon_left_Tex_3 = glTexImageTGAFile("simon_left_3.tga", NULL, NULL);
	simon_right_Tex_1 = glTexImageTGAFile("simon_right_1.tga", NULL, NULL);
	simon_right_Tex_2 = glTexImageTGAFile("simon_right_2.tga", NULL, NULL);
	simon_right_Tex_3 = glTexImageTGAFile("simon_right_3.tga", NULL, NULL);
	crouch_leftTex = glTexImageTGAFile("crouch_left.tga", NULL, NULL);
	crouch_rightTex = glTexImageTGAFile("crouch_right.tga", NULL, NULL);
	medusa1Tex = glTexImageTGAFile("medusa1.tga", NULL, NULL);
	medusa2Tex = glTexImageTGAFile("medusa2.tga", NULL, NULL);
	sk1RightTex = glTexImageTGAFile("sk1_right.tga", NULL, NULL);
	sk2RightTex = glTexImageTGAFile("sk2_right.tga", NULL, NULL);
	sk1LeftTex = glTexImageTGAFile("sk1_left.tga", NULL, NULL);
	sk2LeftTex = glTexImageTGAFile("sk2_left.tga", NULL, NULL);
	pillarTex = glTexImageTGAFile("pillar.tga", NULL, NULL);
	bone1_Tex = glTexImageTGAFile("bone1.tga", NULL, NULL);
	bone2_Tex = glTexImageTGAFile("bone2.tga", NULL, NULL);
	bone3_Tex = glTexImageTGAFile("bone3.tga", NULL, NULL);
	bone4_Tex = glTexImageTGAFile("bone4.tga", NULL, NULL);
	bone5_Tex = glTexImageTGAFile("bone5.tga", NULL, NULL);
	bone6_Tex = glTexImageTGAFile("bone6.tga", NULL, NULL);
	bone7_Tex = glTexImageTGAFile("bone7.tga", NULL, NULL);
	bone8_Tex = glTexImageTGAFile("bone8.tga", NULL, NULL);
	smallBat1_Tex = glTexImageTGAFile("smallbat1.tga", NULL, NULL);
	smallBat2_Tex = glTexImageTGAFile("smallbat2.tga", NULL, NULL);
	smallBat3_Tex = glTexImageTGAFile("smallbat3.tga", NULL, NULL);
	smallBat4_Tex = glTexImageTGAFile("smallbat4.tga", NULL, NULL);
	hub_Tex = glTexImageTGAFile("hub.tga", NULL, NULL);
	logo_Tex = glTexImageTGAFile("logo.tga", NULL, NULL);
	enter_Tex = glTexImageTGAFile("press_enter.tga", NULL, NULL);
	whipLeft1_Tex = glTexImageTGAFile("whip_left_1.tga", NULL, NULL);
	whipLeft2_Tex = glTexImageTGAFile("whip_left_2.tga", NULL, NULL);
	whipLeft3_Tex = glTexImageTGAFile("whip_left_3.tga", NULL, NULL);
	whipRight1_Tex = glTexImageTGAFile("whip_right_1.tga", NULL, NULL);
	whipRight2_Tex = glTexImageTGAFile("whip_right_2.tga", NULL, NULL);
	whipRight3_Tex = glTexImageTGAFile("whip_right_3.tga", NULL, NULL);
	frank1_Tex = glTexImageTGAFile("frank1.tga", NULL, NULL);
	frank2_Tex = glTexImageTGAFile("frank2.tga", NULL, NULL);
	frank3_Tex = glTexImageTGAFile("frank3.tga", NULL, NULL);
	frame_Tex = glTexImageTGAFile("frame.tga", NULL, NULL);
	castle_Tex = glTexImageTGAFile("castle.tga", NULL, NULL);
	lifeFull_Tex = glTexImageTGAFile("life_full.tga", NULL, NULL);
	lifeEmpty_Tex = glTexImageTGAFile("life_empty.tga", NULL, NULL);
	playerText_Tex = glTexImageTGAFile("player.tga", NULL, NULL);
	gameOver_Tex = glTexImageTGAFile("gameover.tga", NULL, NULL);
	heart_Tex = glTexImageTGAFile("heart.tga", NULL, NULL);
	lvl1_Tex = glTexImageTGAFile("lvl1.tga", NULL, NULL);
	lvl2_Tex = glTexImageTGAFile("lvl2.tga", NULL, NULL);
	lvl3_Tex = glTexImageTGAFile("lvl3.tga", NULL, NULL);
	ending_Tex = glTexImageTGAFile("ending.tga", NULL, NULL);

	dusa_Tex = glTexImageTGAFile("dusa.tga", NULL, NULL);
	deathLeft_Tex = glTexImageTGAFile("death_left.tga", NULL, NULL);
	deathRight_Tex = glTexImageTGAFile("death_right.tga", NULL, NULL);
	scythe1_Tex = glTexImageTGAFile("scythe1.tga", NULL, NULL);
	scythe2_Tex = glTexImageTGAFile("scythe2.tga", NULL, NULL);

	textures[0] = simon_right_Tex_1;
	textures[1] = simon_left_Tex_1;
	textures[2] = simon_left_Tex_2;
	textures[3] = simon_left_Tex_3;
	textures[4] = simon_right_Tex_1;
	textures[5] = simon_right_Tex_2;
	textures[6] = simon_right_Tex_3;
	textures[7] = bat1Tex;
	textures[8] = bat2Tex;
	textures[9] = medusa1Tex;
	textures[10] = medusa2Tex;
	textures[11] = crouch_leftTex;
	textures[12] = crouch_rightTex;
	textures[13] = sk1RightTex;
	textures[14] = sk2RightTex;
	textures[15] = sk1LeftTex;
	textures[16] = sk2LeftTex;
	textures[17] = bone1_Tex;
	textures[18] = bone2_Tex;
	textures[19] = bone3_Tex;
	textures[20] = bone4_Tex;
	textures[21] = bone5_Tex;
	textures[22] = bone6_Tex;
	textures[23] = bone7_Tex;
	textures[24] = bone8_Tex;
	textures[25] = smallBat1_Tex;
	textures[26] = smallBat2_Tex;
	textures[27] = smallBat3_Tex;
	textures[28] = smallBat4_Tex;
	textures[29] = whipLeft1_Tex;
	textures[30] = whipLeft2_Tex;
	textures[31] = whipLeft3_Tex;
	textures[32] = whipRight1_Tex;
	textures[33] = whipRight2_Tex;
	textures[34] = whipRight3_Tex;
	textures[35] = frank1_Tex;
	textures[36] = frank2_Tex;
	textures[37] = frank3_Tex;
	textures[38] = scythe1_Tex;
	textures[39] = scythe2_Tex;


LOOP:;

	medusa.draw = true;
	medusa.posX = medusaX;
	medusa.posY = medusaY;
	medusaX = 700;
	medusaY = 300;
	dusa1.draw = true;
	dusa1.posX = dusa1X;
	dusa1.posY = dusa1Y;
	dusa1.coll = true;
	dusa2.draw = true;
	dusa2.posX = dusa2X;
	dusa2.posY = dusa2Y;
	dusa2.coll = true;
	dusa3.draw = true;
	dusa3.posX = medusaX;
	dusa3.posY = medusaY;
	dusa3.coll = true;
	dusa4.draw = true;
	dusa4.posX = medusaX;
	dusa4.posY = medusaY;
	dusa4.coll = true;
	dusa1X = 700;
	dusa1Y = 400;
	dusa2X = 700;
	dusa2Y = 400;
	skeleton.posX = 600;
	skeleton.posY = 635;
	skeleton.draw = true;
	bone.draw = true;
	bone.posX = skeleton.posX;
	bone.posY = skeleton.posY + 15;
	smallBat.draw = true;
	smallBat.posX = 2185;
	smallBat.posY = 570;
	smallBat.coll = true;
	frank.draw = true;
	frank.posX = frankX;
	frank.posY = frankY;
	frankX = 600;
	frankY = 270;
	death.draw = true;
	death.posX = deathX;
	death.posY = deathY;
	death.walkLeft = true;
	deathX = 650;
	deathY = 300;
	scythe1.coll = true;
	scythe1.draw = true;
	scythe1.walkLeft = true;
	scythe2.coll = true;
	scythe2.draw = true;
	scythe2.walkLeft = true;
	scythe3.coll = true;
	scythe3.draw = true;
	scythe3.walkLeft = true;
	scythe1Range = 0;
	scythe2Range = 0;
	scythe3Range = 0;
	boss = 20;
	life = 5; 
	x = 0;
	y = 329;
	camera.posX = x;
	camera.posY = y;

	currentFrameMs = SDL_GetTicks();

	prevX = x;
	prevY = y;

	animSet(&bat.anim, &batIdle);
	animSet(&medusa.anim, &medusaIdle);
	animSet(&skeleton.anim, &skRight);
	animSet(&bone.anim, &boneDef);
	animSet(&smallBat.anim, &smallBatIdle);
	animSet(&frank.anim, &frankDef);
	animSet(&scythe1.anim, &scythe);
	animSet(&scythe2.anim, &scythe);
	animSet(&scythe3.anim, &scythe);


	/* The game loop */
	while (!shouldExit) {
		// Save last frame's value
		lastFrameMs = currentFrameMs;
		//Keep a pointer to SDL's internal keyboard state */
		kbState = SDL_GetKeyboardState(NULL);
		memcpy(kbPrevState, kbState, sizeof(kbPrevState));

		// Handle OS message pump
		SDL_Event event;
		while (SDL_PollEvent(&event)) {
			switch (event.type) {
			case SDL_QUIT:
				shouldExit = 1;
			}
		}

		if (jump)
		{
			if (height <= 100 && !descend)
			{
				height += yVelocity;
			}

			else
			{
				descend = true;
				height -= yVelocity;

				/*
				if (height <= 0.0)
				{
					jump = false;
					descend = false;
				}*/
			}
			
		}

		currentFrameMs = SDL_GetTicks();
		deltaTime = 0.01;
		//deltaTime = 10;

		keys = kbState;

		//background black
		glClearColor(0, 0, 0, 1);
		glClear(GL_COLOR_BUFFER_BIT);


		if (gameOver)
		{
			glDrawSprite(gameOver_Tex, 60, 0, 650, 500);
			
			if (keys[SDL_SCANCODE_UP])
			{
				cont = 258;
			}

			else if (keys[SDL_SCANCODE_DOWN])
			{
				cont = 320;
			}

			glDrawSprite(heart_Tex, 280, cont, size, size);

			if (keys[SDL_SCANCODE_RETURN])
			{
				if (cont == 258)
				{
					x = 0;
					camera.posX = 0;
					playerX = 290;
					title = true;
					gameOver = false;
					life = 5;
					goto LOOP;
				}

				else
				{
					exit(0);
				}
			}
		}

		// title screen
		if (title)
		{
			glDrawSprite(castle_Tex, 450, 100, 300, 300);
			glDrawSprite(logo_Tex, 75, 10, 500, 300);
			glDrawSprite(enter_Tex, 275, 290, 150, 75);

			// if the return key is pressed
			if (keys[SDL_SCANCODE_RETURN])
			{
				title = false;
				hub = true;
			}
		}

		else if (ending)
		{
			glDrawSprite(ending_Tex, 0, 0, 800, 500);

			if (keys[SDL_SCANCODE_RETURN])
			{
				gameOver = true;
				ending = false;
				
			}
		}

		/*
		-------------------------
				HUB
		-------------------------
		*/
		else if (hub)
		{
			if (bat.anim.isPlaying == false)
			{
				animReset(&bat.anim);
			}

			if (skeleton.anim.isPlaying == false)
			{
				animReset(&skeleton.anim);
			}

			if (bone.anim.isPlaying == false)
			{
				animReset(&bone.anim);
			}

			if (smallBat.anim.isPlaying == false)
			{
				animReset(&smallBat.anim);
			}

			animTick(&player.anim, deltaTime);
			animTick(&bat.anim, deltaTime);
			animTick(&skeleton.anim, deltaTime);
			animTick(&bone.anim, deltaTime);
			animTick(&smallBat.anim, deltaTime);

			// if the left arrow is pressed
			if (keys[SDL_SCANCODE_LEFT] && x > 0)
			{
				if (player.anim.def != &walkingLeft)
				{
					animSet(&player.anim, &walkingLeft);
				}

				if (player.anim.isPlaying == false)
				{
					animReset(&player.anim);
				}

				prevX = x;
				camera.posX -= xcam;
				cameraUpdate(camera, deltaTime);
			}

			// if the right arrow is pressed
			else if (keys[SDL_SCANCODE_RIGHT])
			{

				if (x > 2400)
				{
					
					x = 0;
					camera.posX = 0;
					level1 = true;
					hub = false;
					playerX = 10;
					goto LOOP;
				}

				if (player.anim.def != &walkingRight)
				{
					animSet(&player.anim, &walkingRight);
				}

				if (player.anim.isPlaying == false)
				{
					animReset(&player.anim);
				}

				prevX = x;
				camera.posX += xcam;
				cameraUpdate(camera, deltaTime);
			}

			else if (keys[SDL_SCANCODE_C])
			{
				if (player.anim.def != &whipLeft && player.anim.def != &whipRight)
				{
					if (player.anim.def == &walkingLeft || player.anim.def == &idleLeft)
					{
						animSet(&player.anim, &whipLeft);
					}

					else if (player.anim.def == &walkingRight || player.anim.def == &idleRight)
					{
						animSet(&player.anim, &whipRight);
					}
				}

				if (player.anim.isPlaying == false)
				{
					animReset(&player.anim);
				}

			}

			//if no arrow is being pressed
			else
			{
				if (player.anim.def == &walkingLeft || player.anim.def == &idleLeft || player.anim.def == &whipLeft || player.anim.def == &crouchLeft)
				{
					animSet(&player.anim, &idleLeft);
				}

				else
				{
					animSet(&player.anim, &idleRight);
				}
			}

			

			// if the down arrow is pressed
			if (keys[SDL_SCANCODE_DOWN] && y < 720)
			{
				if (player.anim.def == &crouchRight || player.anim.def == &walkingRight || player.anim.def == &idleRight)
				{
					animSet(&player.anim, &crouchRight);
				}

				else if (player.anim.def == &crouchLeft || player.anim.def == &walkingLeft || player.anim.def == &idleLeft)
				{
					animSet(&player.anim, &crouchLeft);
				}

				if (player.anim.isPlaying == false)
				{
					animReset(&player.anim);
				}

				prevY = y;
				camera.posY += ycam;
				cameraUpdate(camera, deltaTime);
			}

			// if the up arrow is pressed
			else if (keys[SDL_SCANCODE_X] && y > 0)
			{
				//prevY = y;
				//camera.posY -= ycam;
				//cameraUpdate(camera, deltaTime);
				if (height <= 80 && !descend)
				{
					jump = true;
					//playerY -= height;
				}
				
			}

			

			//background black
			glClearColor(0, 0, 0, 1);
			glClear(GL_COLOR_BUFFER_BIT);

			glDrawSprite(hub_Tex, 0 - x, 250 - y, 3000, 500);

			// draw background
			for (i = 0; i < 30; i++)
			{
				for (j = 0; j < length; j++)
				{
					currentTile = level[i][j].value;

					// background drawing optimization

					if (i*size < y || i*size > 640 + y || j*size < x - size || j*size > 800 + x)
					{
						continue;
					}

					switch (currentTile)
					{
					case 0:
						break;

					case 1:
						glDrawSprite(groundTex, (size * j) - x, (size * i) - y, size, size);
						level[i][j].box.x = (size * j) - x;
						level[i][j].box.y = (size * i) - y;
						level[i][j].box.w = size;
						level[i][j].box.h = size;

						if (AABBintersect(&playerBox, &level[i][j].box))
						{
							camera.posX = prevX;
							camera.posY = prevY;
							cameraUpdate(camera, deltaTime);
							height = 0.0;
							jump = false;
							descend = false;
						}

						break;

					case 2:
						/*
						glDrawSprite(candleTex, (size * j) - x, (size * i) - y, size, size);
						level[i][j].box.x = (size * j) - x;
						level[i][j].box.y = (size * i) - y;
						level[i][j].box.w = size;
						level[i][j].box.h = size;
						level[i][j + 1].box.x = (size * j) - x + size;
						level[i][j + 1].box.y = (size * i) - y;
						level[i][j + 1].box.w = size;
						level[i][j + 1].box.h = size;

						if (AABBintersect(&playerBox, &level[i][j].box))
						{
						camera.posX = prevX;
						camera.posY = prevY;
						cameraUpdate(camera, deltaTime);
						}
						*/
						break;
					}
				}
			}

			playerBox.h = simonSize * 2;
			playerBox.w = size * 2;
			playerBox.x = playerX;
			playerBox.y = playerY - height;

			skBox.h = size * 4;
			skBox.w = size * 2;
			skBox.x = skeleton.posX - x;
			skBox.y = skeleton.posY - y;

			boneBox.h = size * 2;
			boneBox.w = size * 2;
			boneBox.x = skeleton.posX - boneRange - x;
			boneBox.y = bone.posY - y;

			smallBatBox.h = size + 10;
			smallBatBox.w = size + 10;
			smallBatBox.x = smallBat.posX - x;
			smallBatBox.y = smallBat.posY - y;

			

			// draw player crouched (modify y)
			if (player.anim.def == &crouchLeft || player.anim.def == &crouchRight)
			{
				animDraw(&player.anim, playerX, playerY + 60, size * 2, simonSize);
				playerBox.h = simonSize;
				playerBox.w = size * 2;
				playerBox.y = playerY - height + 60;
			}

			

			// draw player standing idle
			else
			{
				if (player.anim.def == &whipLeft)
				{
					if (player.anim.curFrame == 0 || player.anim.curFrame == 1)
					{
						animDraw(&player.anim, playerX, playerY - height, size * 3, simonSize * 2);
						whip.draw = false;
					}

					else if (player.anim.curFrame == 2)
					{
						animDraw(&player.anim, playerX - size * 4, playerY - height, size * 5, simonSize * 2);
						whipBox.w = size * 3;
						whipBox.h = 26;
						whipBox.x = playerX - size * 4;
						whipBox.y = playerY - height + simonSize/2 - height;

						whip.draw = true;
					}
					
					else
					{
						whip.draw = false;
					}
				}

				else if (player.anim.def == &whipRight)
				{
					if (player.anim.curFrame == 0 || player.anim.curFrame == 1)
					{
						animDraw(&player.anim, playerX - size, playerY - height, size * 3, simonSize * 2);
						whip.draw = false;
					}

					else if (player.anim.curFrame == 2)
					{
						animDraw(&player.anim, playerX + size, playerY - height, size * 5, simonSize * 2);

						whipBox.w = size * 4;
						whipBox.h = 26;
						whipBox.x = playerX + simonSize;
						whipBox.y = playerY - height + simonSize / 2 - height;

						whip.draw = true;
					}

					else
					{
						whip.draw = false;
					}
				}

				else
				{
					whip.draw = false;
					animDraw(&player.anim, playerX, playerY - height, size * 2, simonSize * 2);

				}
				
			}

			/*
			// draw bats (big)
			for (i = 600; i < 3000; i += 300)
			{
			animDraw(&bat.anim, batX + i - x, batY - y, size * 3, size * 2);
			}
			*/




			if (AABBintersect(&whipBox, &smallBatBox) && whip.draw)
			{
				smallBat.draw = false;
			}

			if (AABBintersect(&playerBox, &smallBatBox) && smallBat.draw && smallBat.coll)
			{
				smallBat.coll = false;
				life--;
			}

			if (smallBatBox.x - playerBox.x < 300)
			{
				if (smallBat.anim.def != &smallBatDef)
				{
					animSet(&smallBat.anim, &smallBatDef);
				}
				smallBat.posX -= 3;

				if (smallBat.posY < 650)
				{
					smallBat.posY += 3;
				}

			}

			if (smallBat.draw)
			{
				animDraw(&smallBat.anim, smallBat.posX - x, smallBat.posY - y, size + 10, size + 10);
			}
		}


		
		/*
		---------------------------------
					frankenstein
		---------------------------------
		*/
		else if (level1)
		{
			if (frank.anim.isPlaying == false)
			{
				animReset(&frank.anim);
			}

			animTick(&player.anim, deltaTime);
			animTick(&frank.anim, deltaTime);

			// if the left arrow is pressed
			if (keys[SDL_SCANCODE_LEFT] && playerX > 0)
			{
				if (player.anim.def != &walkingLeft)
				{
					animSet(&player.anim, &walkingLeft);
				}

				if (player.anim.isPlaying == false)
				{
					animReset(&player.anim);
				}

				prevX = x;
				playerX -= xcam;
				cameraUpdate(camera, deltaTime);
			}

			// if the right arrow is pressed
			else if (keys[SDL_SCANCODE_RIGHT])
			{

				if (playerX > 800)
				{

					x = 0;
					camera.posX = 0;
					level2 = true;
					level1 = false;
					playerX = 0;
					goto LOOP;
				}

				if (player.anim.def != &walkingRight)
				{
					animSet(&player.anim, &walkingRight);
				}

				if (player.anim.isPlaying == false)
				{
					animReset(&player.anim);
				}

				prevX = x;
				playerX += xcam;
				cameraUpdate(camera, deltaTime);
			}

			else if (keys[SDL_SCANCODE_C])
			{
				if (player.anim.def != &whipLeft && player.anim.def != &whipRight)
				{
					if (player.anim.def == &walkingLeft || player.anim.def == &idleLeft)
					{
						animSet(&player.anim, &whipLeft);
					}

					else if (player.anim.def == &walkingRight || player.anim.def == &idleRight)
					{
						animSet(&player.anim, &whipRight);
					}
				}

				if (player.anim.isPlaying == false)
				{
					animReset(&player.anim);
				}

			}

			//if no arrow is being pressed
			else
			{
				if (player.anim.def == &walkingLeft || player.anim.def == &idleLeft || player.anim.def == &whipLeft || player.anim.def == &crouchLeft)
				{
					animSet(&player.anim, &idleLeft);
				}

				else
				{
					animSet(&player.anim, &idleRight);
				}
			}



			// if the down arrow is pressed
			if (keys[SDL_SCANCODE_DOWN] && y < 720)
			{
				if (player.anim.def == &crouchRight || player.anim.def == &walkingRight || player.anim.def == &idleRight)
				{
					animSet(&player.anim, &crouchRight);
				}

				else if (player.anim.def == &crouchLeft || player.anim.def == &walkingLeft || player.anim.def == &idleLeft)
				{
					animSet(&player.anim, &crouchLeft);
				}

				if (player.anim.isPlaying == false)
				{
					animReset(&player.anim);
				}
			}

			// if the X key is pressed
			else if (keys[SDL_SCANCODE_X])
			{
				//prevY = y;
				//camera.posY -= ycam;
				//cameraUpdate(camera, deltaTime);
				if (height <= 80 && !descend)
				{
					jump = true;
					//playerY -= height;
				}

			}



			//background black
			glClearColor(0, 0, 0, 1);
			glClear(GL_COLOR_BUFFER_BIT);

			glDrawSprite(lvl1_Tex, 0 - x, 250 - y, 1000, 500);

			// draw background
			for (i = 0; i < 30; i++)
			{
				for (j = 0; j < length; j++)
				{
					currentTile = level[i][j].value;

					// background drawing optimization

					if (i*size < y || i*size > 640 + y || j*size < x - size || j*size > 800 + x)
					{
						continue;
					}

					switch (currentTile)
					{
					case 0:
						break;

					case 1:
						glDrawSprite(groundTex, (size * j) - x, (size * i) - y, size, size);
						level[i][j].box.x = (size * j) - x;
						level[i][j].box.y = (size * i) - y;
						level[i][j].box.w = size;
						level[i][j].box.h = size;

						if (AABBintersect(&playerBox, &level[i][j].box))
						{
							camera.posX = prevX;
							camera.posY = prevY;
							cameraUpdate(camera, deltaTime);
							height = 0.0;
							jump = false;
							descend = false;
						}

						break;

					case 2:
						/*
						glDrawSprite(candleTex, (size * j) - x, (size * i) - y, size, size);
						level[i][j].box.x = (size * j) - x;
						level[i][j].box.y = (size * i) - y;
						level[i][j].box.w = size;
						level[i][j].box.h = size;
						level[i][j + 1].box.x = (size * j) - x + size;
						level[i][j + 1].box.y = (size * i) - y;
						level[i][j + 1].box.w = size;
						level[i][j + 1].box.h = size;

						if (AABBintersect(&playerBox, &level[i][j].box))
						{
						camera.posX = prevX;
						camera.posY = prevY;
						cameraUpdate(camera, deltaTime);
						}
						*/
						break;
					}
				}
			}


			playerBox.h = simonSize * 2;
			playerBox.w = size * 2;
			playerBox.x = playerX;
			playerBox.y = playerY - height;

			frankBox.h = frankSize;
			frankBox.w = simonSize + size;
			frankBox.x = frank.posX;
			frankBox.y = frank.posY;



			// draw player crouched (modify y)
			if (player.anim.def == &crouchLeft || player.anim.def == &crouchRight)
			{
				animDraw(&player.anim, playerX, playerY + 60, size * 2, simonSize);
				playerBox.h = simonSize;
				playerBox.w = size * 2;
				playerBox.y = playerY - height + 60;
			}



			// draw player standing idle
			else
			{
				if (player.anim.def == &whipLeft)
				{
					if (player.anim.curFrame == 0 || player.anim.curFrame == 1)
					{
						animDraw(&player.anim, playerX, playerY - height, size * 3, simonSize * 2);
						whip.draw = false;
					}

					else if (player.anim.curFrame == 2)
					{
						animDraw(&player.anim, playerX - size * 4, playerY - height, size * 5, simonSize * 2);
						whipBox.w = size * 3;
						whipBox.h = 26;
						whipBox.x = playerX - size * 4;
						whipBox.y = playerY - height + simonSize / 2 - height;

						whip.draw = true;
					}

					else
					{
						whip.draw = false;
					}
				}

				else if (player.anim.def == &whipRight)
				{
					if (player.anim.curFrame == 0 || player.anim.curFrame == 1)
					{
						animDraw(&player.anim, playerX - size, playerY - height, size * 3, simonSize * 2);
						whip.draw = false;
					}

					else if (player.anim.curFrame == 2)
					{
						animDraw(&player.anim, playerX + size, playerY - height, size * 5, simonSize * 2);

						whipBox.w = size * 4;
						whipBox.h = 26;
						whipBox.x = playerX + simonSize;
						whipBox.y = playerY - height + simonSize / 2 - height;

						whip.draw = true;
					}

					else
					{
						whip.draw = false;
					}
				}

				else
				{
					whip.draw = false;
					animDraw(&player.anim, playerX, playerY - height, size * 2, simonSize * 2);

				}

			}


			if (frank.draw)
			{
				animDraw(&frank.anim, frank.posX, frank.posY, simonSize + size, frankSize);
			}

			frankWalk();

			if (AABBintersect(&playerBox, &frankBox) && frank.draw)
			{
				life--;
				playerX -= 100;
			}

			if (AABBintersect(&frankBox, &whipBox) && whip.draw)
			{
				boss--;

				if (boss <= 0)
				{
					frank.draw = false;
					life = 5;
				}

			}
		}
		



		/*
		------------------
		 level 2 (medusa)
		 -----------------
		 */

		else if (level2)
		{
			if (medusa.anim.isPlaying == false)
			{
				animReset(&medusa.anim);
			}

			animTick(&player.anim, deltaTime);
			animTick(&medusa.anim, deltaTime);

			// if the left arrow is pressed
			if (keys[SDL_SCANCODE_LEFT] && playerX > 0)
			{
				if (player.anim.def != &walkingLeft)
				{
					animSet(&player.anim, &walkingLeft);
				}

				if (player.anim.isPlaying == false)
				{
					animReset(&player.anim);
				}

				prevX = x;
				playerX -= xcam;
				cameraUpdate(camera, deltaTime);
			}

			// if the right arrow is pressed
			else if (keys[SDL_SCANCODE_RIGHT])
			{

				if (playerX > 800)
				{

					x = 0;
					camera.posX = 0;
					level3 = true;
					level2 = false;
					playerX = 0;
					goto LOOP;
				}

				if (player.anim.def != &walkingRight)
				{
					animSet(&player.anim, &walkingRight);
				}

				if (player.anim.isPlaying == false)
				{
					animReset(&player.anim);
				}

				prevX = x;
				playerX += xcam;
				cameraUpdate(camera, deltaTime);
			}

			else if (keys[SDL_SCANCODE_C])
			{
				if (player.anim.def != &whipLeft && player.anim.def != &whipRight)
				{
					if (player.anim.def == &walkingLeft || player.anim.def == &idleLeft)
					{
						animSet(&player.anim, &whipLeft);
					}

					else if (player.anim.def == &walkingRight || player.anim.def == &idleRight)
					{
						animSet(&player.anim, &whipRight);
					}
				}

				if (player.anim.isPlaying == false)
				{
					animReset(&player.anim);
				}

			}

			//if no arrow is being pressed
			else
			{
				if (player.anim.def == &walkingLeft || player.anim.def == &idleLeft || player.anim.def == &whipLeft || player.anim.def == &crouchLeft)
				{
					animSet(&player.anim, &idleLeft);
				}

				else
				{
					animSet(&player.anim, &idleRight);
				}
			}



			// if the down arrow is pressed
			if (keys[SDL_SCANCODE_DOWN] && y < 720)
			{
				if (player.anim.def == &crouchRight || player.anim.def == &walkingRight || player.anim.def == &idleRight)
				{
					animSet(&player.anim, &crouchRight);
				}

				else if (player.anim.def == &crouchLeft || player.anim.def == &walkingLeft || player.anim.def == &idleLeft)
				{
					animSet(&player.anim, &crouchLeft);
				}

				if (player.anim.isPlaying == false)
				{
					animReset(&player.anim);
				}
			}

			// if the X key is pressed
			else if (keys[SDL_SCANCODE_X])
			{
				//prevY = y;
				//camera.posY -= ycam;
				//cameraUpdate(camera, deltaTime);
				if (height <= 80 && !descend)
				{
					jump = true;
					//playerY -= height;
				}

			}



			//background black
			glClearColor(0, 0, 0, 1);
			glClear(GL_COLOR_BUFFER_BIT);

			glDrawSprite(lvl2_Tex, 0 - x, 250 - y, 1100, 500);

			// draw background
			for (i = 0; i < 30; i++)
			{
				for (j = 0; j < length; j++)
				{
					currentTile = level[i][j].value;

					// background drawing optimization

					if (i*size < y || i*size > 640 + y || j*size < x - size || j*size > 800 + x)
					{
						continue;
					}

					switch (currentTile)
					{
					case 0:
						break;

					case 1:
						glDrawSprite(groundTex, (size * j) - x, (size * i) - y, size, size);
						level[i][j].box.x = (size * j) - x;
						level[i][j].box.y = (size * i) - y;
						level[i][j].box.w = size;
						level[i][j].box.h = size;

						if (AABBintersect(&playerBox, &level[i][j].box))
						{
							camera.posX = prevX;
							camera.posY = prevY;
							cameraUpdate(camera, deltaTime);
							height = 0.0;
							jump = false;
							descend = false;
						}

						break;

					case 2:
						/*
						glDrawSprite(candleTex, (size * j) - x, (size * i) - y, size, size);
						level[i][j].box.x = (size * j) - x;
						level[i][j].box.y = (size * i) - y;
						level[i][j].box.w = size;
						level[i][j].box.h = size;
						level[i][j + 1].box.x = (size * j) - x + size;
						level[i][j + 1].box.y = (size * i) - y;
						level[i][j + 1].box.w = size;
						level[i][j + 1].box.h = size;

						if (AABBintersect(&playerBox, &level[i][j].box))
						{
						camera.posX = prevX;
						camera.posY = prevY;
						cameraUpdate(camera, deltaTime);
						}
						*/
						break;
					}
				}
			}


			playerBox.h = simonSize * 2;
			playerBox.w = size * 2;
			playerBox.x = playerX;
			playerBox.y = playerY - height;

			medusaBox.h = size * 4;
			medusaBox.w = size * 4;
			medusaBox.x = medusa.posX;
			medusaBox.y = medusa.posY;

			dusa1.posX = dusa1X;
			dusa1.posY = dusa1Y;
			dusaBox1.x = dusa1.posX;
			dusaBox1.y = dusa1.posY;
			
			dusa2.posX = dusa2X;
			dusa2.posY = dusa2Y;
			dusaBox2.x = dusa2.posX;
			dusaBox2.y = dusa2.posY;


			// draw player crouched (modify y)
			if (player.anim.def == &crouchLeft || player.anim.def == &crouchRight)
			{
				animDraw(&player.anim, playerX, playerY + 60, size * 2, simonSize);
				playerBox.h = simonSize;
				playerBox.w = size * 2;
				playerBox.y = playerY - height + 60;
			}



			// draw player standing idle
			else
			{
				if (player.anim.def == &whipLeft)
				{
					if (player.anim.curFrame == 0 || player.anim.curFrame == 1)
					{
						animDraw(&player.anim, playerX, playerY - height, size * 3, simonSize * 2);
						whip.draw = false;
					}

					else if (player.anim.curFrame == 2)
					{
						animDraw(&player.anim, playerX - size * 4, playerY - height, size * 5, simonSize * 2);
						whipBox.w = size * 3;
						whipBox.h = 26;
						whipBox.x = playerX - size * 4;
						whipBox.y = playerY - height + simonSize / 2 - height;

						whip.draw = true;
					}

					else
					{
						whip.draw = false;
					}
				}

				else if (player.anim.def == &whipRight)
				{
					if (player.anim.curFrame == 0 || player.anim.curFrame == 1)
					{
						animDraw(&player.anim, playerX - size, playerY - height, size * 3, simonSize * 2);
						whip.draw = false;
					}

					else if (player.anim.curFrame == 2)
					{
						animDraw(&player.anim, playerX + size, playerY - height, size * 5, simonSize * 2);

						whipBox.w = size * 4;
						whipBox.h = 26;
						whipBox.x = playerX + simonSize;
						whipBox.y = playerY - height + simonSize / 2 - height;

						whip.draw = true;
					}

					else
					{
						whip.draw = false;
					}
				}

				else
				{
					whip.draw = false;
					animDraw(&player.anim, playerX, playerY - height, size * 2, simonSize * 2);

				}

			}


			if (medusa.draw)
			{
				animDraw(&medusa.anim, medusa.posX, medusa.posY, size * 4, size * 4);
				
			}

			medusaWalk();


			if (AABBintersect(&playerBox, &medusaBox) && medusa.draw)
			{
				life--;
				playerX -= 100;
			}

			if (AABBintersect(&medusaBox, &whipBox) && whip.draw)
			{
				boss--;

				if (boss <= 0)
				{
					medusa.draw = false;
					life = 5;
				}

			}

			dusa1_trayectory();
			dusa2_trayectory();
			

			if (dusa1.draw && medusa.draw)
			{
				glDrawSprite(dusa_Tex, dusa1X, dusa1Y, dusaSize, dusaSize);
			}

			if (AABBintersect(&whipBox, &dusaBox1) && whip.draw)
			{
				dusa1.draw = false;
			}

			if (AABBintersect(&playerBox, &dusaBox1) && dusa1.draw && dusa1.coll && medusa.draw)
			{
				life--;
				dusa1.coll = false;
				if (playerX > 100)
				{
					playerX -= 100;
				}
				
			}

			if (dusa2.draw && medusa.draw)
			{
				glDrawSprite(dusa_Tex, dusa2X, dusa2Y, dusaSize, dusaSize);
			}

			if (AABBintersect(&whipBox, &dusaBox2) && whip.draw)
			{
				dusa2.draw = false;
			}

			if (AABBintersect(&playerBox, &dusaBox2) && dusa2.draw && dusa2.coll && medusa.draw)
			{
				life--;
				dusa2.coll = false;
				if (playerX > 100)
				{
					playerX -= 100;
				}
			}
		}




		/*
		------------------
		level 3 (death)
		-----------------
		*/

		else if (level3)
		{
			if (scythe1.anim.isPlaying == false)
			{
				animReset(&scythe1.anim);
			}

			if (scythe2.anim.isPlaying == false)
			{
				animReset(&scythe2.anim);
			}

			if (scythe3.anim.isPlaying == false)
			{
				animReset(&scythe3.anim);
			}

			animTick(&player.anim, deltaTime);
			animTick(&scythe1.anim, deltaTime);
			animTick(&scythe2.anim, deltaTime);
			animTick(&scythe3.anim, deltaTime);

			// if the left arrow is pressed
			if (keys[SDL_SCANCODE_LEFT] && playerX > 0)
			{
				if (player.anim.def != &walkingLeft)
				{
					animSet(&player.anim, &walkingLeft);
				}

				if (player.anim.isPlaying == false)
				{
					animReset(&player.anim);
				}

				prevX = x;
				playerX -= xcam;
				cameraUpdate(camera, deltaTime);
			}

			// if the right arrow is pressed
			else if (keys[SDL_SCANCODE_RIGHT] && playerX < 825)
			{

				if (playerX > 800 && boss <= 0)
				{

					x = 0;
					camera.posX = 0;
					ending = true;
					level3 = false;
					playerX = 290;
					goto LOOP;
				}

				if (player.anim.def != &walkingRight)
				{
					animSet(&player.anim, &walkingRight);
				}

				if (player.anim.isPlaying == false)
				{
					animReset(&player.anim);
				}

				prevX = x;
				playerX += xcam;
				cameraUpdate(camera, deltaTime);
			}

			else if (keys[SDL_SCANCODE_C])
			{
				if (player.anim.def != &whipLeft && player.anim.def != &whipRight)
				{
					if (player.anim.def == &walkingLeft || player.anim.def == &idleLeft)
					{
						animSet(&player.anim, &whipLeft);
					}

					else if (player.anim.def == &walkingRight || player.anim.def == &idleRight)
					{
						animSet(&player.anim, &whipRight);
					}
				}

				if (player.anim.isPlaying == false)
				{
					animReset(&player.anim);
				}

			}

			//if no arrow is being pressed
			else
			{
				if (player.anim.def == &walkingLeft || player.anim.def == &idleLeft || player.anim.def == &whipLeft || player.anim.def == &crouchLeft)
				{
					animSet(&player.anim, &idleLeft);
				}

				else
				{
					animSet(&player.anim, &idleRight);
				}
			}



			// if the down arrow is pressed
			if (keys[SDL_SCANCODE_DOWN] && y < 720)
			{
				if (player.anim.def == &crouchRight || player.anim.def == &walkingRight || player.anim.def == &idleRight)
				{
					animSet(&player.anim, &crouchRight);
				}

				else if (player.anim.def == &crouchLeft || player.anim.def == &walkingLeft || player.anim.def == &idleLeft)
				{
					animSet(&player.anim, &crouchLeft);
				}

				if (player.anim.isPlaying == false)
				{
					animReset(&player.anim);
				}
			}

			// if the X key is pressed
			else if (keys[SDL_SCANCODE_X])
			{
				//prevY = y;
				//camera.posY -= ycam;
				//cameraUpdate(camera, deltaTime);
				if (height <= 80 && !descend)
				{
					jump = true;
					//playerY -= height;
				}

			}



			//background black
			glClearColor(0, 0, 0, 1);
			glClear(GL_COLOR_BUFFER_BIT);

			glDrawSprite(lvl3_Tex, 0 - x, 250 - y, 800, 500);

			// draw background
			for (i = 0; i < 30; i++)
			{
				for (j = 0; j < length; j++)
				{
					currentTile = level[i][j].value;

					// background drawing optimization

					if (i*size < y || i*size > 640 + y || j*size < x - size || j*size > 800 + x)
					{
						continue;
					}

					switch (currentTile)
					{
					case 0:
						break;

					case 1:
						glDrawSprite(groundTex, (size * j) - x, (size * i) - y, size, size);
						level[i][j].box.x = (size * j) - x;
						level[i][j].box.y = (size * i) - y;
						level[i][j].box.w = size;
						level[i][j].box.h = size;

						if (AABBintersect(&playerBox, &level[i][j].box))
						{
							camera.posX = prevX;
							camera.posY = prevY;
							cameraUpdate(camera, deltaTime);
							height = 0.0;
							jump = false;
							descend = false;
						}

						break;

					case 2:
						/*
						glDrawSprite(candleTex, (size * j) - x, (size * i) - y, size, size);
						level[i][j].box.x = (size * j) - x;
						level[i][j].box.y = (size * i) - y;
						level[i][j].box.w = size;
						level[i][j].box.h = size;
						level[i][j + 1].box.x = (size * j) - x + size;
						level[i][j + 1].box.y = (size * i) - y;
						level[i][j + 1].box.w = size;
						level[i][j + 1].box.h = size;

						if (AABBintersect(&playerBox, &level[i][j].box))
						{
						camera.posX = prevX;
						camera.posY = prevY;
						cameraUpdate(camera, deltaTime);
						}
						*/
						break;
					}
				}
			}


			playerBox.h = simonSize * 2;
			playerBox.w = size * 2;
			playerBox.x = playerX;
			playerBox.y = playerY - height;

			deathBox.x = deathX;
			deathBox.y = deathY;

			scythe1.posX = scythe1X;

			scythe1Box.x = scythe1.posX;
			scythe1Box.y = scythe1Y;
			scythe2Box.x = scythe2X;
			scythe2Box.y = scythe2Y;
			scythe3Box.x = scythe3X;
			scythe3Box.y = scythe3Y;

			


			// draw player crouched (modify y)
			if (player.anim.def == &crouchLeft || player.anim.def == &crouchRight)
			{
				animDraw(&player.anim, playerX, playerY + 60, size * 2, simonSize);
				playerBox.h = simonSize;
				playerBox.w = size * 2;
				playerBox.y = playerY - height + 60;
			}



			// draw player standing idle
			else
			{
				if (player.anim.def == &whipLeft)
				{
					if (player.anim.curFrame == 0 || player.anim.curFrame == 1)
					{
						animDraw(&player.anim, playerX, playerY - height, size * 3, simonSize * 2);
						whip.draw = false;
					}

					else if (player.anim.curFrame == 2)
					{
						animDraw(&player.anim, playerX - size * 4, playerY - height, size * 5, simonSize * 2);
						whipBox.w = size * 3;
						whipBox.h = 26;
						whipBox.x = playerX - size * 4;
						whipBox.y = playerY - height + simonSize / 2 - height;

						whip.draw = true;
					}

					else
					{
						whip.draw = false;
					}
				}

				else if (player.anim.def == &whipRight)
				{
					if (player.anim.curFrame == 0 || player.anim.curFrame == 1)
					{
						animDraw(&player.anim, playerX - size, playerY - height, size * 3, simonSize * 2);
						whip.draw = false;
					}

					else if (player.anim.curFrame == 2)
					{
						animDraw(&player.anim, playerX + size, playerY - height, size * 5, simonSize * 2);

						whipBox.w = size * 4;
						whipBox.h = 26;
						whipBox.x = playerX + simonSize;
						whipBox.y = playerY - height + simonSize / 2 - height;

						whip.draw = true;
					}

					else
					{
						whip.draw = false;
					}
				}

				else
				{
					whip.draw = false;
					animDraw(&player.anim, playerX, playerY - height, size * 2, simonSize * 2);

				}

			}

			scythe1_trayectory();
			scythe2_trayectory();
			scythe3_trayectory();

			if (death.draw)
			{
				if (death.walkLeft)
				{
					deathX = 650;
					glDrawSprite(deathLeft_Tex, deathX, deathY, deathSize, deathSize);
				}
				
				else
				{
					deathX = 5;
					glDrawSprite(deathRight_Tex, deathX, deathY, deathSize, deathSize);
				}

			}


			if (AABBintersect(&playerBox, &deathBox) && death.draw)
			{
				life--;

				if (death.walkLeft)
				{
					playerX -= 100;
				}

				else
				{
					playerX += 100;
				}
				
			}

			if (AABBintersect(&deathBox, &whipBox) && whip.draw)
			{
				boss--;

				if (boss <= 0)
				{
					death.draw = false;
					life = 5;
				}
			}

			if (scythe1.draw && death.draw)
			{
				animDraw(&scythe1.anim, scythe1.posX, scythe1Y, dusaSize, dusaSize);
			}

			if (AABBintersect(&whipBox, &scythe1Box) && whip.draw)
			{
				scythe1.draw = false;
			}

			if (AABBintersect(&playerBox, &scythe1Box) && scythe1.draw && scythe1.coll && death.draw)
			{
				life--;
				scythe1.coll = false;
				playerX -= 100;
			}


			if (scythe2.draw && death.draw)
			{
				animDraw(&scythe2.anim, scythe2X, scythe2Y, dusaSize, dusaSize);
			}

			if (AABBintersect(&whipBox, &scythe2Box) && whip.draw)
			{
				scythe2.draw = false;
			}

			if (AABBintersect(&playerBox, &scythe2Box) && scythe2.draw && scythe2.coll && death.draw)
			{
				life--;
				scythe2.coll = false;
				playerX -= 100;
			}

			if (scythe3.draw && death.draw)
			{
				animDraw(&scythe3.anim, scythe3X, scythe3Y, dusaSize, dusaSize);
			}

			if (AABBintersect(&whipBox, &scythe3Box) && whip.draw)
			{
				scythe3.draw = false;
			}

			if (AABBintersect(&playerBox, &scythe3Box) && scythe3.draw && scythe3.coll && death.draw)
			{
				life--;
				scythe3.coll = false;
				playerX -= 100;
			}
		}




		/*
		------------------
				HUB
		------------------
		*/
		if (!title && !gameOver && !ending)
		{
			for (i = 0; i < 2; i++)
			{
				for (j = 0; j < 27; j++)
				{
					glDrawSprite(bgTex, j * size, i * size, size + 10, size + 10);
				}
			}

			for (i = 0; i < 5; i++)
			{
				glDrawSprite(lifeEmpty_Tex, 120 + (i * 15), 20, 10, 20);
			}

			for (i = 0; i < life; i++)
			{
				glDrawSprite(lifeFull_Tex, 120 + (i * 15), 20, 10, 20);
			}

			glDrawSprite(playerText_Tex, 10, 20, 100, 20);
		}

		if (life <= 0)
		{
			gameOver = true;
			level1 = false;
			level2 = false;
			level3 = false;
			hub = false;
			title = false;
		}
		SDL_GL_SwapWindow(window);
	}

	SDL_Quit();

	return 0;
}

// Update he animation for time passing
void animTick(AnimData* data, float dt)
{
	if (!data->isPlaying)
	{
		return;
	}

	int numFrames = data->def->numFrames;
	data->timeToNextFrame -= dt;

	if (data->timeToNextFrame < 0)
	{
		++data->curFrame;
		if (data->curFrame >= numFrames)
		{
			// end of the animation, stop it
			data->curFrame = numFrames - 1;
			data->timeToNextFrame = 0;
			data->isPlaying = false;
		}

		else
		{
			AnimFrameDef* curFrame = &data->def->frames[data->curFrame];
			data->timeToNextFrame += curFrame->frameTime;
		}
	}
}

void animSet(AnimData* anim, AnimDef* toPlay)
{
	anim->def = toPlay;
	anim->curFrame = 0;
	anim->timeToNextFrame = toPlay->frames[0].frameTime;
	anim->isPlaying = true;
}

void animReset(AnimData* anim)
{
	animSet(anim, anim->def);
}

void animDraw(AnimData* anim, int x, int y, int w, int h)
{
	AnimDef* def = anim->def;
	int curFrameNum = def->frames[anim->curFrame].frameNum;
	GLuint tex = textures[curFrameNum];
	glDrawSprite(tex, x, y, w, h);
}

void playerUpdate(Player* player, float deltaTime)
{
}
void cameraUpdate(Camera cam, float deltaTime)
{
	x = cam.posX;
	y = cam.posY;
}

bool AABBintersect(const AABB* box1, const AABB* box2)
{
	//box1 to the right
	if (box1->x > box2->x + box2->w)
	{
		return false;
	}
	//box1 to the left
	if (box1->x + box1->w < box2->x)
	{
		return false;
	}
	//box1 below
	if (box1->y > box2->y + box2->h)
	{
		return false;
	}
	//box1 above
	if (box1->y + box1->h < box2->y)
	{
		return false;
	}

	return true;
}

void frankWalk()
{
	if (frankRange <= 0 || frank.posX > 800)
	{
		frank.walkLeft = true;
		limit = generate_number(150, 300);
	}



	else if (frankRange > limit)
	{
		frank.walkLeft = false;
	}


	if (frank.walkLeft)
	{

		frankRange += 4;
		frank.posX -= 4;
		frankBox.x = frank.posX;
	}

	else
	{

		frankRange -= 4;
		frank.posX += 4;
		frankBox.x = frank.posX;
	}
}

void medusaWalk()
{
	if (medusaRange <= 0 || medusa.posX > 800)
	{
		medusa.walkLeft = true;
		limit = 100;
	}



	else if (medusaRange > limit)
	{
		medusa.walkLeft = false;
	}


	if (medusa.walkLeft)
	{

		medusaRange += 2;
		medusa.posX -= 2;
		medusaBox.x = medusa.posX;
	}

	else
	{

		medusaRange -= 2;
		medusa.posX += 2;
		medusaBox.x = medusa.posX;
	}
}


void dusa1_trayectory()
{
	if (dusa1Range > 800 && medusaRange == 0)
	{
		dusa1X = 800;
		dusa1Range = 0;
		dusa1.coll = true;
	}

	dusa1Range += 5;
	dusa1X -= 5;
	dusaBox1.x = dusa1X;

	if (dusa1Up)
	{
		if (dusa1Y < 200)
		{
			dusa1Up = false;
		}

		dusa1Y -= 4;
		dusaBox1.y = dusa1Y;
	}

	else
	{
		if (dusa1Y > 500)
		{
			dusa1Up = true;
		}

		dusa1Y += 4;
		dusaBox1.y = dusa1Y;
	}

}

void dusa2_trayectory()
{
	if (dusa2Range > 800 && dusa1Range >= 500)
	{
		dusa2X = 800;
		dusa2Range = 0;
		dusa2.coll = true;
	}

	dusa2Range +=5;
	dusa2X -= 5;
	dusaBox2.x = dusa2X;

	if (dusa2Up)
	{
		if (dusa2Y < 200)
		{
			dusa2Up = false;
		}

		dusa2Y -= 4;
		dusaBox2.y = dusa2Y;
	}

	else
	{
		if (dusa2Y > 500)
		{
			dusa2Up = true;
		}

		dusa2Y += 4;
		dusaBox2.y = dusa2Y;
	}

}

void scythe1_trayectory()
{
	if (death.walkLeft)
	{
		if (scythe1Range < 800)
		{
			scythe1Range += scytheVelocity;
			scythe1X -= scytheVelocity;
		}
	}

	else
	{
		scythe1Range -= scytheVelocity;
		scythe1X += scytheVelocity;
	}
}

void scythe2_trayectory()
{
	if (death.walkLeft)
	{
		if (scythe1Range > 500)
		{
			if (scythe2Range < 800)
			{
				scythe2Range += scytheVelocity;
				scythe2X -= scytheVelocity;
			}
		}
		
	}

	else
	{
		if (scythe1Range < 300)
		{
			scythe2Range -= scytheVelocity;
			scythe2X += scytheVelocity;
		}
	}
}

void scythe3_trayectory()
{
	if (scythe3Range > 750)
	{
		death.walkLeft = false;
		scythe1.coll = true;
		scythe2.coll = true;
		scythe3.coll = true;
		scythe1.draw = true;
		scythe2.draw = true;
		scythe3.draw = true;
	}

	if (death.walkLeft)
	{
		if (scythe2Range > 500)
		{
			if (scythe3Range < 800)
			{
				scythe3Range += scytheVelocity;
				scythe3X -= scytheVelocity;
			}
		}
		
	}

	else
	{
		death.walkLeft = false;
		if (scythe2Range < 300)
		{
			scythe3Range -= scytheVelocity;
			scythe3X += scytheVelocity;
		}

		if (scythe3Range < -200)
		{
			death.walkLeft = true;
			scythe1Range = 0;
			scythe2Range = 0;
			scythe3Range = 0;
			scythe1X = 700;
			scythe2X = 700;
			scythe3X = 700;
			scythe1.coll = true;
			scythe2.coll = true;
			scythe3.coll = true;
			scythe1.draw = true;
			scythe2.draw = true;
			scythe3.draw = true;
		}
	}
}

int generate_number(int min, int max)
{
	return rand() % (max - min + 1) + min;
}
