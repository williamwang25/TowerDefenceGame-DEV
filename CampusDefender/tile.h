//��ͼ��Ƭͷ�ļ�

#ifndef _TILE_H_
#define _TILE_H_

#include<vector>

#define SIZE_TILE 48 //��ͼ��Ƭ��С 48*48

//��ͼ��Ƭ�ṹ��
struct Tile
{
	enum class Direction//����ö��
	{
		None = 0,
		Up = 1,
		Down = 2,
		Left = 3,
		Right = 4
	};
	//����ͼ��
	int terrain = 0;
	//װ��ͼ��
	int decoration = -1;
	//������
	int special_flag = -1;
	//����
	Direction direction = Direction::None;
	//���������
	bool has_tower = false;

};

typedef std::vector<std::vector<Tile>> TileMap;


#endif // !_TILE_H_
