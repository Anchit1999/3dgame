#include "parachute.h"
#include "main.h"

#define N 100
int flag=0;
Parachute::Parachute(float x, float y,float z, color_t color)
{
    this->position = glm::vec3(x, y, z);
    this->rotation = glm::vec3(0, 0, 0);
    this->collision = 0;
    speed = 1;
    speedz = 0.0f;
    speedy = 5.0f;
    
    GLfloat r = 40.0f, h = 40.0f;
    this->len = h;
    GLfloat Cos1, Cos2, Sin1, Sin2, theta = 360.0 / N;
    // cout << theta << endl;
    GLfloat vertex_buffer_data1[2 * 9 * N];
    for (int i = 0; i < 2 * N / 3; i++)
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
    this->object = create3DObject(GL_TRIANGLES, 2 * N/3.0 * 3, vertex_buffer_data1, cc, GL_FILL);

    r = 2.0f, h = 40.0f;
    GLfloat vertex_buffer_data2[2 * 9 * N];
    for (int i = 0; i < 2 * N; i++)
    {
        Cos1 = cos(i / 2 * theta * M_PI / 180.0);
        Sin1 = sin(i / 2 * theta * M_PI / 180.0);
        Cos2 = cos((i / 2 + 1) * theta * M_PI / 180.0);
        Sin2 = sin((i / 2 + 1) * theta * M_PI / 180.0);
        // cout << Cos1 << " " << Cos2 << endl;
        vertex_buffer_data2[9 * i] = r * Cos1;
        vertex_buffer_data2[9 * i + 1] = h;
        vertex_buffer_data2[9 * i + 2] = r * Sin1;
        vertex_buffer_data2[9 * i + 3] = r * Cos2;
        vertex_buffer_data2[9 * i + 4] = 0.0f;
        vertex_buffer_data2[9 * i + 5] = r * Sin2;
        if (i % 2 == 0)
        {
            vertex_buffer_data2[9 * i + 6] = r * Cos1;
            vertex_buffer_data2[9 * i + 7] = 0.0f;
            vertex_buffer_data2[9 * i + 8] = r * Sin1;
        }
        else
        {
            vertex_buffer_data2[9 * i + 6] = r * Cos2;
            vertex_buffer_data2[9 * i + 7] = h;
            vertex_buffer_data2[9 * i + 8] = r * Sin2;
        }
    }
    // color_t cc = {26, 26, 26};
    this->rod1 = create3DObject(GL_TRIANGLES, 2 * N * 3, vertex_buffer_data2, COLOR_RED, GL_FILL);
    this->rod2 = create3DObject(GL_TRIANGLES, 2 * N * 3, vertex_buffer_data2, COLOR_RED, GL_FILL);

    r = 20.0f;

    GLfloat vertex_buffer_data3[] = {
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
    this->width = this->height = this->depth=h/2;
    this->box = create3DObject(GL_TRIANGLES, 12 * 3, vertex_buffer_data3, COLOR_GREEN, GL_FILL);


}

void Parachute::draw(glm::mat4 VP)
{
    Matrices.model = glm::mat4(1.0f);
    glm::mat4 translate = glm::translate(this->position); // glTranslatef
    glm::mat4 rotate = glm::rotate((float)(this->rotation.y * M_PI / 180.0f), glm::vec3(0, 1, 0));
    rotate *= glm::rotate((float)(this->rotation.x * M_PI / 180.0f), glm::vec3(1, 0, 0));
    rotate *= glm::rotate((float)((this->rotation.z + 30) * M_PI / 180.0f), glm::vec3(0, 0, 1));

    Matrices.model *= (translate * rotate);
    glm::mat4 MVP = VP * Matrices.model;
    glUniformMatrix4fv(Matrices.MatrixID, 1, GL_FALSE, &MVP[0][0]);
    draw3DObject(this->object);

    Matrices.model = glm::mat4(1.0f);
    this->position.z += 20.0;
    translate = glm::translate(this->position);
    rotate = glm::rotate((float)((this->rotation.z + 55) * M_PI / 180.0f), glm::vec3(0, 0, 1));
    Matrices.model *= (translate * rotate);
    MVP = VP * Matrices.model;
    glUniformMatrix4fv(Matrices.MatrixID, 1, GL_FALSE, &MVP[0][0]);
    draw3DObject(this->rod1);

    Matrices.model = glm::mat4(1.0f);
    rotate = glm::rotate((float)((this->rotation.z + -55) * M_PI / 180.0f), glm::vec3(0, 0, 1));

    Matrices.model *= (translate * rotate);
    MVP = VP * Matrices.model;
    glUniformMatrix4fv(Matrices.MatrixID, 1, GL_FALSE, &MVP[0][0]);
    draw3DObject(this->rod2);

    rotate = glm::rotate((float)(this->rotation.z * M_PI / 180.0f), glm::vec3(0, 0, 1));
    MVP = VP * translate * rotate;
    glUniformMatrix4fv(Matrices.MatrixID, 1, GL_FALSE, &MVP[0][0]);
    draw3DObject(this->box);
    this->position.z -=20.0;
}

void Parachute::set_position(float x, float y,float z)
{
    this->position = glm::vec3(x, y, z);
    this->collision = 0;
}

void Parachute::tick()
{
    if(flag==0)
    {
        this->rotation.z += 0.5;
        if(this->rotation.z > 30)
            flag=1;
    }
    if(flag==1)
    {
        this->rotation.z -= 0.5;
        if(this->rotation.z < -30)
            flag=0;
    }
    this->position.y -= 1;
    if(this->position.y < -600)
        this->collision = 1;
}

bounding_box_t Parachute::bounding_box()
{
    float x = this->position.x, y = this->position.y + 20, z = this->position.z + 20;
    bounding_box_t para = {x, y, z,this->width, this->height, this->depth};
    return para;
}
