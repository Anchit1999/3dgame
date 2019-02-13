#include "missile.h"
#include "main.h"

#define N 100
Missile::Missile(float x, float y,float z,float r,float h,int type, color_t color)
{
    this->position = glm::vec3(x, y, z);
    this->rotation = glm::vec3(0, 0, 0);
    this->shoot = 0;
    this->type = type;
    speed = 1;
    speedz = 0.0f;
    speedy = 5.0f;
    // Our vertices. Three consecutive floats give a 3D vertex; Three consecutive vertices give a triangle.
    // A cube has 6 faces with 2 triangles each, so this makes 6*2=12 triangles, and 12*3 vertices
    // this->r = 20.0f, h = 50.0f;
    this->r = r;
    this->h = h;
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
    color_t cc = {26, 26, 26};
    this->body = create3DObject(GL_TRIANGLES, 2 * N * 3, vertex_buffer_data1, cc, GL_FILL);

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

        if(this->type == 3)
        {
            vertex_buffer_data2[9 * i + 2] = h;
            vertex_buffer_data2[9 * i + 5] = h;
            vertex_buffer_data2[9 * i + 8] = h + h/2;
        }
    }
    color_t dd = {255,255,0};
    this->front = create3DObject(GL_TRIANGLES, N * 3, vertex_buffer_data2, dd, GL_FILL);
}

void Missile::draw(glm::mat4 VP)
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
    draw3DObject(this->front);
}

void Missile::set_position(float x, float y,float z)
{
    this->position = glm::vec3(x, y, z);
}

void Missile::tick()
{
    // this->rotation += speed;
    // this->position.z += 30.0f;
    if(this->type == 1)
    {
        this->position.z -= 30 * cos(this->rotation.y * M_PI/180.0) * cos(this->rotation.x * M_PI/180.0);
        this->position.x -= 30 * sin(this->rotation.y * M_PI/180.0);
        this->position.y += 15 * sin(this->rotation.x * M_PI/180.0);
    }
    else if(this->type == 2)
    {
        this->position.y -= 10;
    }
    if(this->position.z > 1000000 or this->position.z < -1000000) this->shoot = 0;
    if(this->position.x > 1000000 or this->position.x < -1000000) this->shoot = 0;
    if(this->position.y > 3000 or this->position.y < -600) this->shoot = 0;

    // this->position.x -= speed;
    // this->position.y -= speed;
}
