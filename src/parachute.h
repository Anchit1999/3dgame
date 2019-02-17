#include "main.h"

#ifndef PARACHUTE
#define PARACHUTE


class Parachute {
public:
    Parachute() {}
    Parachute(float x, float y,float z, color_t color);
    glm::vec3 position,rotation;
    void draw(glm::mat4 VP);
    void set_position(float x, float y,float z);
    void tick();
    bool collision;
    double speed,len,speedz,speedy;
    double width,height,depth;
    bounding_box_t bounding_box();
private:
    VAO *object;
    VAO *rod1,*rod2;
    VAO *box;
};

#endif // BALL_H
