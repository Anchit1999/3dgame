#include "checkpoint.h"
#include "main.h"

double r=200.0,re = 50.0;
CheckPoint::CheckPoint(float x, float y,float z, color_t color) {
    this->position = glm::vec3(x, y, 0);
    this->rotation = glm::vec3(0,0,0);
    speed = 1;
    // Our vertices. Three consecutive floats give a 3D vertex; Three consecutive vertices give a triangle.
    // A cube has 6 faces with 2 triangles each, so this makes 6*2=12 triangles, and 12*3 vertices
    GLfloat vertex_buffer_data[] = {
        r,0,r,
        r,0,-r,
        0,r/20,0,

        r,0,-r,
        -r,0,-r,
        0,r/20,0,

        -r,0,-r,
        -r,0,r,
        0,r/20,0,

        -r,0,r,
        r,0,r,
        0,r/20,0,
    };

    // color_t cc = {222,184,135};

    GLfloat vertex_buffer_color[] = {
        255/255.0,255/255.0,204/255.0,
        255/255.0,255/255.0,204/255.0,
        153/255.0,255/255.0,255/255.0,

        255/255.0,255/255.0,204/255.0,
        255/255.0,255/255.0,204/255.0,
        153/255.0,255/255.0,255/255.0,

        255/255.0,255/255.0,204/255.0,
        255/255.0,255/255.0,204/255.0,
        153/255.0,255/255.0,255/255.0,

        255/255.0,255/255.0,204/255.0,
        255/255.0,255/255.0,204/255.0,
        153/255.0,255/255.0,255/255.0,
    };
    this->ground = create3DObject(GL_TRIANGLES, 4 * 3, vertex_buffer_data, vertex_buffer_color, GL_FILL);

    GLfloat vertex_buffer_data1[] = {
        //bottom
        re,0,re,
        re,0,-re,
        -re,0,re,

        -re,0,re,
        re,0,-re,
        -re,0,-re,

        //top
        re,re,re,
        re,re,-re,
        -re,re,re,

        -re,re,re,
        re,re,-re,
        -re,re,-re,

        //right
        re,0,re,
        re,0,-re,
        re,re,re,

        re,re,re,
        re,re,-re,
        re,0,-re,

        //left
        -re,0,re,
        -re,0,-re,
        -re,re,re,

        -re,re,re,
        -re,re,-re,
        -re,0,-re,

        //front
        re,0,re,
        -re,0,re,
        re,re,re,

        re,re,re,
        -re,0,re,
        -re,re,re,

        // back
        re,0,-re,
        -re,0,-re,
        re,re,-re,

        re,re,-re,
        -re,re,-re,
        -re,0,-re,
    };
    this->width = this->height = this->depth = 2*re;
    this->enemy = create3DObject(GL_TRIANGLES, 12 * 3, vertex_buffer_data1, COLOR_BLACK, GL_FILL);

    int N = 100;
    GLfloat Cos1, Cos2, Sin1, Sin2, theta = 360.0 / N,h = 100.0;
    GLfloat vertex_buffer_data2[2 * 9 * N];
    for (int i = 0; i < 2 * N; i++)
    {
        Cos1 = cos(i / 2 * theta * M_PI / 180.0);
        Sin1 = sin(i / 2 * theta * M_PI / 180.0);
        Cos2 = cos((i / 2 + 1) * theta * M_PI / 180.0);
        Sin2 = sin((i / 2 + 1) * theta * M_PI / 180.0);
        // cout << Cos1 << " " << Cos2 << endl;
        vertex_buffer_data2[9 * i] = re/4.0 * Cos1;
        vertex_buffer_data2[9 * i + 1] = re/2 + re/4.0 * Sin1;
        vertex_buffer_data2[9 * i + 2] = h;
        vertex_buffer_data2[9 * i + 3] = re/4.0 * Cos2;
        vertex_buffer_data2[9 * i + 4] = re/2 + re/4.0 * Sin2;
        vertex_buffer_data2[9 * i + 5] = 0.0f;
        if (i % 2 == 0)
        {
            vertex_buffer_data2[9 * i + 6] = re/4.0 * Cos1;
            vertex_buffer_data2[9 * i + 7] = re/2 + re/4.0 * Sin1;
            vertex_buffer_data2[9 * i + 8] = 0.0f;
        }
        else
        {
            vertex_buffer_data2[9 * i + 6] = re/4.0 * Cos2;
            vertex_buffer_data2[9 * i + 7] = re/2 + re/4.0 * Sin2;
            vertex_buffer_data2[9 * i + 8] = h;
        }
    }
    color_t cc = {26, 26, 26};
    this->cannon = create3DObject(GL_TRIANGLES, 2 * N * 3, vertex_buffer_data2, cc, GL_FILL);

}

void CheckPoint::draw(glm::mat4 VP) {
    Matrices.model = glm::mat4(1.0f);
    glm::mat4 translate = glm::translate (this->position);    // glTranslatef
    glm::mat4 rotate = glm::rotate((float)(0 * M_PI / 180.0f), glm::vec3(0, 1, 0));
    
    Matrices.model *= (translate * rotate);
    glm::mat4 MVP = VP * Matrices.model;
    glUniformMatrix4fv(Matrices.MatrixID, 1, GL_FALSE, &MVP[0][0]);
    draw3DObject(this->ground);
    draw3DObject(this->enemy);

    Matrices.model = glm::mat4(1.0f);
    rotate = glm::rotate((float)(this->rotation.y * M_PI / 180.0f), glm::vec3(0, 1, 0));
    rotate *= glm::rotate((float)(this->rotation.x * M_PI / 180.0f), glm::vec3(1, 0, 0));
    rotate *= glm::rotate((float)(this->rotation.z * M_PI / 180.0f), glm::vec3(0, 0, 1));
    // No need as coords centered at 0, 0, 0 of cube arouund which we waant to rotate
    // rotate          = rotate * glm::translate(glm::vec3(0, -0.6, 0));
    Matrices.model *= (translate * rotate);
    MVP = VP * Matrices.model;
    glUniformMatrix4fv(Matrices.MatrixID, 1, GL_FALSE, &MVP[0][0]);
    draw3DObject(this->cannon);
}

void CheckPoint::set_position(float x, float y,float z) {
    this->position = glm::vec3(x, y, z);
}

void CheckPoint::tick() {
    // this->rotation += speed;
    // this->position.x -= speed;
    // this->position.y -= speed;
}

bounding_box_t CheckPoint::bounding_box()
{
    float x = this->position.x, y = this->position.y, z = this->position.z;
    bounding_box_t ball = {x, y, z,this->width, this->height, this->depth};
    return ball;
}


Arrow::Arrow(float x, float y,float z, color_t color) {
    this->position = glm::vec3(x, y, z);
    this->rotation = glm::vec3(0,0,0);
    speed = 1;
    float d=100.0;
    // Our vertices. Three consecutive floats give a 3D vertex; Three consecutive vertices give a triangle.
    // A cube has 6 faces with 2 triangles each, so this makes 6*2=12 triangles, and 12*3 vertices
    float w1=10.0,w2=5.0,hg1=20.0,hg2=20.0;
    // float w1=50.0,w2=25.0,hg1=100.0,hg2=100.0;
    static const GLfloat vertex_buffer_data[] = {
        0,hg1,0,
        w1,0,0,
        -w1,0,0,

        w2,-hg2,0,
        -w2,0,0,
        w2,0,0,

        w2,-hg2,0,
        -w2,-hg2,0,
        -w2,0,0,
    };

    this->object = create3DObject(GL_TRIANGLES, 3*3, vertex_buffer_data, COLOR_RED, GL_FILL);
}

void Arrow::draw(glm::mat4 VP) {
    Matrices.model = glm::mat4(1.0f);
    glm::mat4 translate = glm::translate (this->position);    // glTranslatef
    glm::mat4 rotate = glm::rotate((float)(this->rotation.y * M_PI / 180.0f), glm::vec3(0, 1, 0));
    rotate *= glm::rotate((float)(this->rotation.x * M_PI / 180.0f), glm::vec3(1, 0, 0));
    rotate *= glm::rotate((float)(this->rotation.z * M_PI / 180.0f), glm::vec3(0, 0, 1));
    // No need as coords centered at 0, 0, 0 of cube arouund which we waant to rotate
    // rotate          = rotate * glm::translate(glm::vec3(0, -0.6, 0));
    Matrices.model *= (translate * rotate);
    glm::mat4 MVP = VP * Matrices.model;
    glUniformMatrix4fv(Matrices.MatrixID, 1, GL_FALSE, &MVP[0][0]);
    draw3DObject(this->object);
}

void Arrow::set_position(float x, float y,float z) {
    this->position = glm::vec3(x, y, z);
}

void Arrow::tick() {
    // this->rotation += speed;
    // this->position.x -= speed;
    // this->position.y -= speed;
}