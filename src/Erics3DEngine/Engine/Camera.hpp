#ifndef ERICS3DENGINE_CAMERA_HPP
#define ERICS3DENGINE_CAMERA_HPP

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/quaternion.hpp>
using namespace glm;

class Camera {
    public:
        Camera();

        const float getFieldOfView() const;
        void setFieldOfView(float fov);

        const float getAspectRatio() const;
        void setAspectRatio(float ratio);

        const float getNearPlane() const;
        void setNearPlane(float nearPlane);

        const float getFarPlane() const;
        void setFarPlane(float farPlane);

        const vec3 & getEyeLocation() const;
        void setEyeLocation(vec3 eye);

        const vec3 & getCenterLocation() const;
        void setCenterLocation(vec3 center);

        const vec3 & getUp() const;
        void setUp(vec3 up);

        const mat4 & getProjectionMatrix() const;
        const mat4 & getViewMatrix() const;

        void processInput();

    private:
        float mFieldOfView;
        float mAspectRatio;
        float mNearPlane;
        float mFarPlane;
        float mHorizontalAngle = 3.14f;
        float mVerticalAngle = 0.0f;
        float mSpeed = 3.0f;
        float mMouseSpeed = 0.005f;
        vec3 mEyeLocation;
        vec3 mCenterLocation;
        vec3 mUp;

        mat4 mProjectionMatrix;
        mat4 mViewMatrix;

        void recomputeMatrices();
};

#endif //ERICS3DENGINE_CAMERA_HPP