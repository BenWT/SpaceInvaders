#pragma once

class Vector3 {
public:
    GLfloat x, y, z;

    Vector3() {
        this->x = 0.0f;
        this->y = 0.0f;
        this->z = 0.0f;
    }
    Vector3(GLfloat v) {
        this->x = v;
        this->y = v;
        this->z = v;
    }
    Vector3(GLfloat x, GLfloat y, GLfloat z) {
        this->x = x;
        this->y = y;
        this->z = z;
    }

    void Move(GLfloat x, GLfloat y, GLfloat z) {
        this->x += x;
        this->y += y;
        this->z += z;
    }
    void Set(GLfloat x, GLfloat y, GLfloat z) {
        this->x = x;
        this->y = y;
        this->z = z;
    }

    GLfloat Magnitude() {
        return (x * x) + (y * y) + (z * z);
    }
};
