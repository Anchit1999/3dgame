#include "main.h"

#ifndef PLANE_H
#define PLANE_H


class Plane {
public:
    Plane() {}
    Plane(float x, float y,float z, color_t color);
    glm::vec3 position,rotation;
    void draw(glm::mat4 VP);
    void set_position(float x, float y);
    void tick();
    double speed,len,speedz;
private:
    VAO *object,*wings;
    VAO *body;
    VAO *front;
};

#endif // BALL_H
