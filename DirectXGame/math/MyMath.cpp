#include "MyMath.h"
#include "Math.h"

float MyMath::Linear(float t, float start, float end, float time) {
	return (float)((1.0 - t / time) * start + t / time * end);
}

int MyMath::Linear(float t, int start, int end, float time) {
	return (int)((1.0 - t / time) * (float)start + t / time * (float)end);
}

Vector3 MyMath::Linear(float t, Vector3 start, Vector3 end, float time) {
	// 結果格納用
	Vector3 result;

	result.x = (float)((1.0 - t / time) * start.x + t / time * end.x);
	result.y = (float)((1.0 - t / time) * start.y + t / time * end.y);
	result.z = (float)((1.0 - t / time) * start.z + t / time * end.z);

	return result;
}

float MyMath::Linear(float t, float start, float end) {
	return (float)((1.0 - t) * start + t * end);
}

int MyMath::Linear(float t, int start, int end) {
	return (int)((1.0 - t) * (float)start + t * (float)end);
}

float MyMath::EaseIn(float t, float start, float end, float time) {
	float easeT = (t / time) * (t / time);
	return (float)((1.0 - easeT / time) * start + easeT / time * end);
}

int MyMath::EaseIn(float t, int start, int end, float time) {
	float easeT = (t / time) * (t / time);
	return (int)((1.0 - easeT / time) * (float)start + easeT / time * (float)end);
}

Vector3 MyMath::EaseIn(float t, Vector3 start, Vector3 end, float time) {
	// 結果格納用
	Vector3 result;

	float easeT = (t / time) * (t / time);

	result.x = (float)((1.0 - easeT) * start.x + easeT * end.x);
	result.y = (float)((1.0 - easeT) * start.y + easeT * end.y);
	result.z = (float)((1.0 - easeT) * start.z + easeT * end.z);

	return result;
}

float MyMath::EaseIn(float t, float start, float end) {
	float easeT = t * t;
	return (float)((1.0 - easeT) * start + easeT * end);
}

int MyMath::EaseIn(float t, int start, int end) {
	float easeT = t * t;
	return (int)((1.0 - easeT) * (float)start + easeT * (float)end);
}

float MyMath::EaseOut(float t, float start, float end, float time) {
	float easeT = 1.0f - powf(1.0f - t / time, 3.0f);
	return (float)((1.0 - easeT) * start + easeT * end);
}

int MyMath::EaseOut(float t, int start, int end, float time) {
	float easeT = 1.0f - powf(1.0f - t / time, 3.0f);
	return (int)((1.0 - easeT) * (float)start + easeT * (float)end);
}

Vector3 MyMath::EaseOut(float t, Vector3 start, Vector3 end, float time) {
	// 結果格納用
	Vector3 result;

	float easeT = 1.0f - powf(1.0f - t / time, 3.0f);

	result.x = (float)((1.0 - easeT) * start.x + easeT * end.x);
	result.y = (float)((1.0 - easeT) * start.y + easeT * end.y);
	result.z = (float)((1.0 - easeT) * start.z + easeT * end.z);

	return result;
}

float MyMath::EaseOut(float t, float start, float end) {
	float easeT = 1.0f - powf(1.0f - t, 3.0f);
	return (float)((1.0 - easeT) * start + easeT * end);
}

int MyMath::EaseOut(float t, int start, int end) {
	float easeT = 1.0f - powf(1.0f - t, 3.0f);
	return (int)((1.0 - easeT) * (float)start + easeT * (float)end);
}

float MyMath::Clamp(float a, float min, float max) {
	if (a < min) {
		return min;
	} else if (max < a) {
		return max;
	}
	return a;
}

void MyMath::SetSrand() { srand(static_cast<unsigned int>(time(NULL))); }

int MyMath::Random(int min, int max) {
	return min +
	       static_cast<int>(
	           rand() * static_cast<float>(max - min + 1.0f) / static_cast<float>(1.0f + RAND_MAX));
}

float MyMath::RandomF(float min, float max, int num) {
	int Num = (int)pow(10, num);
	return static_cast<float>(
	    Random(static_cast<int>(max * Num), static_cast<int>(min * Num)) *
	    (1.0f / static_cast<float>(Num)));
}

float MyMath::Dot(const Vector3& v1, const Vector3& v2) {
	// 結果格納用
	float result;

	// 計算処理
	result = (v1.x * v2.x) + (v1.y * v2.y) + (v1.z * v2.z);

	return result;
}

float MyMath::Length(const Vector3& v) {
	// 計算処理
	return sqrtf(Dot(v, v));
}

Vector3 MyMath::Normalize(const Vector3& v) {
	// 正規化するベクトルの長さを求める
	float length = Length(v);
	// 結果格納用
	Vector3 result;

	// 計算処理
	if (v.x != 0.0f) {
		result.x = v.x / length;
	} else {
		result.x = 0.0f;
	}
	if (v.y != 0.0f) {
		result.y = v.y / length;
	} else {
		result.y = 0.0f;
	}
	if (v.z != 0.0f) {
		result.z = v.z / length;
	} else {
		result.z = 0.0f;
	}

	return result;
}

Vector3 MyMath::Transform(const Vector3& v, const Matrix4x4& m) {
	// 結果格納用
	Vector3 result;

	// 生成処理
	result.x = (v.x * m.m[0][0]) + (v.y * m.m[1][0]) + (v.z * m.m[2][0]) + (1.0f * m.m[3][0]);
	result.y = (v.x * m.m[0][1]) + (v.y * m.m[1][1]) + (v.z * m.m[2][1]) + (1.0f * m.m[3][1]);
	result.z = (v.x * m.m[0][2]) + (v.y * m.m[1][2]) + (v.z * m.m[2][2]) + (1.0f * m.m[3][2]);
	float w = (v.x * m.m[0][3]) + (v.y * m.m[1][3]) + (v.z * m.m[2][3]) + (1.0f * m.m[3][3]);

	assert(w != 0.0f);

	result.x /= w;
	result.y /= w;
	result.z /= w;

	return result;
}

Matrix4x4 MyMath::Inverse(const Matrix4x4 m) {
	// 結果格納用
	Matrix4x4 result = {};

	// 行列式
	float d;

	// 行列式を求める
	d = (m.m[0][0] * m.m[1][1] * m.m[2][2] * m.m[3][3]) +
	    (m.m[0][0] * m.m[1][2] * m.m[2][3] * m.m[3][1]) +
	    (m.m[0][0] * m.m[1][3] * m.m[2][1] * m.m[3][2]) -

	    (m.m[0][0] * m.m[1][3] * m.m[2][2] * m.m[3][1]) -
	    (m.m[0][0] * m.m[1][2] * m.m[2][1] * m.m[3][3]) -
	    (m.m[0][0] * m.m[1][1] * m.m[2][3] * m.m[3][2]) -

	    (m.m[0][1] * m.m[1][0] * m.m[2][2] * m.m[3][3]) -
	    (m.m[0][2] * m.m[1][0] * m.m[2][3] * m.m[3][1]) -
	    (m.m[0][3] * m.m[1][0] * m.m[2][1] * m.m[3][2]) +

	    (m.m[0][3] * m.m[1][0] * m.m[2][2] * m.m[3][1]) +
	    (m.m[0][2] * m.m[1][0] * m.m[2][1] * m.m[3][3]) +
	    (m.m[0][1] * m.m[1][0] * m.m[2][3] * m.m[3][2]) +

	    (m.m[0][1] * m.m[1][2] * m.m[2][0] * m.m[3][3]) +
	    (m.m[0][2] * m.m[1][3] * m.m[2][0] * m.m[3][1]) +
	    (m.m[0][3] * m.m[1][1] * m.m[2][0] * m.m[3][2]) -

	    (m.m[0][3] * m.m[1][2] * m.m[2][0] * m.m[3][1]) -
	    (m.m[0][2] * m.m[1][1] * m.m[2][0] * m.m[3][3]) -
	    (m.m[0][1] * m.m[1][3] * m.m[2][0] * m.m[3][2]) -

	    (m.m[0][1] * m.m[1][2] * m.m[2][3] * m.m[3][0]) -
	    (m.m[0][2] * m.m[1][3] * m.m[2][1] * m.m[3][0]) -
	    (m.m[0][3] * m.m[1][1] * m.m[2][2] * m.m[3][0]) +

	    (m.m[0][3] * m.m[1][2] * m.m[2][1] * m.m[3][0]) +
	    (m.m[0][2] * m.m[1][1] * m.m[2][3] * m.m[3][0]) +
	    (m.m[0][1] * m.m[1][3] * m.m[2][2] * m.m[3][0]);

	// 計算処理
	if (d != 0) {

		result.m[0][0] =
		    (1.0f / d) * (m.m[1][1] * m.m[2][2] * m.m[3][3] + m.m[1][2] * m.m[2][3] * m.m[3][1] +
		                  m.m[1][3] * m.m[2][1] * m.m[3][2] - m.m[1][3] * m.m[2][2] * m.m[3][1] -
		                  m.m[1][2] * m.m[2][1] * m.m[3][3] - m.m[1][1] * m.m[2][3] * m.m[3][2]);
		result.m[0][1] =
		    (1.0f / d) * (-m.m[0][1] * m.m[2][2] * m.m[3][3] - m.m[0][2] * m.m[2][3] * m.m[3][1] -
		                  m.m[0][3] * m.m[2][1] * m.m[3][2] + m.m[0][3] * m.m[2][2] * m.m[3][1] +
		                  m.m[0][2] * m.m[2][1] * m.m[3][3] + m.m[0][1] * m.m[2][3] * m.m[3][2]);
		result.m[0][2] =
		    (1.0f / d) * (+m.m[0][1] * m.m[1][2] * m.m[3][3] + m.m[0][2] * m.m[1][3] * m.m[3][1] +
		                  m.m[0][3] * m.m[1][1] * m.m[3][2] - m.m[0][3] * m.m[1][2] * m.m[3][1] -
		                  m.m[0][2] * m.m[1][1] * m.m[3][3] - m.m[0][1] * m.m[1][3] * m.m[3][2]);
		result.m[0][3] =
		    (1.0f / d) * (-m.m[0][1] * m.m[1][2] * m.m[2][3] - m.m[0][2] * m.m[1][3] * m.m[2][1] -
		                  m.m[0][3] * m.m[1][1] * m.m[2][2] + m.m[0][3] * m.m[1][2] * m.m[2][1] +
		                  m.m[0][2] * m.m[1][1] * m.m[2][3] + m.m[0][1] * m.m[1][3] * m.m[2][2]);

		result.m[1][0] =
		    (1.0f / d) * (-m.m[1][0] * m.m[2][2] * m.m[3][3] - m.m[1][2] * m.m[2][3] * m.m[3][0] -
		                  m.m[1][3] * m.m[2][0] * m.m[3][2] + m.m[1][3] * m.m[2][2] * m.m[3][0] +
		                  m.m[1][2] * m.m[2][0] * m.m[3][3] + m.m[1][0] * m.m[2][3] * m.m[3][2]);
		result.m[1][1] =
		    (1.0f / d) * (m.m[0][0] * m.m[2][2] * m.m[3][3] + m.m[0][2] * m.m[2][3] * m.m[3][0] +
		                  m.m[0][3] * m.m[2][0] * m.m[3][2] - m.m[0][3] * m.m[2][2] * m.m[3][0] -
		                  m.m[0][2] * m.m[2][0] * m.m[3][3] - m.m[0][0] * m.m[2][3] * m.m[3][2]);
		result.m[1][2] =
		    (1.0f / d) * (-m.m[0][0] * m.m[1][2] * m.m[3][3] - m.m[0][2] * m.m[1][3] * m.m[3][0] -
		                  m.m[0][3] * m.m[1][0] * m.m[3][2] + m.m[0][3] * m.m[1][2] * m.m[3][0] +
		                  m.m[0][2] * m.m[1][0] * m.m[3][3] + m.m[0][0] * m.m[1][3] * m.m[3][2]);
		result.m[1][3] =
		    (1.0f / d) * (m.m[0][0] * m.m[1][2] * m.m[2][3] + m.m[0][2] * m.m[1][3] * m.m[2][0] +
		                  m.m[0][3] * m.m[1][0] * m.m[2][2] - m.m[0][3] * m.m[1][2] * m.m[2][0] -
		                  m.m[0][2] * m.m[1][0] * m.m[2][3] - m.m[0][0] * m.m[1][3] * m.m[2][2]);

		result.m[2][0] =
		    (1.0f / d) * (m.m[1][0] * m.m[2][1] * m.m[3][3] + m.m[1][1] * m.m[2][3] * m.m[3][0] +
		                  m.m[1][3] * m.m[2][0] * m.m[3][1] - m.m[1][3] * m.m[2][1] * m.m[3][0] -
		                  m.m[1][1] * m.m[2][0] * m.m[3][3] - m.m[1][0] * m.m[2][3] * m.m[3][1]);
		result.m[2][1] =
		    (1.0f / d) * (-m.m[0][0] * m.m[2][1] * m.m[3][3] - m.m[0][1] * m.m[2][3] * m.m[3][0] -
		                  m.m[0][3] * m.m[2][0] * m.m[3][1] + m.m[0][3] * m.m[2][1] * m.m[3][0] +
		                  m.m[0][1] * m.m[2][0] * m.m[3][3] + m.m[0][0] * m.m[2][3] * m.m[3][1]);
		result.m[2][2] =
		    (1.0f / d) * (m.m[0][0] * m.m[1][1] * m.m[3][3] + m.m[0][1] * m.m[1][3] * m.m[3][0] +
		                  m.m[0][3] * m.m[1][0] * m.m[3][1] - m.m[0][3] * m.m[1][1] * m.m[3][0] -
		                  m.m[0][1] * m.m[1][0] * m.m[3][3] - m.m[0][0] * m.m[1][3] * m.m[3][1]);
		result.m[2][3] =
		    (1.0f / d) * (-m.m[0][0] * m.m[1][1] * m.m[2][3] - m.m[0][1] * m.m[1][3] * m.m[2][0] -
		                  m.m[0][3] * m.m[1][0] * m.m[2][1] + m.m[0][3] * m.m[1][1] * m.m[2][0] +
		                  m.m[0][1] * m.m[1][0] * m.m[2][3] + m.m[0][0] * m.m[1][3] * m.m[2][1]);

		result.m[3][0] =
		    (1.0f / d) * (-m.m[1][0] * m.m[2][1] * m.m[3][2] - m.m[1][1] * m.m[2][2] * m.m[3][0] -
		                  m.m[1][2] * m.m[2][0] * m.m[3][1] + m.m[1][2] * m.m[2][1] * m.m[3][0] +
		                  m.m[1][1] * m.m[2][0] * m.m[3][2] + m.m[1][0] * m.m[2][2] * m.m[3][1]);
		result.m[3][1] =
		    (1.0f / d) * (m.m[0][0] * m.m[2][1] * m.m[3][2] + m.m[0][1] * m.m[2][2] * m.m[3][0] +
		                  m.m[0][2] * m.m[2][0] * m.m[3][1] - m.m[0][2] * m.m[2][1] * m.m[3][0] -
		                  m.m[0][1] * m.m[2][0] * m.m[3][2] - m.m[0][0] * m.m[2][2] * m.m[3][1]);
		result.m[3][2] =
		    (1.0f / d) * (-m.m[0][0] * m.m[1][1] * m.m[3][2] - m.m[0][1] * m.m[1][2] * m.m[3][0] -
		                  m.m[0][2] * m.m[1][0] * m.m[3][1] + m.m[0][2] * m.m[1][1] * m.m[3][0] +
		                  m.m[0][1] * m.m[1][0] * m.m[3][2] + m.m[0][0] * m.m[1][2] * m.m[3][1]);
		result.m[3][3] =
		    (1.0f / d) * (m.m[0][0] * m.m[1][1] * m.m[2][2] + m.m[0][1] * m.m[1][2] * m.m[2][0] +
		                  m.m[0][2] * m.m[1][0] * m.m[2][1] - m.m[0][2] * m.m[1][1] * m.m[2][0] -
		                  m.m[0][1] * m.m[1][0] * m.m[2][2] - m.m[0][0] * m.m[1][2] * m.m[2][1]);
	}

	return result;
}

Matrix4x4 MyMath::MakeIdentity4x4() {
	// 結果格納用
	Matrix4x4 result{};

	// 生成処理
	result.m[0][0] = 1.0f;
	result.m[0][1] = 0.0f;
	result.m[0][2] = 0.0f;
	result.m[0][3] = 0.0f;

	result.m[1][0] = 0.0f;
	result.m[1][1] = 1.0f;
	result.m[1][2] = 0.0f;
	result.m[1][3] = 0.0f;

	result.m[2][0] = 0.0f;
	result.m[2][1] = 0.0f;
	result.m[2][2] = 1.0f;
	result.m[2][3] = 0.0f;

	result.m[3][0] = 0.0f;
	result.m[3][1] = 0.0f;
	result.m[3][2] = 0.0f;
	result.m[3][3] = 1.0f;

	// 生成したものを返す
	return result;
}

Matrix4x4 MyMath::MakeTranslateMatrix(const Vector3& translate) {
	// 結果格納用単位行列を生成
	Matrix4x4 result = MakeIdentity4x4();

	// 計算処理
	result.m[3][0] = translate.x;
	result.m[3][1] = translate.y;
	result.m[3][2] = translate.z;

	// 生成結果を返す
	return result;
}

Matrix4x4 MyMath::MakeScaleMatrix(const Vector3& scale) {
	// 結果格納用単位行列を生成
	Matrix4x4 result = MakeIdentity4x4();

	// 計算処理
	result.m[0][0] = scale.x;
	result.m[1][1] = scale.y;
	result.m[2][2] = scale.z;

	// 生成結果を返す
	return result;
}

Matrix4x4 MyMath::MakeRotateXMatrix(const float& X) {
	// 結果格納用単位行列を生成
	Matrix4x4 result = MakeIdentity4x4();

	// 計算処理
	result.m[1][1] = std::cosf(X);
	result.m[1][2] = std::sinf(X);
	result.m[2][1] = -(std::sinf(X));
	result.m[2][2] = std::cosf(X);

	// 生成結果を返す
	return result;
}

Matrix4x4 MyMath::MakeRotateYMatrix(const float& Y) {
	// 結果格納用単位行列を生成
	Matrix4x4 result = MakeIdentity4x4();

	// 計算処理
	result.m[0][0] = std::cosf(Y);
	result.m[0][2] = -(std::sinf(Y));
	result.m[2][0] = std::sinf(Y);
	result.m[2][2] = std::cosf(Y);

	// 生成結果を返す
	return result;
}

Matrix4x4 MyMath::MakeRotateZMatrix(const float& Z) {
	// 結果格納用単位行列を生成
	Matrix4x4 result = MakeIdentity4x4();

	// 計算処理
	result.m[0][0] = std::cosf(Z);
	result.m[0][1] = std::sinf(Z);
	result.m[1][0] = -(std::sinf(Z));
	result.m[1][1] = std::cosf(Z);
	// 生成結果を返す
	return result;
}

Matrix4x4 MyMath::MakeRotateXYZMatrix(const Vector3& rotate) {
	// 結果格納用単位行列を生成
	Matrix4x4 result = MakeIdentity4x4();

	// 計算処理
	Matrix4x4 tempRotateXMatrix = MakeRotateXMatrix(rotate.x);
	Matrix4x4 tempRotateYMatrix = MakeRotateYMatrix(rotate.y);
	Matrix4x4 tempRotateZMatrix = MakeRotateZMatrix(rotate.z);

	result = tempRotateXMatrix * tempRotateYMatrix * tempRotateZMatrix;

	// 生成結果を返す
	return result;
}

Matrix4x4
    MyMath::MakeAffineMatrix(const Vector3& scale, const Vector3& rotate, const Vector3& translate) {
	// 結果格納用単位行列を生成
	Matrix4x4 result = MakeIdentity4x4();

	// 計算処理
	Matrix4x4 S = MakeScaleMatrix(scale);
	Matrix4x4 R = MakeRotateXYZMatrix(rotate);
	Matrix4x4 T = MakeTranslateMatrix(translate);

	result = S * R * T;

	// 生成結果を返す
	return result;
}

Matrix4x4 MyMath::MakeOrthGraphicMatrix(
    const float& left, const float& top, const float& right, const float& bottom,
    const float& nearClip, const float& farClip) {
	// 結果格納用単位行列を生成
	Matrix4x4 result = MakeIdentity4x4();

	// 計算処理
	result.m[0][0] = 2 / (right - left);
	result.m[1][1] = 2 / (top - bottom);
	result.m[2][2] = 1 / (farClip - nearClip);
	result.m[3][0] = (left + right) / (left - right);
	result.m[3][1] = (top + bottom) / (bottom - top);
	result.m[3][2] = nearClip / (nearClip - farClip);

	return result;
}

Matrix4x4 MyMath::MakePerspectiveFovMatrix(
    const float& fovY, const float& aspectRatio, const float& nearClip, const float& farClip) {
	// 結果格納用単位行列を生成
	Matrix4x4 result = MakeIdentity4x4();

	// 計算処理
	result.m[0][0] = (1 / aspectRatio) * (1 / tanf(fovY / 2));
	result.m[1][1] = (1 / tanf(fovY / 2));
	result.m[2][2] = farClip / (farClip - nearClip);
	result.m[2][3] = 1.0f;
	result.m[3][2] = -(nearClip * farClip) / (farClip - nearClip);
	result.m[3][3] = 0.0f;

	return result;
}

Matrix4x4 MyMath::MakeViewportMatrix(
    float left, float top, float width, float height, float minDepth, float maxDepth) {
	// 結果格納用単位行列を生成
	Matrix4x4 result = MakeIdentity4x4();

	// 計算処理
	result.m[0][0] = width / 2;
	result.m[1][1] = -height / 2;
	result.m[2][2] = maxDepth - minDepth;
	result.m[3][0] = left + (width / 2);
	result.m[3][1] = top + (height / 2);
	result.m[3][2] = minDepth;

	return result;
}

Vector3 MyMath::TransformNormal(const Vector3& vector, const Matrix4x4& matrix) {

	Vector3 result = {};

	result.x = vector.x * matrix.m[0][0] + vector.y * matrix.m[1][0] + vector.z * matrix.m[2][0];
	result.y = vector.x * matrix.m[0][1] + vector.y * matrix.m[1][1] + vector.z * matrix.m[2][1];
	result.z = vector.x * matrix.m[0][2] + vector.y * matrix.m[1][2] + vector.z * matrix.m[2][2];

	return result;
}

Vector3 MyMath::Cross(const Vector3& v1, const Vector3& v2) { 
	
	Vector3 result = {
	    v1.y * v2.z - v1.z * v2.y,
	    v1.z * v2.x - v1.x * v2.z,
	    v1.x * v2.y - v1.y * v2.x,
	};

	return result;
}
