#include"stdafx.h"
#include "Character.h"
#include <SDL_messagebox.h>

Character::Character(){
	i_frame_= 0;
	x_pos_ = 0;
	y_pos_ = 0;
	x_val_ = 0;
	y_val_ = 0;
	width_frame_ = 0;
	height_frame_ = 0;
	status_ = WALK_RIGHT;
	input_type_.jump_=0;
	input_type_.left_=0;
	input_type_.right_ =0;
	on_ground_ = false;
	map_x_ = 0;
	map_y_ = 0;
	money_count = 0;
};

Character::~Character(){}

bool Character::LoadImg(std::string path, SDL_Renderer* screen){
	bool ret = BaseObject::LoadImg(path,screen);
	if (ret == true){
		width_frame_ = rect_.w / SPRITE_NUM;
		height_frame_ = rect_.h;
	}
	return ret;
}

void Character::set_clips() {
	if (width_frame_> 0 && height_frame_> 0) {
		for (int i = 0; i < SPRITE_NUM; i++) {
            frame_clip[i].x = width_frame_ * i;
            frame_clip[i].y = 0;
            frame_clip[i].w = width_frame_;
            frame_clip[i].h = height_frame_;
        }
	}
}
void Character::UpdateImagePlayer(SDL_Renderer* des){
	std::string img_path;
	if (on_ground_)
		{
			img_path = (status_ == WALK_LEFT) ? "img//walk_left.png" : "img//walk_right.png";
		}
		else
		{
			img_path = (status_ == WALK_LEFT) ? "img//jump_left.png" : "img//jump_right.png";
		}
		LoadImg(img_path, des);
}

void Character::Show(SDL_Renderer* des){
	UpdateImagePlayer(des);
	if (input_type_.left_||input_type_.right_ ){
		i_frame_  = (i_frame_ + 1) % SPRITE_NUM;
	}
	else {
			i_frame_ = 0;
	}
	 //vị trí = quãng đường đi được trừ đi một lượng map đã bị cuốn
	rect_.x = x_pos_ - map_x_;   //x_pos_ vtri tren map, rect.x vtri trên screen
	rect_.y = y_pos_ - map_y_;

	SDL_Rect* current_clip = &frame_clip[i_frame_];
	SDL_Rect renderQuad = {rect_.x, rect_.y, width_frame_, height_frame_}; //tọa độ vị trí hiện tại
	SDL_RenderCopy(des, p_object_, current_clip, &renderQuad);
}

void Character::HandleInputAction(SDL_Event event, SDL_Renderer* renderer) {
    switch (event.type) {
        case SDL_KEYDOWN:
            switch (event.key.keysym.sym) {
                case SDLK_RIGHT:
                    status_ = WALK_RIGHT;
                    input_type_.right_ = 1;
                    input_type_.left_ = 0;
                    break;
                case SDLK_LEFT:
                    status_ = WALK_LEFT;
                    input_type_.left_ = 1;
                    input_type_.right_ = 0;
                    break;
				case SDLK_SPACE:
				case SDLK_UP:
                    input_type_.jump_ = 1;
                    break;
            }
            break;
        case SDL_KEYUP:
            switch (event.key.keysym.sym) {
                case SDLK_RIGHT:
                    input_type_.right_ = 0;
                    break;
                case SDLK_LEFT:
                    input_type_.left_ = 0;
                    break;
            }
            break;
    }
    UpdateImagePlayer(renderer);
}


void Character::DoPlayer(Map& map_data){
		x_val_ = 0; //vận tốc theo trục x
		y_val_ += GRAVITY_SPEED;  //vận tốc theo trục y
		if (y_val_ >= MAX_FALL_SPEED) {
			y_val_ = MAX_FALL_SPEED;
		}
			// Move left or right
		if (input_type_.left_ == 1) {
			x_val_ -= PLAYER_SPEED;
		} else if (input_type_.right_ == 1) {
			x_val_ += PLAYER_SPEED;
		}
			// Jump if on the ground
		if (input_type_.jump_) {
			if (on_ground_) {
				y_val_ = -PLAYER_JUMP_VAL;
				on_ground_ = false;
			}
			input_type_.jump_ = 0;
		}
		CheckToMap(map_data);
		CenterEntityOnMap(map_data);
	}

void Character::CheckToMap(Map& map_data){
	int x1 = 0; //chỉ số các ô kề nv theo chiều ngang
	int x2 = 0;

	int y1 = 0; //chỉ số các ô kề nv theo chiều dọc
	int y2 = 0;

	//CHECK THEO CHIỀU NGANG
	int height_min = height_frame_ < TILE_SIZE ? height_frame_ : TILE_SIZE;
	x1 = (x_pos_ + x_val_)/TILE_SIZE;   //ô hiện tại
	x2 = (x_pos_ + x_val_ + width_frame_ - err)/TILE_SIZE;

	y1 = (y_pos_)/TILE_SIZE;
	y2 = (y_pos_ + height_min - err)/TILE_SIZE;

	 /*
            (x1,y1)*****************(x2,y1)
               *     *           *     *
               *********** o ***********
               *     *    \|/    *     *
               *     *    / \    *     *
               *     *           *     *
               *     *           *     *
               *     *           *     *
            (x1,y2)*****************(x2,y2)
    */

if (x1 >= 0 && x2 < MAX_MAP_X && y1 >= 0 && y2 < MAX_MAP_Y){
 if (x_val_ > 0) { //nhân vật di chuyển tiến sang phải
        if ((map_data.tile[y1][x2] != BLANK && map_data.tile[y1][x2] != EXPLOSION) || 
            (map_data.tile[y2][x2] != BLANK && map_data.tile[y2][x2] != EXPLOSION)) {
            if (map_data.tile[y1][x2] == MONEY) {
                map_data.tile[y1][x2] = BLANK;
                IncreaseMoney();
            } else if (map_data.tile[y2][x2] == MONEY) {
                map_data.tile[y2][x2] = BLANK;
                IncreaseMoney();
            } else if (map_data.tile[y1][x2] == BOMB) {
                map_data.tile[y1][x2] = EXPLOSION;
                DecreaseMoney();
            } else if (map_data.tile[y2][x2] == BOMB) {
                map_data.tile[y2][x2] = EXPLOSION;
                DecreaseMoney();
            } else if (map_data.tile[y1][x2] == TREASURE || map_data.tile[y2][x2] == TREASURE) {
                LoadWin();
            } else {
			//cập nhật lại tọa độ x của nhân vật để đặt nó trở lại bên trong ô đó
                x_pos_ = x2 * TILE_SIZE; //va chạm vào ô ko trống nên bị chặn lại
                x_pos_ -= width_frame_ + err; 
                x_val_ = 0; // không thể đi đc, khoảng thay đổi là 0
            }
        }
	}

    else if (x_val_ < 0) {// Nhân vật đang di chuyển lùi sang trái
       if (map_data.tile[y1][x1] != BLANK && map_data.tile[y1][x1] != EXPLOSION || 
            map_data.tile[y2][x1] != BLANK && map_data.tile[y2][x1] != EXPLOSION) {
            if (map_data.tile[y1][x1] == MONEY) {
                map_data.tile[y1][x1] = BLANK;
                IncreaseMoney();
            } else if (map_data.tile[y2][x1] == MONEY) {
                map_data.tile[y2][x1] = BLANK;
                IncreaseMoney();
            } else if (map_data.tile[y1][x1] == BOMB) {
                map_data.tile[y1][x1] = EXPLOSION;
                DecreaseMoney();
            } else if (map_data.tile[y2][x1] == BOMB) {
                map_data.tile[y2][x1] = EXPLOSION;
                DecreaseMoney();
            } else if (map_data.tile[y1][x1] == TREASURE || map_data.tile[y2][x1] == TREASURE) {
                LoadWin();
            } else {
                x_pos_ = (x1 + err) * TILE_SIZE;
                x_val_ = 0; //không thể lùi đc
            }
        }
    }
}	
	//CHECK THEO CHIỀU DỌC
	int width_min = width_frame_ < TILE_SIZE ? width_frame_ : TILE_SIZE;
	x1 = x_pos_/TILE_SIZE;
	x2= (x_pos_ + width_min)/TILE_SIZE;

	y1=(y_pos_ + y_val_)/TILE_SIZE;
	y2=(y_pos_ + y_val_ + height_frame_ -err)/TILE_SIZE;

if (x1 >= 0 && x2 < MAX_MAP_X && y1 >=0 && y2 < MAX_MAP_Y){
	if (y_val_ > 0){ // Nhân vật đang rơi
       if ((map_data.tile[y2][x1] != BLANK && map_data.tile[y2][x1] != EXPLOSION) || 
		   (map_data.tile[y2][x2] != BLANK && map_data.tile[y2][x2] != EXPLOSION)) {
            if (map_data.tile[y2][x1] == MONEY) {
                map_data.tile[y2][x1] = BLANK;
                IncreaseMoney();
            } else if (map_data.tile[y2][x1] == BOMB) {
                map_data.tile[y2][x1] = EXPLOSION;
                DecreaseMoney();
            } else if (map_data.tile[y2][x2] == MONEY) {
                map_data.tile[y2][x2] = BLANK;
                IncreaseMoney();
            } else if (map_data.tile[y2][x2] == BOMB) {
                map_data.tile[y2][x2] = EXPLOSION;
                DecreaseMoney();
            } else if (map_data.tile[y2][x2] == TREASURE || map_data.tile[y2][x1] == TREASURE) {
                LoadWin();
            } else {
                y_pos_ = y2 * TILE_SIZE;
                y_pos_ -= height_frame_ + err;
                y_val_ = 0;
                on_ground_ = true;
            }
        }
      }
   
    else if (y_val_ < 0){ // Nhân vật đang nhảy lên
        if ((map_data.tile[y1][x1] != BLANK && map_data.tile[y1][x1] != EXPLOSION) || (map_data.tile[y1][x2] != BLANK && map_data.tile[y1][x2] != EXPLOSION)) {
            if (map_data.tile[y1][x1] == MONEY) {
                map_data.tile[y1][x1] = BLANK;
                IncreaseMoney();
            } else if (map_data.tile[y1][x1] == BOMB) {
                map_data.tile[y1][x1] = EXPLOSION;
                DecreaseMoney();
            } else if (map_data.tile[y1][x2] == MONEY) {
                map_data.tile[y1][x2] = BLANK;
                IncreaseMoney();
            } else if (map_data.tile[y1][x2] == BOMB) {
                map_data.tile[y1][x2] = EXPLOSION;
                DecreaseMoney();
            } else if (map_data.tile[y1][x1] == TREASURE || map_data.tile[y1][x2] == TREASURE) {
                LoadWin();
            } else {
                y_pos_ = (y1 + err) * TILE_SIZE;
                y_val_ = 0;
                on_ground_ = false;
            }
        }
    }
}
//nếu không va chạm với ô nào, sẽ đi bình thường
	x_pos_ += x_val_;
	y_pos_ += y_val_;

	if (x_pos_<0)  x_pos_= 0;//nếu đến mép bản đồ rồi thì ko cho lùi nữa
	else if (x_pos_ + width_frame_ > map_data.max_x_){
		x_pos_ = map_data.max_x_ - width_frame_ - err;
	}

}
void Character::CenterEntityOnMap(Map& map_data){
	//Set vị trí trục x: Nv đi theo chiều ngang
	map_data.start_x_ = x_pos_ - SCREEN_HALF_WIDTH;
	if (map_data.start_x_ < 0) map_data.start_x_ = 0;
	else if (map_data.start_x_ + SCREEN_WIDTH > map_data.max_x_) 
		map_data.start_x_  = map_data.max_x_ - SCREEN_WIDTH;

	//Set vị trí trục y: Nv đi theo chiều dọc
	map_data.start_y_ = y_pos_ - SCREEN_HALF_HEIGHT;
	if (map_data.start_y_ < 0) map_data.start_y_ = 0;
	else if (map_data.start_y_ + SCREEN_HEIGHT > map_data.max_y_)
		map_data.start_y_  = map_data.max_y_ - SCREEN_HEIGHT;
}

void Character::LoadWin()
{
	Mix_HaltMusic();
	win = Mix_LoadWAV("music//win.wav");
	Mix_PlayChannel(-1,win , 0);
	MessageBoxA(NULL, "YOU WON!", "Congratulations", MB_OK);
	Free();
}
void Character::IncreaseMoney() {
    money_count++;
    Mix_PlayChannelTimed(-1, Mix_LoadWAV("music//money.wav"), 0, 1000);
}

void Character::DecreaseMoney(){
	money_count -= 10;
}
void Character::Free()
{
	Mix_FreeChunk(win);
	win = NULL;
	SDL_Quit();
	exit(0); 
}



