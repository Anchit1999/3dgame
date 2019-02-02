#include "Meters.h"
#include "main.h"

Meter::Meter(float x, float y,float z, color_t color) {
    this->position = glm::vec3(x, y, z);
    this->hg = 200.0f;
    this->wd = 5.0f;
    this->rotation = 90;
    // Our vertices. Three consecutive floats give a 3D vertex; Three consecutive vertices give a triangle.
    // A cube has 6 faces with 2 triangles each, so this makes 6*2=12 triangles, and 12*3 vertices
    static const GLfloat vertex_buffer_data[] = {
        0.0,0.0,0.0,
        wd,0.0,0.0,
        wd,hg,0.0,

        0.0,0.0,0.0,
        0.0,hg,0.0,
        wd,hg,0.0,

    };

    this->object = create3DObject(GL_TRIANGLES, 2*3, vertex_buffer_data, color, GL_FILL);
}

void Meter::draw(glm::mat4 VP) {
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

void Meter::set_position(float x, float y) {
    this->position = glm::vec3(x, y, 0);
}

void Meter::tick() {
    // this->rotation += speed;
    // this->position.x -= speed;
    // this->position.y -= speed;
}

OdoMeter::OdoMeter(float x, float y,float z, color_t color) {
    this->position = glm::vec3(x, y, z);
    this->hg = 20.0f;
    this->wd = 0.5f;
    this->rotation = 0;
    int N = 100;
    // Our vertices. Three consecutive floats give a 3D vertex; Three consecutive vertices give a triangle.
    // A cube has 6 faces with 2 triangles each, so this makes 6*2=12 triangles, and 12*3 vertices
    this->r1 = 80.0f, this->r2 = r1 - 2.0f;
    // Our vertices. Three consecutive floats give a 3D vertex; Three consecutive vertices give a triangle.
    // A cube has 6 faces with 2 triangles each, so this makes 6*2=12 triangles, and 12*3 vertices
    GLfloat Cos1,Cos2,Sin1,Sin2,theta = 360.0/N;
    // cout << theta << endl;
    GLfloat vertex_buffer_data[9*N];
    for(int i=0;i<N;i++)
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
    this->object = create3DObject(GL_TRIANGLES, N*3, vertex_buffer_data, color, GL_FILL);

    GLfloat vertex_buffer_data1[]={
    	2.0f,0.0f,0.0f,
    	-2.0f,0.0f,0.0f,
    	0.0f,r2,0.0f,
    };
    this->line = create3DObject(GL_TRIANGLES, 3, vertex_buffer_data1, color, GL_FILL);
}

void OdoMeter::draw(glm::mat4 VP) {
    Matrices.model = glm::mat4(1.0f);
    glm::mat4 translate = glm::translate (this->position);    // glTranslatef
    glm::mat4 rotate    = glm::rotate((float) (this->rotation * M_PI / 180.0f), glm::vec3(0, 0, 1));

    Matrices.model *= (translate * rotate);
    glm::mat4 MVP = VP * Matrices.model;
    glUniformMatrix4fv(Matrices.MatrixID, 1, GL_FALSE, &MVP[0][0]);
    draw3DObject(this->line);

    Matrices.model = glm::mat4(1.0f);
    rotate    = glm::rotate((float) (0 * M_PI / 180.0f), glm::vec3(0, 0, 1));
    Matrices.model *= (translate * rotate);
    MVP = VP * Matrices.model;
    glUniformMatrix4fv(Matrices.MatrixID, 1, GL_FALSE, &MVP[0][0]);
    draw3DObject(this->object);
}

void OdoMeter::set_position(float x, float y) {
    this->position = glm::vec3(x, y, 0);
}

void OdoMeter::tick() {
    // this->rotation += speed;
    // this->position.x -= speed;
    // this->position.y -= speed;
}