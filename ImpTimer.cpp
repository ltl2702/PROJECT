#include "stdafx.h"
#include "ImpTimer.h"
#include "CommonFunc.h"

ImpTimer::ImpTimer(){
	start_tick_= 0;
	paused_tick_ = 0;
	is_paused_ = false;
	is_started_ = false;
}

ImpTimer::~ImpTimer(){}

void ImpTimer::start(){
	is_started_ = true;
	is_paused_ = false;
	start_tick_ = SDL_GetTicks();//Lấy thời gian đã trôi qua
}

void ImpTimer::stop(){
	is_paused_ = false;
	is_started_ = false;
}

void ImpTimer::paused(){
	if (is_started_ == true && is_paused_ == false){ 
		is_paused_ = true; 
		paused_tick_ = SDL_GetTicks() - start_tick_; 
//Thời gian tạm dừng bằng thời gian đã trôi qua trừ đi thời gian đã trôi qua khi game chạy
	}
}

void ImpTimer::resume(){
	if (is_paused_ == true){ 
		is_paused_ = false;
		//Thời điểm cuối cùng còn chưa pause, để tiếp tục tính toán thời gian sau khi đã tạm dừng
		start_tick_ = SDL_GetTicks() - paused_tick_;
		paused_tick_= 0; //đánh dấu việc đã tiếp tục tính toán thời gian
	}
}

//Hàm lấy khoảng thời gian thực hiện khung hình thứ n
int ImpTimer::get_ticks(){
	if (is_started_ == true){ 
		if (is_paused_ == true) 
			return paused_tick_; 
		else return SDL_GetTicks() - start_tick_; 
	}
	//nếu đối tượng ImpTimer đang tính toán thời gian, nó sẽ trả về khoảng thời gian giữa các khung hình.
	//Nếu không, nó sẽ trả về giá trị 0.
	return 0;
}

bool ImpTimer::isStarted(){
	return is_started_; 
}

bool ImpTimer::isPaused(){
	return is_paused_;
}

