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
	if (ImGui::MenuItem("Save Animation File"))
	{

	}
}

void GUIAnimationReadWrite::Render()
{
}
