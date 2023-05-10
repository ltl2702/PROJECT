#ifndef IMP_TIMER_H_
#define IMP_TIMER_H_

class ImpTimer{
public:
	ImpTimer();
	~ImpTimer();

	void start(); //bắt đầu đếm thời gian
	void stop(); // dừng đếm thời gian
	void paused(); // tạm dừng đếm thời gian
	void resume(); //tiếp tục đếm thời gian sau khi đã tạm dừng

	int get_ticks(); //trả về thời gian đã trôi qua từ khi bđ đếm thời gian

	bool isStarted(); 
	bool isPaused();

private:
	int start_tick_;  //thời điểm đã trôi qua khi game bắt đầu chạy
	int paused_tick_; //thời gian khi tạm dừng

	bool is_paused_;   // kiểm tra xem đếm tg đã tạm dừng chưa
	bool is_started_; // kiểm tra xem đếm tg đã bắt đầu chưa
};


#endif 
