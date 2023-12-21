#include "pch.h"
#include "GUIAnimationReadWrite.h"

GUIAnimationReadWrite::GUIAnimationReadWrite() : Super(GUIType::AnimationReadWrite)
{
}

GUIAnimationReadWrite::~GUIAnimationReadWrite()
{
}

void GUIAnimationReadWrite::Update()
{
	ImGui::MenuItem("(Animation)", NULL, false, false);
	if (ImGui::MenuItem("Read Animation Asset File"))
	{

	}

	ImGui::Separator();

	if (ImGui::MenuItem("Save Asset File as Anim File"))
	{

	}

	ImGui::Separator();

	if (ImGui::MenuItem("Read Anim File and Show Animation"))
	{

	}

	ImGui::Separator();

}

void GUIAnimationReadWrite::Render()
{
}
