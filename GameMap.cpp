#include "stdafx.h"
#include "GameMap.h"

void GameMap::LoadMap(char* name){
	FILE* fp = NULL;  
	fopen_s(&fp,name,"rb");  //read binary
	if (fp == NULL){
		return;
	}
	game_map_.max_x_= 0;
	game_map_.max_y_= 0;

	for (int i = 0; i < MAX_MAP_Y; i++){
		for (int j = 0; j < MAX_MAP_X; j++){
			fscanf_s(fp, "%d", &game_map_.tile[i][j]); 
			int val = game_map_.tile[i][j];
			if (val > 0){
//xđ kích thước tối đa của bản đồ dựa trên giá trị lớn nhất trong các phần tử của ma trận
				if (j > game_map_.max_x_) game_map_.max_x_ = j;
				if (i > game_map_.max_y_) game_map_.max_y_= i;
			}	
		}
	}
	//kích thước bản đồ được cộng thêm 1 tilesize->kích thước tối đa
		game_map_.max_x_ = (game_map_.max_x_ + 1) * TILE_SIZE;
		game_map_.max_y_ = (game_map_.max_y_ + 1) * TILE_SIZE;
//đảm bảo rằng kích thước của bản đồ sẽ đủ lớn để chứa tất cả các ô,
//bao gồm cả ô cuối cùng ở phía bên phải và phía dưới cùng.

		game_map_.start_x_ = 0;//vị trí bđ bản đồ
		game_map_.start_y_ = 0;

		game_map_.file_name_ = name;
		fclose(fp);
}

void GameMap::LoadTiles(SDL_Renderer* screen){
	char file_img[30];
	FILE* fp = NULL;

	for (int i = 0 ; i < MAX_TILES; i++){
		sprintf_s(file_img, "map/%d.png", i );
		fopen_s(&fp, file_img, "rb");
		if (fp == NULL){
			continue;
		}
		fclose(fp); 
		tile_mat[i].LoadImg(file_img, screen);
	}
}

void GameMap::DrawMap(SDL_Renderer* screen){
	int x1 = 0;
    int x2 = 0;
    int y1 = 0;
    int y2 = 0;
	
    int map_x = game_map_.start_x_ / TILE_SIZE;    //vị trí ô trên bản đồ game_map
    x1 = (game_map_.start_x_ % TILE_SIZE) * -1;
	x2 = x1 + SCREEN_WIDTH + (x1 == 0 ? 0 : TILE_SIZE);

    int map_y = game_map_.start_y_ / TILE_SIZE;
    y1 = (game_map_.start_y_ % TILE_SIZE) * -1;
    y2 = y1 + SCREEN_HEIGHT + (y1 == 0 ? 0 : TILE_SIZE);

    for (int i = y1; i < y2; i += TILE_SIZE) {
        map_x = game_map_.start_x_ / TILE_SIZE;
	//biến map_x được cập nhật trở lại vị trí ban đầu của nó sau mỗi lần hoàn thành một hàng của bản đồ.
        for (int j = x1; j < x2; j += TILE_SIZE) {
            const int val = game_map_.tile[map_y][map_x]; //tile[map_y][map_x]: vị trí ô thứ bao nhiêu
            if (val == EXPLOSION && time_exp_g_ > 0) {
                explosion = Mix_LoadWAV("music//explosion.wav");
                Mix_PlayChannel(-1, explosion, 0);
                time_exp_g_--;
                if (time_exp_g_ == 0) {
                    game_map_.tile[map_y][map_x] = BLANK;
                    time_exp_g_ = 3;
                }
            }

            if (val > 0) {  //>0 thì vẽ
                tile_mat[val].SetRect(j, i);
                tile_mat[val].Render(screen);
            }
            map_x++;
        }
        map_y++; //chuyển sang vẽ các ô tiếp theo
    }
}
