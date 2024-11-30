//地图瓦片头文件

#ifndef _TILE_H_
#define _TILE_H_

#include<vector>

#define SIZE_TILE 48 //地图瓦片大小 48*48

//地图瓦片结构体
struct Tile
{
	enum class Direction//方向枚举
	{
		None = 0,
		Up = 1,
		Down = 2,
		Left = 3,
		Right = 4
	};
	//地形图层
	int terrain = 0;
	//装饰图层
	int decoration = -1;
	//特殊标记
	int special_flag = -1;
	//方向
	Direction direction = Direction::None;
	//防御塔标记
	bool has_tower = false;

};

typedef std::vector<std::vector<Tile>> TileMap;


#endif // !_TILE_H_
