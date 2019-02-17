#include "main.h"

#ifndef FUEL
#define FUEL


class Fuel {
public:
    Fuel() {}
    Fuel(float x, float y,float z, color_t color);
    glm::vec3 position,rotation;
    void draw(glm::mat4 VP);
    void set_position(float x, float y,float z);
    void tick();
    double speed,len,speedz,speedy;
    double width,height,depth;
    bounding_box_t bounding_box();
private:
    VAO *body;
};

#endif // BALL_H
