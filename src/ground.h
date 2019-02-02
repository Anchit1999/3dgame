#include "main.h"

#ifndef GROUND
#define GROUND


class Ground {
public:
    Ground() {}
    Ground(float x, float y, color_t color);
    glm::vec3 position;
    float rotation,depth;
    void draw(glm::mat4 VP);
    void set_position(float x, float y);
    void tick();
    double speed;
private:
    VAO *object;
};

class Square {
public:
    Square() {}
    Square(float x, float y,float z, color_t color);
    glm::vec3 position;
    float rotation;
    void draw(glm::mat4 VP);
    void set_position(float x, float y);
    void tick();
    double speed;
private:
    VAO *object;
};
#endif // GROUND
