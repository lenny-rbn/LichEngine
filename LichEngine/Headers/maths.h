#pragma once
#include <Define.h>
#define _USE_MATH_DEFINES 
#include <math.h>
#include <cmath>
#include <stdint.h>
#include <string>
#include <iostream>
#include <ImGUI/imgui.h>
#include <irrKlang/irrKlang.h>
#include <Generated/maths.rfkh.h>


namespace EditorUI
{
	typedef ImVec2 UiVec2D;
	typedef ImVec4 UiVec4D;
}

namespace AudioAPI
{
	typedef irrklang::vec3df SoundVec3;
}

namespace Physics
{
	class Transform;
}

#define DEG2RAD(angleInDegrees) ((angleInDegrees) * (float)(M_PI / 180.0))
#define RAD2DEG(angleInRadians) ((angleInRadians) * (float)(180.0 / M_PI))

class CLASS() LICH_ENGINE Vector2D
{
public:
	union
	{
		struct
		{
			float x, y;
		};

		float coord[2];
	};

	Vector2D();
	Vector2D(int number);
	Vector2D(int x, int y);
	Vector2D(float x, float y);
	~Vector2D();

	const static Vector2D zero;
	const static Vector2D one;
	const static Vector2D up;
	const static Vector2D left;
	const static Vector2D down;
	const static Vector2D right;

	void Normalize(); // Get the direction of vector with two point

	float Magnitude() const;
	float DotProduct(Vector2D vector);
	float Determinant(Vector2D vector);

	Vector2D Normal();
	Vector2D GetDirection(Vector2D vector);
	std::string ToString();
	void FromString(std::string data);

	bool operator!=(Vector2D const vector);
	bool operator==(Vector2D const vector);
	bool operator==(Vector2D const vector) const;

	const Vector2D operator*(float const k) const;

	Vector2D operator-();
	Vector2D operator/(float const k);
	Vector2D operator*(float const k);
	Vector2D operator/=(float const k);
	Vector2D operator+(Vector2D const vector);
	Vector2D operator-(Vector2D const vector) const;
	Vector2D operator+=(Vector2D const vector);
	Vector2D operator-=(Vector2D const vector);

	operator EditorUI::UiVec2D() const { return EditorUI::UiVec2D(x,y); }

	Vector2D_GENERATED
};

////////////////////////////////////////////////////////////////////////////////////////////////////

class CLASS() LICH_ENGINE Vector3D
{
public:
	union
	{
		struct
		{
			float x, y, z;
		};

		float coord[3];
	};

	Vector3D();
	Vector3D(const float k);
	Vector3D(const float c1, const float c2, const float c3);
	~Vector3D();

	const static Vector3D zero;
	const static Vector3D one;
	const static Vector3D up;
	const static Vector3D left;
	const static Vector3D down;
	const static Vector3D right;
	const static Vector3D forward;
	const static Vector3D back;
	const static Vector3D x0z;

	Vector3D ConvertVector3ToRadians(Vector3D vec);
	Vector3D ConvertRadVec3ToDegree(Vector3D vec);
	void Normalize();
	void ToString(const float* coord, const char* name);
	Vector3D ToInt();
	std::string ToString();
	void FromString(std::string data);
	static Vector3D GetFromString(std::string data);
	static Vector3D Normalize(Vector3D vector);

	float Magnitude() const;

	bool operator==(const Vector3D vector);
	bool operator==(Vector3D const vector) const;


	Vector3D operator*(const float k);
	Vector3D operator*(const Vector3D vector);
	Vector3D operator*=(float const k);
	Vector3D operator/=(const float k);
	Vector3D operator*(const float k) const;
	friend Vector3D operator+(Vector3D lhs, Vector3D rhs);
	Vector3D operator-(const Vector3D vector);
	Vector3D operator+=(const Vector3D vector);

	operator AudioAPI::SoundVec3() const { return AudioAPI::SoundVec3(x,y,z); }
	Vector3D_GENERATED
};

Vector3D operator+(Vector3D lhs, Vector3D rhs);

////////////////////////////////////////////////////////////////////////////////////////////////////

union LICH_ENGINE Coord4
{
	struct
	{
		float x, y, z, w;
	};

	float coord[4];
};

////////////////////////////////////////////////////////////////////////////////////////////////////

class CLASS() LICH_ENGINE Vector4D
{
public:
	union
	{
		struct
		{
			float x, y, z, w;
		};

		float coord[4];
	};

	Vector4D();
	Vector4D(const Vector3D & vec3, float w = 1.f);
	Vector4D(float x, float y, float z, float w = 1.f);
	~Vector4D();

	const static Vector4D zero;
	const static Vector4D one;
	const static Vector4D up;
	const static Vector4D left;
	const static Vector4D down;
	const static Vector4D right;
	const static Vector4D forward;
	const static Vector4D back;

	void Normalize();
	void Homogenize();
	float Magnitude() const;
	std::string ToString();
	void FromString(std::string data);
	static Vector4D GetFromString(std::string data);

	bool operator==(const Vector4D vector);
	bool operator!=(const Vector4D vector);

	Vector4D operator*(float const k);
	Vector4D operator/=(float const k);
	Vector4D operator+(const Vector4D vector);
	Vector4D operator-(const Vector4D vector);
	Vector4D& operator=(const Vector3D vector);

	operator Vector3D() const { return Vector3D(x,y,z); }
	operator EditorUI::UiVec4D() const { return EditorUI::UiVec4D(x,y,z,w); }
	Vector4D_GENERATED
};

using Color = Vector4D;

////////////////////////////////////////////////////////////////////////////////////////////////////

class LICH_ENGINE Quaternion
{
public:
	union
	{
		struct
		{
			float scalar;
			Vector3D imaginaries;
		};

		struct
		{
			float w, x, y, z;
		};

		float coord[4];
	};

	const static Quaternion Identity;

	Quaternion();
	Quaternion(float s, Vector3D vector);
	Quaternion(float w, float x, float y, float z);
	~Quaternion();

	void ToString(const float* coord, const char* name);

	void Inverse();
	void Normalize();
	void Conjugate();
	void GetAngleAxis(Vector3D& axis, float& angle, bool normalized);

	float GetNorm();
	float DotProduct(const Quaternion quat) const;

	static Vector3D GetEulerAnglesFromQuaternion(Quaternion quat);
	static Quaternion GetQuaternionFromEulerAngles(Vector3D euler); 
	static Quaternion AngleAxisToQuat(Vector3D axis, double angle);

	Vector3D AngleAxis(Vector3D vector, float angle, Vector3D axis);

	Quaternion GetInversedQuat();
	Quaternion GetNormalizedQuat();
	Quaternion GetConjugatedQuat();
	Quaternion ConvertZYXToXYZ();

	void operator*=(float k);
	void operator*=(const Quaternion quat);
	void operator+=(const Quaternion quat);
	void operator-=(const Quaternion quat);

	bool operator==(const Quaternion quat);

	Vector3D operator*(Vector3D vector);

	Quaternion operator*(float k);
	Quaternion operator*(float k) const;
	Quaternion operator*(const Quaternion quat);
	Quaternion operator+(const Quaternion quat);
	Quaternion operator-(const Quaternion quat);
};

////////////////////////////////////////////////////////////////////////////////////////////////////

class LICH_ENGINE Matrix4D // MATRICES ARE IN RAW MAJOR
{
public:
	union
	{
		float e[16];
		float coord[4][4];
	};

	Matrix4D();
	Matrix4D(float a);
	Matrix4D(float tab[4][4]);
	Matrix4D(float tab[16]);
	Matrix4D(float a, float b, float c, float d, float e, float f, float g, float h, float i, float j, float k, float l, float m, float n, float o, float p);
	~Matrix4D();

	void Identity();
	void ToString(const float* coord, const char* name);

	static int MatrixSignElement(int index);

	static float Determinant2x2(Matrix4D matrix);
	static float Determinant3x3(Matrix4D matrix);
	static float Determinant4x4(Matrix4D matrix);
	static Matrix4D Mat3x3ToMat2x2(int row, int columns, Matrix4D matrix);
	static Matrix4D Mat4x4ToMat3x3(int row, int columns, Matrix4D matrix);

	static Vector3D GetScaleFromMatrix(Matrix4D mat);
	static Vector3D	GetPositionFromMatrix(Matrix4D mat);
	static Vector3D	GetRotationFromMatrix(Matrix4D mat);

	static Quaternion GetQuaternionFromMatrix(Matrix4D mat);
	static Matrix4D GetMatrixFromQuaternion(Quaternion quat);
	static Physics::Transform GetTransformFromMatrix(Matrix4D mat, Physics::Transform& transform);

	static Matrix4D InverseMatrix(Matrix4D matrix);
	static Matrix4D MatrixCofacteur(Matrix4D matrix);
	static Matrix4D TransposeMatrix(Matrix4D matrix);
	static Matrix4D LookAt(Vector3D eye, Vector3D at, Vector3D up);
	static Matrix4D ProjectionMatrix(const float fov, const float aspectRatio, const float nearValue, const float farValue);
	static Matrix4D OrthographicMatrix(float left, float right, float top, float bottom, float nearValue, float farValue);
	static Matrix4D OrthographicMatrix(float left, float right, float bottom, float top);

	static Matrix4D CreateXRotationMatrix(float angle);
	static Matrix4D CreateYRotationMatrix(float angle);
	static Matrix4D CreateZRotationMatrix(float angle);
	static Matrix4D CreateRotationMatrix(Vector3D angle);
	static Matrix4D CreateScaleMatrix(const Vector3D& scale);

	static Matrix4D CreateTranslationMatrix(const Vector3D& translation);
	static Matrix4D CreateTransformMatrix(Physics::Transform& trans);
	static Matrix4D CreateTransformMatrix(const Vector3D position, const Quaternion rotation, const Vector3D scale = Vector3D(1, 1, 1));
	static Matrix4D CreateTransformMatrix(const Vector3D position, const Vector3D rotation, const Vector3D scale = Vector3D(1, 1, 1));

	float operator[](const unsigned int index);

	Vector3D operator*(Vector3D vector);
	Vector4D operator*(Vector4D vector);

	Matrix4D operator+(const Matrix4D matrix);
	Matrix4D operator-(const Matrix4D matrix);
	Matrix4D operator*(const Matrix4D matrix);
	bool operator==(const Matrix4D matrix);
	bool operator!=(const Matrix4D matrix);
};

////////////////////////////////////////////////////////////////////////////////////////////////////

class LICH_ENGINE Matrix3D // MATRICES ARE IN RAW MAJOR
{
public:
	union
	{
		float e[9];
		float coord[3][3];
	};

	Matrix3D();
	Matrix3D(Matrix4D mat);
	~Matrix3D();

	void ToString(const float* coord, const char* name);
};

////////////////////////////////////////////////////////////////////////////////////////////////////

template <typename T>
inline T Min(T x, T y);

template <typename T>
inline T Max(T x, T y);

template <typename T>
inline T Sign(T x);

template <typename T>
inline T Absolute(T x);

//////////////////////////////

template <typename T>
T Lerp(T start, T end, const float& t);
float Lerp(const float& start, const float& end, const float& t);
Vector2D Lerp(const Vector2D& start, const Vector2D& end, const float& t);
Quaternion Slerp(const Quaternion& start, const Quaternion& end, const float& t);

//////////////////////////////

LAPI int Clamp(int number, int min, int max);
float Clamp(float number, float min, float max);

Vector3D ClampRotation(const Vector3D& rotation);

//////////////////////////////

float DotProduct2D(Vector2D vecA, Vector2D vecB);
float DotProduct3D(Vector3D vecA, Vector3D vecB);

//////////////////////////////

float GetAngle(Vector2D vecA, Vector2D vecB);       //    0 to 180 degrees
float GetAngle(Vector3D vecA, Vector3D vecB);
float GetSignedAngle(Vector2D vecA, Vector2D vecB); // -180 to 180 degrees
float GetDeterminant(Vector2D vecA, Vector2D vecB);
Vector2D GetDirection(Vector2D vecA, Vector2D vecB);
void RotateVector(Vector2D* direction, float angle);

//////////////////////////////

Vector3D LICH_ENGINE CrossProduct(Vector3D vecA, Vector3D vecB);

//////////////////////////////

float DistanceAB(Vector2D A, Vector2D B);
float DistanceAB(Vector3D A, Vector3D B);

//////////////////////////////

// min inclued but max exclued
int32_t GetSeed();
int32_t GetRandomNumber(int32_t state);
int RandomNumber(int32_t& state, int min, int max);
float RandomNumber(int32_t& state, float min, float max);

//////////////////////////////

void ExtractScaleFromMatrix(Matrix3D& mat);
void ExtractScaleFromMatrix(Matrix3D& mat, Vector3D& scale);
void ExtractScaleFromMatrix(Matrix4D& mat);
void ExtractScaleFromMatrix(Matrix4D& mat, Vector3D& scale);

////////////////////////////

Vector3D LICH_ENGINE GetPointInPlaneFromRay(Vector3D rayDir, Vector3D rayOrigin, Vector3D planeOrigin, Vector3D planeNormal);

////////////////////////////

//LAPI int  RoundToInt(float value);

#include <Maths.inl>

File_maths_GENERATED