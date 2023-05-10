#ifndef CHARACTER_H_INCLUDED
#define CHARACTER_H_INCLUDED
#include "stdafx.h"
#include "CommonFunc.h"
#include "BaseObject.h"
class Character : public BaseObject{ 
public:
    Character();
    ~Character();

	bool LoadImg(std::string path, SDL_Renderer* screen); 
	void set_clips();
	void UpdateImagePlayer(SDL_Renderer* des); 
	void Show(SDL_Renderer* des); 
	void HandleInputAction(SDL_Event events, SDL_Renderer * screen); 
	void DoPlayer(Map& map_data); //hàm điều khiển nhân vật chạy đc trên map
	void CheckToMap(Map& map_data);
	void SetMapXY(const int map_x, const int map_y) {map_x_= map_x; map_y_ = map_y;} //thiết lập tọa độ nv trên map
	void CenterEntityOnMap(Map& map_data); 
	void IncreaseMoney(); 
	int GetMoneyCount() const {return money_count;} 
	void DecreaseMoney(); 
	void LoadWin(); 
	float getY() const {return y_pos_;}
	void Free(); 
private:
	int money_count;  

	float x_val_; //lượng thay đổi khi di chuyển
	float y_val_;

	float x_pos_;//vị trí hiện tại
	float y_pos_;

	int width_frame_; 
	int height_frame_;
	SDL_Rect frame_clip[8]; 
	Input input_type_; //biến lưu kiểu trạng thái cho nhân vật
	int i_frame_;
	int status_;
	bool on_ground_; 

	int map_x_;//vị trí bản đồ: khi nhân vật di chuyển
	int map_y_;
};
#endif