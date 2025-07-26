#pragma once

template <typename T>
class _Vector2
{
public:
	T x, y;

	inline bool operator==(const _Vector2<T>& v) const
	{
		return x == v.x && y == v.y;
	}

	inline bool operator!=(const _Vector2<T>& v) const
	{
		return x != v.x || y != v.y;
	}

	inline _Vector2<T> operator+(const _Vector2<T>& v) const
	{
		return { x + v.x, y + v.y };
	}

	inline _Vector2<T> operator-(const _Vector2<T>& v) const
	{
		return { x - v.x, y - v.y };
	}

	inline _Vector2<T> operator*(const _Vector2<T>& v) const
	{
		return { x * v.x, y * v.y };
	}

	inline _Vector2<T> operator/(const _Vector2<T>& v) const
	{
		return { x / v.x, y / v.y };
	}

	inline void operator+=(const _Vector2<T>& v)
	{
		x += v.x;
		y += v.y;
	}

	inline void operator-=(const _Vector2<T>& v)
	{
		x -= v.x;
		y -= v.y;
	}

	inline void operator*=(const _Vector2<T>& v)
	{
		x *= v.x;
		y *= v.y;
	}

	inline void operator/=(const _Vector2<T>& v)
	{
		x /= v.x;
		y /= v.y;
	}

	std::string dump()
	{
		return "x: " + std::to_string(x) + " y: " + std::to_string(y);
	}
};

template <typename T>
class _Vector3
{
public:
	T x, y, z;

	inline bool operator==(const _Vector3<T>& v) const
	{
		return x == v.x && y == v.y && z == v.z;
	}

	inline bool operator!=(const _Vector3<T>& v) const
	{
		return x != v.x || y != v.y || z != v.z;
	}

	inline _Vector3<T> operator+(const _Vector3<T>& v) const
	{
		return { x + v.x, y + v.y, z - v.z };
	}

	inline _Vector3<T> operator-(const _Vector3<T>& v) const
	{
		return { x - v.x, y - v.y, z - v.z };
	}

	inline _Vector3<T> operator*(const _Vector3<T>& v) const
	{
		return { x * v.x, y * v.y, z * v.z };
	}

	inline _Vector3<T> operator/(const _Vector3<T>& v) const
	{
		return { x / v.x, y / v.y, z / v.z };
	}

	inline void operator+=(const _Vector3<T>& v)
	{
		x += v.x;
		y += v.y;
		z += v.z;
	}

	inline void operator-=(const _Vector3<T>& v)
	{
		x -= v.x;
		y -= v.y;
		z -= v.z;
	}

	inline void operator*=(const _Vector3<T>& v)
	{
		x *= v.x;
		y *= v.y;
		z *= v.z;
	}

	inline void operator/=(const _Vector3<T>& v)
	{
		x /= v.x;
		y /= v.y;
		z /= v.z;
	}

	std::string dump()
	{
		return "x: " + std::to_string(x) + " y: " + std::to_string(y) + " z: " + std::to_string(z);
	}
};

template <typename T>
class _Vector4
{
public:
	T x, y, z, w;

	inline bool operator ==(const _Vector4<T>& v) const
	{
		return x == v.x
			&& y == v.y
			&& z == v.z
			&& w == v.w;
	}

	inline void operator +=(const _Vector4<T>& v) const
	{
		x += v.x;
		y += v.y;
		z += v.z;
		w += v.w;
	}

	std::string dump()
	{
		return "x: " + std::to_string(x) + " y: " + std::to_string(y) + " z: " + std::to_string(z) + " w: " + std::to_string(w);
	}
};

using Vector2 = _Vector2<float>;
using Vector = _Vector3<float>;
using Vector4 = _Vector4<float>;