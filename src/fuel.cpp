#include "fuel.h"
#include "main.h"

#define N 100
Fuel::Fuel(float x, float y,float z, color_t color)
{
    this->position = glm::vec3(x, y, z);
    this->rotation = glm::vec3(0, 0, 0);
    speed = 1;
    speedz = 0.0f;
    speedy = 5.0f;
    // Our vertices. Three consecutive floats give a 3D vertex; Three consecutive vertices give a triangle.
    // A cube has 6 faces with 2 triangles each, so this makes 6*2=12 triangles, and 12*3 vertices
    GLfloat r = 100.0f,d = 10.0f;
    this->width = this->height = this-> depth = r;
    GLfloat vertex_buffer_data[] = {
        //front
        -r/2,-r/2,r/2,
        r/2,-r/2,r/2,
        r/2,r/2,r/2,

        -r/2,-r/2,r/2,
        -r/2,r/2,r/2,
        r/2,r/2,r/2,

        // back
        -r/2,-r/2,-r/2,
        r/2,-r/2,-r/2,
        r/2,r/2,-r/2,

        -r/2,-r/2,-r/2,
        -r/2,r/2,-r/2,
        r/2,r/2,-r/2,

        // right

        r/2,-r/2,r/2,
        r/2,r/2,r/2,
        r/2,-r/2,-r/2,

        r/2,r/2,r/2,
        r/2,r/2,-r/2,
        r/2,-r/2,-r/2,

        //left

        -r/2,-r/2,r/2,
        -r/2,r/2,r/2,
        -r/2,-r/2,-r/2,

        -r/2,r/2,r/2,
        -r/2,r/2,-r/2,
        -r/2,-r/2,-r/2,

        // top
        r/2,r/2,r/2,
        -r/2,r/2,r/2,
        r/2,r/2,-r/2,

        r/2,r/2,-r/2,
        -r/2,r/2,-r/2,
        -r/2,r/2,r/2,

        // bottom
        r/2,-r/2,r/2,
        -r/2,-r/2,r/2,
        r/2,-r/2,-r/2,

        r/2,-r/2,-r/2,
        -r/2,-r/2,-r/2,
        -r/2,-r/2,r/2,
    };

    GLfloat vertex_buffer_color[] = {
        //front
        (-r+r)/255,(-r+r)/255,(r+r)/255,
        (r+r)/255,(-r+r)/255,(r+r)/255,
        (r+r)/255,(r+r)/255,(r+r)/255,

        (-r+r)/255,(-r+r)/255,(r+r)/255,
        (-r+r)/255,(r+r)/255,(r+r)/255,
        (r+r)/255,(r+r)/255,(r+r)/255,

        // back
        (-r+r)/255,(-r+r)/255,(-r+r)/255,
        (r+r)/255,(-r+r)/255,(-r+r)/255,
        (r+r)/255,(r+r)/255,(-r+r)/255,

        (-r+r)/255,(-r+r)/255,(-r+r)/255,
        (-r+r)/255,(r+r)/255,(-r+r)/255,
        (r+r)/255,(r+r)/255,(-r+r)/255,

        // right

        (r+r)/255,(-r+r)/255,(r+r)/255,
        (r+r)/255,(r+r)/255,(r+r)/255,
        (r+r)/255,(-r+r)/255,(-r+r)/255,

        (r+r)/255,(r+r)/255,(r+r)/255,
        (r+r)/255,(r+r)/255,(-r+r)/255,
        (r+r)/255,(-r+r)/255,(-r+r)/255,

        //left

        (-r+r)/255,(-r+r)/255,(r+r)/255,
        (-r+r)/255,(r+r)/255,(r+r)/255,
        (-r+r)/255,(-r+r)/255,(-r+r)/255,

        (-r+r)/255,(r+r)/255,(r+r)/255,
        (-r+r)/255,(r+r)/255,(-r+r)/255,
        (-r+r)/255,(-r+r)/255,(-r+r)/255,

        // top
        (r+r)/255,(r+r)/255,(r+r)/255,
        (-r+r)/255,(r+r)/255,(r+r)/255,
        (r+r)/255,(r+r)/255,(-r+r)/255,

        (r+r)/255,(r+r)/255,(-r+r)/255,
        (-r+r)/255,(r+r)/255,(-r+r)/255,
        (-r+r)/255,(r+r)/255,(r+r)/255,

        // bottom
        (r+r)/255,(-r+r)/255,(r+r)/255,
        (-r+r)/255,(-r+r)/255,(r+r)/255,
        (r+r)/255,(-r+r)/255,(-r+r)/255,

        (r+r)/255,(-r+r)/255,(-r+r)/255,
        (-r+r)/255,(-r+r)/255,(-r+r)/255,
        (-r+r)/255,(-r+r)/255,(r+r)/255,
    };
    this->body = create3DObject(GL_TRIANGLES, 12 * 3, vertex_buffer_data, vertex_buffer_color, GL_FILL);
}

void Fuel::draw(glm::mat4 VP)
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
    // draw3DObject(this->ground);
}

void Fuel::set_position(float x, float y,float z)
{
    this->position = glm::vec3(x, y, z);
}

void Fuel::tick()
{
    this->rotation.y += 1;
}

bounding_box_t Fuel::bounding_box()
{
    float x = this->position.x, y = this->position.y, z = this->position.z;
    bounding_box_t ball = {x, y, z,this->width, this->height, this->depth};
    return ball;
}
