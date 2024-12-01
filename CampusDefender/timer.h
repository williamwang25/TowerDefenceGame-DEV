//��ʱ����
//�ṩ��ʱ���������δ�������ͣ���ָ��ȹ���

#ifndef _TIMER_H_
#define _TIMER_H_

#include<functional>

class Timer
{
public:

	Timer() = default;
	~Timer() = default;

	//���ü�ʱ��	
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

	//���º���
	void on_update(double delta)
	{
		if (paused)
		{
			return;
		}

		pass_time += delta;

		if (pass_time >= wait_time)
		{
			bool can_shot = (!one_shot || (one_shot && !shotted)); //����������û�е��δ��� && ���δ�����û�д�����
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
