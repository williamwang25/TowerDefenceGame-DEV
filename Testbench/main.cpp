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

//json�ļ���������
void test_json()
{
	std::ifstream file("test.json");
	if (!file.good())
	{
		std::cout << "�޷����ļ�" << std::endl;
		return;
	}
	std::stringstream str_stream;
	str_stream << file.rdbuf();
	file.close();
	//json���ļ�
	cJSON* json_root = cJSON_Parse(str_stream.str().c_str());

	cJSON* json_name = cJSON_GetObjectItem(json_root, "name");
	cJSON* json_age = cJSON_GetObjectItem(json_root, "age");
	cJSON* json_pets = cJSON_GetObjectItem(json_root, "pets");
	cJSON* json_key = cJSON_GetObjectItem(json_root, "key");

	std::cout << json_name->string << ":" << json_name->valuestring << std::endl;
	std::cout << json_age->string << ":" << json_age->valueint << std::endl;
	std::cout << json_key->string << ":" << json_key->valueint << std::endl;

	//����json�����鷽��
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
		std::cout << "�޷����ļ�" << std::endl;
		return;
	}
	std::string str_line;
	while (std::getline(file, str_line))
	{
		//���ж�ȡ����
		std::string str_grid;
		std::stringstream str_stream(str_line);

		//�����������
		while (std::getline(str_stream, str_grid, ','))
		{
			std::cout << str_grid << " ";

		}
		//����
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

	SDL_Init(SDL_INIT_EVERYTHING); //SDL���ʼ��
	IMG_Init(IMG_INIT_JPG | IMG_INIT_PNG); //ͼ����ʼ��
	Mix_Init(MIX_INIT_MP3); //��Ƶ���ʼ��
	TTF_Init(); //���ֿ��ʼ��

	Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048); //��ʼ����Ƶͨ��

	//��������
	SDL_Window* window = SDL_CreateWindow(u8"����", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1280, 720, SDL_WINDOW_SHOWN);
	//Ӳ��������Ⱦ
	SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
	//��ʾͼƬ������1������ͼƬ 2����ȾͼƬ
	SDL_Surface* sur_img = IMG_Load("bg.jpg"); //����ͼƬ�ļ�
	SDL_Texture* tex_img = SDL_CreateTextureFromSurface(renderer, sur_img); //��renderer��Ⱦ


	//��ʾ����
	TTF_Font* font = TTF_OpenFont("ipix.ttf",32);
	SDL_Color font_color = { 0,0,0,255 }; //������ɫ ��ɫ
	SDL_Surface* sur_text = TTF_RenderUTF8_Blended(font, u8"��ã�SDL", font_color);
	SDL_Texture* tex_text = SDL_CreateTextureFromSurface(renderer, sur_text); 


	//��Ϸ����״̬
	bool is_quit = false;
	SDL_Event event;
	//��¼���״̬
	SDL_Point pos_cursor = { 0,0 };
	//ͼ�ο�
	SDL_Rect rect_img, rect_text;

	rect_img.w = sur_img->w;
	rect_img.h = sur_img->h;

	rect_text.w = sur_text->w;
	rect_text.h = sur_text->h;

	//��������

	Mix_Music* music = Mix_LoadMUS("bgm.mp3");
	Mix_FadeInMusic(music, -1, 1500); 
	// music -1����ѭ�� ����ʱ��1500ms

	//������Ⱦ֡��
	int fps = 60;
	//��̬��ʱ
	Uint64 last_counter = SDL_GetPerformanceCounter(); //�����ܼ���
	Uint64 counter_freq = SDL_GetPerformanceFrequency(); //Ƶ��


	while (!is_quit)
	{
		//������Ϣ
		while (SDL_PollEvent(&event))
		{
			if (event.type == SDL_QUIT)
			{
				is_quit = true;
			}
			else if (event.type == SDL_MOUSEMOTION)
			{
				//�������λ��
				pos_cursor.x = event.motion.x;
				pos_cursor.y = event.motion.y;
			}
			else if (event.type == SDL_MOUSEBUTTONDOWN)
			{

			}
		}
		//��ʱ��
		Uint64 current_counter = SDL_GetPerformanceCounter();
		double delta = (double)(current_counter = last_counter) / counter_freq;
		last_counter = current_counter;
		if (delta * 1000 < 1000.0 / 60) //���֡�ʳ���60
		{
			SDL_Delay((Uint32)(1000.0 / 60 - delta * 1000)); //Delay ��̬�ӳ�
		}

		//��������
		//���ο��λ��=���λ��
		rect_img.x = pos_cursor.x;
		rect_img.y = pos_cursor.y;
		//�����
		rect_text.x = pos_cursor.x;
		rect_text.y = pos_cursor.y;
		//��Ⱦ��ͼ
		SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255); //���ñ�����ɫ����ɫ
		SDL_RenderClear(renderer);//����

		SDL_RenderCopy(renderer, tex_img, nullptr, &rect_img); //��ȾͼƬ
		filledCircleRGBA(renderer, pos_cursor.x, pos_cursor.y, 50, 255, 0, 0, 125);
		SDL_RenderCopy(renderer, tex_text, nullptr, &rect_text); //��Ⱦ�����
		SDL_RenderPresent(renderer); //��ʾͼƬ
	}

	return 0;
}