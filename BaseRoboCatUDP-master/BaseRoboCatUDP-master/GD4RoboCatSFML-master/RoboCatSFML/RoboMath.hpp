class Vector3
{
public:
	

	float		mX, mY, mZ;

	Vector3(float x, float y, float z) :
		mX(x),
		mY(y),
		mZ(z)
	{}

	Vector3() :
		mX(0.0f),
		mY(0.0f),
		mZ(0.0f)
	{}

	void Set(float x, float y, float z)
	{
		mX = x;
		mY = y;
		mZ = z;
	}

	float GetX() {
		return mX;
	}

	float GetY() {
		return mY;
	}

	void SetX(float x) {
		mX = x;
	
	};
	void SetY(float y) {
		mY = y;
	};

	friend Vector3 operator+(const Vector3& inLeft, const Vector3& inRight)
	{
		return Vector3(inLeft.mX + inRight.mX, inLeft.mY + inRight.mY, inLeft.mZ + inRight.mZ);
	}

	friend Vector3 operator-(const Vector3& inLeft, const Vector3& inRight)
	{
		return Vector3(inLeft.mX - inRight.mX, inLeft.mY - inRight.mY, inLeft.mZ - inRight.mZ);
	}

	// Component-wise multiplication
	friend Vector3 operator*(const Vector3& inLeft, const Vector3& inRight)
	{
		return Vector3(inLeft.mX * inRight.mX, inLeft.mY * inRight.mY, inLeft.mZ * inRight.mZ);
	}

	// Scalar multiply
	friend Vector3 operator*(float inScalar, const Vector3& inVec)
	{
		return Vector3(inVec.mX * inScalar, inVec.mY * inScalar, inVec.mZ * inScalar);
	}

	friend Vector3 operator*(const Vector3& inVec, float inScalar)
	{
		return Vector3(inVec.mX * inScalar, inVec.mY * inScalar, inVec.mZ * inScalar);
	}

	Vector3& operator*=(float inScalar)
	{
		mX *= inScalar;
		mY *= inScalar;
		mZ *= inScalar;
		return *this;
	}

	Vector3& operator+=(const Vector3& inRight)
	{
		mX += inRight.mX;
		mY += inRight.mY;
		mZ += inRight.mZ;
		return *this;
	}

	Vector3& operator-=(const Vector3& inRight)
	{
		mX -= inRight.mX;
		mY -= inRight.mY;
		mZ -= inRight.mZ;
		return *this;
	}

	float Length()
	{
		return sqrtf(mX * mX + mY * mY + mZ * mZ);
	}

	float LengthSq()
	{
		return mX * mX + mY * mY + mZ * mZ;
	}

	float Length2D()
	{
		return sqrtf(mX * mX + mY * mY);
	}

	float LengthSq2D()
	{
		return mX * mX + mY * mY;
	}

	void Normalize()
	{
		float length = Length();
		mX /= length;
		mY /= length;
		mZ /= length;
	}

	void Normalize2D()
	{
		float length = Length2D();
		mX /= length;
		mY /= length;
	}

	friend float Dot(const Vector3& inLeft, const Vector3& inRight)
	{
		return (inLeft.mX * inRight.mX + inLeft.mY * inRight.mY + inLeft.mZ * inRight.mZ);
	}

	friend float Dot2D(const Vector3& inLeft, const Vector3& inRight)
	{
		return (inLeft.mX * inRight.mX + inLeft.mY * inRight.mY);
	}

	friend Vector3 Cross(const Vector3& inLeft, const Vector3& inRight)
	{
		Vector3 temp;
		temp.mX = inLeft.mY * inRight.mZ - inLeft.mZ * inRight.mY;
		temp.mY = inLeft.mZ * inRight.mX - inLeft.mX * inRight.mZ;
		temp.mZ = inLeft.mX * inRight.mY - inLeft.mY * inRight.mX;
		return temp;
	}

	friend Vector3 Lerp(const Vector3& inA, const Vector3& inB, float t)
	{
		return Vector3(inA + t * (inB - inA));
	}

	static const Vector3 Zero;
	static const Vector3 UnitX;
	static const Vector3 UnitY;
	static const Vector3 UnitZ;
};


class Quaternion
{
public:

	float		mX, mY, mZ, mW;

};


template< int tValue, int tBits >
struct GetRequiredBitsHelper
{
	enum { Value = GetRequiredBitsHelper< (tValue >> 1), tBits + 1 >::Value };
};

template< int tBits >
struct GetRequiredBitsHelper< 0, tBits >
{
	enum { Value = tBits };
};

template< int tValue >
struct GetRequiredBits
{
	enum { Value = GetRequiredBitsHelper< tValue, 0 >::Value };
};

namespace RoboMath
{
	const float PI = 3.1415926535f;
	float GetRandomFloat();

	Vector3 GetRandomVector(const Vector3& inMin, const Vector3& inMax);
	Vector3 GetPlayerSpawnPosition(int playerId); 

	inline bool	Is2DVectorEqual(const Vector3& inA, const Vector3& inB)
	{
		return (inA.mX == inB.mX && inA.mY == inB.mY);
	}

	inline float ToDegrees(float inRadians)
	{
		return inRadians * 180.0f / PI;
	}

	inline float ToRadians(float inDegrees)
	{
		return (inDegrees / 180.0f) * PI;
	}
}

namespace Colors
{
	// Text colours
	static const Vector3 TBlack(0.0f, 0.0f, 0.0f);
	static const Vector3 TWhite(255.0f, 255.0f, 255.0f);
	static const Vector3 TRed(255.0f, 0.0f, 0.0f);
	static const Vector3 TGreen(0.0f, 255.0f, 0.0f);
	static const Vector3 TBlue(0.0f, 0.0f, 255.0f);

	// Player colours
	static const Vector3 NeonRed(255.0f, 0.0f, 0.0f);
	static const Vector3 NeonOrange(255.0f, 102.0f, 0.0f);
	static const Vector3 NeonYellow(255.0f, 255.0f, 0.0f);
	static const Vector3 NeonGreen(57.0f, 255.0f, 20.0f);
	static const Vector3 NeonTurquoise(0.0f, 255.0f, 255.0f);
	static const Vector3 NeonBlue(0.0f, 153.0f, 255.0f);
	static const Vector3 NeonLavender(221.0f, 160.0f, 255.0f);
	static const Vector3 NeonViolet(148.0f, 0.0f, 211.0f);
	static const Vector3 NeonPink(255.0f, 20.0f, 147.0f);
	static const Vector3 NeonWhite(255.0f, 255.0f, 255.0f);

	static const Vector3 PastelRed(255.0f, 179.0f, 186.0f);
	static const Vector3 PastelOrange(255.0f, 223.0f, 186.0f);
	static const Vector3 PastelYellow(255.0f, 255.0f, 186.0f);
	static const Vector3 PastelGreen(186.0f, 255.0f, 201.0f);
	static const Vector3 PastelMint(204.0f, 255.0f, 229.0f);
	static const Vector3 PastelBlue(186.0f, 223.0f, 255.0f);
	static const Vector3 PastelIndigo(204.0f, 186.0f, 255.0f);
	static const Vector3 PastelViolet(230.0f, 186.0f, 255.0f);
	static const Vector3 PastelPink(255.0f, 186.0f, 250.0f);
	static const Vector3 PastelBrown(210.0f, 180.0f, 140.0f);
}

