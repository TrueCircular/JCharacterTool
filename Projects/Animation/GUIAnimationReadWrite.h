#pragma once
#include "GUIInterface.h"
class GUIAnimationReadWrite : public GUIInterface
{
	using Super = GUIInterface;
public:
	GUIAnimationReadWrite();
	virtual ~GUIAnimationReadWrite();
public:
	virtual void Update() override;
	virtual void Render() override;
};

