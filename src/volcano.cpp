#include "volcano.h"
#include "main.h"

#define N 100
Volcano::Volcano(float x, float y,float z, color_t color)
{
    this->position = glm::vec3(x, y, z);
    this->rotation = glm::vec3(0, 0, 0);
    speed = 1;
    speedz = 0.0f;
    speedy = 5.0f;
    // Our vertices. Three consecutive floats give a 3D vertex; Three consecutive vertices give a triangle.
    // A cube has 6 faces with 2 triangles each, so this makes 6*2=12 triangles, and 12*3 vertices
    GLfloat r = 200.0f;
    // cout << theta << endl;
    GLfloat vertex_buffer_data0[] = {
        1.1*r,0,1.1*r,
        1.1*r,0,-1.1*r,
        -1.1*r,0,1.1*r,

        -1.1*r,0,-1.1*r,
        1.1*r,0,-1.1*r,
        -1.1*r,0,1.1*r,
    };
    color_t br = {128,0,0};
    this->ground = create3DObject(GL_TRIANGLES, 2*3, vertex_buffer_data0, br, GL_FILL);

    GLfloat vertex_buffer_data[] = {
        r,0,r,
        r,0,-r,
        0,r,0,

        r,0,-r,
        -r,0,-r,
        0,r,0,

        -r,0,-r,
        -r,0,r,
        0,r,0,

        -r,0,r,
        r,0,r,
        0,r,0,
    };

    // color_t cc = {222,184,135};

    GLfloat vertex_buffer_color[] = {
        222/255.0,184/255.0,135/255.0,
        222/255.0,184/255.0,135/255.0,
        255/255.0,128/255.0,0/255.0,

        222/255.0,184/255.0,135/255.0,
        222/255.0,184/255.0,135/255.0,
        255/255.0,128/255.0,0/255.0,

        222/255.0,184/255.0,135/255.0,
        222/255.0,184/255.0,135/255.0,
        255/255.0,128/255.0,0/255.0,

        222/255.0,184/255.0,135/255.0,
        222/255.0,184/255.0,135/255.0,
        255/255.0,128/255.0,0/255.0,
    };

    color_t dd = {255,128,0};
    this->body = create3DObject(GL_TRIANGLES, 4 * 3, vertex_buffer_data, vertex_buffer_color, GL_FILL);
}

void Volcano::draw(glm::mat4 VP)
{
    Matrices.model = glm::mat4(1.0f);
    glm::mat4 translate = glm::translate(this->position); // glTranslatef
    glm::mat4 rotate = glm::rotate((float)(this->rotation.y * M_PI / 180.0f), glm::vec3(0, 1, 0));
    rotate *= glm::rotate((float)(this->rotation.x * M_PI / 180.0f), glm::vec3(1, 0, 0));
    rotate *= glm::rotate((float)(this->rotation.z * M_PI / 180.0f), glm::vec3(0, 0, 1));
    // No need as coords centered at 0, 0, 0 of cube arouund which we waant to rotate
    // rotate          = rotate * glm::translate(glm::vec3(0, -0.6, 0));
    Matrices.model *= (translate * rotate);
    glm::mat4 MVP = VP * Matrices.model;
    glUniformMatrix4fv(Matrices.MatrixID, 1, GL_FALSE, &MVP[0][0]);
    draw3DObject(this->body);
    // draw3DObject(this->top);
    draw3DObject(this->ground);
}

void Volcano::set_position(float x, float y)
{
    this->position = glm::vec3(x, y, 0);
}

void Volcano::tick()
{

}
