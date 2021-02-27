#pragma once
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

#include "Actor.h"

namespace Hazel {

    // An abstract camera class that processes input and calculates the corresponding Euler Angles, Vectors and Matrices for use in OpenGL
    class Camera : public Actor 
    {
    public:
        // camera Attributes
        glm::vec3 Position;
        glm::vec3 Front;
        glm::vec3 Up;
        glm::vec3 Right;
        glm::vec3 WorldUp;
        // euler Angles
        float Yaw;
        float Pitch;
        // camera options
        float MovementSpeed;
        bool forward = false;
        bool backward = false;
        bool left = false;
        bool right = false;
        float MouseSensitivity;
        float Zoom;
        bool constrainPitch = true;

        // Default camera values, defined at the bottom of this file
        static float YAW ;
        static float PITCH ;
        static float SPEED ;
        static float SENSITIVITY ;
        static float ZOOM ;


        // constructor with vectors
        Camera(glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f), float yaw = YAW, float pitch = PITCH) : Front(glm::vec3(0.0f, 0.0f, -1.0f)), MovementSpeed(SPEED), MouseSensitivity(SENSITIVITY), Zoom(ZOOM)
        {
            Position = position;
            WorldUp = up;
            Yaw = yaw;
            Pitch = pitch;
            updateCameraVectors();
        }
        // constructor with scalar values
        Camera(float posX, float posY, float posZ, float upX, float upY, float upZ, float yaw, float pitch) : Front(glm::vec3(0.0f, 0.0f, -1.0f)), MovementSpeed(SPEED), MouseSensitivity(SENSITIVITY), Zoom(ZOOM)
        {
            Position = glm::vec3(posX, posY, posZ);
            WorldUp = glm::vec3(upX, upY, upZ);
            Yaw = yaw;
            Pitch = pitch;
            updateCameraVectors();
        }

        // returns the view matrix calculated using Euler Angles and the LookAt Matrix
        glm::mat4 GetViewMatrix()
        {
            return glm::lookAt(Position, Position + Front, Up);
        }

        void OnEvent(Event& event)
        {
            EventDispatcher dispatcher(event);
            dispatcher.Dispatch<MouseMovedEvent>(BIND_EVENT_FN(Camera::OnMouseMoved));
            dispatcher.Dispatch<MouseScrolledEvent>(BIND_EVENT_FN(Camera::OnMouseScrolled));
            dispatcher.Dispatch<KeyPressedEvent>(BIND_EVENT_FN(Camera::OnKeyPressed));
            dispatcher.Dispatch<KeyReleasedEvent>(BIND_EVENT_FN(Camera::OnKeyReleased));
        }

        void OnUpdate()
        {

            float velocity = MovementSpeed * Application::deltaTime;

            if (forward)
                Position += Front * velocity;
            //Position += glm::vec3(Front.x, .0f, Front.z) * velocity; //Make the camera move only on a flat surface
            if (backward)
                Position -= Front * velocity;
            //Position -= glm::vec3(Front.x, .0f, Front.z) * velocity; //Make the camera move only on a flat surface
            if (left)
                Position -= Right * velocity;
            if (right)
                Position += Right * velocity;

        }

        bool OnMouseMoved(MouseMovedEvent& e) {

            //if the cursor is not captured, ignore the MouseMoved events
            if (!Application::m_Window->m_Viewport->cursorCaptured) return false;


            if (Application::m_Window->m_Viewport->firstCursor)
            {
                Application::m_Window->m_Viewport->lastX = e.GetX();
                Application::m_Window->m_Viewport->lastY = e.GetY();
                Application::m_Window->m_Viewport->firstCursor = false;
            }

            float xoffset = e.GetX() - Application::m_Window->m_Viewport->lastX;
            float yoffset = Application::m_Window->m_Viewport->lastY - e.GetY(); // reversed since y-coordinates go from bottom to top

            Application::m_Window->m_Viewport->lastX = e.GetX();
            Application::m_Window->m_Viewport->lastY = e.GetY();

            xoffset *= MouseSensitivity;
            yoffset *= MouseSensitivity;

            Yaw += xoffset;
            Pitch += yoffset;

            // make sure that when pitch is out of bounds, screen doesn't get flipped
            if (constrainPitch)
            {
                if (Pitch > 89.0f)
                    Pitch = 89.0f;
                if (Pitch < -89.0f)
                    Pitch = -89.0f;
            }

            // update Front, Right and Up Vectors using the updated Euler angles
            updateCameraVectors();

            return false;
        }

        bool OnMouseScrolled(MouseScrolledEvent& e)
        {

            //if the cursor is not captured, ignore the MouseScrolled events
            if (!Application::m_Window->m_Viewport->cursorCaptured) return false;

            MovementSpeed += (float)e.GetYOffset();
            MovementSpeed = glm::clamp(MovementSpeed, 1.0f, 100.0f);

            return false;
        }

        bool OnKeyPressed(KeyPressedEvent& e)
        {

            //if the cursor is not captured, ignore the WSAD key events
            if (!Application::m_Window->m_Viewport->cursorCaptured) return false;

            switch (e.GetKeyCode()) {
            case HZ_KEY_W: {
                forward = true;
                break;
            }
            case HZ_KEY_S: {
                backward = true;
                break;
            }
            case HZ_KEY_A: {
                left = true;
                break;
            }
            case HZ_KEY_D: {
                right = true;
                break;
            }
            }

            return false;
        }

        bool OnKeyReleased(KeyReleasedEvent& e)
        {

            //if the cursor is not captured, ignore the WSAD key events
            if (!Application::m_Window->m_Viewport->cursorCaptured) return false;

            switch (e.GetKeyCode()) {
            case HZ_KEY_W: {
                forward = false;
                break;
            }
            case HZ_KEY_S: {
                backward = false;
                break;
            }
            case HZ_KEY_A: {
                left = false;
                break;
            }
            case HZ_KEY_D: {
                right = false;
                break;
            }
            }

            return false;
        }


    private:
        // calculates the front vector from the Camera's (updated) Euler Angles
        void updateCameraVectors()
        {
            // calculate the new Front vector
            glm::vec3 front;
            front.x = cos(glm::radians(Yaw)) * cos(glm::radians(Pitch));
            front.y = sin(glm::radians(Pitch));
            front.z = sin(glm::radians(Yaw)) * cos(glm::radians(Pitch));
            Front = glm::normalize(front);
            // also re-calculate the Right and Up vector
            Right = glm::normalize(glm::cross(Front, WorldUp));  // normalize the vectors, because their length gets closer to 0 the more you look up or down which results in slower movement.
            Up = glm::normalize(glm::cross(Right, Front));
        }
    };

    float Camera::YAW = -90.0f;
    float Camera::PITCH = 0.0f;
    float Camera::SPEED = 2.5f;
    float Camera::SENSITIVITY = 0.1f;
    float Camera::ZOOM = 45.0f;

}
