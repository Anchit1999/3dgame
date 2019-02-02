#include "main.h"

#ifndef METER
#define METER


class Meter {
public:
    Meter() {}
    Meter(float x, float y,float z, color_t color);
    glm::vec3 position;
    float rotation;
    void draw(glm::mat4 VP);
    void set_position(float x, float y);
    void tick();
    double speed,wd,hg;
private:
    VAO *object;
};

class OdoMeter {
public:
    OdoMeter() {}
    OdoMeter(float x, float y,float z, color_t color);
    glm::vec3 position;
    float rotation;
    void draw(glm::mat4 VP);
    void set_position(float x, float y);
    void tick();
    double speed,wd,hg,r1,r2;
private:
    VAO *object,*line;
};

#endif
