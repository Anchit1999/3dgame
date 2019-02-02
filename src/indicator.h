#include "main.h"

#ifndef INDICATOR
#define INDICATOR


class Indicator {
public:
    Indicator() {}
    Indicator(float x, float y,float z, color_t color);
    glm::vec3 position;
    float rotation;
    void draw(glm::mat4 VP);
    void set_position(float x, float y,float z);
    void tick();
    double speed,wd,hg;
private:
    VAO *object;
};

#endif
