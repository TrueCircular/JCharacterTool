#pragma once

class CameraMove : public MonoBehaviour
{
public:
	float _dt = 0.f;
	float _speed = 10.f;
	bool _isActive = false;
	Vec3 _lastMousePos = Vec3(0.f);
public:
	void SetActive(bool st) { _isActive = st; }
public:
	virtual void Start() override;
	virtual void Update() override;
};

