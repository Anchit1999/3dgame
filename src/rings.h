#include "main.h"

#ifndef RINGS
#define RINGS

class Rings {
public:
    Rings() {}
    Rings(float x, float y,float z, color_t color);
    glm::vec3 position;
    float rotation;
    void draw(glm::mat4 VP);
    void set_position(float x, float y);
    void tick();
    double speed,wd,hg,r1,r2;
    bool collision;
private:
    VAO *object;
};

#endif
