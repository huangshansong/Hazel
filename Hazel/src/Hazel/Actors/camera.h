#pragma once
#include "hzpch.h"

#include <glm.hpp>

#include "Hazel/Events/KeyEvent.h"
#include "Hazel/Events/MouseEvent.h"

#include "Actor.h"

namespace Hazel {

    // An abstract camera class that processes input and calculates the corresponding Euler Angles, Vectors and Matrices for use in OpenGL
    class Camera : public Actor 
    {
    public:
        
        // constructor with vectors
        Camera(glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f), float yaw = YAW, float pitch = PITCH);
        // constructor with scalar values
        Camera(float posX, float posY, float posZ, float upX, float upY, float upZ, float yaw, float pitch);

        // returns the view matrix calculated using Euler Angles and the LookAt Matrix
        glm::mat4 getViewMatrix();

        inline float getZoom() const { return m_Zoom; }

        inline glm::vec3 getCameraPosition() const { return m_Position; }

        void onEvent(Event& event);

        void onUpdate();

        bool onMouseMoved(MouseMovedEvent& e);

        bool onMouseScrolled(MouseScrolledEvent& e);

        bool onKeyPressed(KeyPressedEvent& e);

        bool onKeyReleased(KeyReleasedEvent& e);


    private:
        // calculates the front vector from the Camera's (updated) Euler Angles
        void updateCameraVectors();

        // camera Attributes
        glm::vec3 m_Position;
        glm::vec3 m_Front;
        glm::vec3 m_Up;
        glm::vec3 m_Right;
        glm::vec3 m_WorldUp;
        // euler Angles
        float m_Yaw;
        float m_Pitch;
        // camera options
        float m_MovementSpeed;
        bool m_ForwardMove = false;
        bool m_BackwardMove = false;
        bool m_LeftMove = false;
        bool m_RightMove = false;
        float m_MouseSensitivity;
        float m_Zoom;
        bool m_ConstrainPitch = true;

        // Default camera values, defined at the bottom of this file
        static float YAW;
        static float PITCH;
        static float SPEED;
        static float SENSITIVITY;
        static float ZOOM;
    };
}
