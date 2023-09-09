#include "Sphere.h"
#include <PrimitiveDrawer.h>
#include "../config/GlobalVariables.h"

void Sphere::Initialize(Vector3 posision, float radius) {

	colliderType_ = ColliderType::Sphere;

	center_ = posision;
	radius_ = radius;

}

void Sphere::Update(Vector3 posision, float radius) {

	center_ = posision;
	radius_ = radius;

}

void Sphere::Draw() {

	Vector4 color = {1.0f, 1.0f, 1.0f, 1.0f};

	const uint32_t kSubdivision = 30; // 分割数
	const float kLonEvery = 2.0f * float(std::numbers::pi) / float(kSubdivision); // 経度分割1つ分の角度
	const float kLatEvery = float(std::numbers::pi) / float(kSubdivision); // 緯度分割1つ分の角度
	// 緯度の方向に分割
	for (uint32_t latIndex = 0; latIndex < kSubdivision; ++latIndex) {
		float lat = -1.0f * float(std::numbers::pi) / 2.0f + kLatEvery * latIndex; // 現在の緯度
		// 経度の方向に分割
		for (uint32_t lonIndex = 0; lonIndex < kSubdivision; ++lonIndex) {
			float lon = lonIndex * kLonEvery; // 現在の経度
			// world座標系でのa,b,cを求める
			Vector3 a, b, c;
			a = {
			    radius_ * std::cosf(lat) * std::cosf(lon), radius_ * std::sinf(lat),
			    radius_ * std::cosf(lat) * std::sinf(lon)};
			a = a + center_;
			b = {
			    radius_ * std::cosf(lat + kLatEvery) * std::cosf(lon),
			    radius_ * std::sinf(lat + kLatEvery),
			    radius_ * std::cosf(lat + kLatEvery) * std::sinf(lon)};
			b = b + center_;
			c = {
			    radius_ * std::cosf(lat) * std::cosf(lon + kLonEvery),
			    radius_ * std::sinf(lat),
			    radius_ * std::cosf(lat) * std::sinf(lon + kLonEvery)};
			c = c + center_;

			// 線を引く
			PrimitiveDrawer::GetInstance()->DrawLine3d(a, b, color);
			PrimitiveDrawer::GetInstance()->DrawLine3d(a, c, color);
		}
	}

}

void Sphere::AddToGlobalVariables(const char* groupName) {
	
	// 調整項目クラスのインスタンス取得
	GlobalVariables* globalVariables = GlobalVariables::GetInstance();

	// メンバ変数の調整したい項目をグローバル変数に追加
	globalVariables->AddItem(groupName, "SphereRadius", radius_);

}

void Sphere::ApplyGlobalVariables(const char* groupName) {

	// 調整項目クラスのインスタンス取得
	GlobalVariables* globalVariables = GlobalVariables::GetInstance();

	// メンバ変数の調整項目をグローバル変数に追加
	radius_ = globalVariables->GetFloatValue(groupName, "SphereRadius");

}

Vector3 Sphere::LengthSphere(Vector3 rotation) { 

	Matrix4x4 rotateMatrix = MyMath::MakeRotateXYZMatrix(rotation);

	Vector3 unit =
	    MyMath::Normalize(MyMath::TransformNormal(Vector3(1.0f, 1.0f, 1.0f), rotateMatrix));

	return unit * radius_;

}


