//计时器类
//提供计时方法，单次触发，暂停，恢复等功能

#ifndef _TIMER_H_
#define _TIMER_H_

#include<functional>

class Timer
{
public:

	Timer() = default;
	~Timer() = default;

	//重置计时器	
	void restart()
	{
		pass_time = 0;
		shotted = false;
	} 

	void set_wait_time(double time)
	{
		wait_time = time;
	}

	void set_one_shot(bool flag)
	{
		one_shot = flag;
	}

	void set_on_timeout(std::function<void()> func)
	{
		on_timeout = func;
	}

	void pause()
	{
		paused = true;
	}

	void resume()
	{
		paused = false;
	}

	//更新函数
	void on_update(double delta)
	{
		if (paused)
		{
			return;
		}

		pass_time += delta;

		if (pass_time >= wait_time)
		{
			bool can_shot = (!one_shot || (one_shot && !shotted)); //触发条件：没有单次触发 && 单次触发但没有触发过
			shotted = true;
			if (can_shot && on_timeout)
			{
				on_timeout();
			}

			pass_time -= wait_time;
		}
	}

private:

	double pass_time = 0;
	double wait_time = 0;
	bool paused = false;
	bool shotted = false;
	bool one_shot = false;
	std::function<void()> on_timeout = nullptr;

};

#endif // !_TIMER_H_
