#include "Camera.hpp"
#include <glm/gtc/matrix_transform.hpp>

Camera::Camera() :
    mFieldOfView(45.0f),
    mAspectRatio(4.0f / 3.0f),
    mNearPlane(0.1f),
    mFarPlane(100.0f),
    mEyeLocation(0.0f, 0.0f, 0.0f),
    mCenterLocation(0.0f, 0.0f, 0.0f),
    mUp(0.0f, 1.0f, 0.0f) {

    recomputeMatrices();
}

const float Camera::getFieldOfView() const {
    return mFieldOfView;
}

void Camera::setFieldOfView(float fov) {
    mFieldOfView = fov;
    recomputeMatrices();
}

const float Camera::getAspectRatio() const {
    return mAspectRatio;
}

void Camera::setAspectRatio(float ratio) {
    mAspectRatio = ratio;
    recomputeMatrices();
}

const float Camera::getNearPlane() const {
    return mNearPlane;
}

void Camera::setNearPlane(float nearPlane) {
    mNearPlane = nearPlane;
    recomputeMatrices();
}

const float Camera::getFarPlane() const {
    return mFarPlane;
}

void Camera::setFarPlane(float farPlane) {
    mFarPlane = farPlane;
    recomputeMatrices();
}

const vec3 & Camera::getEyeLocation() const {
    return mEyeLocation;
}

void Camera::setEyeLocation(vec3 eye) {
    mEyeLocation = eye;
    recomputeMatrices();
}

const vec3 & Camera::getCenterLocation() const {
    return mCenterLocation;
}

void Camera::setCenterLocation(vec3 center) {
    mCenterLocation = center;
    recomputeMatrices();
}

const vec3 & Camera::getUp() const {
    return mUp;
}

void Camera::setUp(vec3 up) {
    mUp = up;
    recomputeMatrices();
}

const mat4 & Camera::getProjectionMatrix() const {
    return mProjectionMatrix;
}

const mat4 & Camera::getViewMatrix() const {
    return mViewMatrix;
}

void Camera::recomputeMatrices() {
    mProjectionMatrix = glm::perspective(glm::radians(mFieldOfView), mAspectRatio, mNearPlane, mFarPlane);
    mViewMatrix = glm::lookAt(mEyeLocation, mCenterLocation, mUp);
}

void Camera::processInput() {
    static double lastTime = glfwGetTime();
    static int winWidth, winHeight;
    glfwGetWindowSize(glfwGetCurrentContext(), &winWidth, &winHeight);

    // Compute the time difference between the current and last frame
    double currentTime = glfwGetTime();
    float deltaTime = float(currentTime - lastTime);

    // Get mouse position
    double xPos, yPos;
    glfwGetCursorPos(glfwGetCurrentContext(), &xPos, &yPos);

    // Reset mouse position for next frame
    glfwSetCursorPos(glfwGetCurrentContext(), winWidth / 2, winHeight / 2);

    // Compute new orientation
    mHorizontalAngle += mMouseSpeed * float(winWidth / 2 - xPos);
    mVerticalAngle += mMouseSpeed * float(winHeight / 2 - yPos);

    // Direction : Spherical coordinates to Cartesian coordinates conversion
    vec3 direction (
        cos(mVerticalAngle) * sin(mHorizontalAngle),
        sin(mVerticalAngle),
        cos(mVerticalAngle) * cos(mHorizontalAngle)
    );

    // Right vector
    vec3 right = vec3(
        sin(mHorizontalAngle - 3.14f / 2.0f),
        0,
        cos(mHorizontalAngle - 3.14f / 2.0f)
    );

    // Up vector
    mUp = cross(right, direction);

    // Move forward
    if (glfwGetKey(glfwGetCurrentContext(), GLFW_KEY_UP) == GLFW_PRESS)
        mEyeLocation += direction * deltaTime * mSpeed;
    // Move backward
    if (glfwGetKey(glfwGetCurrentContext(), GLFW_KEY_DOWN) == GLFW_PRESS)
        mEyeLocation -= direction * deltaTime * mSpeed;
    // Strafe right
    if (glfwGetKey(glfwGetCurrentContext(), GLFW_KEY_RIGHT) == GLFW_PRESS)
        mEyeLocation += right * deltaTime * mSpeed;
    // Move backward
    if (glfwGetKey(glfwGetCurrentContext(), GLFW_KEY_LEFT) == GLFW_PRESS)
        mEyeLocation -= right * deltaTime * mSpeed;

    mCenterLocation = mEyeLocation + direction;

    float fov = mFieldOfView;

    mProjectionMatrix = perspective(radians(fov), mAspectRatio, mNearPlane, mFarPlane);
    mViewMatrix = lookAt(mEyeLocation, mCenterLocation, mUp);

    lastTime = currentTime;
}