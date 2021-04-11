#pragma once
#include <cstdint>

struct fvec2
{
	float x;
	float y;

	bool operator==(const fvec2& other) { if (other.x == this->x && other.y == this->y) { return true; } return false; }
	fvec2& operator+(const fvec2& other) { this->x += other.x; this->y += other.y; return *this; }
	fvec2& operator-(const fvec2& other) { this->x -= other.x; this->y -= other.y; return *this; }
	fvec2& operator*(const fvec2& other) { this->x *= other.x; this->y *= other.y; return *this; }
	fvec2& operator*(const float& other) { this->x *= other; this->y *= other; return *this; }
	fvec2& operator/(const fvec2& other) { this->x /= other.x; this->y /= other.y; return *this; }
	fvec2& operator/(const float& other) { this->x /= other; this->y /= other; return *this; }
};
struct fvec3
{
	float x;
	float y;
	float z;

	bool operator==(const fvec3& other) { if (other.x == this->x && other.y == this->y && other.z == this->z) { return true; } return false; }
	fvec3& operator+(const fvec3& other) { this->x += other.x; this->y += other.y; this->z += other.z; return *this; }
	fvec3& operator-(const fvec3& other) { this->x -= other.x; this->y -= other.y; this->z -= other.z; return *this; }
	fvec3& operator*(const fvec3& other) { this->x *= other.x; this->y *= other.y; this->z *= other.z; return *this; }
	fvec3& operator*(const float& other) { this->x *= other; this->y *= other; this->z *= other; return *this; }
	fvec3& operator/(const fvec3& other) { this->x /= other.x; this->y /= other.y; this->z /= other.z; return *this; }
	fvec3& operator/(const float& other) { this->x /= other; this->y /= other; this->z /= other; return *this; }
};
struct fvec4
{
	float x;
	float y;
	float z;
	float w;

	bool operator==(const fvec4& other) { if (other.x == this->x && other.y == this->y && other.z == this->z && other.w == this->w) { return true; } return false; }
	fvec4& operator+(const fvec4& other) { this->x += other.x; this->y += other.y; this->z += other.z; this->w += other.w; return *this; }
	fvec4& operator-(const fvec4& other) { this->x -= other.x; this->y -= other.y; this->z -= other.z; this->w -= other.w; return *this; }
	fvec4& operator*(const fvec4& other) { this->x *= other.x; this->y *= other.y; this->z *= other.z; this->w *= other.w; return *this; }
	fvec4& operator*(const float& other) { this->x *= other; this->y *= other; this->z *= other; this->w *= other;       return *this; }
	fvec4& operator/(const fvec4& other) { this->x /= other.x; this->y /= other.y; this->z /= other.z; this->w /= other.w; return *this; }
	fvec4& operator/(const float& other) { this->x /= other; this->y /= other; this->z /= other; this->w /= other; return *this; }
};

struct ivec2
{
	int x;
	int y;

	bool operator==(const ivec2& other) { if (other.x == this->x && other.y == this->y) { return true; } return false; }
	ivec2& operator+(const ivec2& other) { this->x += other.x; this->y += other.y; return *this; }
	ivec2& operator-(const ivec2& other) { this->x -= other.x; this->y -= other.y; return *this; }
	ivec2& operator*(const ivec2& other) { this->x *= other.x; this->y *= other.y; return *this; }
	ivec2& operator*(const float& other) { this->x *= other; this->y *= other; return *this; }
	ivec2& operator/(const ivec2& other) { this->x /= other.x; this->y /= other.y; return *this; }
	ivec2& operator/(const float& other) { this->x /= other; this->y /= other; return *this; }
};
struct ivec3
{
	int x;
	int y;
	int z;

	bool operator==(const ivec3& other) { if (other.x == this->x && other.y == this->y && other.z == this->z) { return true; } return false; }
	ivec3& operator+(const ivec3& other) { this->x += other.x; this->y += other.y; this->z += other.z; return *this; }
	ivec3& operator-(const ivec3& other) { this->x -= other.x; this->y -= other.y; this->z -= other.z; return *this; }
	ivec3& operator*(const ivec3& other) { this->x *= other.x; this->y *= other.y; this->z *= other.z; return *this; }
	ivec3& operator*(const float& other) { this->x *= other; this->y *= other; this->z *= other; return *this; }
	ivec3& operator/(const ivec3& other) { this->x /= other.x; this->y /= other.y; this->z /= other.z; return *this; }
	ivec3& operator/(const float& other) { this->x /= other; this->y /= other; this->z /= other; return *this; }
};
struct ivec4
{
	int x;
	int y;
	int z;
	int w;

	bool operator==(const ivec4& other) { if (other.x == this->x && other.y == this->y && other.z == this->z && other.w == this->w) { return true; } return false; }
	ivec4& operator+(const ivec4& other) { this->x += other.x; this->y += other.y; this->z += other.z; this->w += other.w; return *this; }
	ivec4& operator-(const ivec4& other) { this->x -= other.x; this->y -= other.y; this->z -= other.z; this->w -= other.w; return *this; }
	ivec4& operator*(const ivec4& other) { this->x *= other.x; this->y *= other.y; this->z *= other.z; this->w *= other.w; return *this; }
	ivec4& operator*(const float& other) { this->x *= other; this->y *= other; this->z *= other; this->w *= other;       return *this; }
	ivec4& operator/(const ivec4& other) { this->x /= other.x; this->y /= other.y; this->z /= other.z; this->w /= other.w; return *this; }
	ivec4& operator/(const float& other) { this->x /= other; this->y /= other; this->z /= other; this->w /= other; return *this; }
};
struct bvec2
{
	uint8_t x;
	uint8_t y;

	bool operator==(const bvec2& other) { if (other.x == this->x && other.y == this->y) { return true; } return false; }
	bvec2& operator+(const bvec2& other) { this->x += other.x; this->y += other.y; return *this; }
	bvec2& operator-(const bvec2& other) { this->x -= other.x; this->y -= other.y; return *this; }
	bvec2& operator*(const bvec2& other) { this->x *= other.x; this->y *= other.y; return *this; }
	bvec2& operator*(const float& other) { this->x *= other; this->y *= other; return *this; }
	bvec2& operator/(const bvec2& other) { this->x /= other.x; this->y /= other.y; return *this; }
	bvec2& operator/(const float& other) { this->x /= other; this->y /= other; return *this; }
};
struct bvec3
{
	uint8_t x;
	uint8_t y;
	uint8_t z;

	bool operator==(const bvec3& other) { if (other.x == this->x && other.y == this->y && other.z == this->z) { return true; } return false; }
	bvec3& operator+(const bvec3& other) { this->x += other.x; this->y += other.y; this->z += other.z; return *this; }
	bvec3& operator-(const bvec3& other) { this->x -= other.x; this->y -= other.y; this->z -= other.z; return *this; }
	bvec3& operator*(const bvec3& other) { this->x *= other.x; this->y *= other.y; this->z *= other.z; return *this; }
	bvec3& operator*(const float& other) { this->x *= other; this->y *= other; this->z *= other; return *this; }
	bvec3& operator/(const bvec3& other) { this->x /= other.x; this->y /= other.y; this->z /= other.z; return *this; }
	bvec3& operator/(const float& other) { this->x /= other; this->y /= other; this->z /= other; return *this; }
};
struct bvec4
{
	uint8_t x;
	uint8_t y;
	uint8_t z;
	uint8_t w;

	bool operator==(const bvec4& other) { if (other.x == this->x && other.y == this->y && other.z == this->z && other.w == this->w) { return true; } return false; }
	bvec4& operator+(const bvec4& other) { this->x += other.x; this->y += other.y; this->z += other.z; this->w += other.w; return *this; }
	bvec4& operator-(const bvec4& other) { this->x -= other.x; this->y -= other.y; this->z -= other.z; this->w -= other.w; return *this; }
	bvec4& operator*(const bvec4& other) { this->x *= other.x; this->y *= other.y; this->z *= other.z; this->w *= other.w; return *this; }
	bvec4& operator*(const float& other) { this->x *= other; this->y *= other; this->z *= other; this->w *= other;       return *this; }
	bvec4& operator/(const bvec4& other) { this->x /= other.x; this->y /= other.y; this->z /= other.z; this->w /= other.w; return *this; }
	bvec4& operator/(const float& other) { this->x /= other; this->y /= other; this->z /= other; this->w /= other; return *this; }
};
struct fRec
{
	enum Start
	{
		topleft,
		topright,
		bottomleft,
		bottomright
	};
	fRec(const fvec2& pos,const fvec2& size, Start start = topleft)
	{
		recSize = size;
		if (start == topleft)
		{
			topLeft = pos;
		}
		else if (start == topright)
		{
			topLeft = { pos.x - size.x,  pos.y };
		}
		else if (start == bottomleft)
		{
			topLeft = { pos.x, pos.y - size.y };
		}
		else
		{
			topLeft = { pos.x - size.x, pos.y - size.y };
		}
		topRight = { topLeft.x + size.x, topLeft.y };
		bottomLeft = { topLeft.x, topLeft.y - size.y };
		bottomRight = { topLeft.x + size.x, topLeft.y - size.y };
	}
	fRec()
	{
		recSize = { 0,0 };
		topLeft = { 0,0 };
		topRight = { 0,0 };
		bottomLeft = { 0,0 };
		bottomRight = { 0,0 };
	}

	bool vecInRec(const fvec2& pos)
	{
		if (pos.x > topLeft.x && pos.x < topLeft.x + recSize.x)
		{
			if (pos.y > topLeft.y - recSize.y && pos.y < topLeft.y)
			{
				return true;
			}
		}
		return false;
	}

	fvec2 topLeft;
	fvec2 topRight;
	fvec2 bottomLeft;
	fvec2 bottomRight;
	fvec2 recSize;
};