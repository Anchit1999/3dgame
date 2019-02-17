#include "main.h"

#ifndef VOLCANO
#define VOLCANO


class Volcano {
public:
    Volcano() {}
    Volcano(float x, float y,float z, color_t color);
    glm::vec3 position,rotation;
    void draw(glm::mat4 VP);
    void set_position(float x, float y);
    void tick();
    double speed,len,speedz,speedy;
    double width,height,depth;
    bounding_box_t bounding_box();
private:
    VAO *ground;
    VAO *body;
    VAO *top;
};

#endif // BALL_H
