#include <pch.h>
#include <Physics/Transform.h>
#include <Generated/maths.rfks.h>

const Vector2D Vector2D::zero = { 0.f,  0.f };
const Vector2D Vector2D::one = { 1.f,  1.f };
const Vector2D Vector2D::up = { 0.f,  1.f };
const Vector2D Vector2D::left = { -1.f,  0.f };
const Vector2D Vector2D::down = { 0.f, -1.f };
const Vector2D Vector2D::right = { 1.f,  0.f };

Vector2D::Vector2D()
{
	x = 0.f;
	y = 0.f;
}

Vector2D::Vector2D(float x, float y)
{
	this->x = x;
	this->y = y;
}

Vector2D::Vector2D(int x, int y)
{
	this->x = (float)x;
	this->y = (float)y;
}

Vector2D::Vector2D(int number)
{
	x = (float)number;
	y = (float)number;
}

std::string Vector2D::ToString()
{
	std::string data = "";
	for (int i = 0; i < 2; i++)
		data += std::to_string(coord[i]) + ' ';
	return data;
}

void Vector2D::FromString(std::string data)
{
	std::string::size_type sz;
	x = std::stof(data, &sz);
	y = std::stof(data.substr(sz), &sz);
}

Vector2D::~Vector2D() {}

float GetAngle(Vector2D vecA, Vector2D vecB)
{
	vecA.Normalize();
	vecB.Normalize();
	float dot = vecA.DotProduct(vecB);

	return RAD2DEG(acosf(dot));
}

float GetAngle(Vector3D vecA, Vector3D vecB)
{
	vecA.Normalize();
	vecB.Normalize();
	float dot = DotProduct3D(vecA, vecB);

	return RAD2DEG(acosf(dot));
}

float GetSignedAngle(Vector2D vecA, Vector2D vecB)
{
	float angle = atan2f(vecB.y, vecB.x) - atan2f(vecA.y, vecA.x);
	return RAD2DEG(angle);
}

void RotateVector(Vector2D* vec, float angle)
{
	angle = DEG2RAD(angle);
	Vector2D startVec = *vec;
	vec->x = startVec.x * cosf(angle) - (startVec.y * sinf(angle));
	vec->y = startVec.x * sinf(angle) + (startVec.y * cosf(angle));
}

////////////////////////////////////////////////////////////////////////////////////////////////////

const Vector3D Vector3D::zero = { 0.f,  0.f,  0.f };
const Vector3D Vector3D::one = { 1.f,  1.f,  1.f };
const Vector3D Vector3D::up = { 0.f,  1.f,  0.f };
const Vector3D Vector3D::down = { 0.f, -1.f,  0.f };
const Vector3D Vector3D::left = { -1.f,  0.f,  0.f };
const Vector3D Vector3D::right = { 1.f,  0.f,  0.f };
const Vector3D Vector3D::forward = { 0.f,  0.f,  1.f };
const Vector3D Vector3D::back = { 0.f,  0.f, -1.f };
const Vector3D Vector3D::x0z = { 1.f,  0.f, 1.f };

Vector3D::Vector3D()
{
	x = 0.f;
	y = 0.f;
	z = 0.f;
}

Vector3D::Vector3D(const float k)
{
	x = k;
	y = k;
	z = k;
}

Vector3D::Vector3D(const float c1, const float c2, const float c3)
{
	x = c1;
	y = c2;
	z = c3;
}

Vector3D::~Vector3D() {}

Vector3D CrossProduct(Vector3D vecA, Vector3D vecB)
{
	Vector3D vector;
	vector.coord[0] = vecA.coord[1] * vecB.coord[2] - vecB.coord[1] * vecA.coord[2];
	vector.coord[1] = vecA.coord[2] * vecB.coord[0] - vecB.coord[2] * vecA.coord[0];
	vector.coord[2] = vecA.coord[0] * vecB.coord[1] - vecB.coord[0] * vecA.coord[1];
	return vector;
}

void Vector3D::ToString(const float* coord, const char* name)
{
	std::cout << name << "\n[" << std::endl;

	for (int i = 0; i < 3; i++)
		std::cout << " " << coord[i];

	std::cout << " ]\n" << std::endl;
}

Vector3D Vector3D::ToInt()
{
	return Vector3D((int)x, (int)y, (int)z);
}

std::string Vector3D::ToString()
{
	std::string data = "";
	for (int i = 0; i < 3; i++)
		data += std::to_string(coord[i]) + ' ';
	return data;
}

void Vector3D::FromString(std::string data)
{
	std::string::size_type sz;
	x = std::stof(data, &sz);
	size_t size = sz;
	y = std::stof(data.substr(sz), &sz);
	size += sz;
	z = std::stof(data.substr(size));
}

Vector3D Vector3D::GetFromString(std::string data)
{
	Vector3D a;

	std::string::size_type sz;
	a.x = std::stof(data, &sz);
	size_t size = sz;
	a.y = std::stof(data.substr(sz), &sz);
	size += sz;
	a.z = std::stof(data.substr(size));

	return a;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

const Vector4D Vector4D::zero = { 0.f,  0.f,  0.f,  1.f };
const Vector4D Vector4D::one = { 1.f,  1.f,  1.f,  1.f };
const Vector4D Vector4D::up = { 0.f,  1.f,  0.f,  1.f };
const Vector4D Vector4D::down = { 0.f, -1.f,  0.f,  1.f };
const Vector4D Vector4D::left = { -1.f,  0.f,  0.f,  1.f };
const Vector4D Vector4D::right = { 1.f,  0.f,  0.f,  1.f };
const Vector4D Vector4D::forward = { 0.f,  0.f,  1.f,  1.f };
const Vector4D Vector4D::back = { 0.f,  0.f, -1.f,  1.f };

Vector4D::Vector4D()
{
	x = 0.f;
	y = 0.f;
	z = 0.f;
	w = 0.f;
}

Vector4D::Vector4D(float x, float y, float z, float w)
{
	this->x = x;
	this->y = y;
	this->z = z;
	this->w = w;
}
Vector4D::Vector4D(const Vector3D& vec3, const float w)
{
	x = vec3.x;
	y = vec3.y;
	z = vec3.z;
	this->w = w;
}

Vector4D::~Vector4D() {}

std::string Vector4D::ToString()
{
	std::string data = "";
	for (int i = 0; i < 4; i++)
		data += std::to_string(coord[i]) + ' ';
	return data;
}

Vector4D Vector4D::GetFromString(std::string data)
{
	Vector4D a;

	std::string::size_type sz;
	a.x = std::stof(data, &sz);
	size_t size = sz;
	a.y = std::stof(data.substr(sz), &sz);
	size += sz;
	a.z = std::stof(data.substr(size));
	size += sz;
	a.w = std::stof(data.substr(size));

	return a;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

Matrix3D::Matrix3D()
{
	for (int j = 0; j < 3; j++)
		for (int i = 0; i < 3; i++)
			coord[i][j] = 0.f;
}

Matrix3D::Matrix3D(Matrix4D mat)
{
	for (int j = 0; j < 3; j++)
		for (int i = 0; i < 3; i++)
			coord[i][j] = mat.coord[i][j];
}

Matrix3D::~Matrix3D() {}

void Matrix3D::ToString(const float* coord, const char* name)
{
	for (int j = 0; j < 3; j++)
	{
		std::cout << "[";
		for (int i = 0; i < 3; i++)
		{
			std::cout << " " << coord[i + j * 3];
		}
		std::cout << " ]" << std::endl;
	}

	std::cout << "\n" << std::endl;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

Matrix4D::Matrix4D()
{
	for (int j = 0; j < 4; j++)
		for (int i = 0; i < 4; i++)
			coord[i][j] = 0.f;


}

Matrix4D::Matrix4D(float a)
{
	for (int j = 0; j < 4; j++)
		for (int i = 0; i < 4; i++)
			coord[i][j] = a;

}

Matrix4D::Matrix4D(float tab[4][4])
{
	for (int j = 0; j < 4; j++)
		for (int i = 0; i < 4; i++)
			coord[i][j] = tab[i][j];
}

Matrix4D::Matrix4D(float tab[16])
{
	for (int j = 0; j < 16; j++)
		e[j] = tab[j];
}

Matrix4D::Matrix4D(float a, float b, float c, float d, float f, float g, float h, float i, float j, float k, float l, float m, float n, float o, float p, float q)
{
	e[0] = a;
	e[1] = b;
	e[2] = c;
	e[3] = d;
	e[4] = f;
	e[5] = g;
	e[6] = h;
	e[7] = i;
	e[8] = j;
	e[9] = k;
	e[10] = l;
	e[11] = m;
	e[12] = n;
	e[13] = o;
	e[14] = p;
	e[15] = q;
}

Matrix4D::~Matrix4D() {}

void Matrix4D::Identity()
{
	for (int i = 0; i < 4; i++)
		coord[i][i] = 1.f;
}

void Matrix4D::ToString(const float* coord, const char* name)
{
	std::cout << name << "\n" << std::endl;
	for (int j = 0; j < 4; j++)
	{
		std::cout << "[";

		for (int i = 0; i < 4; i++)
			std::cout << " " << coord[i + j * 4];

		std::cout << " ]" << std::endl;
	}
	std::cout << "\n" << std::endl;
}

float Matrix4D::Determinant2x2(Matrix4D matrix)
{
	float det = 0.f;
	det = matrix.coord[0][0] * matrix.coord[1][1] - matrix.coord[0][1] * matrix.coord[1][0];
	return det;
}

float Matrix4D::Determinant3x3(Matrix4D matrix)
{
	float det = 0.f;
	for (int j = 0; j < 3; j++)
	{
		Matrix4D mat2x2 = Matrix4D::Mat3x3ToMat2x2(0, j, matrix);
		det += MatrixSignElement(j) * matrix.coord[j][0] * Determinant2x2(mat2x2);
	}
	return det;
}

float Matrix4D::Determinant4x4(Matrix4D matrix)
{
	float det = 0.f;

	for (int j = 0; j < 4; j++)
	{
		Matrix4D mat3x3 = Matrix4D::Mat4x4ToMat3x3(0, j, matrix);
		det += MatrixSignElement(j) * matrix.coord[j][0] * Determinant3x3(mat3x3);
	}
	return det;
}

Matrix4D Matrix4D::Mat3x3ToMat2x2(int row, int columns, Matrix4D mat)
{
	Matrix4D mat2x2;
	int index = 0;
	for (int j = 0; j < 3; j++)
	{
		if (j != columns)
		{
			for (int i = 0; i < 3; i++)
			{
				if (i != row)
				{
					mat2x2.coord[index / 2][index % 2] = mat.coord[j][i];
					index++;
				}
			}
		}
	}
	return mat2x2;
}

Matrix4D Matrix4D::Mat4x4ToMat3x3(int row, int columns, Matrix4D mat)
{
	Matrix4D mat3x3;
	int index = 0;
	for (int j = 0; j < 4; j++)
	{
		if (j != columns)
		{
			for (int i = 0; i < 4; i++)
			{
				if (i != row)
				{
					mat3x3.coord[index / 3][index % 3] = mat.coord[j][i];
					index++;
				}
			}
		}
	}
	return mat3x3;
}

void ExtractScaleFromMatrix(Matrix3D& mat)
{
	for (int i = 0; i < 3; i++)
	{
		Vector3D xScale = Vector3D(mat.coord[0][i], mat.coord[1][i], mat.coord[2][i]);
		float length = xScale.Magnitude();
		mat.coord[0][i] = mat.coord[0][i] / length;
		mat.coord[1][i] = mat.coord[1][i] / length;
		mat.coord[2][i] = mat.coord[2][i] / length;
	}
}

void ExtractScaleFromMatrix(Matrix3D& mat, Vector3D& scale)
{
	for (int i = 0; i < 3; i++)
	{
		Vector3D xScale = Vector3D(mat.coord[0][i], mat.coord[1][i], mat.coord[2][i]);
		float length = xScale.Magnitude();
		scale.coord[i] = xScale.Magnitude();
		mat.coord[0][i] = mat.coord[0][i] / length;
		mat.coord[1][i] = mat.coord[1][i] / length;
		mat.coord[2][i] = mat.coord[2][i] / length;
	}
}

void ExtractScaleFromMatrix(Matrix4D& mat)
{
	for (int i = 0; i < 3; i++)
	{
		Vector3D xScale = Vector3D(mat.coord[0][i], mat.coord[1][i], mat.coord[2][i]);
		float length = xScale.Magnitude();
		mat.coord[0][i] = mat.coord[0][i] / length;
		mat.coord[1][i] = mat.coord[1][i] / length;
		mat.coord[2][i] = mat.coord[2][i] / length;
	}
}

void ExtractScaleFromMatrix(Matrix4D& mat, Vector3D& scale)
{
	for (int i = 0; i < 3; i++)
	{
		Vector3D xScale = Vector3D(mat.coord[0][i], mat.coord[1][i], mat.coord[2][i]);
		float length = xScale.Magnitude();
		scale.coord[i] = xScale.Magnitude();
		mat.coord[0][i] = mat.coord[0][i] / length;
		mat.coord[1][i] = mat.coord[1][i] / length;
		mat.coord[2][i] = mat.coord[2][i] / length;
	}
}

LAPI int RoundToInt(float value)
{
	int test = (int)Absolute<float>(value);
	int test1 = (int)(Absolute<float>(value) + 0.5f);
	if (test != test1)
		return test + 1;
	else
		return 0;
}

Vector3D Matrix4D::GetScaleFromMatrix(Matrix4D mat)
{
	Vector3D scale;
	Matrix3D mat3 = Matrix3D(mat);
	ExtractScaleFromMatrix(mat3, scale);
	return scale;
}

Vector3D Matrix4D::GetPositionFromMatrix(Matrix4D mat)
{
	Vector3D pos;
	pos.x = mat.coord[3][0];
	pos.y = mat.coord[3][1];
	pos.z = mat.coord[3][2];
	return pos;
}

Vector3D Matrix4D::GetRotationFromMatrix(Matrix4D mat)
{
	Vector3D angle;
	Matrix3D mat3 = Matrix3D(mat);

	ExtractScaleFromMatrix(mat3); // extract Scale 

	float  thetaX = 0.f, thetaY = 0.f, thetaZ = 0.f;
	if (mat3.coord[1][2] < 1)
	{
		if (mat3.coord[1][2] > -1)
		{
			thetaX = asin(-mat3.coord[1][2]);
			thetaY = atan2(mat3.coord[0][2], mat3.coord[2][2]);
			thetaZ = atan2(mat3.coord[1][0], mat3.coord[1][1]);
		}
		else
		{
			thetaX = (float)M_PI / 2.f;
			thetaY = -atan2(-mat3.coord[0][1], mat3.coord[0][0]);
			thetaZ = 0.f;
		}
	}
	else
	{
		thetaX = (float)-M_PI / 2.f;
		thetaY = atan2(-mat3.coord[0][1], mat3.coord[0][0]);
		thetaZ = 0.f;
	}

	angle.x = RAD2DEG(thetaX);
	angle.y = RAD2DEG(thetaY);
	angle.z = RAD2DEG(thetaZ);
	return angle;
}

Quaternion Matrix4D::GetQuaternionFromMatrix(Matrix4D mat)
{
	Matrix3D mat3 = Matrix3D(mat);

	ExtractScaleFromMatrix(mat3); // extract Scale 

	float a, b, c, d;
	a = sqrtf(1.f + mat3.coord[0][0] + mat3.coord[1][1] + mat3.coord[2][2]) / 2.f;
	b = (mat3.coord[1][2] - mat3.coord[2][1]) / (4.f * a);
	c = (mat3.coord[2][0] - mat3.coord[0][2]) / (4.f * a);
	d = (mat3.coord[0][1] - mat3.coord[1][0]) / (4.f * a);

	return Quaternion(a, b, c, d);
}

Matrix4D Matrix4D::GetMatrixFromQuaternion(Quaternion quat)
{
	float a = quat.coord[0], b = quat.coord[1], c = quat.coord[2], d = quat.coord[3];

	float tab[4][4] = { {2.f * (a * a + b * b) - 1.f,	2.f * (b * c + d * a),			2.f * (b * d - c * a),		    0.f},
						{2.f * (b * c - d * a),			2.f * (a * a + c * c) - 1.f,	2.f * (c * d + b * a),		    0.f},
						{2.f * (b * d + c * a),			2.f * (c * d - b * a),			2.f * (a * a + d * d) - 1.f,	0.f},
						{0.f,							0.f,							0.f,							1.f} };

	return Matrix4D(tab);
}

Physics::Transform Matrix4D::GetTransformFromMatrix(Matrix4D mat, Physics::Transform& transform)
{
	transform.SetLocalRotation(GetQuaternionFromMatrix(mat));
	//transform.scale = GetScaleFromMatrix(mat);
	transform.SetLocalPosition(GetPositionFromMatrix(mat));

	return transform;
}

Matrix4D Matrix4D::MatrixCofacteur(Matrix4D mat4x4)
{
	Matrix4D mat4x4Cof;
	for (int j = 0; j < 4; j++)
	{
		for (int i = 0; i < 4; i++)
		{
			Matrix4D mat3x3 = Matrix4D::Mat4x4ToMat3x3(i, j, mat4x4);
			mat4x4Cof.coord[j][i] = Matrix4D::MatrixSignElement(j + i) * Determinant3x3(mat3x3);
		}
	}
	return mat4x4Cof;
}

Matrix4D Matrix4D::TransposeMatrix(Matrix4D mat)
{
	Matrix4D matTransp;

	for (int j = 0; j < 4; j++)
		for (int i = 0; i < 4; i++)
			matTransp.coord[i][j] = mat.coord[j][i];

	return matTransp;
}

Matrix4D Matrix4D::InverseMatrix(Matrix4D mat)
{
	Matrix4D inverseMat;
	Matrix4D mat4x4 = MatrixCofacteur(mat);
	mat4x4 = TransposeMatrix(mat4x4);
	float det = Matrix4D::Determinant4x4(mat);

	for (int j = 0; j < 4; j++)
		for (int i = 0; i < 4; i++)
			inverseMat.coord[j][i] = (1.f / det) * mat4x4.coord[j][i];

	return inverseMat;
}

Matrix4D Matrix4D::LookAt(Vector3D eye, Vector3D at, Vector3D up)
{
	Vector3D Z = Vector3D::Normalize(at - eye);
	Vector3D X = Vector3D::Normalize(CrossProduct(Z, up));
	Vector3D Y = CrossProduct(X, Z);

	float tab[4][4] = { { X.x, Y.x, -Z.x, 0.f },
						{ X.y, Y.y, -Z.y, 0.f },
						{ X.z, Y.z, -Z.z, 0.f },
						{ -DotProduct3D(X, eye), -DotProduct3D(Y, eye), DotProduct3D(Z, eye), 1.f } };

	Matrix4D result(tab);
	return result;
}

Matrix4D Matrix4D::ProjectionMatrix(const float fov, const float aspectRatio, const float nearValue, const float farValue)
{
	float top = tanf(DEG2RAD(fov) / 2.f) * nearValue;
	float bottom = -top;
	float right = top * aspectRatio;
	float left = -top * aspectRatio;

	float tab[4][4] = { { (2.f * nearValue) / (right - left),	0.f,								(right + left) / (right - left),						0.f },
						{ 0.f,									(2.f * nearValue) / (top - bottom),	(top + bottom) / (top - bottom),						0.f },
						{ 0.f,									0.f,								-(farValue + nearValue) / (farValue - nearValue),	   -1.f },
						{ 0.f,									0.f,								-(2.f * farValue * nearValue) / (farValue - nearValue),	0.f } };

	Matrix4D Projection(tab);

	return Projection;
}

Matrix4D Matrix4D::OrthographicMatrix(float left, float right, float top, float bottom, float nearValue, float farValue)
{
	float tab[4][4] = { { 2.f / (right - left),	            0.f,							    0.f,												0.f },
						{ 0.f,							    2.f / (top - bottom),	            0.f,												0.f },
						{ 0.f,							    0.f,						        -2.f / (farValue - nearValue),						0.f },
						{ -(right + left) / (right - left),	-(top + bottom) / (top - bottom),	-(farValue + nearValue) / (farValue - nearValue),	1.f } };

	Matrix4D Ortho(tab);
	return Ortho;
}

Matrix4D Matrix4D::OrthographicMatrix(float left, float right, float bottom, float top)
{
	float tab[4][4] = { { 2.f / (right - left),	              0.f,							       0.f,												0.f},
						{ 0.f,							      2.f / (top - bottom),	               0.f,												0.f},
						{ 0.f,							      0.f,						          -1.f,						                        0.f},
						{ -(right + left) / (right - left),	  -(top + bottom) / (top - bottom),	   0.f,	                                            1.f } };

	Matrix4D Ortho(tab);
	return Ortho;
}

Matrix4D Matrix4D::CreateXRotationMatrix(float angle)
{
	Matrix4D rotXMat;
	rotXMat.coord[0][0] = 1.f;
	rotXMat.coord[3][3] = 1.f;

	angle = DEG2RAD(angle);
	rotXMat.coord[1][1] = cosf(angle);
	rotXMat.coord[1][2] = sinf(angle);
	rotXMat.coord[2][1] = -sinf(angle);
	rotXMat.coord[2][2] = cosf(angle);
	return rotXMat;
}

Matrix4D Matrix4D::CreateYRotationMatrix(float angle)
{
	Matrix4D rotYMat;
	rotYMat.coord[1][1] = 1.f;
	rotYMat.coord[3][3] = 1.f;

	angle = DEG2RAD(angle);
	rotYMat.coord[0][0] = cosf(angle);
	rotYMat.coord[0][2] = -sinf(angle);
	rotYMat.coord[2][0] = sinf(angle);
	rotYMat.coord[2][2] = cosf(angle);
	return rotYMat;
}

Matrix4D Matrix4D::CreateZRotationMatrix(float angle)
{
	Matrix4D rotZMat;
	rotZMat.coord[2][2] = 1.f;
	rotZMat.coord[3][3] = 1.f;

	angle = DEG2RAD(angle);
	rotZMat.coord[0][0] = cosf(angle);
	rotZMat.coord[0][1] = sinf(angle);
	rotZMat.coord[1][0] = -sinf(angle);
	rotZMat.coord[1][1] = cosf(angle);
	return rotZMat;
}

Matrix4D Matrix4D::CreateRotationMatrix(Vector3D angle)
{
	Matrix4D transformMat;
	transformMat.Identity();
	transformMat = transformMat * CreateZRotationMatrix(angle.z);
	transformMat = transformMat * CreateYRotationMatrix(angle.y);
	transformMat = transformMat * CreateXRotationMatrix(angle.x);
	return transformMat;
}

Matrix4D Matrix4D::CreateScaleMatrix(const Vector3D& scale)
{
	Matrix4D scaleMat;
	scaleMat.coord[0][0] = scale.x;
	scaleMat.coord[1][1] = scale.y;
	scaleMat.coord[2][2] = scale.z;
	scaleMat.coord[3][3] = 1.f;
	return scaleMat;
}

Matrix4D Matrix4D::CreateTranslationMatrix(const Vector3D& translation)
{
	Matrix4D transformMat;
	transformMat.Identity();
	transformMat.coord[3][0] = translation.x;
	transformMat.coord[3][1] = translation.y;
	transformMat.coord[3][2] = translation.z;
	return transformMat;
}

Matrix4D Matrix4D::CreateTransformMatrix(Physics::Transform& transform)
{
	Matrix4D transformMat = Matrix4D();
	transformMat = CreateTranslationMatrix(transform.GetLocalPosition());
	transformMat = transformMat * GetMatrixFromQuaternion(transform.GetLocalRotationInQuaternion());
	transformMat = transformMat * CreateScaleMatrix(transform.GetLocalScale());
	return transformMat;
}

Matrix4D Matrix4D::CreateTransformMatrix(const Vector3D position, const Quaternion rotation, const Vector3D scale)
{
	Matrix4D transformMat = Matrix4D(0.f);
	transformMat = CreateTranslationMatrix(position);
	transformMat = transformMat * GetMatrixFromQuaternion(rotation);
	transformMat = transformMat * CreateScaleMatrix(scale);
	return transformMat;
}

Matrix4D Matrix4D::CreateTransformMatrix(const Vector3D position, const Vector3D rotation, const Vector3D scale)
{
	Matrix4D transformMat = Matrix4D(0.f);
	transformMat = CreateTranslationMatrix(position);
	transformMat = transformMat * CreateRotationMatrix(rotation);
	transformMat = transformMat * CreateScaleMatrix(scale);
	return transformMat;
}

int Matrix4D::MatrixSignElement(int index)
{
	return index % 2 == 0 ? 1 : -1;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

const Quaternion Quaternion::Identity = { 1.f,  0.f,  0.f,  0.f };

Quaternion::Quaternion()
{
	this->scalar = 0.f;
	this->imaginaries = Vector3D();
}

Quaternion::Quaternion(float scalar, Vector3D vec)
{
	this->scalar = scalar;
	this->imaginaries = vec;
}

Quaternion::Quaternion(float w, float x, float y, float z)
{
	this->scalar = w;
	this->imaginaries = Vector3D(x, y, z);
}

Quaternion::~Quaternion() {}

void Quaternion::GetAngleAxis(Vector3D& axis, float& angle, bool normalized)
{
	if (!normalized)
		this->Normalize();

	angle = 2 * acosf(w);
	axis.x = x / sqrtf(1.f - w * w);
	axis.y = y / sqrtf(1.f - w * w);
	axis.z = z / sqrtf(1.f - w * w);

	/*Quaternion q = Quaternion();
	q.scalar = 2 * acos(this->scalar);
	q.imaginaries = Vector3D(this->imaginaries.x / sqrtf(1 - pow(this->scalar, 2.f)), this->imaginaries.y / sqrtf(1 - pow(this->scalar, 2.f)), this->imaginaries.z / sqrtf(1 - pow(this->scalar, 2.f)));
	return q;*/
}

Vector3D Quaternion::GetEulerAnglesFromQuaternion(Quaternion quat)
{
	quat.Normalize();
	Vector3D angles;

	// roll (x-axis rotation)
	double sinr_cosp = 2 * (quat.w * quat.x + quat.y * quat.z);
	double cosr_cosp = 1 - 2 * (quat.x * quat.x + quat.y * quat.y);
	angles.x = std::atan2(sinr_cosp, cosr_cosp);

	// pitch (y-axis rotation)
	double sinp = std::sqrt(1 + 2 * (quat.w * quat.y - quat.x * quat.z));
	double cosp = std::sqrt(1 - 2 * (quat.w * quat.y - quat.x * quat.z));
	angles.y = 2 * std::atan2(sinp, cosp) - M_PI / 2;

	// yaw (z-axis rotation)
	double siny_cosp = 2 * (quat.w * quat.z + quat.x * quat.y);
	double cosy_cosp = 1 - 2 * (quat.y * quat.y + quat.z * quat.z);
	angles.z = std::atan2(siny_cosp, cosy_cosp);
	
	angles = Vector3D(RAD2DEG(angles.x), RAD2DEG(angles.y), RAD2DEG(angles.z));
	return angles;
}

Quaternion Quaternion::GetQuaternionFromEulerAngles(Vector3D euler)
{
	euler = Vector3D(DEG2RAD(euler.x), DEG2RAD(euler.y), DEG2RAD(euler.z));

	double cr = cos(euler.x * 0.5f);
	double sr = sin(euler.x * 0.5f);
	double cp = cos(euler.y * 0.5f);
	double sp = sin(euler.y * 0.5f);
	double cy = cos(euler.z * 0.5f);
	double sy = sin(euler.z * 0.5f);

	Quaternion q;
	q.w = cr * cp * cy + sr * sp * sy;
	q.x = sr * cp * cy - cr * sp * sy;
	q.y = cr * sp * cy + sr * cp * sy;
	q.z = cr * cp * sy - sr * sp * cy;

	return q;
}

Quaternion Quaternion::AngleAxisToQuat(Vector3D axis, double angle)
{
	double angleRad = DEG2RAD(angle);
	return Quaternion(cosf(angleRad / 2.f),
		sinf(angleRad / 2.f) * axis.x,
		sinf(angleRad / 2.f) * axis.y,
		sinf(angleRad / 2.f) * axis.z);
}

Quaternion Quaternion::ConvertZYXToXYZ()
{
	Vector3D eulerAngle = GetEulerAnglesFromQuaternion(*this);
	Quaternion qX = AngleAxisToQuat(Vector3D::right, eulerAngle.x);
	Quaternion qY = AngleAxisToQuat(Vector3D::up, eulerAngle.y);
	Quaternion qZ = AngleAxisToQuat(Vector3D::forward, eulerAngle.z);
	return qZ * qY * qX;
}

Vector3D Quaternion::AngleAxis(Vector3D vec, float angle, Vector3D axis)
{
	Vector3D v = vec * cosf(angle) + CrossProduct(axis, vec) * sinf(angle) + axis * DotProduct3D(axis, vec) * (1 - cosf(angle));
	return v;
}

void Quaternion::ToString(const float* coord, const char* name)
{
	std::cout << name << " [ ";

	for (int i = 0; i < 4; i++)
		std::cout << coord[i] << " ";

	std::cout << "]\n" << std::endl;
}

Quaternion Slerp(const Quaternion& start, const Quaternion& end, const float& t)
{
	Clamp(t, 0.f, 1.f);

	float cosTheta = start.DotProduct(end);
	Quaternion newEnd = end;

	if (cosTheta < 0)
	{
		newEnd *= -1;
		cosTheta *= -1;
	}

	float absdotproduct = Absolute(cosTheta);
	float theta = acosf(cosTheta);

	float mult1, mult2;
	if (1.f - absdotproduct < 0.0001f)
	{
		// Linear interpolation for close orientations.
		mult1 = 1.f - t;
		mult2 = t;
	}
	else
	{
		mult1 = sinf((1.f - t) * theta) / sinf(theta);
		mult2 = sinf(t * theta) / sinf(theta);
	}

	Quaternion quat = start * mult1 + newEnd * mult2;
	return quat.GetNormalizedQuat();
}

////////////////////////////////////////////////////////////////////////////////////////////////////

int32_t GetSeed()
{
	time_t currentTime;
	struct tm* localTime = new tm();
	time(&currentTime);
	localtime_s(localTime, &currentTime);
	int32_t seed = localTime->tm_min * localTime->tm_sec * localTime->tm_hour;
	delete(localTime);
	return seed;
}

int32_t GetRandomNumber(int32_t state)
{
	uint64_t x = (uint64_t)state;
	int m = (int)pow(2, 31);
	int a = 1103515245;
	int c = 12345;
	uint64_t result = (a * x + c) % m; // La formule

	return (int32_t)result;
}

int RandomNumber(int32_t& state, int min, int max)
{
	state = GetRandomNumber(state);
	int number = state % (max - 1 - min);
	number += min;
	return number;
}

float RandomNumber(int32_t& state, float min, float max)
{
	float random = ((float)rand()) / (float)RAND_MAX;
	float diff = max - min;
	float r = random * diff;
	return min + r;
}

void Vector4D::FromString(std::string data)
{
	std::string::size_type sz;
	x = std::stof(data, &sz);
	size_t size = sz;
	y = std::stof(data.substr(sz), &sz);
	size += sz;
	z = std::stof(data.substr(size));
	size += sz;
	w = std::stof(data.substr(size));
}

Vector3D GetPointInPlaneFromRay(Vector3D rayDir, Vector3D rayOrigin, Vector3D planeOrigin, Vector3D planeNormal)
{
	float multiplier = (planeNormal.x * (planeOrigin.x - rayOrigin.x) + planeNormal.y * (planeOrigin.y - rayOrigin.y) + planeNormal.z * (planeOrigin.z - rayOrigin.z)) / (planeNormal.x * rayDir.x + planeNormal.y * rayDir.y + planeNormal.z * rayDir.z);
	Vector3D final = rayOrigin + rayDir * multiplier;
	return final;
}

Vector3D ClampRotation(const Vector3D& rotation)
{
	Vector3D newRotation = rotation;
	if (newRotation.x < -180.f)
		newRotation.x += 360;
	else if (newRotation.x > 180.f)
		newRotation.x -= 360.f;

	if (newRotation.y < -180.f)
		newRotation.y += 360;
	else if (newRotation.y > 180.f)
		newRotation.y -= 360.f;

	if (newRotation.y < -180.f)
		newRotation.y += 360;
	else if (newRotation.y > 180.f)
		newRotation.y -= 360.f;

	return newRotation;
}