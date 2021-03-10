#pragma once
#include "hzpch.h"

#include <glm.hpp>
#include <gtc/matrix_transform.hpp>

#include "Hazel/Events/KeyEvent.h"
#include "Hazel/Events/MouseEvent.h"

#include "Actor.h"

namespace Hazel {

    // An abstract camera class that processes input and calculates the corresponding Euler Angles, Vectors and Matrices for use in OpenGL
    class Camera : public Actor 
    {
    public:
        class Interface
        {
            friend class Level;
        private:
            static Actor* create(glm::vec3& transform = glm::vec3(0.0f, 0.0f, 0.0f)) { return new Camera(transform); }
        };
    public:
        float getZoom() { return m_Zoom; }
        glm::mat4 getViewMatrix(){ return glm::lookAt(m_Transform, m_Transform + m_Front, m_Up); }

    protected:
        // constructor with vectors
        Camera(glm::vec3 transform = glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f), float yaw = s_YAW, float pitch = s_PITCH);
        // constructor with scalar values
        Camera(float posX, float posY, float posZ, float upX, float upY, float upZ, float yaw, float pitch);
 
        virtual void onEvent(Event& event);
        virtual void onUpdate();

    private:
        bool onMouseMoved(MouseMovedEvent& e);
        bool onMouseScrolled(MouseScrolledEvent& e);
        bool onKeyPressed(KeyPressedEvent& e);
        bool onKeyReleased(KeyReleasedEvent& e);

    protected:
        // calculates the front vector from the Camera's (updated) Euler Angles
        void updateCameraVectors();

        // camera Attributes
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
        // Default camera values
        static float s_YAW;
        static float s_PITCH;
        static float s_SPEED;
        static float s_SENSITIVITY;
        static float s_ZOOM;
    };
}
