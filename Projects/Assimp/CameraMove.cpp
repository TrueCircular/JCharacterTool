#include "pch.h"
#include "CameraMove.h"

void CameraMove::Start()
{
}

void CameraMove::Update()
{
	_dt = MANAGER_TIME()->GetDeltaTime();

	Vec3 pos = GetTransform()->GetPosition();

	//translate
	if (MANAGER_INPUT()->GetButton(KEY_TYPE::W))
	{
		pos += GetTransform()->GetLookVector() * _speed * _dt;
	}
	if (MANAGER_INPUT()->GetButton(KEY_TYPE::S))
	{
		pos -= GetTransform()->GetLookVector() * _speed * _dt;
	}

	GetTransform()->SetPosition(pos);

	//Camera Rotation
	{
		Vec3 mPos = MANAGER_INPUT()->GetWorldMousePos();
		wstring wx = L"POS.X :";
		wx += to_wstring(mPos.x);
		wx += L"\n";
		wstring wy = L"POS.Y :";
		wy += to_wstring(mPos.x);
		wy += L"\n";
		OutputDebugString(wx.c_str());
		OutputDebugString(wy.c_str());
	}
}
