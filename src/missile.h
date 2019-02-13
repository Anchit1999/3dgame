#include "main.h"

#ifndef MISSILE
#define MISSILE


class Missile {
public:
    Missile() {}
    Missile(float x, float y,float z,float r,float h,int type, color_t color);
    glm::vec3 position,rotation;
    void draw(glm::mat4 VP);
    void set_position(float x, float y,float z);
    void tick();
    int type;
    double speed,len,speedz,speedy,r,h;
    bool shoot;
private:
    VAO *object;
    VAO *body;
    VAO *front;
};

#endif // BALL_H
