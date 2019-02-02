#include "plane.h"
#include "main.h"

#define N 100
Plane::Plane(float x, float y,float z, color_t color)
{
    this->position = glm::vec3(x, y, z);
    this->rotation = glm::vec3(0, 0, 0);
    speed = 1;
    speedz = 0.0f;
    // Our vertices. Three consecutive floats give a 3D vertex; Three consecutive vertices give a triangle.
    // A cube has 6 faces with 2 triangles each, so this makes 6*2=12 triangles, and 12*3 vertices
    GLfloat r = 50.0f, h = 150.0f;
    this->len = h;
    GLfloat Cos1, Cos2, Sin1, Sin2, theta = 360.0 / N;
    // cout << theta << endl;
    GLfloat vertex_buffer_data1[2 * 9 * N];
    for (int i = 0; i < 2 * N; i++)
    {
        Cos1 = cos(i / 2 * theta * M_PI / 180.0);
        Sin1 = sin(i / 2 * theta * M_PI / 180.0);
        Cos2 = cos((i / 2 + 1) * theta * M_PI / 180.0);
        Sin2 = sin((i / 2 + 1) * theta * M_PI / 180.0);
        // cout << Cos1 << " " << Cos2 << endl;
        vertex_buffer_data1[9 * i] = r * Cos1;
        vertex_buffer_data1[9 * i + 1] = r * Sin1;
        vertex_buffer_data1[9 * i + 2] = h;
        vertex_buffer_data1[9 * i + 3] = r * Cos2;
        vertex_buffer_data1[9 * i + 4] = r * Sin2;
        vertex_buffer_data1[9 * i + 5] = 0.0f;
        if (i % 2 == 0)
        {
            vertex_buffer_data1[9 * i + 6] = r * Cos1;
            vertex_buffer_data1[9 * i + 7] = r * Sin1;
            vertex_buffer_data1[9 * i + 8] = 0.0f;
        }
        else
        {
            vertex_buffer_data1[9 * i + 6] = r * Cos2;
            vertex_buffer_data1[9 * i + 7] = r * Sin2;
            vertex_buffer_data1[9 * i + 8] = h;
        }
    }
    this->body = create3DObject(GL_TRIANGLES, 2 * N * 3, vertex_buffer_data1, color, GL_FILL);

    // this->body = Cylinder(x,y,z,r,r,h,COLOR_GREEN);

    GLfloat vertex_buffer_data2[9 * N];
    for (int i = 0; i < N; i++)
    {
        Cos1 = cos(i * theta * M_PI / 180.0);
        Sin1 = sin(i * theta * M_PI / 180.0);
        Cos2 = cos((i + 1) * theta * M_PI / 180.0);
        Sin2 = sin((i + 1) * theta * M_PI / 180.0);
        // cout << Cos1 << " " << Cos2 << endl;
        vertex_buffer_data2[9 * i] = r * Cos1;
        vertex_buffer_data2[9 * i + 1] = r * Sin1;
        vertex_buffer_data2[9 * i + 2] = 0.0f;

        vertex_buffer_data2[9 * i + 3] = r * Cos2;
        vertex_buffer_data2[9 * i + 4] = r * Sin2;
        vertex_buffer_data2[9 * i + 5] = 0.0f;

        vertex_buffer_data2[9 * i + 6] = 0.0f;
        vertex_buffer_data2[9 * i + 7] = 0.0f;
        vertex_buffer_data2[9 * i + 8] = -h/2;
    }
    this->front = create3DObject(GL_TRIANGLES, N * 3, vertex_buffer_data2, COLOR_BLACK, GL_FILL);

    GLfloat vertex_buffer_data[] = {
        0.0,0.0,0.0+20,
        2*r,0.0,0.0+20,
        0.0,0.0,h/2+20,
        
        0.0,0.0,0.0+20,
        -2*r,0.0,0.0+20,
        0.0,0.0,h/2+20,
        
        0.0,0.0,h/2,
        0.0,0.0,h,
        0.0,2*r,h,
    };
    this->wings = create3DObject(GL_TRIANGLES, 3*3, vertex_buffer_data,COLOR_BLACK, GL_FILL);
}

void Plane::draw(glm::mat4 VP)
{
    Matrices.model = glm::mat4(1.0f);
    glm::mat4 translate = glm::translate(this->position); // glTranslatef
    glm::mat4 rotate = glm::rotate((float)(this->rotation.x * M_PI / 180.0f), glm::vec3(1, 0, 0));
    rotate *= glm::rotate((float)(this->rotation.y * M_PI / 180.0f), glm::vec3(0, 1, 0));
    rotate *= glm::rotate((float)(this->rotation.z * M_PI / 180.0f), glm::vec3(0, 0, 1));
    // No need as coords centered at 0, 0, 0 of cube arouund which we waant to rotate
    // rotate          = rotate * glm::translate(glm::vec3(0, -0.6, 0));
    Matrices.model *= (translate * rotate);
    glm::mat4 MVP = VP * Matrices.model;
    glUniformMatrix4fv(Matrices.MatrixID, 1, GL_FALSE, &MVP[0][0]);
    draw3DObject(this->wings);
    draw3DObject(this->body);
    draw3DObject(this->front);
}

void Plane::set_position(float x, float y)
{
    this->position = glm::vec3(x, y, 0);
}

void Plane::tick()
{
    // this->rotation += speed;
    if(this->speedz > 0)
    {
        this->speedz -= 0.1;
        if(this->speedz<0) this->speedz = 0;
    }
    // this->position.x -= speed;
    // this->position.y -= speed;
}
