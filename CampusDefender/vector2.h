//��ά����
//�ṩ��ά���������ѧ���㷽��

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

	//���ι��캯�� 
	Vector2(double x, double y) : x(x), y(y) {}

	//���������
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

	//�������
	double operator*(const Vector2& vec) const
	{
		return x * vec.x + y * vec.y;
	}

	//��������
	Vector2 operator*(double val) const
	{
		return Vector2(x * val, y * val);
	}

	void operator*=(double val)
	{
		x *= val;
		y *= val;
	}

	//�ж����
	bool operator ==(const Vector2& vec)
	{
		return x == vec.x && y == vec.y;
	}

	//��������ȡģ
	double length() const
	{
		return sqrt(x * x + y * y);
	}

	//����
	bool operator>(const Vector2& vec) const
	{
		return length() > vec.length();
	}

	//С��
	bool operator<(const Vector2& vec) const
	{
		return length() < vec.length();
	}

	//������λ��
	Vector2 normalize() const
	{
		double len = length();
		if (len == 0)
		{
			return Vector2(0, 0);
		}
		return Vector2(x / len, y / len);
	}

	//����С����Ϊ0 
	bool approx_zero() const
	{
		return length() < 1e-6;
	}


private:

};
#endif // !_VECTOR2_H_
