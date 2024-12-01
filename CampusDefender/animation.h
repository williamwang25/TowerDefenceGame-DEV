//动画类
// 用于实现动画效果

#ifndef _ANIMATION_H_
#define _ANIMATION_H_

#include"timer.h"

#include<functional>
#include<vector>

#include<SDL.h>

class Animation
{
public:

	typedef std::function<void()> PlayCallback;
	
public:

	Animation()
	{
		timer.set_one_shot(false);
		timer.set_on_timeout  //按引用调用
		(
			[&]()
			{
				idx_frame++;

				if (idx_frame >= rect_src_list.size())
				{
					idx_frame = is_loop ? 0 : rect_src_list.size() - 1; //如果循环播放，重复第一帧，否则到最后一帧
				}
				if (!is_loop && on_finished)
				{
					on_finished();
				}

			}
		);

	}


	void reset()
	{
		timer.restart();

		idx_frame = 0;
	}

	//设置帧图片数据
	void set_frame_data(SDL_Texture* texture, int num_x, int num_y, const std::vector<int>& idx_list)
	{
		int width_tex, height_tex;

		this->texture = texture;
		SDL_QueryTexture(texture, nullptr, nullptr, &width_tex, &height_tex);

		width_frame = width_tex / num_x;
		height_frame = height_tex / num_y;

		rect_src_list.resize(idx_list.size());

		for (size_t i = 0; i < idx_list.size(); i++)
		{
			int idx = idx = idx_list[i];
			SDL_Rect& rect_src = rect_src_list[i];

			//计算帧图片的坐标
			rect_src.x = (idx % num_x) * width_frame;
			rect_src.y = (idx / num_x) * height_frame;
			rect_src.w = width_frame;
			rect_src.h = height_frame;
		}
	}

	void set_loop(bool is_loop)
	{
		this->is_loop = is_loop;
	}

	void set_interval(double interval)
	{
		timer.set_wait_time(interval);
	}

	void set_on_finished(PlayCallback on_finished)
	{
		this->on_finished = on_finished;
	}

	void on_update(double delta)
	{
		timer.on_update(delta);
	}

	/// <summary>
	/// 渲染动画函数
	/// </summary>
	void on_render(SDL_Renderer* renderer, const SDL_Point& pos_dst, double angle = 0)const
	{
		static SDL_Rect rect_dst;

		rect_dst.x = pos_dst.x;
		rect_dst.y = pos_dst.y;
		rect_dst.w = width_frame;
		rect_dst.h = height_frame;

		SDL_RenderCopyEx(renderer, texture, &rect_src_list[idx_frame], &rect_dst, angle, nullptr, SDL_RendererFlip::SDL_FLIP_NONE);
	}

	~Animation() = default;

private:


	Timer timer;
	bool is_loop = true; //是否循环播放
	size_t idx_frame = 0; //动画帧索引
	PlayCallback on_finished; //动画播放完毕回调函数

	SDL_Texture* texture = nullptr;
	std::vector<SDL_Rect> rect_src_list;
	int width_frame = 0;
	int height_frame = 0;


};
#endif // !_ANIMATION_H_
