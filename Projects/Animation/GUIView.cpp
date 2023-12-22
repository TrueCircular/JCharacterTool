#include "pch.h"
#include "GUIView.h"

GUIView::GUIView() : Super(GUIType::View)
{
}

GUIView::~GUIView()
{
}

void GUIView::DrawGrid()
{
	if (_isViewGrid)
	{
		Matrix tempViewMat = Camera::S_MatView;
		tempViewMat.Transpose();
		Matrix tempProjMat = Camera::S_MatProjection;
		Matrix iden = Matrix::Identity;
		//iden = iden *tempViewMat * tempProjMat;
	
		const float* cameraView = ConvertMatrixToFloat(tempViewMat);
		const float* cameraProj = ConvertMatrixToFloat(tempProjMat);
		float* ident = const_cast<float*>(ConvertMatrixToFloat(iden));

		ImGuizmo::SetRect(0, 0, 1600.f, 900.f);
		ImGuizmo::DrawGrid(cameraView, cameraProj, ident, 1000.f);
		//ImGuizmo::SetOrthographic(false);

		//ImGuizmo::Manipulate(cameraView, cameraProj, mCurrentGizmoOperation, mCurrentGizmoMode, ident, NULL, NULL);
		//ImGuizmo::ViewManipulate(cameraView, camDistance, ImVec2(viewManipulateRight - 128, viewManipulateTop), ImVec2(128, 128), 0x10101010);

	}
}

const float* GUIView::ConvertMatrixToFloat(Matrix& mat)
{
	float ReturnFloat[16] = { 0, };

	ReturnFloat[0] = mat._11; ReturnFloat[1] = mat._12; ReturnFloat[2] = mat._13; ReturnFloat[3] = mat._14;
	ReturnFloat[4] = mat._21; ReturnFloat[5] = mat._22; ReturnFloat[6] = mat._23; ReturnFloat[7] = mat._24;
	ReturnFloat[8] = mat._31; ReturnFloat[9] = mat._32; ReturnFloat[10] = mat._33; ReturnFloat[11] = mat._34;
	ReturnFloat[12] = mat._41; ReturnFloat[13] = mat._42; ReturnFloat[14] = mat._43; ReturnFloat[15] = mat._44;

	return ReturnFloat;
}

void GUIView::Update()
{
	if (ImGui::MenuItem("View Grid"))
	{
		_isViewGrid = true;
	}

}

void GUIView::Render()
{
	DrawGrid();
}
