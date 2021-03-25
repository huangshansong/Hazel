#pragma once
#include "hzpch.h"

#include <glm.hpp>
#include <gtc/matrix_transform.hpp>

#include "Hazel/Events/KeyEvent.h"
#include "Hazel/Events/MouseEvent.h"

#include "Actor.h"

namespace Hazel {

    // An abstract camera class that processes input and calculates the corresponding Euler Angles, Vectors and Matrices for use in OpenGL
    class HAZEL_API Camera : public Actor 
    {
    public:
        // constructor with vectors
        Camera(void* level, std::string name, glm::vec3 transform = glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3 worldUp = glm::vec3(0.0f, 1.0f, 0.0f));

        const float getZoom() const { return m_Zoom; }

        const glm::mat4& getCameraViewMatrix() const { return m_CameraViewMatrix; }

        const glm::mat4& getCameraProjectionMatrix() const { return m_CameraProjectionMatrix; }

        float m_MouseSensitivity;

        float m_Zoom;

        float m_NearPlaneDistance;

        float m_FarPlaneDistance;

        glm::mat4 m_CameraViewMatrix;

        glm::mat4 m_CameraProjectionMatrix;

    protected:
        
        virtual void onPlayerInputEvent(Event& event);

        virtual void onUpdate();

        // calculates the front vector from the Camera's (updated) Euler Angles
        void updateCameraVectors();

        void updateCameraViewMatrix();

        void updateCameraProjectionMatrix();

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
        bool m_ConstrainPitch = true;
        // Default camera values
        static float s_YAW;
        static float s_PITCH;
        static float s_SPEED;
        static float s_SENSITIVITY;
        static float s_ZOOM;
        static float s_NearPlaneDistance;
        static float s_FarPlaneDistance;


    private:
        bool onMouseMoved(MouseMovedEvent& e);
        bool onMouseScrolled(MouseScrolledEvent& e);
        bool onKeyPressed(KeyPressedEvent& e);
        bool onKeyReleased(KeyReleasedEvent& e);

    };
}
