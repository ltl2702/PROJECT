#include "stdafx.h"
#include "text.h"
#include "CommonFunc.h"

Text::Text(){
	text_color_.r = 255;
	text_color_.g = 255; //trắng
	text_color_.b = 255;
	texture_ = NULL;
}

Text::~Text(){}

bool Text::LoadFromRenderText(TTF_Font* font, SDL_Renderer* screen){
	//Tạo một surface từ chuỗi văn bản str_val_ sử dụng font và màu sắc text_color
	SDL_Surface* text_surface = TTF_RenderText_Solid(font, str_val_.c_str(),text_color_);
	if (text_surface){
		texture_ = SDL_CreateTextureFromSurface(screen, text_surface);
		width_ = text_surface->w;
		height_ = text_surface->h;
	}
		SDL_FreeSurface(text_surface);
	return texture_!= NULL;
}

void Text::Free(){
	if(texture_=NULL){
		SDL_DestroyTexture(texture_);
		texture_ = NULL;
	}
}

void Text::SetColor(Uint8 red, Uint8 green, Uint8 blue){ //dựa trên giá trị RGB 
	text_color_.r = red;
	text_color_.g = green;
	text_color_.b = blue;
}

void Text::SetColor(int type){ //dựa trên giá trị enum type 
	if (type == RED_TEXT){
		SDL_Color color = {255,0,0};
		text_color_ = color;
	}

	else if (type == WHITE_TEXT){
		SDL_Color color = {255,255,255};
		text_color_ = color;
	}

	else if (type == BLACK_TEXT){
		SDL_Color color = {0,0,0};
		text_color_ = color;
	}
}

void Text::RenderText(SDL_Renderer* screen, 
					int xp, int yp, 
					SDL_Rect* clip /*= NULL*/,
					double angle/*=0.0*/, 
					SDL_Point* center /*= NULL*/,
					SDL_RendererFlip flip /*= SDL_FLIP_NONE)*/){
	SDL_Rect renderQuad = {xp, yp, width_, height_};
	if (clip != NULL){
		//kích thước vẽ sẽ bị thu nhỏ theo kích thước của clip
		renderQuad.w = clip -> w;
		renderQuad.h = clip -> h;
	}
	SDL_RenderCopyEx (screen, texture_, clip, &renderQuad, angle, center, flip);
	//Thông số angle và center dùng để quay văn bản theo góc quay và điểm xoay tương ứng
//Thông số flip dùng để phóng to/thu nhỏ và lật ngược văn bản nếu cần.
}

SDL_Rect Text::GetRectFrame()
{
		SDL_Rect rect;
		rect.x = rect_.x;
		rect.y = rect_.y;
		rect.w = width_;
		rect.h = height_;

		return rect;
}
