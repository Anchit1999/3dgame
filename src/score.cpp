#include "score.h"
#include "main.h"
#include <math.h>
using namespace std;

// #define N 4
Score::Score(float x, float y, float rot, color_t color) {
    this->position = glm::vec3(x, y, 0);
    this->rotation = rot;
    double r1=10.0f,r2 = 2.0f;
    this->h = r1;
    this->wd = r2;
    // Our vertices. Three consecutive floats give a 3D vertex; Three consecutive vertices give a triangle.
    // A cube has 6 faces with 2 triangles each, so this makes 6*2=12 triangles, and 12*3 vertices
    // cout << theta << endl;
    GLfloat vertex_buffer_data[18] = {
        0.0f,0.0f,0.0f,
        r1,0,0.0f,
        r1,r2,0.0f,

        0.0f,0.0f,0.0f,
        r1,r2,0.0f,
        0,r2,0.0f,
    };
    // for(int i=0;i<)
    color_t black = {0,0,0};
    this->object = create3DObject(GL_TRIANGLES, 2*3, vertex_buffer_data,black, GL_FILL);
}

void Score::draw(glm::mat4 VP) {
    Matrices.model = glm::mat4(1.0f);
    glm::mat4 translate = glm::translate (this->position);    // glTranslatef
    glm::mat4 rotate    = glm::rotate((float) (this->rotation * M_PI / 180.0f), glm::vec3(0, 0, 1));
    // No need as coords centered at 0, 0, 0 of cube arouund which we waant to rotate
    // rotate          = rotate * glm::translate(glm::vec3(0, -0.6, 0));
    Matrices.model *= (translate * rotate);
    glm::mat4 MVP = VP * Matrices.model;
    glUniformMatrix4fv(Matrices.MatrixID, 1, GL_FALSE, &MVP[0][0]);
    draw3DObject(this->object);
}

void Score::set_position(float x, float y) {
    this->position = glm::vec3(x, y, 0);
}

void Score::tick(int dir) {
    //pass
}

Health::Health(float x, float y, color_t color) {
    this->position = glm::vec3(x, y, 0);
    double r1=10.0f;
    this->h = r1;
    // Our vertices. Three consecutive floats give a 3D vertex; Three consecutive vertices give a triangle.
    // A cube has 6 faces with 2 triangles each, so this makes 6*2=12 triangles, and 12*3 vertices
    // cout << theta << endl;
    GLfloat vertex_buffer_data[18] = {
        0,0,0,
        r1,0,0,
        r1,r1,0,

        0,0,0,
        r1,r1,0,
        0,r1,0,
    };
    // for(int i=0;i<)
    color_t black = {0,0,0};
    this->object = create3DObject(GL_TRIANGLES, 2*3, vertex_buffer_data,black, GL_FILL);
}

void Health::draw(glm::mat4 VP) {
    Matrices.model = glm::mat4(1.0f);
    glm::mat4 translate = glm::translate (this->position);    // glTranslatef
    glm::mat4 rotate    = glm::rotate((float) (this->rotation * M_PI / 180.0f), glm::vec3(0, 0, 1));
    // No need as coords centered at 0, 0, 0 of cube arouund which we waant to rotate
    // rotate          = rotate * glm::translate(glm::vec3(0, -0.6, 0));
    Matrices.model *= (translate * rotate);
    glm::mat4 MVP = VP * Matrices.model;
    glUniformMatrix4fv(Matrices.MatrixID, 1, GL_FALSE, &MVP[0][0]);
    draw3DObject(this->object);
}

void Health::set_position(float x, float y) {
    this->position = glm::vec3(x, y, 0);
}