#pragma once
#include "Matrix4x4.h"
#include "Vector3.h"
#include <assert.h>
#include <cmath>


	static Vector3 Add(const Vector3& v1, const Vector3& v2) {
		Vector3 result{};
		result.x = v1.x + v2.x;
		result.y = v1.y + v2.y;
		result.z = v1.z + v2.z;
		return result;
	}

	static Vector3 Subtract(const Vector3& v1, const Vector3& v2) {
		Vector3 result{};

		result.x = v1.x - v2.x;
		result.y = v1.y - v2.y;
		result.z = v1.z - v2.z;
		return result;
	}

	static Matrix4x4 Multiply(const Matrix4x4& m1, const Matrix4x4& m2) {
		Matrix4x4 result{};
		for (int x = 0; x < 4; x++) {
			for (int y = 0; y < 4; y++) {
				result.m[x][y] = m1.m[x][0] * m2.m[0][y] + m1.m[x][1] * m2.m[1][y] +
				                 m1.m[x][2] * m2.m[2][y] + m1.m[x][3] * m2.m[3][y];
			}
		}
		return result;
	}

	static Vector3 FVMultiply(const float& v1, const Vector3& v2) {
		Vector3 result{};
		result.x = v1 * v2.x;
		result.y = v1 * v2.y;
		result.z = v1 * v2.z;
		return result;
	}

	static Vector3 MVMultiply(const Vector3& v, const Matrix4x4& m) {
		Vector3 result{};

		result.x = m.m[0][0] * v.x + m.m[0][1] * v.y + m.m[0][2] * v.z;
		result.y = m.m[1][0] * v.x + m.m[1][1] * v.y + m.m[1][2] * v.z;
		result.z = m.m[2][0] * v.x + m.m[2][1] * v.y + m.m[2][2] * v.z;

		return result;
	}

	static Matrix4x4 MMMultiply(const Matrix4x4& m1, const Matrix4x4& m2) {
		Matrix4x4 result{};

		for (int row = 0; row < 4; row++) {
			for (int column = 0; column < 4; column++) {
				result.m[row][column] =
				    m1.m[row][0] * m2.m[0][column] + m1.m[row][1] * m2.m[1][column] +
				    m1.m[row][2] * m2.m[2][column] + m1.m[row][3] * m2.m[3][column];
			}
		}
		return result;
	}

	static Vector3 TransformCoord(Vector3& vector, Matrix4x4& matrix) {
		Vector3 result{};
		result.x = vector.x * matrix.m[0][0] + vector.y * matrix.m[1][0] +
		           vector.z * matrix.m[2][0] + 1.0f * matrix.m[3][0];
		result.y = vector.x * matrix.m[0][1] + vector.y * matrix.m[1][1] +
		           vector.z * matrix.m[2][1] + 1.0f * matrix.m[3][1];
		result.z = vector.x * matrix.m[0][2] + vector.y * matrix.m[1][2] +
		           vector.z * matrix.m[2][2] + 1.0f * matrix.m[3][2];

		float w = vector.x * matrix.m[0][3] + vector.y * matrix.m[1][3] +
		          vector.z * matrix.m[2][3] + 1.0f * matrix.m[3][3];
		assert(w != 0.0f);
		result.x /= w;
		result.y /= w;
		result.z /= w;

		return result;
	}

	static Matrix4x4
	    MakePerspectiveFovMatrix(float fovY, float aspectRetio, float nearClip, float farClip) {
		Matrix4x4 result{};

		for (int i = 0; i < 4; i++) {
			for (int j = 0; j < 4; j++) {
				result.m[i][j] = 0;
			}
		}

		result.m[0][0] = (1 / aspectRetio) * (1 / tan(fovY / 2));
		result.m[1][1] = (1 / tan(fovY / 2));
		result.m[2][2] = farClip / (farClip - nearClip);
		result.m[2][3] = 1;
		result.m[3][2] = (-nearClip * farClip) / (farClip - nearClip);

		return result;
	}

	static Matrix4x4 MakeOrthographicMatrix(
	    float left, float top, float right, float bottom, float nearClip, float farClip) {
		Matrix4x4 result{};
		for (int i = 0; i < 4; i++) {
			for (int j = 0; j < 4; j++) {
				result.m[i][j] = 0;
			}
		}

		result.m[0][0] = 2 / (right - left);
		result.m[1][1] = 2 / (top - bottom);
		result.m[2][2] = 1 / (farClip - nearClip);
		result.m[3][3] = 1;

		result.m[3][0] = (left + right) / (left - right);
		result.m[3][1] = (top + bottom) / (bottom - top);
		result.m[3][2] = (nearClip) / (nearClip - farClip);

		return result;
	}

	static Matrix4x4 MakeViewPortMatrix(
	    float left, float top, float width, float height, float minDepth, float maxDepth) {
		Matrix4x4 result{};
		for (int i = 0; i < 4; i++) {
			for (int j = 0; j < 4; j++) {
				result.m[i][j] = 0;
			}
		}
		result.m[0][0] = width / 2;
		result.m[1][1] = -(height / 2);
		result.m[2][2] = maxDepth - minDepth;
		result.m[3][0] = left + (width / 2);
		result.m[3][1] = top + (height / 2);
		result.m[3][2] = minDepth;
		result.m[3][3] = 1;

		return result;
	}

	static Matrix4x4 MakeTranslateMatrix(const Vector3& translate) {
		Matrix4x4 result{};
		for (int i = 0; i < 4; i++) {
			for (int y = 0; y < 4; y++) {
				result.m[i][y] = 0;
			}
		}

		for (int i = 0; i < 4; i++) {
			result.m[i][i] = 1;
		}

		result.m[3][0] = translate.x;
		result.m[3][1] = translate.y;
		result.m[3][2] = translate.z;

		return result;
	}

	static Matrix4x4 MakeScaleMatrix(const Vector3& scale) {
		Matrix4x4 result{};
		for (int i = 0; i < 4; i++) {
			for (int y = 0; y < 4; y++) {
				result.m[i][y] = 0;
			}
		}

		result.m[0][0] = scale.x;
		result.m[1][1] = scale.y;
		result.m[2][2] = scale.z;
		result.m[3][3] = 1;

		return result;
	}

	static Matrix4x4 MakeRotateXMatrix(float radian) {
		Matrix4x4 result;
		result.m[0][0] = 1;
		result.m[0][1] = 0;
		result.m[0][2] = 0;
		result.m[0][3] = 0;

		result.m[1][0] = 0;
		result.m[1][1] = std::cos(radian);
		result.m[1][2] = std::sin(radian);
		result.m[1][3] = 0;

		result.m[2][0] = 0;
		result.m[2][1] = -std::sin(radian);
		result.m[2][2] = std::cos(radian);
		result.m[2][3] = 0;

		result.m[3][0] = 0;
		result.m[3][1] = 0;
		result.m[3][2] = 0;
		result.m[3][3] = 1;
		return result;
	}

	static Matrix4x4 MakeRotateYMatrix(float radian) {
		Matrix4x4 result;
		result.m[0][0] = std::cos(radian);
		result.m[0][1] = 0;
		result.m[0][2] = -std::sin(radian);
		result.m[0][3] = 0;

		result.m[1][0] = 0;
		result.m[1][1] = 1;
		result.m[1][2] = 0;
		result.m[1][3] = 0;

		result.m[2][0] = std::sin(radian);
		result.m[2][1] = 0;
		result.m[2][2] = std::cos(radian);
		result.m[2][3] = 0;

		result.m[3][0] = 0;
		result.m[3][1] = 0;
		result.m[3][2] = 0;
		result.m[3][3] = 1;
		return result;
	}

	static Matrix4x4 MakeRotateZMatrix(float radian) {
		Matrix4x4 result;
		result.m[0][0] = std::cos(radian);
		result.m[0][1] = std::sin(radian);
		result.m[0][2] = 0;
		result.m[0][3] = 0;

		result.m[1][0] = -std::sin(radian);
		result.m[1][1] = std::cos(radian);
		result.m[1][2] = 0;
		result.m[1][3] = 0;

		result.m[2][0] = 0;
		result.m[2][1] = 0;
		result.m[2][2] = 1;
		result.m[2][3] = 0;

		result.m[3][0] = 0;
		result.m[3][1] = 0;
		result.m[3][2] = 0;
		result.m[3][3] = 1;
		return result;
	}

	static Matrix4x4
	    MakeAffineMatrix(const Vector3& scale, const Vector3& rotate, const Vector3& translate) {
		Matrix4x4 rotateXMatrix = MakeRotateXMatrix(rotate.x);
		Matrix4x4 rotateYMatrix = MakeRotateYMatrix(rotate.y);
		Matrix4x4 rotateZMatrix = MakeRotateZMatrix(rotate.z);

		Matrix4x4 rotateMatrix = Multiply(rotateXMatrix, Multiply(rotateYMatrix, rotateZMatrix));

		Matrix4x4 result;

		result.m[0][0] = scale.x * rotateMatrix.m[0][0];
		result.m[0][1] = scale.x * rotateMatrix.m[0][1];
		result.m[0][2] = scale.x * rotateMatrix.m[0][2];
		result.m[0][3] = 0.0f;

		result.m[1][0] = scale.y * rotateMatrix.m[1][0];
		result.m[1][1] = scale.y * rotateMatrix.m[1][1];
		result.m[1][2] = scale.y * rotateMatrix.m[1][2];
		result.m[1][3] = 0.0f;

		result.m[2][0] = scale.z * rotateMatrix.m[2][0];
		result.m[2][1] = scale.z * rotateMatrix.m[2][1];
		result.m[2][2] = scale.z * rotateMatrix.m[2][2];
		result.m[2][3] = 0.0f;

		result.m[3][0] = translate.x;
		result.m[3][1] = translate.y;
		result.m[3][2] = translate.z;
		result.m[3][3] = 1.0f;

		return result;
	}

	static Vector3 TransformNormal(const Vector3& v, const Matrix4x4& m) {
		Vector3 result{
		    v.x * m.m[0][0] + v.y * m.m[1][0] + v.z * m.m[2][0],
		    v.x * m.m[0][1] + v.y * m.m[1][1] + v.z * m.m[2][1],
		    v.x * m.m[0][2] + v.y * m.m[1][2] + v.z * m.m[2][2]};
		return result;
	}

	static float Length(const Vector3& v) {
		float result{};

		result = sqrtf(v.x * v.x + v.y * v.y + v.z * v.z);

		return result;
	}

	static Vector3 Normalize(const Vector3& v) {
		Vector3 result{};

		result.x = v.x / Length(v);
		result.y = v.y / Length(v);
		result.z = v.z / Length(v);

		return result;
	}

	static Matrix4x4 Inverse(const Matrix4x4& m) {
		float a = m.m[0][0] * m.m[1][1] * m.m[2][2] * m.m[3][3] +
		          m.m[0][0] * m.m[1][2] * m.m[2][3] * m.m[3][1] +
		          m.m[0][0] * m.m[1][3] * m.m[2][1] * m.m[3][2]

		          - m.m[0][0] * m.m[1][3] * m.m[2][2] * m.m[3][1] -
		          m.m[0][0] * m.m[1][2] * m.m[2][1] * m.m[3][3] -
		          m.m[0][0] * m.m[1][1] * m.m[2][3] * m.m[3][2]

		          - m.m[0][1] * m.m[1][0] * m.m[2][2] * m.m[3][3] -
		          m.m[0][2] * m.m[1][0] * m.m[2][3] * m.m[3][1] -
		          m.m[0][3] * m.m[1][0] * m.m[2][1] * m.m[3][2]

		          + m.m[0][3] * m.m[1][0] * m.m[2][2] * m.m[3][1] +
		          m.m[0][2] * m.m[1][0] * m.m[2][1] * m.m[3][3] +
		          m.m[0][1] * m.m[1][0] * m.m[2][3] * m.m[3][2]

		          + m.m[0][1] * m.m[1][2] * m.m[2][0] * m.m[3][3] +
		          m.m[0][2] * m.m[1][3] * m.m[2][0] * m.m[3][1] +
		          m.m[0][3] * m.m[1][1] * m.m[2][0] * m.m[3][2]

		          - m.m[0][3] * m.m[1][2] * m.m[2][0] * m.m[3][1] -
		          m.m[0][2] * m.m[1][1] * m.m[2][0] * m.m[3][3] -
		          m.m[0][1] * m.m[1][3] * m.m[2][0] * m.m[3][2]

		          - m.m[0][1] * m.m[1][2] * m.m[2][3] * m.m[3][0] -
		          m.m[0][2] * m.m[1][3] * m.m[2][1] * m.m[3][0] -
		          m.m[0][3] * m.m[1][1] * m.m[2][2] * m.m[3][0]

		          + m.m[0][3] * m.m[1][2] * m.m[2][1] * m.m[3][0] +
		          m.m[0][2] * m.m[1][1] * m.m[2][3] * m.m[3][0] +
		          m.m[0][1] * m.m[1][3] * m.m[2][2] * m.m[3][0];

		Matrix4x4 result{};
		result.m[0][0] = (m.m[1][1] * m.m[2][2] * m.m[3][3] + m.m[1][2] * m.m[2][3] * m.m[3][1] +
		                  m.m[1][3] * m.m[2][1] * m.m[3][2]

		                  - m.m[1][3] * m.m[2][2] * m.m[3][1] - m.m[1][2] * m.m[2][1] * m.m[3][3] -
		                  m.m[1][1] * m.m[2][3] * m.m[3][2]) /
		                 a;

		result.m[0][1] = (-m.m[0][1] * m.m[2][2] * m.m[3][3] - m.m[0][2] * m.m[2][3] * m.m[3][1] -
		                  m.m[0][3] * m.m[2][1] * m.m[3][2]

		                  + m.m[0][3] * m.m[2][2] * m.m[3][1] + m.m[0][2] * m.m[2][1] * m.m[3][3] +
		                  m.m[0][1] * m.m[2][3] * m.m[3][2]) /
		                 a;

		result.m[0][2] = (m.m[0][1] * m.m[1][2] * m.m[3][3] + m.m[0][2] * m.m[1][3] * m.m[3][1] +
		                  m.m[0][3] * m.m[1][1] * m.m[3][2]

		                  - m.m[0][3] * m.m[1][2] * m.m[3][1] - m.m[0][2] * m.m[1][1] * m.m[3][3] -
		                  m.m[0][1] * m.m[1][3] * m.m[3][2]) /
		                 a;

		result.m[0][3] = (-m.m[0][1] * m.m[1][2] * m.m[2][3] - m.m[0][2] * m.m[1][3] * m.m[2][1] -
		                  m.m[0][3] * m.m[1][1] * m.m[2][2]

		                  + m.m[0][3] * m.m[1][2] * m.m[2][1] + m.m[0][2] * m.m[1][1] * m.m[2][3] +
		                  m.m[0][1] * m.m[1][3] * m.m[2][2]) /
		                 a;

		result.m[1][0] = (-m.m[1][0] * m.m[2][2] * m.m[3][3] - m.m[1][2] * m.m[2][3] * m.m[3][0] -
		                  m.m[1][3] * m.m[2][0] * m.m[3][2]

		                  + m.m[1][3] * m.m[2][2] * m.m[3][0] + m.m[1][2] * m.m[2][0] * m.m[3][3] +
		                  m.m[1][0] * m.m[2][3] * m.m[3][2]) /
		                 a;

		result.m[1][1] = (m.m[0][0] * m.m[2][2] * m.m[3][3] + m.m[0][2] * m.m[2][3] * m.m[3][0] +
		                  m.m[0][3] * m.m[2][0] * m.m[3][2]

		                  - m.m[0][3] * m.m[2][2] * m.m[3][0] - m.m[0][2] * m.m[2][0] * m.m[3][3] -
		                  m.m[0][0] * m.m[2][3] * m.m[3][2]) /
		                 a;

		result.m[1][2] = (-m.m[0][0] * m.m[1][2] * m.m[3][3] - m.m[0][2] * m.m[1][3] * m.m[3][0] -
		                  m.m[0][3] * m.m[1][0] * m.m[3][2]

		                  + m.m[0][3] * m.m[1][2] * m.m[3][0] + m.m[0][2] * m.m[1][0] * m.m[3][3] +
		                  m.m[0][0] * m.m[1][3] * m.m[3][2]) /
		                 a;

		result.m[1][3] = (m.m[0][0] * m.m[1][2] * m.m[2][3] + m.m[0][2] * m.m[1][3] * m.m[2][0] +
		                  m.m[0][3] * m.m[1][0] * m.m[2][2]

		                  - m.m[0][3] * m.m[1][2] * m.m[2][0] - m.m[0][2] * m.m[1][0] * m.m[2][3] -
		                  m.m[0][0] * m.m[1][3] * m.m[2][2]) /
		                 a;

		result.m[2][0] = (m.m[1][0] * m.m[2][1] * m.m[3][3] + m.m[1][1] * m.m[2][3] * m.m[3][0] +
		                  m.m[1][3] * m.m[2][0] * m.m[3][1]

		                  - m.m[1][3] * m.m[2][1] * m.m[3][0] - m.m[1][1] * m.m[2][0] * m.m[3][3] -
		                  m.m[1][0] * m.m[2][3] * m.m[3][1]) /
		                 a;

		result.m[2][1] = (-m.m[0][0] * m.m[2][1] * m.m[3][3] - m.m[0][1] * m.m[2][3] * m.m[3][0] -
		                  m.m[0][3] * m.m[2][0] * m.m[3][1]

		                  + m.m[0][3] * m.m[2][1] * m.m[3][0] + m.m[0][1] * m.m[2][0] * m.m[3][3] +
		                  m.m[0][0] * m.m[2][3] * m.m[3][1]) /
		                 a;

		result.m[2][2] = (m.m[0][0] * m.m[1][1] * m.m[3][3] + m.m[0][1] * m.m[1][3] * m.m[3][0] +
		                  m.m[0][3] * m.m[1][0] * m.m[3][1]

		                  - m.m[0][3] * m.m[1][1] * m.m[3][0] - m.m[0][1] * m.m[1][0] * m.m[3][3] -
		                  m.m[0][0] * m.m[1][3] * m.m[3][1]) /
		                 a;

		result.m[2][3] = (-m.m[0][0] * m.m[1][1] * m.m[2][3] - m.m[0][1] * m.m[1][3] * m.m[2][0] -
		                  m.m[0][3] * m.m[1][0] * m.m[2][1]

		                  + m.m[0][3] * m.m[1][1] * m.m[2][0] + m.m[0][1] * m.m[1][0] * m.m[2][3] +
		                  m.m[0][0] * m.m[1][3] * m.m[2][1]) /
		                 a;

		result.m[3][0] = (-m.m[1][0] * m.m[2][1] * m.m[3][2] - m.m[1][1] * m.m[2][2] * m.m[3][0] -
		                  m.m[1][2] * m.m[2][0] * m.m[3][1]

		                  + m.m[1][2] * m.m[2][1] * m.m[3][0] + m.m[1][1] * m.m[2][0] * m.m[3][2] +
		                  m.m[1][0] * m.m[2][2] * m.m[3][1]) /
		                 a;

		result.m[3][1] = (m.m[0][0] * m.m[2][1] * m.m[3][2] + m.m[0][1] * m.m[2][2] * m.m[3][0] +
		                  m.m[0][2] * m.m[2][0] * m.m[3][1]

		                  - m.m[0][2] * m.m[2][1] * m.m[3][0] - m.m[0][1] * m.m[2][0] * m.m[3][2] -
		                  m.m[0][0] * m.m[2][2] * m.m[3][1]) /
		                 a;

		result.m[3][2] = (-m.m[0][0] * m.m[1][1] * m.m[3][2] - m.m[0][1] * m.m[1][2] * m.m[3][0] -
		                  m.m[0][2] * m.m[1][0] * m.m[3][1]

		                  + m.m[0][2] * m.m[1][1] * m.m[3][0] + m.m[0][1] * m.m[1][0] * m.m[3][2] +
		                  m.m[0][0] * m.m[1][2] * m.m[3][1]) /
		                 a;

		result.m[3][3] = (m.m[0][0] * m.m[1][1] * m.m[2][2] + m.m[0][1] * m.m[1][2] * m.m[2][0] +
		                  m.m[0][2] * m.m[1][0] * m.m[2][1]

		                  - m.m[0][2] * m.m[1][1] * m.m[2][0] - m.m[0][1] * m.m[1][0] * m.m[2][2] -
		                  m.m[0][0] * m.m[1][2] * m.m[2][1]) /
		                 a;

		return result;
	}
