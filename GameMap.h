#ifndef GAME_MAP_H
#define GAME_MAP_H
//Chuyên quản lí vấn đề xây dựng map
#include "CommonFunc.h"
#include "BaseObject.h"

//Tilemat đại diện cho 1 hình ảnh tile
class TileMat : public BaseObject  {
public:
	TileMat(){;}
	~TileMat(){;}
};

class GameMap{
public:
	GameMap(){time_exp_g_ = 3;}
	~GameMap(){;}
	void LoadMap(char* name); 
	void LoadTiles(SDL_Renderer* screen); // load image cho từng tilemap tương ứng
	void DrawMap(SDL_Renderer* screen); 
	Map getMap() const {return game_map_;}
	void SetMap(Map& map_data) {game_map_ = map_data;}
private:
	Map game_map_;  //chứa toàn bộ thông tin của bản đồ nhập từ file map
	TileMat tile_mat[MAX_TILES]; //chứa thông tin về đối tượng hình ảnh fill vào map
	int time_exp_g_; //thời gian vụ nổ
};


#endif 
