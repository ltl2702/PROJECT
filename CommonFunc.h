//Hàm tạo các thông số, hàm chung của chương trình
#ifndef COMMONFUNC_H_INCLUDED
#define COMMONFUNC_H_INCLUDED
#include<Windows.h>
#include<string>
#include<SDL.h>
#include<SDL_image.h>
#include<SDL_mixer.h>
#include<SDL_ttf.h>
#include<iostream>


static SDL_Window* g_window = NULL;
static SDL_Renderer* g_screen = NULL;
static TTF_Font* font_1 = NULL;
static TTF_Font* font_2 = NULL;
static Mix_Music* bg_music = NULL;
static SDL_Event g_event;
static Mix_Chunk* money = NULL;
static Mix_Chunk* explosion = NULL;
static Mix_Chunk* die = NULL;
static Mix_Chunk* win = NULL;

//Screen
const int FRAME_PER_SECOND = 35; //fps (số frame trên 1s)
const int SCREEN_WIDTH = 1280;
const int SCREEN_HEIGHT = 640;
const int SCREEN_BPP = 32;
const int RENDER_DRAW_COLOR = 0xff;
const int SCREEN_HALF_WIDTH = SCREEN_WIDTH / 2;
const int SCREEN_HALF_HEIGHT = SCREEN_HEIGHT / 2;

//Tile map
#define TILE_SIZE 64
#define MAX_MAP_X 400
#define MAX_MAP_Y 10
#define MAX_TILES 24 
#define TREASURE 24
#define EXPLOSION 23
#define BOMB 21
#define MONEY 4
#define BLANK 0

//thông số nhân vật
#define GRAVITY_SPEED 0.8  
#define MAX_FALL_SPEED 10 
#define PLAYER_SPEED 10 
#define PLAYER_JUMP_VAL 19
#define SPRITE_NUM 8

enum WalkType : int{  
		WALK_RIGHT = 1,
		WALK_LEFT = 0,
};

typedef struct Input{
	int left_;
	int right_;
	int jump_;
}Input;

const int err = 1; //sai số

typedef struct Map{ //trạng thái, thông tin, vị trí của 1 ô map
    int start_x_; //vị trí của ô
    int start_y_;
    
    int max_x_;//chỉ số của ô = start_x_/tilesize
    int max_y_; 
    
	int tile[MAX_MAP_Y][MAX_MAP_X]; 
	int data_[MAX_TILES][MAX_TILES];
    char* file_name_; 
} Map;

#define NumMenu 3

namespace CommonFunc
{
    bool CheckFocusWithRect(const int& x, const int& y, const SDL_Rect& rect);
}
#endif // COMMONFUNC_H_INCLUDED

