#include <Maths.h>

inline Vector2D Vector2D::operator-()
{
    return Vector2D{ -x, -y };
}

inline Vector2D Vector2D::operator+(Vector2D const other)
{
    return Vector2D{ x + other.x, y + other.y };
}

inline Vector2D Vector2D::operator-(Vector2D const other) const
{
    return Vector2D{ x - other.x, y - other.y };
}

inline Vector2D Vector2D::operator*(float k)
{
    Vector2D result = { 0.f, 0.f };
    result.x = x * k;
    result.y = y * k;
    return result;
}

inline Vector2D Vector2D::operator/(float k)
{
    x /= k;
    y /= k;
    return *this;
}

inline bool Vector2D::operator==(Vector2D const other)
{
    if (x != other.x) return false;
    if (y != other.y) return false;
    return true;
}

inline bool Vector2D::operator==(Vector2D const other) const
{
    if (x != other.x) return false;
    if (y != other.y) return false;
    return true;
}

inline bool Vector2D::operator!=(Vector2D const other)
{
    if (x != other.x) return true;
    if (y != other.y) return true;
    return false;
}

inline Vector2D Vector2D::operator+=(Vector2D const other)
{
    this->x += other.x;
    this->y += other.y;
    return *this;
}

inline Vector2D Vector2D::operator-=(Vector2D const other)
{
    this->x -= other.x;
    this->y -= other.y;
    return *this;
}

inline Vector2D Vector2D::operator/=(float const k)
{
    this->x /= k;
    this->y /= k;
    return *this;
}

inline const Vector2D Vector2D::operator*(const float n) const
{
    const Vector2D newVec = { x * n, y * n };
    return newVec;
}

inline Vector2D Vector2D::GetDirection(Vector2D other)
{
    return other - *this;
}

inline float Vector2D::DotProduct(Vector2D other)
{
    return x * other.x + y * other.y;
}

inline float Vector2D::Determinant(Vector2D other)
{
    return x * other.y - y * other.x;
}

inline float Vector2D::Magnitude() const
{
    return sqrt(x * x + y * y);
}

inline void Vector2D::Normalize()
{
    *this /= this->Magnitude();
}

inline Vector2D Vector2D::Normal()
{
    return Vector2D{ y, -x };
}

inline float DotProduct2D(Vector2D vecA, Vector2D vecB)
{
    return vecA.x * vecB.x + vecA.y * vecB.y;
}

inline Vector2D GetDirection(Vector2D vecA, Vector2D vecB)
{
    return vecB - vecA;
}

inline float GetDeterminant(Vector2D vecA, Vector2D vecB)
{
    return vecA.x * vecB.y - vecA.y * vecB.x;
}

inline float DistanceAB(Vector2D A, Vector2D B)
{
    return (B - A).Magnitude();
}

////////////////////////////////////////////////////////////////////////////////////////////////////

inline bool Vector3D::operator==(const Vector3D other)
{
    if (other.x != x) return false;
    if (other.y != y) return false;
    if (other.z != z) return false;
    return true;
}

inline bool Vector3D::operator==(const Vector3D other) const
{
    if (other.x != x) return false;
    if (other.y != y) return false;
    if (other.z != z) return false;
    return true;
}

inline Vector3D operator+(Vector3D lhs,Vector3D rhs)
{
    return Vector3D{ lhs.x + rhs.x, lhs.y + rhs.y, lhs.z + rhs.z };
}

inline Vector3D Vector3D::operator-(const Vector3D other)
{
    return Vector3D{ x - other.x, y - other.y, z - other.z };
}

inline Vector3D Vector3D::operator*(const float k)
{
    return Vector3D{ x * k, y * k, z * k };
}

inline Vector3D Vector3D::operator*(const Vector3D other)
{
    return Vector3D{ x * other.x, y * other.y, z * other.z };
}

inline Vector3D Vector3D::operator*(const float k) const
{
    return Vector3D{ x * k, y * k, z * k };
}

inline Vector3D Vector3D::operator+=(const Vector3D other)
{
    x += other.x;
    y += other.y;
    z += other.z;
    return *this;
}

inline Vector3D Vector3D::operator*=(float const k)
{
    this->x *= k;
    this->y *= k;
    this->z *= k;
    return *this;
}

inline Vector3D Vector3D::operator/=(float const k)
{
    this->x /= k;
    this->y /= k;
    this->z /= k;
    return *this;
}

inline float Vector3D::Magnitude() const
{
    return sqrt(x * x + y * y + z * z);
}

inline Vector3D Vector3D::ConvertVector3ToRadians(Vector3D vec)
{
    vec.x = DEG2RAD(vec.x);
    vec.y = DEG2RAD(vec.y);
    vec.z = DEG2RAD(vec.z);
    return vec;
}

inline Vector3D Vector3D::ConvertRadVec3ToDegree(Vector3D vec)
{
    vec.x = RAD2DEG(vec.x);
    vec.y = RAD2DEG(vec.y);
    vec.z = RAD2DEG(vec.z);
    return vec;
}


inline void Vector3D::Normalize()
{
    *this /= this->Magnitude();
}

inline Vector3D Vector3D::Normalize(Vector3D vec)
{
    return vec /= vec.Magnitude();
}

inline float DotProduct3D(Vector3D vecA, Vector3D vecB)
{
    return vecA.x * vecB.x + vecA.y * vecB.y + vecA.z * vecB.z;
}

inline float DistanceAB(Vector3D A, Vector3D B)
{
    return (B - A).Magnitude();
}

////////////////////////////////////////////////////////////////////////////////////////////////////

inline Vector4D Vector4D::operator+(const Vector4D other)
{
    return Vector4D{ x + other.x, y + other.y, z + other.z, w + other.w };
}

inline Vector4D Vector4D::operator-(const Vector4D other)
{
    return Vector4D{ x - other.x, y - other.y, z - other.z, w - other.w };
}

inline Vector4D Vector4D::operator*(const float k)
{
    return Vector4D{ x * k, y * k, z * k, w * k };
}

inline Vector4D Vector4D::operator/=(float const k)
{
    this->w /= k;
    this->x /= k;
    this->y /= k;
    this->z /= k;
    return *this;
}

inline Vector4D& Vector4D::operator=(const Vector3D other)
{
    x = other.x;
    y = other.y;
    z = other.z;
    w = 1.f;
    return *this;
}

inline bool Vector4D::operator==(const Vector4D other)
{
    if (other.x != x) return false;
    if (other.y != y) return false;
    if (other.z != z) return false;
    if (other.w != w) return false;

    return true;
}

inline bool Vector4D::operator!=(const Vector4D other)
{
    if (other.x != x) return true;
    if (other.y != y) return true;
    if (other.z != z) return true;
    if (other.w != w) return true;

    return false;
}

inline void Vector4D::Homogenize()
{
    if (w != 0)
    {
        x /= w;
        y /= w;
        z /= w;
        w /= w;
    }
    else
    {
        std::cout << "Divide by zero" << __FILE__ << __LINE__ << std::endl;
    }
}

inline float Vector4D::Magnitude() const
{
    return sqrt(x * x + y * y + z * z + w * w);
}

inline void Vector4D::Normalize()
{
    *this /= this->Magnitude();
}

////////////////////////////////////////////////////////////////////////////////////////////////////

inline float Matrix4D::operator[](const unsigned int index)
{
    return coord[index % 4][index / 4];
}

inline Matrix4D Matrix4D::operator+(const Matrix4D mat)
{
    Matrix4D result;
    for (int j = 0; j < 4; j++)
        for (int i = 0; i < 4; i++)
            result.coord[j][i] = coord[j][i] + mat.coord[j][i];

    return result;
}

inline Matrix4D Matrix4D::operator-(const Matrix4D mat)
{
    Matrix4D result;
    for (int j = 0; j < 4; j++)
        for (int i = 0; i < 4; i++)
            result.coord[j][i] = coord[j][i] - mat.coord[j][i];

    return result;
}

inline Vector3D Matrix4D::operator*(Vector3D vec)
{
    Vector4D v2 = { vec.x, vec.y, vec.z, 1.f };
    Vector4D result = { 0.f, 0.f, 0.f, 0.f };

    for (int j = 0; j < 4; j++)
        for (int i = 0; i < 4; i++)
            result.coord[j] += coord[i][j] * v2.coord[i];

    Vector3D res = { result.x, result.y, result.z };
    return res;
}

inline Vector4D Matrix4D::operator*(Vector4D vec)
{
    Vector4D result = { 0.f, 0.f, 0.f, 0.f };

    for (int j = 0; j < 4; j++)
        for (int i = 0; i < 4; i++)
            result.coord[j] += coord[i][j] * vec.coord[i];

    return result;
}

inline Matrix4D Matrix4D::operator*(const Matrix4D mat)
{
    Matrix4D multMatrix;

    for (int k = 0; k < 4; k++)
        for (int j = 0; j < 4; j++)
            for (int i = 0; i < 4; i++)
                multMatrix.coord[j][k] += coord[i][k] * mat.coord[j][i];

    return multMatrix;
}

inline bool Matrix4D::operator==(const Matrix4D matrix)
{
        for (int j = 0; j < 4; j++)
            for (int i = 0; i < 4; i++)
                if(coord[j][i] != matrix.coord[j][i])
                    return false;
    return true;
}

inline bool Matrix4D::operator!=(const Matrix4D matrix)
{
    for (int j = 0; j < 4; j++)
        for (int i = 0; i < 4; i++)
            if (coord[j][i] != matrix.coord[j][i])
                return true;
    return false;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

inline bool Quaternion::operator==(const Quaternion other)
{
    if (other.x != x) return false;
    if (other.y != y) return false;
    if (other.z != z) return false;
    if (other.w != w) return false;

    return true;
}

inline Quaternion Quaternion::operator+(const Quaternion other)
{
    Quaternion quat = Quaternion();
    quat.scalar = this->scalar + other.scalar;
    quat.imaginaries = this->imaginaries + other.imaginaries;
    return quat;
}

inline Quaternion Quaternion::operator-(const Quaternion other)
{
    Quaternion quat = Quaternion();
    quat.scalar = this->scalar - other.scalar;
    quat.imaginaries = this->imaginaries - other.imaginaries;
    return quat;
}

inline Quaternion Quaternion::operator*(float k)
{
    Quaternion quat = Quaternion();
    quat.scalar = this->scalar * k;
    quat.imaginaries = this->imaginaries * k;
    return quat;
}

inline Quaternion Quaternion::operator*(float k) const
{
    Quaternion quat = Quaternion();
    quat.scalar = this->scalar * k;
    quat.imaginaries = this->imaginaries * k;
    return quat;
}

inline Quaternion Quaternion::operator*(const Quaternion other)
{
    return Quaternion(w * other.w - x * other.x - y * other.y - z * other.z,
        w * other.x + x * other.w + y * other.z - z * other.y,
        w * other.y - x * other.z + y * other.w + z * other.x,
        w * other.z + x * other.y - y * other.x + z * other.w);
}

inline void Quaternion::operator+=(const Quaternion other)
{
    this->scalar += other.scalar;
    this->imaginaries = this->imaginaries + other.imaginaries;
}

inline void Quaternion::operator-=(const Quaternion other)
{
    this->scalar -= other.scalar;
    this->imaginaries = this->imaginaries - other.imaginaries;
}

inline void Quaternion::operator*=(const Quaternion other)
{
    Quaternion q(w, x, y, z);
    w = q.w * other.w - q.x * other.x - q.y * other.y - q.z * other.z;
    x = q.w * other.x + q.x * other.w + q.y * other.z - q.z * other.y;
    y = q.w * other.y - q.x * other.z + q.y * other.w + q.z * other.x;
    z = q.w * other.z + q.x * other.y - q.y * other.x + q.z * other.w;
}

inline void Quaternion::operator*=(float k)
{
    this->scalar *= k;
    this->imaginaries = this->imaginaries * k;
}

inline Vector3D Quaternion::operator*(Vector3D other)
{
    Quaternion quat = Quaternion(1.f, other);
    return ((*this) * quat * (*this).GetInversedQuat()).imaginaries;
}

inline float Quaternion::DotProduct(const Quaternion other) const
{
    return x * other.x + y * other.y + z * other.z + w * other.w;
}

inline float Quaternion::GetNorm()
{
    float a = 0.f;
    for (size_t i = 0; i < 4; i++)
        a += pow(this->coord[i], 2.f);

    return sqrt(a);
}

inline void Quaternion::Normalize()
{
    *this *= 1 / this->GetNorm();
}

inline Quaternion Quaternion::GetNormalizedQuat()
{
    Quaternion quat = *this;
    quat.Normalize();
    return quat;
}

inline void Quaternion::Conjugate()
{
    this->imaginaries = this->imaginaries * -1;
}

inline Quaternion Quaternion::GetConjugatedQuat()
{
    Quaternion quat = *this;
    quat.Conjugate();
    return quat;
}

inline void Quaternion::Inverse()
{
    *this = this->GetConjugatedQuat() * (1.f / pow(this->GetNorm(), 2.f));
}

inline Quaternion Quaternion::GetInversedQuat()
{
    Quaternion quat = *this;
    quat.Inverse();
    return quat;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

template <typename T>
inline T Min(T x, T y)
{
    return x <= y ? x : y;
}

template <typename T>
inline T Max(T x, T y)
{
    return x >= y ? x : y;
}

template <typename T>
inline T Sign(T x)
{
    return x >= 0 ? 1 : -1;
}

template <typename T>
inline T Absolute(T x)
{
    return x >= 0 ? x : -x;
}

template <typename T>
inline T Lerp(T start, T end, const float& t)
{
    return (end - start) * t + start;
}

inline float Lerp(const float& start, const float& end, const float& t)
{
    return (end - start) * t + start;
}

inline Vector2D Lerp(const Vector2D& start, const Vector2D& end, const float& t)
{
    return (end - start) * t + start;
}

inline int Clamp(int n, int min, int max)
{
    n = n > max ? max : n;
    n = n < min ? min : n;
    return n;
}

inline float Clamp(float k, float min, float max)
{
    return Min(Max(k, min), max);
}