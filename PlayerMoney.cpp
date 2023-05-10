
#include"stdafx.h"
#include"PlayerMoney.h"

PlayerMoney::PlayerMoney(){
	x_pos_ = 0;
	y_pos_ = 0;
}
PlayerMoney::~PlayerMoney(){}
void PlayerMoney::Init(SDL_Renderer* screen){
	bool ret = LoadImg ("img//money.png", screen);
}
void PlayerMoney::Show (SDL_Renderer* screen){
	rect_.x = x_pos_;
	rect_.y = y_pos_;
	Render(screen);
}