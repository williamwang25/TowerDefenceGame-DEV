//��Ϸ���й�����
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

//��Ϸ��������
class GameManager : public Manager<GameManager>
{
	//������Ԫ��
	friend class Manager<GameManager>;

public:
	//��Ϸ����������
	int run(int argc, char** argv)
	{
		//�߾��ȼ�ʱ��
		Uint64 last_counter = SDL_GetPerformanceCounter(); //�����ܼ���
		const Uint64 counter_freq = SDL_GetPerformanceFrequency(); //Ƶ��

		//��Ϸ������ѭ��
		while (!is_quit)
		{
			while (SDL_PollEvent(&event))
			{
				if (event.type == SDL_QUIT)
				{
					is_quit = true;
				}
				on_input();//��������

			}
			//60֡��ʾ
			int fps = 60;
			Uint64 current_counter = SDL_GetPerformanceCounter();
			double delta = (double)(current_counter - last_counter) / counter_freq;
			last_counter = current_counter;
			if (delta * 1000 < 1000.0 / fps)
			{
				SDL_Delay((Uint32)(1000.0 / fps - delta * 1000));
			}

			on_update(delta);//��������

			SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);//���ñ�����ɫ

			SDL_RenderClear(renderer);//�����Ⱦ��	

			on_render();//����ͼ��

			SDL_RenderPresent(renderer);

		}

		return 0;
	}

protected:
	GameManager()
	{
		//���캯��
		//��ʼ�����п�ͬʱ��׽����
		init_assert(!SDL_Init(SDL_INIT_EVERYTHING), u8"SDL2 ��ʼ��ʧ��");
		init_assert(IMG_Init(IMG_INIT_JPG | IMG_INIT_PNG), u8"SDL_image ��ʼ��ʧ��");
		init_assert(Mix_Init(MIX_INIT_MP3), u8"SDL_mixer ��ʼ��ʧ��");
		init_assert(!TTF_Init(), u8"SDL_ttf ��ʼ��ʧ��");
		Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048);
		SDL_SetHint(SDL_HINT_IME_SHOW_UI, "1");

		//���������ļ�
		ConfigManager* config = ConfigManager::instance();
		//���ص�ͼ
		init_assert(config->map.load("map.csv"), u8"������Ϸ��ͼʧ��");
		//������Ϸ����
		init_assert(config->load_game_config("config.json"), u8"������Ϸ����ʧ��");
		//���عؿ�����
		init_assert(config->load_level_config("level.json"), u8"���عؿ�����ʧ��");

		//������Ϸ����
		window = SDL_CreateWindow(config->basic_template.window_title.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
			config->basic_template.window_width, config->basic_template.window_height,SDL_WINDOW_SHOWN);
		init_assert(window, u8"������Ϸ����ʧ��");

		//������Ⱦ��
		renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED|SDL_RENDERER_PRESENTVSYNC|SDL_RENDERER_TARGETTEXTURE);
		init_assert(renderer, u8"������Ⱦ��ʧ��");		

		//������Ϸ��Դ
		init_assert(ResourcesManager::instance()->load_from_file(renderer), u8"������Ϸ��Դʧ��");


		init_assert(generate_tile_map_texture(), u8"���ɵ�ͼ����ʧ��");
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
	//������
	void init_assert(bool flag, const char* err_msg)
	{
		if (flag) return;

		SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, u8"��Ϸ����ʧ��", err_msg, window);
		exit(-1);
	}
	//���봦��
	void on_input()
	{

	}

	//��������
	void on_update(double delta)
	{
		static ConfigManager* instance = ConfigManager::instance();

		if (!instance->is_game_over)
		{
			WaveManager::instance()->on_update(delta);
			EnemyManager::instance()->on_update(delta);
		}
	}

	//��Ⱦͼ��
	void on_render()
	{
		static ConfigManager* instance = ConfigManager::instance();
		static SDL_Rect& rect_dst = instance->rect_tile_map;
		SDL_RenderCopy(renderer, tex_tile_map, nullptr, &rect_dst);

		EnemyManager::instance()->on_render(renderer);
	}

	//���ɵ�ͼ����
	bool generate_tile_map_texture()
	{
		const Map& map = ConfigManager::instance()->map;
		const TileMap& tile_map = map.get_tile_map();
		SDL_Rect& rect_tile_map = ConfigManager::instance()->rect_tile_map;
		SDL_Texture* tex_tile_set = ResourcesManager::instance()->get_texture_pool().find(ResID::Tex_Tileset)->second;

		int width_tex_tile_set, height_tex_tile_set;
		SDL_QueryTexture(tex_tile_set, nullptr, nullptr, &width_tex_tile_set, &height_tex_tile_set);
		//�ָ�tile
		int num_tile_single_line = (int)std::ceil((double)width_tex_tile_set / SIZE_TILE);

		//��ȡ��Ⱦ��ͼ���
		int width_tex_tile_map = (int)map.get_width() * SIZE_TILE;
		int height_tex_tile_map = (int)map.get_height() * SIZE_TILE;

		tex_tile_map = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_TARGET, width_tex_tile_map, height_tex_tile_map);
		
		//����tex_tile_mapʧ��
		if (!tex_tile_map) return false;

		//��Ⱦ��������(���Ͻǣ�
		ConfigManager* config = ConfigManager::instance();
		rect_tile_map.x = (config->basic_template.window_width - width_tex_tile_map) / 2;
		rect_tile_map.y = (config->basic_template.window_height - height_tex_tile_map) / 2;
		rect_tile_map.w = width_tex_tile_map;
		rect_tile_map.h = height_tex_tile_map;

		SDL_SetTextureBlendMode(tex_tile_map, SDL_BLENDMODE_BLEND); //������Ⱦģʽ�����ͼ��
		SDL_SetRenderTarget(renderer, tex_tile_map); //������ȾĿ��

		//������ͼ����
		for (int y = 0; y < map.get_height(); y++)
		{
			for (int x = 0; x < map.get_width(); x++)
			{
				SDL_Rect rect_src;
				const Tile& tile = tile_map[y][x];

				//���Ƶ���ͼ��
				const SDL_Rect& rect_dst = { x * SIZE_TILE, y * SIZE_TILE, SIZE_TILE, SIZE_TILE };
				rect_src =
				{
					(tile.terrain % num_tile_single_line) * SIZE_TILE,
					(tile.terrain / num_tile_single_line) * SIZE_TILE,
					SIZE_TILE,SIZE_TILE
				};
				//��Ⱦ����ͼ��
				SDL_RenderCopy(renderer, tex_tile_set, &rect_src, &rect_dst);
				
				//����װ��ͼ��
				if (tile.decoration >= 0)
				{
					rect_src =
					{
						(tile.decoration % num_tile_single_line) * SIZE_TILE,
						(tile.decoration / num_tile_single_line) * SIZE_TILE,
						SIZE_TILE,SIZE_TILE
					};
					//��Ⱦװ��ͼ��
					SDL_RenderCopy(renderer, tex_tile_set, &rect_src, &rect_dst);
				}
			}
		}

		//���ƻ���
		const SDL_Point& idx_home = map.get_idx_home();
		const SDL_Rect rect_dst = { idx_home.x * SIZE_TILE, idx_home.y * SIZE_TILE, SIZE_TILE, SIZE_TILE };
		//��Ⱦ����ͼ��
		SDL_RenderCopy(renderer, ResourcesManager::instance()->get_texture_pool().find(ResID::Tex_Home)->second, nullptr, &rect_dst);

		SDL_SetRenderTarget(renderer, nullptr);//�ͷ���ȾĿ��

		return true;
	}
};

#endif // !_GAME_MANAGER_H_
