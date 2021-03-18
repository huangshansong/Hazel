#include "hzpch.h"

#include <Glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>

#include "Hazel/Application.h"
#include "Hazel/Core.h"
#include "Hazel/KeyCode.h"
#include "Hazel/Events/KeyEvent.h"
#include "Hazel/Events/MouseEvent.h"
#include "Hazel/Log.h"

#include "camera.h"

using namespace std;
namespace Hazel
{
    float Camera::s_YAW = -90.0f;
    float Camera::s_PITCH = 0.0f;
    float Camera::s_SPEED = 5.0f;
    float Camera::s_SENSITIVITY = 0.1f;
    float Camera::s_ZOOM = 45.0f;
    float Camera::s_NearPlaneDistance = 0.1f;
    float Camera::s_FarPlaneDistance = 300.0f;


    // constructor with vectors
    Camera::Camera(void* level, string name, glm::vec3 position, glm::vec3 up, float yaw, float pitch) 
        : Actor(level, name), m_Front(glm::vec3(0.0f, 0.0f, -1.0f)),
        m_MovementSpeed(s_SPEED), m_MouseSensitivity(s_SENSITIVITY), m_Zoom(s_ZOOM), m_NearPlaneDistance(s_NearPlaneDistance), m_FarPlaneDistance(s_FarPlaneDistance)
    {
        m_Transform = position;
        m_WorldUp = up;
        m_Yaw = yaw;
        m_Pitch = pitch;
    }

    void Camera::onPlayerInputEvent(Event& event)
    {
        if (((Level*)m_OfLevel)->getCamera() == this)
        {
            EventDispatcher dispatcher(event);
            dispatcher.Dispatch<MouseMovedEvent>(BIND_EVENT_FN(Camera::onMouseMoved));
            dispatcher.Dispatch<MouseScrolledEvent>(BIND_EVENT_FN(Camera::onMouseScrolled));
            dispatcher.Dispatch<KeyPressedEvent>(BIND_EVENT_FN(Camera::onKeyPressed));
            dispatcher.Dispatch<KeyReleasedEvent>(BIND_EVENT_FN(Camera::onKeyReleased));
        }  
    }
    void Camera::onUpdate()
    {
        //no need to update the camera except the main camera
        if (((Level*)m_OfLevel)->getCamera() == this)
        {
            float moveDistance = m_MovementSpeed * Application::getDeltaTime();
            if (m_ForwardMove)
                m_Transform += m_Front * moveDistance;
            //Position += glm::vec3(Front.x, .0f, Front.z) * moveDistance; //Make the camera move only on a flat surface
            if (m_BackwardMove)
                m_Transform -= m_Front * moveDistance;
            //Position -= glm::vec3(Front.x, .0f, Front.z) * moveDistance; //Make the camera move only on a flat surface
            if (m_LeftMove)
                m_Transform -= m_Right * moveDistance;
            if (m_RightMove)
                m_Transform += m_Right * moveDistance;

            updateCameraVectors();
            updateCameraViewMatrix();
            updateCameraProjectionMatrix();
        }
    }
    bool Camera::onMouseMoved(MouseMovedEvent& e) 
    {
        Viewport* viewport = (Viewport*)((Level*)m_OfLevel)->getOfViewport();
        //if the cursor is not captured, ignore the MouseMoved events
        if (!viewport->isCursorCaptured()) return false;
        if (viewport->isFirstCursor()) return false;

        float xoffset = viewport->getCursorOffsetX();
        // reversed since y-coordinates go from bottom to top
        float yoffset = -viewport->getCursorOffsetY();
        xoffset *= m_MouseSensitivity;
        yoffset *= m_MouseSensitivity;
        m_Yaw += xoffset;
        m_Pitch += yoffset;
        // make sure that when pitch is out of bounds, screen doesn't get flipped
        if (m_ConstrainPitch)
        {
            if (m_Pitch > 89.0f)
                m_Pitch = 89.0f;
            if (m_Pitch < -89.0f)
                m_Pitch = -89.0f;
        }
        //HZ_CORE_TRACE(m_Yaw);

        return false;
    }
    bool Camera::onMouseScrolled(MouseScrolledEvent& e)
    {
        Viewport* viewport = (Viewport*)((Level*)m_OfLevel)->getOfViewport();
        //if the cursor is not captured, ignore the MouseScrolled events
        if (!viewport->isCursorCaptured()) return false;

        m_MovementSpeed += (float)e.getYOffset();
        //min speed 1.0f, max speed 100.0f
        m_MovementSpeed = glm::clamp(m_MovementSpeed, 1.0f, 100.0f);

        return false;
    }
    bool Camera::onKeyPressed(KeyPressedEvent& e)
    {
        Viewport* viewport = (Viewport*)((Level*)m_OfLevel)->getOfViewport();

        //if the cursor is not captured, ignore the WSAD key events
        if (!viewport->isCursorCaptured()) return false;

        switch (e.getKeyCode()) {
        case HZ_KEY_W: {
            m_ForwardMove = true;
            break;
        }
        case HZ_KEY_S: {
            m_BackwardMove = true;
            break;
        }
        case HZ_KEY_A: {
            m_LeftMove = true;
            break;
        }
        case HZ_KEY_D: {
            m_RightMove = true;
            break;
        }
        }

        return false;
    }
    bool Camera::onKeyReleased(KeyReleasedEvent& e)
    {
        Viewport* viewport = (Viewport*)((Level*)m_OfLevel)->getOfViewport();

        //if the cursor is not captured, ignore the WSAD key events
        if (!viewport->isCursorCaptured()) return false;

        switch (e.getKeyCode()) {
        case HZ_KEY_W: {
            m_ForwardMove = false;
            break;
        }
        case HZ_KEY_S: {
            m_BackwardMove = false;
            break;
        }
        case HZ_KEY_A: {
            m_LeftMove = false;
            break;
        }
        case HZ_KEY_D: {
            m_RightMove = false;
            break;
        }
        }

        return false;
    }

    // calculates the front vector from the Camera's (updated) Euler Angles
    void Camera::updateCameraVectors()
    {
        // calculate the new Front vector
        glm::vec3 front;
        front.x = cos(glm::radians(m_Yaw)) * cos(glm::radians(m_Pitch));
        front.y = sin(glm::radians(m_Pitch));
        front.z = sin(glm::radians(m_Yaw)) * cos(glm::radians(m_Pitch));
        m_Front = glm::normalize(front);
        // also re-calculate the Right and Up vector
        m_Right = glm::normalize(glm::cross(m_Front, m_WorldUp));  // normalize the vectors, because their length gets closer to 0 the more you look up or down which results in slower movement.
        m_Up = glm::normalize(glm::cross(m_Right, m_Front));

        //HZ_CORE_INFO(m_Front.x);
    }
    void Camera::updateCameraViewMatrix()
    {
        m_CameraViewMatrix = glm::lookAt(m_Transform, m_Transform + m_Front, m_Up);
    }
    void Camera::updateCameraProjectionMatrix()
    {
        Viewport* ofViewport = (Viewport*)((Level*)m_OfLevel)->getOfViewport();
        m_CameraProjectionMatrix = glm::perspective(
            glm::radians(m_Zoom),
            (float)ofViewport->getWidth() / (float)ofViewport->getHeight(),
            m_NearPlaneDistance, m_FarPlaneDistance);
    }
}