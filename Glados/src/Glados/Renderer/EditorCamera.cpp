#include "gladospch.h"
#include "EditorCamera.h"

#include "Glados/Core/Input.h"
#include "Glados/Core/KeyCodes.h"
#include "Glados/Core/MouseButtonCodes.h"

#include <glfw/glfw3.h>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/quaternion.hpp>

namespace Glados {

	EditorCamera::EditorCamera(float fov, float aspectRatio, float nearClip, float farClip)
		: m_FOV(fov), m_AspectRatio(aspectRatio), m_NearClip(nearClip), m_FarClip(farClip), Camera(glm::perspective(glm::radians(fov), aspectRatio, nearClip, farClip))
	{
		UpdateView();
	}
	
	void EditorCamera::OnUpdate(Timestep ts)
	{
		if (Input::IsMouseButtonPressed(GD_MOUSE_BUTTON_LEFT))
		{
			const glm::vec2& mouse{ Input::GetMouseX(), Input::GetMouseY() };
			glm::vec2 delta = (m_InitialMousePosition - mouse) * .003f;
			m_InitialMousePosition = mouse;

			MouseRotate(delta);
			UpdateView();
		}
	}

	void EditorCamera::OnEvent(Event& e)
	{
		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<MouseScrollEvent>(BIND_EVENT_FN(EditorCamera::OnMouseScroll));
	}

	void EditorCamera::SetViewportSize(float width, float height)
	{
		m_ViewportWidth = width; 
		m_ViewportHeight = height; 
		UpdateProjection();
	}

	void EditorCamera::UpdateProjection()
	{
		m_AspectRatio = m_ViewportWidth / m_ViewportHeight;
		m_Projection = glm::perspective(glm::radians(m_FOV), m_AspectRatio, m_NearClip, m_FarClip);
	}

	void EditorCamera::UpdateView()
	{
		m_ViewMatrix = glm::lookAt(m_Position, m_Forward, m_Up);
	}

	bool EditorCamera::OnMouseScroll(MouseScrollEvent& e)
	{
		glm::vec3 eye = m_Position;
		glm::vec3 forward = m_Forward;
		glm::vec3 up = m_Up;
		float dir = e.GetYOffset();
		glm::vec3 dirvec = glm::normalize(eye) * dir * m_ZoomSpeed;
		if (length(dirvec) < length(eye) || dir > 0) eye = eye + dirvec;
		m_Position = eye;
		m_Forward = glm::normalize(forward);
		m_Up = up;
		UpdateView();
		return true;
	}

	void EditorCamera::MouseRotate(const glm::vec2& dmousepos)
	{
		glm::vec3 eye = m_Position;
		glm::vec3 forward = m_Forward;
		glm::vec3 up = m_Up;

		// horizontal rotation
		float hozdir = dmousepos.x > 0 ? 1 : dmousepos.x < 0 ? -1 : 0;
		float hozang = m_RotationSpeed * hozdir;
		glm::mat4 hozrot = glm::rotate(glm::mat4(1), hozang, glm::vec3(0, 1, 0));
		eye = glm::vec3(hozrot * glm::vec4(eye, 0.0f));
		up = glm::vec3(hozrot * glm::vec4(up, 0.0f));
		forward = glm::vec3(hozrot * glm::vec4(forward, 0.0f));

		// vertical rotation
		glm::vec3 axis = glm::normalize(glm::cross(up, forward));
		float vertdir = dmousepos.y > 0 ? -1 : dmousepos.y < 0 ? 1 : 0;
		float vertang = m_RotationSpeed * vertdir;
		eye = glm::vec3(glm::rotate(glm::mat4(1), vertang, axis) * glm::vec4(eye, 0.0f));
		up = glm::cross(axis, glm::normalize(eye));
		forward = glm::cross(axis, up);

		m_Position = eye;
		m_Forward = glm::normalize(forward);
		m_Up = up;
	}

	//EditorCamera::EditorCamera(float fov, float aspectRatio, float nearClip, float farClip)
	//	: m_FOV(fov), m_AspectRatio(aspectRatio), m_NearClip(nearClip), m_FarClip(farClip), Camera(glm::perspective(glm::radians(fov), aspectRatio, nearClip, farClip))
	//{
	//	UpdateView();
	//}

	//void EditorCamera::UpdateProjection()
	//{
	//	m_AspectRatio = m_ViewportWidth / m_ViewportHeight;
	//	m_Projection = glm::perspective(glm::radians(m_FOV), m_AspectRatio, m_NearClip, m_FarClip);
	//}

	//void EditorCamera::UpdateView()
	//{
	//	// m_Yaw = m_Pitch = 0.0f; // Lock the camera's rotation
	//	m_Position = CalculatePosition();

	//	glm::quat orientation = GetOrientation();
	//	m_ViewMatrix = glm::translate(glm::mat4(1.0f), m_Position) * glm::toMat4(orientation);
	//	m_ViewMatrix = glm::inverse(m_ViewMatrix);
	//}

	//std::pair<float, float> EditorCamera::PanSpeed() const
	//{
	//	float x = std::min(m_ViewportWidth / 1000.0f, 2.4f); // max = 2.4f
	//	float xFactor = 0.0366f * (x * x) - 0.1778f * x + 0.3021f;

	//	float y = std::min(m_ViewportHeight / 1000.0f, 2.4f); // max = 2.4f
	//	float yFactor = 0.0366f * (y * y) - 0.1778f * y + 0.3021f;

	//	return { xFactor, yFactor };
	//}

	//float EditorCamera::RotationSpeed() const
	//{
	//	return 0.8f;
	//}

	//float EditorCamera::ZoomSpeed() const
	//{
	//	float distance = m_Distance * 0.2f;
	//	distance = std::max(distance, 0.0f);
	//	float speed = distance * distance;
	//	speed = std::min(speed, 100.0f); // max speed = 100
	//	return speed;
	//}

	//void EditorCamera::OnUpdate(Timestep ts)
	//{
	//	if (Input::IsKeyPressed(GD_KEY_LEFT_ALT))
	//	{
	//		const glm::vec2& mouse{ Input::GetMouseX(), Input::GetMouseY() };
	//		glm::vec2 delta = (mouse - m_InitialMousePosition) * 0.003f;
	//		m_InitialMousePosition = mouse;

	//		if (Input::IsMouseButtonPressed(GD_MOUSE_BUTTON_RIGHT))
	//			MousePan(delta);
	//		else if (Input::IsMouseButtonPressed(GD_MOUSE_BUTTON_RIGHT))
	//			MouseRotate(delta);
	//		else if (Input::IsMouseButtonPressed(GD_MOUSE_BUTTON_MIDDLE))
	//			MouseZoom(delta.y);
	//	}

	//	UpdateView();
	//}

	//void EditorCamera::OnEvent(Event& e)
	//{
	//	EventDispatcher dispatcher(e);
	//	dispatcher.Dispatch<MouseScrollEvent>(BIND_EVENT_FN(EditorCamera::OnMouseScroll));
	//}

	//bool EditorCamera::OnMouseScroll(MouseScrollEvent& e)
	//{
	//	float delta = e.GetYOffset() * 0.1f;
	//	MouseZoom(delta);
	//	UpdateView();
	//	return false;
	//}

	//void EditorCamera::MousePan(const glm::vec2& delta)
	//{
	//	auto [xSpeed, ySpeed] = PanSpeed();
	//	m_FocalPoint += -GetRightDirection() * delta.x * xSpeed * m_Distance;
	//	m_FocalPoint += GetUpDirection() * delta.y * ySpeed * m_Distance;
	//}

	//void EditorCamera::MouseRotate(const glm::vec2& delta)
	//{
	//	float yawSign = GetUpDirection().y < 0 ? -1.0f : 1.0f;
	//	m_Yaw += yawSign * delta.x * RotationSpeed();
	//	m_Pitch += delta.y * RotationSpeed();
	//}

	//void EditorCamera::MouseZoom(float delta)
	//{
	//	m_Distance -= delta * ZoomSpeed();
	//	if (m_Distance < 1.0f)
	//	{
	//		m_FocalPoint += GetForwardDirection();
	//		m_Distance = 1.0f;
	//	}
	//}

	//glm::vec3 EditorCamera::GetUpDirection() const
	//{
	//	return glm::rotate(GetOrientation(), glm::vec3(0.0f, 1.0f, 0.0f));
	//}

	//glm::vec3 EditorCamera::GetRightDirection() const
	//{
	//	return glm::rotate(GetOrientation(), glm::vec3(1.0f, 0.0f, 0.0f));
	//}

	//glm::vec3 EditorCamera::GetForwardDirection() const
	//{
	//	return glm::rotate(GetOrientation(), glm::vec3(0.0f, 0.0f, -1.0f));
	//}

	//glm::vec3 EditorCamera::CalculatePosition() const
	//{
	//	return m_FocalPoint - GetForwardDirection() * m_Distance;
	//}

	//glm::quat EditorCamera::GetOrientation() const
	//{
	//	return glm::quat(glm::vec3(-m_Pitch, -m_Yaw, 0.0f));
	//}

}