#include "main.h"

#ifndef SCORE_H
#define SCORE_H


class Score {
public:
    Score() {}
    Score(float x, float y,float rot, color_t color);
    glm::vec3 position;
    float rotation,width;
    void draw(glm::mat4 VP);
    void set_position(float x, float y);
    void tick(int dir);
    double speedx,speedy;
    double wd,h;
    bounding_box_t bounding_box();
private:
    VAO *object;
};

class Health {
public:
    Health() {}
    Health(float x, float y, color_t color);
    glm::vec3 position;
    float rotation,width;
    void draw(glm::mat4 VP);
    void set_position(float x, float y);
    void tick(int dir);
    double speedx,speedy;
    double wd,h;
    bounding_box_t bounding_box();
private:
    VAO *object;
};

#endif // SCORE_H