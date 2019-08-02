#pragma once
#include "Vector.h"
#include "VMatrix.h"
#include "..\Interfaces\Engine.h"

#define M_PI		3.14159265358979323846
#define RAD2DEG( x  )  ( (float)(x) * (float)(180.f / (float)M_PI) )
#define DEG2RAD( x  )  ( (float)(x) * (float)((float)M_PI / 180.f) )

class Math
{
public:
	float GetFOV(const QAngle &_from, const QAngle &_to);
	float GetFOV(const QAngle & _from, const QAngle & _to, Vector dir);
	float GetDistanceBasedFOV(const QAngle &_from, const QAngle &_to, float _distance);
	float GetDistance(const Vector &_from, const Vector &_to);
	QAngle CalculateAngle(const Vector &_from, const Vector &_to);
	void VectorAngles(const Vector vForward, Vector & vAngle);
	QAngle VectorToAngle(const Vector &_vec);
	Vector AngleToVector(const QAngle &_angle);
	void AngleVectors(const QAngle & angles, Vector * forward, Vector * right, Vector * up);
	bool WorldToScreen(const Vector &_origin, Vector &_screen);
	//float DotProduct(const Vector & a, const Vector & b);
	void VectorTransform(const Vector & in1, const matrix3x4_t & in2, Vector & out);
private:
	void SinCos(float _radians, float* _sine, float* _cosine);
	bool ScreenTransform(const Vector &_point, Vector &_screen);
	bool FrustumTransform(const VMatrix &_worldToSurface, const Vector &_point, Vector &_screen);
	//float DotProduct(const float & a, const float & b);
	//void VectorTransform(const float & in1, const matrix3x4_t & in2, float & out);
};

extern Math gMath;