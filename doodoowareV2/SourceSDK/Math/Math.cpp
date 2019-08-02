#pragma once
#include "Math.h"
#include "../Misc/IVDebugOverlay.h"

Math gMath;

float point_to_line(Vector from, Vector to, Vector Dir)
{
	auto PointDir = to - from;

	//Vector Dir = gMath.AngleToVector()

	auto TempOffset = PointDir.Dot(Dir) / Dir.LengthSqr();
	if (TempOffset < 0.000001f)
		return FLT_MAX;

	auto PerpendicularPoint = from + (Dir * TempOffset);

	return (to - PerpendicularPoint).Length();
}

float Math::GetFOV(const QAngle &_from, const QAngle &_to, Vector dir)
{
	/*QAngle From = _from;
	From -= _to;
	From.ClampAngles();
	return (abs(From.y) + abs(From.x));*/

	return point_to_line(_from, _to, dir);
}

float Math::GetFOV(const QAngle &_from, const QAngle &_to) {
	QAngle From = _from;
	From -= _to;
	From.ClampAngles();
	return (abs(From.y) + abs(From.x));
}

float Math::GetDistanceBasedFOV(const QAngle &_from, const QAngle &_to, float _distance)
{
	Vector fromVec = this->AngleToVector(_from);
	fromVec *= _distance;

	Vector toVec = this->AngleToVector(_to);
	toVec *= _distance;

	return this->GetDistance(fromVec, toVec);
}

float Math::GetDistance(const Vector &_from, const Vector &_to)
{
	return (float)(sqrt(pow((_from.x - _to.x), 2) + pow((_from.y - _to.y), 2) + pow((_from.z - _to.z), 2)));
}

QAngle Math::CalculateAngle(const Vector &_from, const Vector &_to)
{
	Vector delta = _from - _to;
	return this->VectorToAngle(delta);
}

void Math::VectorAngles(const Vector vForward, Vector& vAngle)
{
	float tmp, yaw, pitch;

	if (vForward[1] == 0 && vForward[0] == 0)
	{
		yaw = 0;
		if (vForward[2] > 0)
		{
			pitch = 270;
		}
		else
		{
			pitch = 90;
		}
	}
	else
	{
		yaw = (atan2(vForward[1], vForward[0]) * 180 / M_PI);
		if (yaw < 0)
		{
			yaw += 360;
		}
		tmp = sqrt(vForward[0] * vForward[0] + vForward[1] * vForward[1]);
		pitch = (atan2(-vForward[2], tmp) * 180 / M_PI);

		if (pitch < 0)
		{
			pitch += 360;
		}
	}

	vAngle[0] = pitch;
	vAngle[1] = yaw;
	vAngle[2] = 0;
}

QAngle Math::VectorToAngle(const Vector& _vec)
{
	float tmp, yaw, pitch;

	if (!_vec.y && !_vec.x)
	{
		yaw = 0;
		if (_vec.z > 0)
			pitch = 270;
		else
			pitch = 90;
	}
	else
	{
		yaw = (atan2(_vec.y, _vec.x) * 180 / M_PI);

		if (yaw > 90)
			yaw -= 180;
		else if (yaw < 90)
			yaw += 180;
		else if (yaw == 90)
			yaw = 0;

		tmp = sqrt(_vec.x * _vec.x + _vec.y * _vec.y);
		pitch = (atan2(_vec.z, tmp) * 180 / M_PI);
	}
	
	return QAngle(pitch, yaw, 0);
}

Vector Math::AngleToVector(const QAngle &_angle)
{
	float sp, sy, cp, cy;

	this->SinCos(DEG2RAD(_angle.y), &sy, &cy);
	this->SinCos(DEG2RAD(_angle.x), &sp, &cp);
		
	return Vector(cp*cy, cp*sy, -sp);
}
void Math::AngleVectors(const QAngle &angles, Vector *forward, Vector *right, Vector *up)
{
	float sr, sp, sy, cr, cp, cy;

	this->SinCos(DEG2RAD(angles.y), &sy, &cy);
	this->SinCos(DEG2RAD(angles.x), &sp, &cp);
	this->SinCos(DEG2RAD(angles.z), &sr, &cr);

	if (forward)
	{
		forward->x = cp*cy;
		forward->y = cp*sy;
		forward->z = -sp;
	}

	if (right)
	{
		right->x = (-1 * sr*sp*cy + -1 * cr*-sy);
		right->y = (-1 * sr*sp*sy + -1 * cr*cy);
		right->z = -1 * sr*cp;
	}

	if (up)
	{
		up->x = (cr*sp*cy + -sr*-sy);
		up->y = (cr*sp*sy + -sr*cy);
		up->z = cr*cp;
	}
}


void Math::SinCos(float _radians, float* _sine, float* _cosine)
{
	*_sine = sin(_radians);
	*_cosine = cos(_radians);
}

bool Math::WorldToScreen(const Vector &in, Vector &out)
{
	/*if (this->ScreenTransform(in, out))
		return false;

	int screenWidth, screenHeight;
	gInterface.engine->GetScreenSize(screenWidth, screenHeight);

	float x = screenWidth / 2.f;
	float y = screenHeight / 2.f;

	x += 0.5f * out.x * screenWidth + 0.5f;
	y -= 0.5f * out.y * screenHeight + 0.5f;

	out.x = x;
	out.y = y;
	
	return true;*/

	return (gInterface.debug->screen_position(in, out) != 1);
}

bool Math::ScreenTransform(const Vector &in, Vector &out)
{
	/*static auto& w2sMatrix = gInterface.Engine()->WorldToScreenMatrix();

	out.x = w2sMatrix.m[0][0] * in.x + w2sMatrix.m[0][1] * in.y + w2sMatrix.m[0][2] * in.z + w2sMatrix.m[0][3];
	out.y = w2sMatrix.m[1][0] * in.x + w2sMatrix.m[1][1] * in.y + w2sMatrix.m[1][2] * in.z + w2sMatrix.m[1][3];
	out.z = 0.0f;

	float w = w2sMatrix.m[3][0] * in.x + w2sMatrix.m[3][1] * in.y + w2sMatrix.m[3][2] * in.z + w2sMatrix.m[3][3];

	if (w < 0.001f) {

		return false;
	}

	out.x /= w;
	out.y /= w;

	return true;*/

	return this->FrustumTransform(gInterface.engine->WorldToScreenMatrix(), in, out);
}

bool Math::FrustumTransform(const VMatrix &_worldToSurface, const Vector &_point, Vector &_screen)
{
	float w;

	_screen.x = _worldToSurface[0][0] * _point[0] + _worldToSurface[0][1] * _point[1] + _worldToSurface[0][2] * _point[2] + _worldToSurface[0][3];
	_screen.y = _worldToSurface[1][0] * _point[0] + _worldToSurface[1][1] * _point[1] + _worldToSurface[1][2] * _point[2] + _worldToSurface[1][3];

	w = _worldToSurface[3][0] * _point[0] + _worldToSurface[3][1] * _point[1] + _worldToSurface[3][2] * _point[2] + _worldToSurface[3][3];

	_screen.z = 0.0f;

	bool behind;
	
	if (w < 0.001f)
	{
		behind = true;
		_screen.x *= 100000;
		_screen.y *= 100000;
	}
	else
	{
		behind = false;
		float invw = 1.0f / w;
		_screen.x *= invw;
		_screen.y *= invw;
	}

	return behind;
}

void Math::VectorTransform(const Vector& in1, const matrix3x4_t& in2, Vector& out)
{
	out[0] = in1.Dot(in2[0]) + in2[0][3];
	out[1] = in1.Dot(in2[1]) + in2[1][3];
	out[2] = in1.Dot(in2[2]) + in2[2][3];
}

#pragma region VMatrix
//-----------------------------------------------------------------------------
// VMatrix inlines.
//-----------------------------------------------------------------------------
inline VMatrix::VMatrix()
{
}

inline VMatrix::VMatrix(
	float m00, float m01, float m02, float m03,
	float m10, float m11, float m12, float m13,
	float m20, float m21, float m22, float m23,
	float m30, float m31, float m32, float m33)
{
	Init(
		m00, m01, m02, m03,
		m10, m11, m12, m13,
		m20, m21, m22, m23,
		m30, m31, m32, m33
	);
}


inline VMatrix::VMatrix(const matrix3x4_t& matrix3x4)
{
	Init(matrix3x4);
}


//-----------------------------------------------------------------------------
// Creates a matrix where the X axis = forward
// the Y axis = left, and the Z axis = up
//-----------------------------------------------------------------------------
inline VMatrix::VMatrix(const Vector& xAxis, const Vector& yAxis, const Vector& zAxis)
{
	Init(
		xAxis.x, yAxis.x, zAxis.x, 0.0f,
		xAxis.y, yAxis.y, zAxis.y, 0.0f,
		xAxis.z, yAxis.z, zAxis.z, 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f
	);
}


inline void VMatrix::Init(
	float m00, float m01, float m02, float m03,
	float m10, float m11, float m12, float m13,
	float m20, float m21, float m22, float m23,
	float m30, float m31, float m32, float m33
)
{
	m[0][0] = m00;
	m[0][1] = m01;
	m[0][2] = m02;
	m[0][3] = m03;

	m[1][0] = m10;
	m[1][1] = m11;
	m[1][2] = m12;
	m[1][3] = m13;

	m[2][0] = m20;
	m[2][1] = m21;
	m[2][2] = m22;
	m[2][3] = m23;

	m[3][0] = m30;
	m[3][1] = m31;
	m[3][2] = m32;
	m[3][3] = m33;
}


//-----------------------------------------------------------------------------
// Initialize from a 3x4
//-----------------------------------------------------------------------------
inline void VMatrix::Init(const matrix3x4_t& matrix3x4)
{
	memcpy(m, matrix3x4.Base(), sizeof(matrix3x4_t));

	m[3][0] = 0.0f;
	m[3][1] = 0.0f;
	m[3][2] = 0.0f;
	m[3][3] = 1.0f;
}

//-----------------------------------------------------------------------------
// Vector3DMultiplyPosition treats src2 as if it's a point (adds the translation)
//-----------------------------------------------------------------------------
// NJS: src2 is passed in as a full vector rather than a reference to prevent the need
// for 2 branches and a potential copy in the body.  (ie, handling the case when the src2
// reference is the same as the dst reference ).
inline void Vector3DMultiplyPosition(const VMatrix& src1, const Vector& src2, Vector& dst)
{
	dst[0] = src1[0][0] * src2.x + src1[0][1] * src2.y + src1[0][2] * src2.z + src1[0][3];
	dst[1] = src1[1][0] * src2.x + src1[1][1] * src2.y + src1[1][2] * src2.z + src1[1][3];
	dst[2] = src1[2][0] * src2.x + src1[2][1] * src2.y + src1[2][2] * src2.z + src1[2][3];
}

//-----------------------------------------------------------------------------
// Methods related to the basis vectors of the matrix
//-----------------------------------------------------------------------------

inline Vector VMatrix::GetForward() const
{
	return Vector(m[0][0], m[1][0], m[2][0]);
}

inline Vector VMatrix::GetLeft() const
{
	return Vector(m[0][1], m[1][1], m[2][1]);
}

inline Vector VMatrix::GetUp() const
{
	return Vector(m[0][2], m[1][2], m[2][2]);
}

inline void VMatrix::SetForward(const Vector &vForward)
{
	m[0][0] = vForward.x;
	m[1][0] = vForward.y;
	m[2][0] = vForward.z;
}

inline void VMatrix::SetLeft(const Vector &vLeft)
{
	m[0][1] = vLeft.x;
	m[1][1] = vLeft.y;
	m[2][1] = vLeft.z;
}

inline void VMatrix::SetUp(const Vector &vUp)
{
	m[0][2] = vUp.x;
	m[1][2] = vUp.y;
	m[2][2] = vUp.z;
}

inline void VMatrix::GetBasisVectors(Vector &vForward, Vector &vLeft, Vector &vUp) const
{
	vForward.Init(m[0][0], m[1][0], m[2][0]);
	vLeft.Init(m[0][1], m[1][1], m[2][1]);
	vUp.Init(m[0][2], m[1][2], m[2][2]);
}

inline void VMatrix::SetBasisVectors(const Vector &vForward, const Vector &vLeft, const Vector &vUp)
{
	SetForward(vForward);
	SetLeft(vLeft);
	SetUp(vUp);
}


//-----------------------------------------------------------------------------
// Methods related to the translation component of the matrix
//-----------------------------------------------------------------------------

inline Vector VMatrix::GetTranslation() const
{
	return Vector(m[0][3], m[1][3], m[2][3]);
}

inline Vector& VMatrix::GetTranslation(Vector &vTrans) const
{
	vTrans.x = m[0][3];
	vTrans.y = m[1][3];
	vTrans.z = m[2][3];
	return vTrans;
}

inline void VMatrix::SetTranslation(const Vector &vTrans)
{
	m[0][3] = vTrans.x;
	m[1][3] = vTrans.y;
	m[2][3] = vTrans.z;
}


//-----------------------------------------------------------------------------
// appply translation to this matrix in the input space
//-----------------------------------------------------------------------------
inline void VMatrix::PreTranslate(const Vector &vTrans)
{
	Vector tmp;
	Vector3DMultiplyPosition(*this, vTrans, tmp);
	m[0][3] = tmp.x;
	m[1][3] = tmp.y;
	m[2][3] = tmp.z;
}


//-----------------------------------------------------------------------------
// appply translation to this matrix in the output space
//-----------------------------------------------------------------------------
inline void VMatrix::PostTranslate(const Vector &vTrans)
{
	m[0][3] += vTrans.x;
	m[1][3] += vTrans.y;
	m[2][3] += vTrans.z;
}

inline const matrix3x4_t& VMatrix::As3x4() const
{
	return *((const matrix3x4_t*)this);
}

inline matrix3x4_t& VMatrix::As3x4()
{
	return *((matrix3x4_t*)this);
}

inline void VMatrix::CopyFrom3x4(const matrix3x4_t &m3x4)
{
	memcpy(m, m3x4.Base(), sizeof(matrix3x4_t));
	m[3][0] = m[3][1] = m[3][2] = 0;
	m[3][3] = 1;
}

inline void VMatrix::Set3x4(matrix3x4_t& matrix3x4) const
{
	memcpy(matrix3x4.Base(), m, sizeof(matrix3x4_t));
}


//-----------------------------------------------------------------------------
// Matrix Math operations
//-----------------------------------------------------------------------------
inline const VMatrix& VMatrix::operator+=(const VMatrix &other)
{
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			m[i][j] += other.m[i][j];
		}
	}

	return *this;
}

inline VMatrix VMatrix::operator+(const VMatrix &other) const
{
	VMatrix ret;
	for (int i = 0; i < 16; i++) {
		((float*)ret.m)[i] = ((float*)m)[i] + ((float*)other.m)[i];
	}
	return ret;
}

inline VMatrix VMatrix::operator-(const VMatrix &other) const
{
	VMatrix ret;

	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			ret.m[i][j] = m[i][j] - other.m[i][j];
		}
	}

	return ret;
}

inline VMatrix VMatrix::operator-() const
{
	VMatrix ret;
	for (int i = 0; i < 16; i++) {
		((float*)ret.m)[i] = -((float*)m)[i];
	}
	return ret;
}

//-----------------------------------------------------------------------------
// Vector transformation
//-----------------------------------------------------------------------------


inline Vector VMatrix::operator*(const Vector &vVec) const
{
	Vector vRet;
	vRet.x = m[0][0] * vVec.x + m[0][1] * vVec.y + m[0][2] * vVec.z + m[0][3];
	vRet.y = m[1][0] * vVec.x + m[1][1] * vVec.y + m[1][2] * vVec.z + m[1][3];
	vRet.z = m[2][0] * vVec.x + m[2][1] * vVec.y + m[2][2] * vVec.z + m[2][3];

	return vRet;
}

inline Vector VMatrix::VMul4x3(const Vector &vVec) const
{
	Vector vResult;
	Vector3DMultiplyPosition(*this, vVec, vResult);
	return vResult;
}


inline Vector VMatrix::VMul4x3Transpose(const Vector &vVec) const
{
	Vector tmp = vVec;
	tmp.x -= m[0][3];
	tmp.y -= m[1][3];
	tmp.z -= m[2][3];

	return Vector(
		m[0][0] * tmp.x + m[1][0] * tmp.y + m[2][0] * tmp.z,
		m[0][1] * tmp.x + m[1][1] * tmp.y + m[2][1] * tmp.z,
		m[0][2] * tmp.x + m[1][2] * tmp.y + m[2][2] * tmp.z
	);
}

inline Vector VMatrix::VMul3x3(const Vector &vVec) const
{
	return Vector(
		m[0][0] * vVec.x + m[0][1] * vVec.y + m[0][2] * vVec.z,
		m[1][0] * vVec.x + m[1][1] * vVec.y + m[1][2] * vVec.z,
		m[2][0] * vVec.x + m[2][1] * vVec.y + m[2][2] * vVec.z
	);
}

inline Vector VMatrix::VMul3x3Transpose(const Vector &vVec) const
{
	return Vector(
		m[0][0] * vVec.x + m[1][0] * vVec.y + m[2][0] * vVec.z,
		m[0][1] * vVec.x + m[1][1] * vVec.y + m[2][1] * vVec.z,
		m[0][2] * vVec.x + m[1][2] * vVec.y + m[2][2] * vVec.z
	);
}


inline void VMatrix::V3Mul(const Vector &vIn, Vector &vOut) const
{
	float rw;

	rw = 1.0f / (m[3][0] * vIn.x + m[3][1] * vIn.y + m[3][2] * vIn.z + m[3][3]);
	vOut.x = (m[0][0] * vIn.x + m[0][1] * vIn.y + m[0][2] * vIn.z + m[0][3]) * rw;
	vOut.y = (m[1][0] * vIn.x + m[1][1] * vIn.y + m[1][2] * vIn.z + m[1][3]) * rw;
	vOut.z = (m[2][0] * vIn.x + m[2][1] * vIn.y + m[2][2] * vIn.z + m[2][3]) * rw;
}

//-----------------------------------------------------------------------------
// Other random stuff
//-----------------------------------------------------------------------------
inline void VMatrix::Identity()
{
	m[0][0] = 1.0f; m[0][1] = 0.0f; m[0][2] = 0.0f; m[0][3] = 0.0f;
	m[1][0] = 0.0f; m[1][1] = 1.0f; m[1][2] = 0.0f; m[1][3] = 0.0f;
	m[2][0] = 0.0f; m[2][1] = 0.0f; m[2][2] = 1.0f; m[2][3] = 0.0f;
	m[3][0] = 0.0f; m[3][1] = 0.0f; m[3][2] = 0.0f; m[3][3] = 1.0f;
}


inline bool VMatrix::IsIdentity() const
{
	return
		m[0][0] == 1.0f && m[0][1] == 0.0f && m[0][2] == 0.0f && m[0][3] == 0.0f &&
		m[1][0] == 0.0f && m[1][1] == 1.0f && m[1][2] == 0.0f && m[1][3] == 0.0f &&
		m[2][0] == 0.0f && m[2][1] == 0.0f && m[2][2] == 1.0f && m[2][3] == 0.0f &&
		m[3][0] == 0.0f && m[3][1] == 0.0f && m[3][2] == 0.0f && m[3][3] == 1.0f;
}

inline Vector VMatrix::ApplyRotation(const Vector &vVec) const
{
	return VMul3x3(vVec);
}
#pragma endregion

