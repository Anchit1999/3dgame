#include "main.h"
#include "timer.h"
#include "ball.h"
#include "plane.h"
#include "ground.h"
#include "Meters.h"
#include "indicator.h"

using namespace std;

GLMatrices Matrices;
GLuint     programID;
GLFWwindow *window;

/**************************
* Customizable functions *
**************************/

Ball ball1;
Plane plane;
Ground ground;
Meter meter1,meter2;
OdoMeter odo;
Indicator ind1,ind2;
Square sq[50];
float screen_zoom = 1, screen_center_x = 0, screen_center_y = 0;
float camera_rotation_angle = 0;
float target_x,target_y,target_z;
float eye_x,eye_y,eye_z;
float up_x=0,up_y=1,up_z=0;
int camera_view=0;
float MaxHeight = 3000.0,MaxSpeed = 20.0;
Timer t60(1.0 / 60);

/* Render the scene with openGL */
/* Edit this function according to your assignment */
void draw() {
    // clear the color and depth in the frame buffer
    glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // use the loaded shader program
    // Don't change unless you know what you are doing
    glUseProgram (programID);

    // Eye - Location of camera. Don't change unless you are sure!!
    // glm::vec3 eye ( 5*cos(camera_rotation_angle*M_PI/180.0f), 0, 5*sin(camera_rotation_angle*M_PI/180.0f) );
    
    glm::vec3 eye1 ( 0, 0, 100 );    
    // Target - Where is the camera looking at.  Don't change unless you are sure!!
    glm::vec3 target1 (0, 0, 0);
    // Up - Up vector defines tilt of camera.  Don't change unless you are sure!!
    glm::vec3 up1 (0, 1, 0);

    Matrices.view = glm::lookAt( eye1, target1, up1 );
    glm::mat4 VP1 = Matrices.projection * Matrices.view;
    meter1.draw(VP1);
    ind1.draw(VP1);
    odo.draw(VP1);

    meter2.draw(VP1);
    ind2.draw(VP1);

    glm::vec3 eye ( eye_x, eye_y, eye_z );    
    // Target - Where is the camera looking at.  Don't change unless you are sure!!
    glm::vec3 target (target_x, target_y, target_z);
    // Up - Up vector defines tilt of camera.  Don't change unless you are sure!!
    glm::vec3 up (up_x, up_y, up_z);

    // Compute Camera matrix (view)
    Matrices.view = glm::lookAt( eye, target, up ); // Rotating Camera for 3D
    // Don't change unless you are sure!!
    // Matrices.view = glm::lookAt(glm::vec3(0, 0, 3), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0)); // Fixed camera for 2D (ortho) in XY plane

    // Compute ViewProject matrix as view/camera might not be changed for this frame (basic scenario)
    // Don't change unless you are sure!!
    glm::mat4 VP = Matrices.projection * Matrices.view;

    // Send our transformation to the currently bound shader, in the "MVP" uniform
    // For each model you render, since the MVP will be different (at least the M part)
    // Don't change unless you are sure!!
    glm::mat4 MVP;  // MVP = Projection * View * Model

    // Scene render
    // ball1.draw(VP);
    plane.draw(VP);
    ground.draw(VP);
    for(int i=0;i<50;i++)
        sq[i].draw(VP);
}

void tick_input(GLFWwindow *window) {
    int left  = glfwGetKey(window, GLFW_KEY_LEFT);
    int right = glfwGetKey(window, GLFW_KEY_RIGHT);
    int space = glfwGetKey(window, GLFW_KEY_SPACE);
    int w = glfwGetKey(window, GLFW_KEY_W);
    int a = glfwGetKey(window, GLFW_KEY_A);
    int s = glfwGetKey(window, GLFW_KEY_S);
    int d = glfwGetKey(window, GLFW_KEY_D);
    int e = glfwGetKey(window, GLFW_KEY_E);
    int q = glfwGetKey(window, GLFW_KEY_Q);
    int c = glfwGetKey(window, GLFW_KEY_C);
    int v = glfwGetKey(window, GLFW_KEY_V);
    int b = glfwGetKey(window, GLFW_KEY_B);
    int n = glfwGetKey(window, GLFW_KEY_N);
    
    if(c) camera_view = 0;
    if(v) camera_view = 1;
    if(b) camera_view = 2;
    if(n) camera_view = 3;
    if(space)
    {
        if(plane.position.y < MaxHeight)
            plane.position.y += 10;
        // ind1.set_position(ind1.position.x+0.02,ind1.position.y,ind1.position.z);
    }
    float pos = (ground.depth + plane.position.y)/(ground.depth + MaxHeight);
    float speed_frac = (plane.speedz)/MaxSpeed;
    odo.rotation = 90.0 - speed_frac*180.0;
    cout << pos << endl;
    ind1.set_position(meter1.position.x-((1-pos)*meter1.hg),meter1.position.y - meter1.wd,ind1.position.z);
    plane.position.y -= 1;
    if (w) {
        if(plane.speedz < MaxSpeed)
        plane.speedz += 0.06;
    }
    else
    {
        plane.tick();
    }
    plane.position.z -= plane.speedz * cos(plane.rotation.y * M_PI/180.0);
    plane.position.x -= plane.speedz * sin(plane.rotation.y * M_PI/180.0);
    cout << plane.speedz << endl;
    if(e)
    {
        plane.rotation.y -= 0.5;
        if(plane.rotation.z > -30)
            plane.rotation.z -=1;
    }
    if(q)
    {
        plane.rotation.y +=0.5;
        if(plane.rotation.z < 30)
            plane.rotation.z +=1;
    }
    if(d)
    {
        if(plane.rotation.z > -30)
            plane.rotation.z -=1;
    }
    if(a)
    {
        if(plane.rotation.z < 30)
            plane.rotation.z +=1;
    }
    if(plane.rotation.z < 0)
        plane.rotation.z+=0.5;
    else if(plane.rotation.z > 0)
        plane.rotation.z-=0.5;

    up_x = 0;
    up_y = 1;
    // follow cam view
    if(camera_view==0)
    {
        target_x = plane.position.x;
        target_y = plane.position.y;
        target_z = plane.position.z;
        // cout << plane.position.x << " " << plane.position.y << " " << plane.position.z << endl;
        // camera_rotation_angle = 90.0;
        // eye_x = target_x + 500*cos(camera_rotation_angle*M_PI/180.0f);
        // eye_y = target_y + 500;
        // eye_z = target_z + 500*sin(camera_rotation_angle*M_PI/180.0f);
        eye_x = plane.position.x + (0 + plane.len)*sin(plane.rotation.y * M_PI/180.0);
        eye_y = plane.position.y + 300;
        eye_z = plane.position.z + (plane.len + 100)*cos(plane.rotation.y * M_PI/180.0);

    }
    // tower view
    if(camera_view==1)
    {
        target_x = plane.position.x;
        target_y = plane.position.y;
        target_z = plane.position.z;
        // cout << plane.position.x << " " << plane.position.y << " " << plane.position.z << endl;
        // camera_rotation_angle = 90.0;
        // eye_x = target_x + 500*cos(camera_rotation_angle*M_PI/180.0f);
        // eye_y = target_y + 500;
        // eye_z = target_z + 500*sin(camera_rotation_angle*M_PI/180.0f);
        eye_x = 500;
        eye_y = 500;
        eye_z = 500;
    }
    // plane view
    if(camera_view==2)
    {
        target_x = plane.position.x  - (plane.len)*sin(plane.rotation.y * M_PI/180.0);
        target_y = plane.position.y;
        target_z = plane.position.z - (plane.len)*cos(plane.rotation.y * M_PI/180.0) ;
        // camera_rotation_angle = 90.0;
        // eye_x = target_x + 500*cos(camera_rotation_angle*M_PI/180.0f);
        // eye_y = target_y + 500;
        // eye_z = target_z + 500*sin(camera_rotation_angle*M_PI/180.0f);
        eye_x = plane.position.x - (plane.len/2)*sin(plane.rotation.y * M_PI/180.0);
        eye_y = plane.position.y + 100;
        eye_z = plane.position.z - (plane.len/2)*cos(plane.rotation.y * M_PI/180.0);
        // cout << plane.position.x << " " << plane.position.y << " " << plane.position.z << endl;
        // cout << eye_x << " " << eye_y << " " << eye_z << endl;
        up_x = -sin(plane.rotation.z * M_PI/180.0) * cos(plane.rotation.y * M_PI / 180.0);
        up_z = sin(plane.rotation.z * M_PI/180.0) * sin(plane.rotation.y * M_PI / 180.0);
        up_y = cos(plane.rotation.z * M_PI/180.0);

    }

    // top view
    if(camera_view==3)
    {
        target_x = plane.position.x;
        target_y = plane.position.y;
        target_z = plane.position.z;

        eye_x = plane.position.x + (plane.len)*sin(plane.rotation.y * M_PI/180.0);
        eye_y = plane.position.y + 600;
        eye_z = plane.position.z + (plane.len)*cos(plane.rotation.y * M_PI/180.0);
    }
}

void tick_elements() {
    // ball1.tick();
    // camera_rotation_angle += 1;
    cout << plane.position.y << endl;
}

/* Initialize the OpenGL rendering properties */
/* Add all the models to be created here */
void initGL(GLFWwindow *window, int width, int height) {
    /* Objects should be created before any other gl function and shaders */
    // Create the models

    ball1       = Ball(0, 0, COLOR_RED);
    plane       = Plane(0,0,0,COLOR_GREEN);
    ground      = Ground(0,0,COLOR_RED);
    odo         = OdoMeter(-100,-150,0,COLOR_GREEN);
    
    meter1      = Meter(200,-120,0,COLOR_GREEN);
    // meter1      = Meter(0,0,0,COLOR_GREEN);
    ind1        = Indicator(meter1.position.x-meter1.hg/2,meter1.position.y-meter1.wd,meter1.position.z,COLOR_BACKGROUND);

    meter2      = Meter(200,-150,0,COLOR_GREEN);
    ind2        = Indicator(meter2.position.x-meter2.hg/2,meter2.position.y-meter2.wd,meter2.position.z,COLOR_BACKGROUND);
    for(int i=0;i<50;i++)
    {
        sq[i] = Square(i*500 + 200,0,0,COLOR_BACKGROUND);
    }
    // Create and compile our GLSL program from the shaders
    programID = LoadShaders("Sample_GL.vert", "Sample_GL.frag");
    // Get a handle for our "MVP" uniform
    Matrices.MatrixID = glGetUniformLocation(programID, "MVP");


    reshapeWindow (window, width, height);

    // Background color of the scene
    glClearColor (COLOR_BACKGROUND.r / 256.0, COLOR_BACKGROUND.g / 256.0, COLOR_BACKGROUND.b / 256.0, 0.0f); // R, G, B, A
    glClearDepth (1.0f);

    glEnable (GL_DEPTH_TEST);
    glDepthFunc (GL_LEQUAL);

    cout << "VENDOR: " << glGetString(GL_VENDOR) << endl;
    cout << "RENDERER: " << glGetString(GL_RENDERER) << endl;
    cout << "VERSION: " << glGetString(GL_VERSION) << endl;
    cout << "GLSL: " << glGetString(GL_SHADING_LANGUAGE_VERSION) << endl;
}


int main(int argc, char **argv) {
    srand(time(0));


    window = initGLFW(WIDTH, HEIGHT);

    initGL (window, WIDTH, HEIGHT);

    /* Draw in loop */
    while (!glfwWindowShouldClose(window)) {
        // Process timers

        if (t60.processTick()) {
            // 60 fps
            // OpenGL Draw commands
            draw();
            // Swap Frame Buffer in double buffering
            glfwSwapBuffers(window);

            tick_elements();
            tick_input(window);
        }

        // Poll for Keyboard and mouse events
        glfwPollEvents();
    }

    quit(window);
}

bool detect_collision(bounding_box_t a, bounding_box_t b) {
    return (abs(a.x - b.x) * 2 < (a.width + b.width)) &&
    (abs(a.y - b.y) * 2 < (a.height + b.height));
}

void reset_screen() {
    float top    = screen_center_y + HEIGHT/2 / screen_zoom;
    float bottom = screen_center_y - HEIGHT/2 / screen_zoom;
    float left   = screen_center_x - WIDTH/2 / screen_zoom;
    float right  = screen_center_x + WIDTH/2 / screen_zoom;
    // Matrices.projection = glm::ortho(left, right, bottom, top, 0.1f, 500.0f);
    Matrices.projection = glm::perspective ((GLfloat)(2.0*M_PI/3.0), (GLfloat)WIDTH / HEIGHT ,0.1f,10000.0f);
}
