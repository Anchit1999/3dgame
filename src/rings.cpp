#include "rings.h"
#include "main.h"

Rings::Rings(float x, float y,float z, color_t color) {
    this->position = glm::vec3(x, y, z);
    this->hg = 20.0f;
    this->wd = 0.5f;
    this->rotation = 0;
    this->collision =0;
    int N = 100;
    // Our vertices. Three consecutive floats give a 3D vertex; Three consecutive vertices give a triangle.
    // A cube has 6 faces with 2 triangles each, so this makes 6*2=12 triangles, and 12*3 vertices
    this->r1 = 150.0f, this->r2 = r1 - 5.0f;
    // Our vertices. Three consecutive floats give a 3D vertex; Three consecutive vertices give a triangle.
    // A cube has 6 faces with 2 triangles each, so this makes 6*2=12 triangles, and 12*3 vertices
    GLfloat Cos1,Cos2,Sin1,Sin2,theta = 360.0/N;
    // cout << theta << endl;
    GLfloat vertex_buffer_data[2*9*N];
    for(int i=0;i<2*N;i++)
    {
        Cos1 = cos ( i/2 * theta * M_PI / 180.0 );
        Sin1 = sin ( i/2 * theta * M_PI / 180.0 );
        Cos2 = cos ( (i/2+1) * theta * M_PI / 180.0 );
        Sin2 = sin ( (i/2+1) * theta * M_PI / 180.0 );
        // cout << Cos1 << " " << Cos2 << endl;
        if (i % 2)
        {
            vertex_buffer_data[9 * i] = r2 * Cos1;
            vertex_buffer_data[9 * i + 1] = r2 * Sin1;
            vertex_buffer_data[9 * i + 2] = 0.0f;
            vertex_buffer_data[9 * i + 3] = r2 * Cos2;
            vertex_buffer_data[9 * i + 4] = r2 * Sin2;
            vertex_buffer_data[9 * i + 5] = 0.0f;
            vertex_buffer_data[9 * i + 6] = r1 * Cos2;
            vertex_buffer_data[9 * i + 7] = r1 * Sin2;
            vertex_buffer_data[9 * i + 8] = 0.0f;
        }
        else
        {
            vertex_buffer_data[9 * i] = r1 * Cos1;
            vertex_buffer_data[9 * i + 1] = r1 * Sin1;
            vertex_buffer_data[9 * i + 2] = 0.0f;
            vertex_buffer_data[9 * i + 3] = r2 * Cos1;
            vertex_buffer_data[9 * i + 4] = r2 * Sin1;
            vertex_buffer_data[9 * i + 5] = 0.0f;
            vertex_buffer_data[9 * i + 6] = r1 * Cos2;
            vertex_buffer_data[9 * i + 7] = r1 * Sin2;
            vertex_buffer_data[9 * i + 8] = 0.0f;
        }
    }
    this->width = this->height = r2;
    this->depth = 0;
    this->object = create3DObject(GL_TRIANGLES, 2*N*3, vertex_buffer_data, color, GL_FILL);
}

void Rings::draw(glm::mat4 VP) {
    Matrices.model = glm::mat4(1.0f);
    glm::mat4 translate = glm::translate (this->position);    // glTranslatef
    glm::mat4 rotate    = glm::rotate((float) (this->rotation * M_PI / 180.0f), glm::vec3(0, 0, 1));

    Matrices.model *= (translate * rotate);
    glm::mat4 MVP = VP * Matrices.model;
    glUniformMatrix4fv(Matrices.MatrixID, 1, GL_FALSE, &MVP[0][0]);
    draw3DObject(this->object);
}

void Rings::set_position(float x, float y) {
    this->position = glm::vec3(x, y, 0);
}

void Rings::tick() {
    // this->rotation += speed;
    // this->position.x -= speed;
    // this->position.y -= speed;
}

bounding_box_t Rings::bounding_box()
{
    float x = this->position.x, y = this->position.y, z = this->position.z;
    bounding_box_t para = {x, y, z,this->width, this->height, this->depth};
    return para;
}