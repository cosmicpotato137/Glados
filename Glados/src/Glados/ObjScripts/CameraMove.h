#pragma once

#include "Glados/Core/Core.h"
#include "Glados/ObjAttrib.h"

class GLADOS_API CameraMove : public Glados::ObjAttrib
{
private:
	std::shared_ptr<Glados::Transform> transf;
	float mousex;
	float mousey;
	float rotSpeed;

public:
	CameraMove(std::shared_ptr<Glados::Transform> transf, float rotspd=1);
	~CameraMove();

	void OnImGuiRender() override;

	void MouseUpdate(bool mousedown, float xpos, float ypos);
	void ScrollUpdate(float xoffset, float yoffset);
};