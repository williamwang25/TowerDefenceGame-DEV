//二维向量
//提供二维向量类和数学运算方法

#ifndef _VECTOR2_H_
#define _VECTOR2_H_

#include<cmath>

class Vector2
{
public:
	double x = 0;
	double y = 0;

public:
	
	Vector2() = default;
	~Vector2() = default;

	//带参构造函数 
	Vector2(double x, double y) : x(x), y(y) {}

	//运算符重载
	Vector2 operator+(const Vector2& vec) const
	{
		return Vector2(x + vec.x, y + vec.y);
	}

	void operator+=(const Vector2& vec)
	{
		x += vec.x;
		y += vec.y;
	}

	Vector2 operator-(const Vector2& vec) const
	{
		return Vector2(x - vec.x, y - vec.y);
	}

	void operator-=(const Vector2& vec)
	{
		x -= vec.x;
		y -= vec.y;
	}

	//向量点乘
	double operator*(const Vector2& vec) const
	{
		return x * vec.x + y * vec.y;
	}

	//向量数乘
	Vector2 operator*(double val) const
	{
		return Vector2(x * val, y * val);
	}

	void operator*=(double val)
	{
		x *= val;
		y *= val;
	}

	//判断相等
	bool operator ==(const Vector2& vec)
	{
		return x == vec.x && y == vec.y;
	}

	//向量长度取模
	double length() const
	{
		return sqrt(x * x + y * y);
	}

	//大于
	bool operator>(const Vector2& vec) const
	{
		return length() > vec.length();
	}

	//小于
	bool operator<(const Vector2& vec) const
	{
		return length() < vec.length();
	}

	//向量单位化
	Vector2 normalize() const
	{
		double len = length();
		if (len == 0)
		{
			return Vector2(0, 0);
		}
		return Vector2(x / len, y / len);
	}

	//无穷小近似为0 
	bool approx_zero() const
	{
		return length() < 1e-6;
	}


private:

};
#endif // !_VECTOR2_H_
