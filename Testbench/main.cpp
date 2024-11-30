#define SDL_MAIN_HANDLED

#include<iostream>
#include<string>
#include<fstream>
#include<sstream>

#include<SDL.h>
#include<SDL_image.h>
#include<SDL_mixer.h>
#include<SDL_ttf.h>
#include<SDL2_gfxPrimitives.h>

#include<cJSON.h>

//json文件解析方法
void test_json()
{
	std::ifstream file("test.json");
	if (!file.good())
	{
		std::cout << "无法打开文件" << std::endl;
		return;
	}
	std::stringstream str_stream;
	str_stream << file.rdbuf();
	file.close();
	//json根文件
	cJSON* json_root = cJSON_Parse(str_stream.str().c_str());

	cJSON* json_name = cJSON_GetObjectItem(json_root, "name");
	cJSON* json_age = cJSON_GetObjectItem(json_root, "age");
	cJSON* json_pets = cJSON_GetObjectItem(json_root, "pets");
	cJSON* json_key = cJSON_GetObjectItem(json_root, "key");

	std::cout << json_name->string << ":" << json_name->valuestring << std::endl;
	std::cout << json_age->string << ":" << json_age->valueint << std::endl;
	std::cout << json_key->string << ":" << json_key->valueint << std::endl;

	//遍历json中数组方法
	std::cout << json_pets->string << ":" << std::endl;
	cJSON* json_item = nullptr;
	cJSON_ArrayForEach(json_item, json_pets)
	{
		std::cout  << json_item ->valuestring << std::endl;
	}

}

void test_csv()
{
	std::ifstream file("test.csv");
	if (!file.good())
	{
		std::cout << "无法打开文件" << std::endl;
		return;
	}
	std::string str_line;
	while (std::getline(file, str_line))
	{
		//按行读取数据
		std::string str_grid;
		std::stringstream str_stream(str_line);

		//输出行中数据
		while (std::getline(str_stream, str_grid, ','))
		{
			std::cout << str_grid << " ";

		}
		//换行
		std::cout << std::endl;
	}
	file.close();
}


int main()
{
	std::cout << "\ntest json\n" << std::endl;
	test_json();
	std::cout << "\ntest csv\n" << std::endl;
	test_csv();

	SDL_Init(SDL_INIT_EVERYTHING); //SDL库初始化
	IMG_Init(IMG_INIT_JPG | IMG_INIT_PNG); //图像库初始化
	Mix_Init(MIX_INIT_MP3); //音频库初始化
	TTF_Init(); //文字库初始化

	Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048); //初始化音频通道

	//创建窗口
	SDL_Window* window = SDL_CreateWindow(u8"启动", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1280, 720, SDL_WINDOW_SHOWN);
	//硬件加速渲染
	SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
	//显示图片两步：1、加载图片 2、渲染图片
	SDL_Surface* sur_img = IMG_Load("bg.jpg"); //加载图片文件
	SDL_Texture* tex_img = SDL_CreateTextureFromSurface(renderer, sur_img); //用renderer渲染


	//显示字体
	TTF_Font* font = TTF_OpenFont("ipix.ttf",32);
	SDL_Color font_color = { 0,0,0,255 }; //字体颜色 黑色
	SDL_Surface* sur_text = TTF_RenderUTF8_Blended(font, u8"你好，SDL", font_color);
	SDL_Texture* tex_text = SDL_CreateTextureFromSurface(renderer, sur_text); 


	//游戏运行状态
	bool is_quit = false;
	SDL_Event event;
	//记录鼠标状态
	SDL_Point pos_cursor = { 0,0 };
	//图形框
	SDL_Rect rect_img, rect_text;

	rect_img.w = sur_img->w;
	rect_img.h = sur_img->h;

	rect_text.w = sur_text->w;
	rect_text.h = sur_text->h;

	//加载音乐

	Mix_Music* music = Mix_LoadMUS("bgm.mp3");
	Mix_FadeInMusic(music, -1, 1500); 
	// music -1永久循环 淡入时间1500ms

	//设置渲染帧率
	int fps = 60;
	//动态延时
	Uint64 last_counter = SDL_GetPerformanceCounter(); //高性能计数
	Uint64 counter_freq = SDL_GetPerformanceFrequency(); //频率


	while (!is_quit)
	{
		//处理消息
		while (SDL_PollEvent(&event))
		{
			if (event.type == SDL_QUIT)
			{
				is_quit = true;
			}
			else if (event.type == SDL_MOUSEMOTION)
			{
				//更新鼠标位置
				pos_cursor.x = event.motion.x;
				pos_cursor.y = event.motion.y;
			}
			else if (event.type == SDL_MOUSEBUTTONDOWN)
			{

			}
		}
		//计时器
		Uint64 current_counter = SDL_GetPerformanceCounter();
		double delta = (double)(current_counter = last_counter) / counter_freq;
		last_counter = current_counter;
		if (delta * 1000 < 1000.0 / 60) //如果帧率超过60
		{
			SDL_Delay((Uint32)(1000.0 / 60 - delta * 1000)); //Delay 动态延迟
		}

		//处理数据
		//矩形框的位置=鼠标位置
		rect_img.x = pos_cursor.x;
		rect_img.y = pos_cursor.y;
		//字体框
		rect_text.x = pos_cursor.x;
		rect_text.y = pos_cursor.y;
		//渲染绘图
		SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255); //设置背景颜色，白色
		SDL_RenderClear(renderer);//清屏

		SDL_RenderCopy(renderer, tex_img, nullptr, &rect_img); //渲染图片
		filledCircleRGBA(renderer, pos_cursor.x, pos_cursor.y, 50, 255, 0, 0, 125);
		SDL_RenderCopy(renderer, tex_text, nullptr, &rect_text); //渲染字体框
		SDL_RenderPresent(renderer); //显示图片
	}

	return 0;
}