//游戏运行管理器
#ifndef _GAME_MANAGER_H_
#define _GAME_MANAGER_H_

#include"manager.h"
#include"config_manager.h"
#include"resources_manager.h"
#include"enemy_manager.h"
#include"wave_manager.h"

#include<SDL.h>
#include<SDL_ttf.h>
#include<SDL_image.h>
#include<SDL_mixer.h>

//游戏管理器类
class GameManager : public Manager<GameManager>
{
	//声明友元类
	friend class Manager<GameManager>;

public:
	//游戏运行主函数
	int run(int argc, char** argv)
	{
		//高精度计时器
		Uint64 last_counter = SDL_GetPerformanceCounter(); //高性能计数
		const Uint64 counter_freq = SDL_GetPerformanceFrequency(); //频率

		//游戏运行主循环
		while (!is_quit)
		{
			while (SDL_PollEvent(&event))
			{
				if (event.type == SDL_QUIT)
				{
					is_quit = true;
				}
				on_input();//处理输入

			}
			//60帧显示
			int fps = 60;
			Uint64 current_counter = SDL_GetPerformanceCounter();
			double delta = (double)(current_counter - last_counter) / counter_freq;
			last_counter = current_counter;
			if (delta * 1000 < 1000.0 / fps)
			{
				SDL_Delay((Uint32)(1000.0 / fps - delta * 1000));
			}

			on_update(delta);//更新数据

			SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);//设置背景颜色

			SDL_RenderClear(renderer);//清空渲染器	

			on_render();//绘制图像

			SDL_RenderPresent(renderer);

		}

		return 0;
	}

protected:
	GameManager()
	{
		//构造函数
		//初始化运行库同时捕捉错误
		init_assert(!SDL_Init(SDL_INIT_EVERYTHING), u8"SDL2 初始化失败");
		init_assert(IMG_Init(IMG_INIT_JPG | IMG_INIT_PNG), u8"SDL_image 初始化失败");
		init_assert(Mix_Init(MIX_INIT_MP3), u8"SDL_mixer 初始化失败");
		init_assert(!TTF_Init(), u8"SDL_ttf 初始化失败");
		Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048);
		SDL_SetHint(SDL_HINT_IME_SHOW_UI, "1");

		//加载配置文件
		ConfigManager* config = ConfigManager::instance();
		//加载地图
		init_assert(config->map.load("map.csv"), u8"加载游戏地图失败");
		//加载游戏配置
		init_assert(config->load_game_config("config.json"), u8"加载游戏配置失败");
		//加载关卡配置
		init_assert(config->load_level_config("level.json"), u8"加载关卡配置失败");

		//创建游戏窗口
		window = SDL_CreateWindow(config->basic_template.window_title.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
			config->basic_template.window_width, config->basic_template.window_height,SDL_WINDOW_SHOWN);
		init_assert(window, u8"创建游戏窗口失败");

		//创建渲染器
		renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED|SDL_RENDERER_PRESENTVSYNC|SDL_RENDERER_TARGETTEXTURE);
		init_assert(renderer, u8"创建渲染器失败");		

		//加载游戏资源
		init_assert(ResourcesManager::instance()->load_from_file(renderer), u8"加载游戏资源失败");


		init_assert(generate_tile_map_texture(), u8"生成地图纹理失败");
	}

	~GameManager()
	{
		SDL_DestroyRenderer(renderer);
		SDL_DestroyWindow(window);
		TTF_Quit();
		Mix_Quit();
		IMG_Quit();
		SDL_Quit();
	}
private:
	SDL_Event event;
	bool is_quit = false;
	SDL_Window* window = nullptr;
	SDL_Renderer* renderer = nullptr;
	SDL_Texture* tex_tile_map = nullptr;

private:
	//报错函数
	void init_assert(bool flag, const char* err_msg)
	{
		if (flag) return;

		SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, u8"游戏启动失败", err_msg, window);
		exit(-1);
	}
	//输入处理
	void on_input()
	{

	}

	//更新数据
	void on_update(double delta)
	{
		static ConfigManager* instance = ConfigManager::instance();

		if (!instance->is_game_over)
		{
			WaveManager::instance()->on_update(delta);
			EnemyManager::instance()->on_update(delta);
		}
	}

	//渲染图像
	void on_render()
	{
		static ConfigManager* instance = ConfigManager::instance();
		static SDL_Rect& rect_dst = instance->rect_tile_map;
		SDL_RenderCopy(renderer, tex_tile_map, nullptr, &rect_dst);

		EnemyManager::instance()->on_render(renderer);
	}

	//生成地图纹理
	bool generate_tile_map_texture()
	{
		const Map& map = ConfigManager::instance()->map;
		const TileMap& tile_map = map.get_tile_map();
		SDL_Rect& rect_tile_map = ConfigManager::instance()->rect_tile_map;
		SDL_Texture* tex_tile_set = ResourcesManager::instance()->get_texture_pool().find(ResID::Tex_Tileset)->second;

		int width_tex_tile_set, height_tex_tile_set;
		SDL_QueryTexture(tex_tile_set, nullptr, nullptr, &width_tex_tile_set, &height_tex_tile_set);
		//分割tile
		int num_tile_single_line = (int)std::ceil((double)width_tex_tile_set / SIZE_TILE);

		//获取渲染地图宽高
		int width_tex_tile_map = (int)map.get_width() * SIZE_TILE;
		int height_tex_tile_map = (int)map.get_height() * SIZE_TILE;

		tex_tile_map = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_TARGET, width_tex_tile_map, height_tex_tile_map);
		
		//创建tex_tile_map失败
		if (!tex_tile_map) return false;

		//渲染矩形坐标(左上角）
		ConfigManager* config = ConfigManager::instance();
		rect_tile_map.x = (config->basic_template.window_width - width_tex_tile_map) / 2;
		rect_tile_map.y = (config->basic_template.window_height - height_tex_tile_map) / 2;
		rect_tile_map.w = width_tex_tile_map;
		rect_tile_map.h = height_tex_tile_map;

		SDL_SetTextureBlendMode(tex_tile_map, SDL_BLENDMODE_BLEND); //设置渲染模式：混合图层
		SDL_SetRenderTarget(renderer, tex_tile_map); //设置渲染目标

		//遍历地图网格
		for (int y = 0; y < map.get_height(); y++)
		{
			for (int x = 0; x < map.get_width(); x++)
			{
				SDL_Rect rect_src;
				const Tile& tile = tile_map[y][x];

				//绘制地形图层
				const SDL_Rect& rect_dst = { x * SIZE_TILE, y * SIZE_TILE, SIZE_TILE, SIZE_TILE };
				rect_src =
				{
					(tile.terrain % num_tile_single_line) * SIZE_TILE,
					(tile.terrain / num_tile_single_line) * SIZE_TILE,
					SIZE_TILE,SIZE_TILE
				};
				//渲染地形图层
				SDL_RenderCopy(renderer, tex_tile_set, &rect_src, &rect_dst);
				
				//绘制装饰图层
				if (tile.decoration >= 0)
				{
					rect_src =
					{
						(tile.decoration % num_tile_single_line) * SIZE_TILE,
						(tile.decoration / num_tile_single_line) * SIZE_TILE,
						SIZE_TILE,SIZE_TILE
					};
					//渲染装饰图层
					SDL_RenderCopy(renderer, tex_tile_set, &rect_src, &rect_dst);
				}
			}
		}

		//绘制基地
		const SDL_Point& idx_home = map.get_idx_home();
		const SDL_Rect rect_dst = { idx_home.x * SIZE_TILE, idx_home.y * SIZE_TILE, SIZE_TILE, SIZE_TILE };
		//渲染基地图标
		SDL_RenderCopy(renderer, ResourcesManager::instance()->get_texture_pool().find(ResID::Tex_Home)->second, nullptr, &rect_dst);

		SDL_SetRenderTarget(renderer, nullptr);//释放渲染目标

		return true;
	}
};

#endif // !_GAME_MANAGER_H_
