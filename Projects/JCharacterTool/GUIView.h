#pragma once
#include "GUIInterface.h"
#include "CameraMove.h"

#define MAX_SKELETAL_ASSET_COUNT (int)500
#define MAX_STATIC_ASSET_COUNT (int)500

class GUIView : public GUIInterface
{
	friend class GUIFile;
	using Super = GUIInterface;
public:
	GUIView();
	virtual ~GUIView();
private:
	shared_ptr<GameObject> _camera;
	shared_ptr<Camera> _cameraCom;
	shared_ptr<CameraMove> _cameraMove;
	shared_ptr<Transform> _camTransfrom;
	shared_ptr<Transform> _cameTarget;
private:
	bool _showAll = false;
	bool _showAssetSection = false;
	bool _showLoadedAsset = false;
	bool _showScene = false;
	bool _showModelSection = false;
	bool _showBoneHierarchy = false;
	bool _showInspector = false;
	bool _showAnimation = false;
	bool _showGrid = false;
	bool _showCameraWindow = false;
private:
	//Loaded Asset
	ImVec2 _loadedAssetPos;
	ImVec2 _loadedAssetSize;
	int _currentSkeletalItemIndex = 0;
	int _currentStaticItemIndex = 0;
	int _currentAnimationItemIndex = 0;
	vector<bool*> _skeletalCheckList;
	vector<bool*> _staticCheckList;
	shared_ptr<GameObject> _selectedModelAsset;

	//Scene
	ImVec2 _scenePos;
	ImVec2 _sceneSize;
	ImVec2 _sceneDrawSize;

	//BoneHierarchy
	ImVec2 _boneHierarchyPos;
	ImVec2 _boneHierarchySize;

	//Inspector
	ImVec2 _inspectorPos;
	ImVec2 _inspectorSize;
	Vec3 _transformPos;
	Vec3 _transformRot;
	Vec3 _transformScale;
	bool _scaleCheck = false;

	//Animation
	ImVec2 _animationPos;
	ImVec2 _animationSize;
	int _currentModelComboIndex = 0;
	int _currentAnimComboIndex = 0;

	//Camera
	ImVec2 _cameraWindowPos;
	ImVec2 _cameraWindowSize;

	ComPtr<ID3D11Texture2D> pTex;
	ComPtr<ID3D11ShaderResourceView> pSRV;
private:
	float* ConvertMatrixToFloat(Matrix& mat);
	void HelpMarker(const char* desc);
	void ButtonManage();
private:
	//
	void LoadedAsset();
	//
	void Scene();
	//
	void BoneHierarchy();
	//
	void Inspector();
	void Transform();
	void Mesh();
	void Material();
	//
	void Animation();
	//
	void Lighting();
	//
	void CameraWindow();
public:
	virtual void Update() override;
	virtual void Render() override;
}; 

