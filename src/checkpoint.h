#include "main.h"

#ifndef CHKPT
#define CHKPT


class CheckPoint {
public:
    CheckPoint() {}
    CheckPoint(float x, float y,float z, color_t color);
    glm::vec3 position,rotation;
    void draw(glm::mat4 VP);
    void set_position(float x, float y,float z);
    void tick();
    double speed;
    double width,height,depth;
    bounding_box_t bounding_box();
private:
    VAO *ground,*enemy,*cannon;
};

class Arrow {
public:
    Arrow() {}
    Arrow(float x, float y,float z, color_t color);
    glm::vec3 position,rotation;
    void draw(glm::mat4 VP);
    void set_position(float x, float y,float z);
    void tick();
    double speed;
private:
    VAO *object;
};
#endif // CHKPT
