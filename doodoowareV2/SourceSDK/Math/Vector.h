#pragma once
#include <math.h>

typedef class Vector
{
public:
	float x, y, z;
	Vector();
	Vector(float X, float Y, float Z);
	Vector(float val);
	void Init(float ix, float iy, float iz);
	float Length2D() const
	{
		return sqrt(x*x + y*y);
	}
	__forceinline void Normalize();
	__forceinline float LengthSqr();
	__forceinline float Length();
	__forceinline float Dot(const Vector & vec) const;
	__forceinline void ClampAngles();
	__forceinline bool IsZero();
	__forceinline float DistTo(const Vector & vOther) const;
public:
	Vector(const float* clr)
	{
		x = clr[0];
		y = clr[1];
		z = clr[2];
	}

	float& operator[](int i)
	{
		return ((float*)this)[i];
	}
	float operator[](int i) const
	{
		return ((float*)this)[i];
	}

	__forceinline operator bool() const;
	__forceinline bool operator!() const;
	__forceinline Vector operator-();
	__forceinline bool operator==(const Vector& v);
	__forceinline bool operator==(const Vector& v) const;
	__forceinline bool operator!=(const Vector& v);
	__forceinline bool operator!=(const Vector& v) const;
	__forceinline Vector& operator-=(const Vector &v);
	__forceinline Vector& operator*=(const float n);
	__forceinline Vector operator-(const Vector& v) const;
	__forceinline Vector operator+(const Vector& v) const;
	__forceinline Vector operator*(float n) const;
	__forceinline Vector operator/(const float n);
	__forceinline Vector operator/(const Vector& v);
} QAngle;

inline Vector::Vector()
{
	x = 0.f;
	y = 0.f;
	z = 0.f;
}

inline Vector::Vector(float X, float Y, float Z)
{
	x = X;
	y = Y;
	z = Z;
}

inline Vector::Vector(float val)
{
	x = val;
	y = val;
	z = val;
}

inline void Vector::Init(float ix, float iy, float iz)
{
	x = ix;
	y = iy;
	z = iz;
}


__forceinline void Vector::Normalize()
{
	float length = Length();
	if (length)
	{
		x = x / length;
		y = y / length;
		z = z / length;
	}
	else
		x = y = 0.0f; z = 1.0f;
}

__forceinline float Vector::LengthSqr()
{
	return (x*x + y*y + z*z);
}

__forceinline float Vector::Length()
{
	return (float)sqrt(LengthSqr());
}

__forceinline float Vector::Dot(const Vector& vec) const
{
	return (x * vec.x + y * vec.y + z * vec.z);
}

__forceinline void Vector::ClampAngles()
{
	if (this->x < -89.0f)
		this->x = -89.0f;

	if (this->x >  89.0f)
		this->x = 89.0f;

	while (this->y < -180.0f)
		this->y += 360.0f;

	while (this->y >  180.0f)
		this->y -= 360.0f;

	this->z = 0.0f;
}

__forceinline bool Vector::IsZero()
{
	return (x < 0.01f && x > -0.01f && y < 0.01f && y > -0.01f);
}

__forceinline float Vector::DistTo(const Vector &vOther) const
{
	Vector delta;

	delta.x = x - vOther.x;
	delta.y = y - vOther.y;
	delta.z = z - vOther.z;

	return delta.Length();
}

__forceinline Vector::operator bool() const
{
	return (x != 0.f && y != 0.f && z != 0.f);
}

__forceinline bool Vector::operator!() const
{
	return (!x && !y && !z);
}

__forceinline Vector Vector::operator-()
{
	return Vector(-x, -y, -z);
}

__forceinline bool Vector::operator==(const Vector& v)
{
	return (x == v.x) && (y == v.y) && (z == v.z);
}

__forceinline bool Vector::operator==(const Vector& v) const
{
	return (x == v.x) && (y == v.y) && (z == v.z);
}

__forceinline bool Vector::operator!=(const Vector& v)
{
	return (x != v.x) || (y != v.y) || (z != v.z);
}

__forceinline bool Vector::operator!=(const Vector& v) const
{
	return (x != v.x) || (y != v.y) || (z != v.z);
}

__forceinline Vector& Vector::operator-=(const Vector &v)
{
	x -= v.x;
	y -= v.y;
	z -= v.z; 
	return *this;
}

__forceinline Vector& Vector::operator*=(const float n)
{
	return Vector(x *= n, y *= n, z *= n);
}

__forceinline Vector Vector::operator-(const Vector& v) const
{
	return Vector(x - v.x, y - v.y, z - v.z);
}

__forceinline Vector Vector::operator+(const Vector& v)  const
{
	return Vector(x + v.x, y + v.y, z + v.z);
}

__forceinline Vector Vector::operator*(float n) const
{
	return Vector(x * n, y * n, z * n);
}

__forceinline Vector Vector::operator/(const float n)
{
	return Vector(x / n, y / n, z / n);
}

__forceinline Vector Vector::operator/(const Vector& v) 
{
	return Vector(x / v.x, y / v.y, z / v.z); 
}

typedef float vec_t;
// 2D Vector
class Vector2D
{
public:
	// Members
	vec_t x, y;

	// Construction/destruction:
	Vector2D(void);
	Vector2D(vec_t X, vec_t Y);
	Vector2D(vec_t* clr);

	Vector2D(const Vector2D &vOther)
	{
		x = vOther.x; y = vOther.y;
	}

	// Initialization
	void Init(vec_t ix = 0.0f, vec_t iy = 0.0f);
	// TODO (Ilya): Should there be an init that takes a single float for consistency?

	// Got any nasty NAN's?
	bool IsValid() const;
	void Invalidate();

	// array access...
	vec_t operator[](int i) const;
	vec_t& operator[](int i);

	// Base address...
	vec_t* Base();
	vec_t const* Base() const;

	// Initialization methods
	void Random(vec_t minVal, vec_t maxVal);
	void Zero(); ///< zero out a vector

				 // equality
	bool operator==(const Vector2D& v) const;
	bool operator!=(const Vector2D& v) const;

	// arithmetic operations
	Vector2D& operator+=(const Vector2D& v)
	{
		x += v.x; y += v.y;
		return *this;
	}

	Vector2D& operator-=(const Vector2D& v)
	{
		x -= v.x; y -= v.y;
		return *this;
	}

	Vector2D& operator*=(float fl)
	{
		x *= fl;
		y *= fl;
		return *this;
	}

	Vector2D& operator*=(const Vector2D& v)
	{
		x *= v.x;
		y *= v.y;
		return *this;
	}

	Vector2D& operator/=(const Vector2D& v)
	{
		x /= v.x;
		y /= v.y;
		return *this;
	}

	// this ought to be an opcode.
	Vector2D& operator+=(float fl)
	{
		x += fl;
		y += fl;
		return *this;
	}

	// this ought to be an opcode.
	Vector2D& operator/=(float fl)
	{
		x /= fl;
		y /= fl;
		return *this;
	}
	Vector2D& operator-=(float fl)
	{
		x -= fl;
		y -= fl;
		return *this;
	}

	// negate the vector components
	void Negate();

	// Get the vector's magnitude.
	vec_t Length() const;

	// Get the vector's magnitude squared.
	vec_t LengthSqr(void) const
	{
		return (x*x + y*y);
	}

	// return true if this vector is (0,0,0) within tolerance
	bool IsZero(float tolerance = 0.01f) const
	{
		return (x > -tolerance && x < tolerance &&
			y > -tolerance && y < tolerance);
	}

	vec_t NormalizeInPlace();
	Vector2D Normalized() const;
	bool IsLengthGreaterThan(float val) const;
	bool IsLengthLessThan(float val) const;

	// check if a vector is within the box defined by two other vectors
	bool WithinAABox(Vector2D const &boxmin, Vector2D const &boxmax);

	// Get the distance from this vector to the other one.
	vec_t DistTo(const Vector2D &vOther) const;

	// Get the distance from this vector to the other one squared.
	// NJS: note, VC wasn't inlining it correctly in several deeply nested inlines due to being an 'out of line' .  
	// may be able to tidy this up after switching to VC7
	vec_t DistToSqr(const Vector2D &vOther) const
	{
		Vector2D delta;

		delta.x = x - vOther.x;
		delta.y = y - vOther.y;

		return delta.LengthSqr();
	}

	// Copy
	void CopyToArray(float* rgfl) const;

	// Multiply, add, and assign to this (ie: *this = a + b * scalar). This
	// is about 12% faster than the actual vector equation (because it's done per-component
	// rather than per-vector).
	void MulAdd(const Vector2D& a, const Vector2D& b, float scalar);

	// Dot product.
	vec_t Dot(const Vector2D& vOther) const;

	// assignment
	Vector2D& operator=(const Vector2D &vOther);

	// 2d
	vec_t Length2D(void) const;
	vec_t Length2DSqr(void) const;

	/// Get the component of this vector parallel to some other given vector
	Vector2D  ProjectOnto(const Vector2D& onto);

	// copy constructors
	// Vector2D(const Vector2D &vOther);

	// arithmetic operations
	Vector2D operator-(void) const;

	Vector2D operator+(const Vector2D& v) const;
	Vector2D operator-(const Vector2D& v) const;
	Vector2D operator*(const Vector2D& v) const;
	Vector2D operator/(const Vector2D& v) const;
	Vector2D operator*(float fl) const;
	Vector2D operator/(float fl) const;

	// Cross product between two vectors.
	Vector2D Cross(const Vector2D &vOther) const;

	// Returns a vector with the min or max in X, Y, and Z.
	Vector2D Min(const Vector2D &vOther) const;
	Vector2D Max(const Vector2D &vOther) const;
};