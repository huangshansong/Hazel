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

#include "Actor.h"
#include "camera.h"

using namespace std;
namespace Hazel
{
    float Camera::YAW = -90.0f;
    float Camera::PITCH = 0.0f;
    float Camera::SPEED = 2.5f;
    float Camera::SENSITIVITY = 0.1f;
    float Camera::ZOOM = 45.0f;

    // constructor with vectors
    Camera::Camera(glm::vec3 position, glm::vec3 up, float yaw, float pitch) : m_Front(glm::vec3(0.0f, 0.0f, -1.0f)), m_MovementSpeed(SPEED), m_MouseSensitivity(SENSITIVITY), m_Zoom(ZOOM)
    {
        m_Position = position;
        m_WorldUp = up;
        m_Yaw = yaw;
        m_Pitch = pitch;
        updateCameraVectors();
    }
    // constructor with scalar values
    Camera::Camera(float posX, float posY, float posZ, float upX, float upY, float upZ, float yaw, float pitch) : m_Front(glm::vec3(0.0f, 0.0f, -1.0f)), m_MovementSpeed(SPEED), m_MouseSensitivity(SENSITIVITY), m_Zoom(ZOOM)
    {
        m_Position = glm::vec3(posX, posY, posZ);
        m_WorldUp = glm::vec3(upX, upY, upZ);
        m_Yaw = yaw;
        m_Pitch = pitch;
        updateCameraVectors();
    }

    // returns the view matrix calculated using Euler Angles and the LookAt Matrix
    glm::mat4 Camera::getViewMatrix()
    {
        return glm::lookAt(m_Position, m_Position + m_Front, m_Up);
    }
    void Camera::onEvent(Event& event)
    {
        EventDispatcher dispatcher(event);
        dispatcher.Dispatch<MouseMovedEvent>(BIND_EVENT_FN(Camera::onMouseMoved));
        dispatcher.Dispatch<MouseScrolledEvent>(BIND_EVENT_FN(Camera::onMouseScrolled));
        dispatcher.Dispatch<KeyPressedEvent>(BIND_EVENT_FN(Camera::onKeyPressed));
        dispatcher.Dispatch<KeyReleasedEvent>(BIND_EVENT_FN(Camera::onKeyReleased));
    }
    void Camera::onUpdate()
    {

        float velocity = m_MovementSpeed * Application::getDeltaTime();

        if (m_ForwardMove)
            m_Position += m_Front * velocity;
        //Position += glm::vec3(Front.x, .0f, Front.z) * velocity; //Make the camera move only on a flat surface
        if (m_BackwardMove)
            m_Position -= m_Front * velocity;
        //Position -= glm::vec3(Front.x, .0f, Front.z) * velocity; //Make the camera move only on a flat surface
        if (m_LeftMove)
            m_Position -= m_Right * velocity;
        if (m_RightMove)
            m_Position += m_Right * velocity;

    }
    bool Camera::onMouseMoved(MouseMovedEvent& e) {

        //if the cursor is not captured, ignore the MouseMoved events
        if (!Application::getWindow()->getViewport()->m_CursorCaptured) return false;


        if (Application::getWindow()->getViewport()->m_FirstCursor)
        {
            Application::getWindow()->getViewport()->m_LastX = e.getX();
            Application::getWindow()->getViewport()->m_LastY = e.getY();
            Application::getWindow()->getViewport()->m_FirstCursor = false;
        }

        float xoffset = e.getX() - Application::getWindow()->getViewport()->m_LastX;
        float yoffset = Application::getWindow()->getViewport()->m_LastY - e.getY(); // reversed since y-coordinates go from bottom to top

        Application::getWindow()->getViewport()->m_LastX = e.getX();
        Application::getWindow()->getViewport()->m_LastY = e.getY();

        xoffset *= m_MouseSensitivity;
        yoffset *= m_MouseSensitivity;

        m_Yaw += xoffset;
        m_Pitch += yoffset;

        //HZ_CORE_INFO(m_Yaw);
        //HZ_CORE_INFO(m_Pitch);

        // make sure that when pitch is out of bounds, screen doesn't get flipped
        if (m_ConstrainPitch)
        {
            if (m_Pitch > 89.0f)
                m_Pitch = 89.0f;
            if (m_Pitch < -89.0f)
                m_Pitch = -89.0f;
        }

        // update Front, Right and Up Vectors using the updated Euler angles
        updateCameraVectors();

        return false;
    }
    bool Camera::onMouseScrolled(MouseScrolledEvent& e)
    {

        //if the cursor is not captured, ignore the MouseScrolled events
        if (!Application::getWindow()->getViewport()->m_CursorCaptured) return false;

        m_MovementSpeed += (float)e.getYOffset();
        //min speed 1.0f, max speed 100.0f
        m_MovementSpeed = glm::clamp(m_MovementSpeed, 1.0f, 100.0f);

        return false;
    }
    bool Camera::onKeyPressed(KeyPressedEvent& e)
    {

        //if the cursor is not captured, ignore the WSAD key events
        if (!Application::getWindow()->getViewport()->m_CursorCaptured) return false;

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

        //if the cursor is not captured, ignore the WSAD key events
        if (!Application::getWindow()->getViewport()->m_CursorCaptured) return false;

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
}