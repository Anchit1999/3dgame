#include "main.h"
#include "timer.h"
#include "ball.h"
#include "plane.h"
#include "ground.h"
#include "Meters.h"
#include "indicator.h"
#include "rings.h"
#include "checkpoint.h"
#include "missile.h"
#include "volcano.h"
#include "fuel.h"
#include "parachute.h"
#include "score.h"

using namespace std;

GLMatrices Matrices;
GLuint     programID;
GLFWwindow *window;

/**************************
* Customizable functions *
**************************/

#define X(a) a.position.x
#define Y(a) a.position.y
#define Z(a) a.position.z
#define SQ(a) abs(a*a)
#define Rings_Num 30
#define Volcano_Num 20
#define Missile_Num 10
#define Fuel_Num 5
#define Parachute_num 15
int missile1_count=0,missile2_count=0,enemy_missile_count=0,Points=0;
int timer1 = 0,timer2=0,timer3=0,parachute_timer = 0,cntr=0;
Ball ball1;
Plane plane;
Ground ground;
Meter meter1,meter2;
OdoMeter odo;
Indicator ind1,ind2;
Rings ring[Rings_Num];
Square sq[50];
CheckPoint cp;
Arrow arrow;
Fuel fuel[Fuel_Num];
Parachute parachute[Parachute_num];
Volcano volcano[Volcano_Num];
Missile missile1[Missile_Num],missile2[Missile_Num],enemy_missile[Missile_Num];
Score score[21];
Health health[5];
int score_flag[21],health_count=5;
float screen_zoom = 1, screen_center_x = 0, screen_center_y = 0;
float camera_rotation_angle = 0;
float target_x,target_y,target_z;
float eye_x,eye_y,eye_z;
float up_x=0,up_y=1,up_z=0;
// int camera_view;
float MaxHeight = 3000.0,MaxSpeed = 20.0,fuel_remaining = 100.0;
float cam_heli_x = 10,cam_heli_y = 10;

int xx=0,yy=0,zz=0,xx1=0,yy1=0,zz1=0,xx2=0,yy2=0,zz2=0;
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
    arrow.draw(VP1);

    for(int i=0;i<21;i++)
    {
        if(score_flag[i])
            score[i].draw(VP1);
    }
    for(int i=0;i<health_count;i++)
        health[i].draw(VP1);
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
    for(int i=0;i<Rings_Num;i++)
    {
        if(ring[i].collision == 0)
            ring[i].draw(VP);
    }
    for(int i=0;i<50;i++)
        sq[i].draw(VP);
    cp.draw(VP);
    for(int i=0;i<Missile_Num;i++)
    {
        if(missile1[i].shoot)
            missile1[i].draw(VP);
        if(missile2[i].shoot)
            missile2[i].draw(VP);
        if(enemy_missile[i].shoot)
            enemy_missile[i].draw(VP);
    }
    // arrow.draw(VP);
    for(int i=0;i<Volcano_Num;i++)
        volcano[i].draw(VP);
    for(int i=0;i<Fuel_Num;i++)
        fuel[i].draw(VP);
    for(int i=0;i<Parachute_num;i++)
        parachute[i].draw(VP);
    // cout << "xx " << xx << " " << yy << " " << zz << endl;
    // cout << "xx1 " << xx1 << " " << yy1 << " " << zz1 << endl;
    // cout << "xx2 " << xx2 << " " << yy2 << " " << zz2 << endl;
    // Matrices.view = glm::lookAt( glm::vec3(xx,yy,zz), glm::vec3(xx1,yy1,zz1), glm::vec3(0,1,0) );
    // Matrices.view = glm::lookAt( glm::vec3(0,30,40), glm::vec3(0,0,0), glm::vec3(0,1,0) );
    // glm::mat4 VP2 = Matrices.projection * Matrices.view;
    // arrow.draw(VP2);

}

void camera()
{
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
    // helicopter view
    if(camera_view==4)
    {
        double xpos, ypos;
        glfwGetCursorPos(window, &xpos, &ypos);
    // cout << xpos << " " << ypos << endl;
        if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS) {
            cam_heli_x += + (xpos - pxpos) * 90 / WIDTH;
            cam_heli_y -= + (ypos - pypos) * 90 / HEIGHT;
            pxpos = xpos;
            pypos = ypos;
        }
        target_x = plane.position.x;
        target_y = plane.position.y;
        target_z = plane.position.z;


        // 10 + (20-camera_zoom/5)*cos(camera_look_x*M_PI/180.0f)*sin(camera_look_y*M_PI/180.0f)
        // (20-camera_zoom/5)*cos(camera_look_y*M_PI/180.0f)
        // 10 + (20-camera_zoom/5)*sin(camera_look_x*M_PI/180.0f)*sin(camera_look_y*M_PI/180.0f)
        eye_x = plane.position.x + (200-10*screen_zoom)*cos(cam_heli_x*M_PI/180.0f)*sin(cam_heli_y*M_PI/180.0f);
        eye_y = plane.position.y + (200-10*screen_zoom)*cos(cam_heli_y*M_PI/180.0f);
        eye_z = plane.position.z + (200-10*screen_zoom)*sin(cam_heli_x*M_PI/180.0f)*sin(cam_heli_y*M_PI/180.0f);
    }
    return;
}

void disp_score(int h, int t, int u)
{
    int SCORE[] = {h, t, u};
    int x = 170, y = 150;
    int j=0;
    for (int i = 0; i < 3; i++)
    {
        if (!(SCORE[i] == 1 or SCORE[i] == 2 or SCORE[i] == 3 or SCORE[i] == 7))
        {
            score[j] = Score(x + (int)(score[0].h + score[0].h/2) * i, y, 90, COLOR_GREEN);
            score_flag[j++]=1;
            // score.pb(Score(x + (int)(score[0].h + score[0].h/2) * i, y, 90, COLOR_GREEN)); // top-left
        }
        else score_flag[j++] = 0;
        if (SCORE[i] == 0 or SCORE[i] == 2 or SCORE[i] == 6 or SCORE[i] == 8)
        {
            score[j] = Score(x + (int)(score[0].h + score[0].h/2) * i, y - (int)score[j].h, 90, COLOR_GREEN);
            score_flag[j++]=1;
            // score.pb(Score(x + (int)(score[0].h + score[0].h/2) * i, y - 40, 90, COLOR_GREEN)); // bottom-left
        }
        else score_flag[j++] = 0;
        if (!(SCORE[i] == 0 or SCORE[i] == 1 or SCORE[i] == 7))
        {
            score[j] = Score(x + (int)(score[0].h + score[0].h/2) * i - (int)score[j].wd, y, 0, COLOR_GREEN);
            score_flag[j++]=1;
            // score.pb(Score(x + (int)(score[0].h + score[0].h/2) * i - 10, y, 0, COLOR_GREEN)); // middle
        }
        else score_flag[j++] = 0;
        if (!(SCORE[i] == 1 or SCORE[i] == 4 or SCORE[i] == 7))
        {
            score[j] = Score(x + (int)(score[0].h + score[0].h/2) * i - (int)score[j].wd, y - (int)score[j].h, 0, COLOR_GREEN);
            score_flag[j++]=1;
            // score.pb(Score(x + (int)(score[0].h + score[0].h/2) * i - 10, y - 40, 0, COLOR_GREEN)); // bottom
        }
        else score_flag[j++] = 0;
        if (!(SCORE[i] == 1 or SCORE[i] == 4))
        {
            score[j] = Score(x + (int)(score[0].h + score[0].h/2) * i - (int)score[j].wd, y + (int)score[j].h, 0, COLOR_GREEN);
            score_flag[j++]=1;
            // score.pb(Score(x + (int)(score[0].h + score[0].h/2) * i - 10, y + 40, 0, COLOR_GREEN)); // top
        }
        else score_flag[j++] = 0;
        if (!(SCORE[i] == 5 or SCORE[i] == 6))
        {
            score[j] = Score(x + (int)(score[0].h + score[0].h/2) * i + (int)score[j].h - (int)score[j].wd, y, 90, COLOR_GREEN);
            score_flag[j++]=1;
            // score.pb(Score(x + (int)(score[0].h + score[0].h/2) * i + 40, y, 90, COLOR_GREEN)); // top-right
        }
        else score_flag[j++] = 0;
        if (!(SCORE[i] == 2))
        {
            score[j] = Score(x + (int)(score[0].h + score[0].h/2) * i + (int)score[j].h - (int)score[j].wd, y - (int)score[j].h, 90, COLOR_GREEN);
            score_flag[j++]=1;
            // score.pb(Score(x + (score[0].h + score[0].h/2) * i + 40, y - 40, 90, COLOR_GREEN)); // bottom-right
        }
        else score_flag[j++] = 0;
    }
}

void tick_input(GLFWwindow *window) {
    int left  = glfwGetKey(window, GLFW_KEY_LEFT);
    int right = glfwGetKey(window, GLFW_KEY_RIGHT);
    int up = glfwGetKey(window, GLFW_KEY_UP);
    int down = glfwGetKey(window, GLFW_KEY_DOWN);
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
    int z = glfwGetKey(window, GLFW_KEY_Z);

    // if(c) camera_view = 0;
    // if(v) camera_view = 1;
    // if(b) camera_view = 2;
    // if(n) camera_view = 3;
    if(space)
    {
        if(plane.position.y < MaxHeight)
            plane.position.y += plane.speedy;
        // ind1.set_position(ind1.position.x+0.02,ind1.position.y,ind1.position.z);
    }
    if(z)
    {
        if(plane.position.y > -600)
            plane.position.y -= plane.speedy;
    }
    float pos = (ground.depth + plane.position.y)/(ground.depth + MaxHeight);
    float fl = (fuel_remaining)/100.0;
    if(fl<0) fl=0;
    fuel_remaining -= 0.05;
    float speed_frac = (plane.speedz)/MaxSpeed;
    odo.rotation = 90.0 - speed_frac*180.0;
    // cout << pos << endl;
    ind1.set_position(meter1.position.x-((1-pos)*meter1.hg),meter1.position.y - meter1.wd,ind1.position.z);

    ind2.set_position(meter2.position.x-((1-fl)*meter2.hg),meter2.position.y - meter2.wd,ind2.position.z);
    if (w) {
        if(plane.speedz < MaxSpeed)
            plane.speedz += 0.06;
    }
    else if(s)
    {
        if(plane.speedz > -MaxSpeed)
            plane.speedz -= 0.06;
    }
    else
    {
        plane.tick();
    }
    plane.position.z -= plane.speedz * cos(plane.rotation.y * M_PI/180.0) * (cos(plane.rotation.x * M_PI/180.0));
    plane.position.x -= plane.speedz * sin(plane.rotation.y * M_PI/180.0) * (cos(plane.rotation.x * M_PI/180.0));
    plane.position.y += plane.speedz/2 * sin(plane.rotation.x * M_PI/180.0);
    // cout << plane.speedz << endl;
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


    if(up)
    {
        plane.rotation.x += 1;
    }
    if(down)
    {
        plane.rotation.x -= 1;
    }
    timer1++;
    if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS and timer1>=10) 
    {
        timer1 = 0;
        missile1[missile1_count].shoot = 1;
        missile1[missile1_count].set_position(X(plane),Y(plane),Z(plane));
        missile1[missile1_count].rotation = plane.rotation;
        missile1_count = (missile1_count + 1)%10;

    }
    timer2++;
    if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS and timer2>=10) 
    {
        timer2=0;
        missile2[missile2_count].shoot = 1;
        missile2[missile2_count].set_position(X(plane),Y(plane),Z(plane));
        missile2[missile2_count].rotation.x = 90;
        missile2_count = (missile2_count + 1)%10;

    }
    for(int i=0;i<Missile_Num;i++)
    {
        if(missile1[i].shoot)
        {
            missile1[i].tick();
        }
        if(missile2[i].shoot)
        {
            missile2[i].tick();
        }
    }
    camera();

    // double arrow_theta = atan( (X(cp) - X(plane)) / (Z(cp) - Z(plane))) * 180.0 / M_PI;
    // cout << arrow_theta << endl;
    if(glfwGetKey(window, GLFW_KEY_U)) xx++;
    if(glfwGetKey(window, GLFW_KEY_I)) yy++;
    // if(glfwGetKey(window, GLFW_KEY_O)) zz++;

    if(glfwGetKey(window, GLFW_KEY_J)) xx--;
    if(glfwGetKey(window, GLFW_KEY_K)) yy--;
    // if(glfwGetKey(window, GLFW_KEY_L)) zz--;

    // if(glfwGetKey(window, GLFW_KEY_R)) xx1++;
    // if(glfwGetKey(window, GLFW_KEY_T)) yy1++;
    // if(glfwGetKey(window, GLFW_KEY_Y)) zz1++;

    // if(glfwGetKey(window, GLFW_KEY_F)) xx1--;
    // if(glfwGetKey(window, GLFW_KEY_G)) yy1--;
    // if(glfwGetKey(window, GLFW_KEY_H)) zz1--;

    // if(glfwGetKey(window, GLFW_KEY_Q)) xx2++;
    // if(glfwGetKey(window, GLFW_KEY_W)) yy2++;
    // if(glfwGetKey(window, GLFW_KEY_E)) zz2++;

    // if(glfwGetKey(window, GLFW_KEY_A)) xx2--;
    // if(glfwGetKey(window, GLFW_KEY_S)) yy2--;
    // if(glfwGetKey(window, GLFW_KEY_D)) zz2--;

    // arrow.rotation.z = plane.rotation.y;
    // arrow.rotation.y = arrow_theta;

    // double cp_theta_xz = atan( (X(cp) - X(plane)) / (Z(cp) - Z(plane))) * 180.0 / M_PI;
    // double cp_theta_yz = atan( (Y(cp) - Y(plane)) / (Z(cp) - Z(plane))) * 180.0 / M_PI;
    // double cp_theta_yx = atan( (Y(cp) - Y(plane)) / (X(cp) - X(plane))) * 180.0 / M_PI;
    // cout << cp_theta_xz << " " << cp_theta_yz << " " << cp_theta_yx << endl;
    // if(Z(plane) > Z(cp))
    // {
    //     cp.rotation.y = cp_theta_xz;
    //     cp.rotation.x = -cp_theta_yz;
    // }
    // else
    // {
    //     cp.rotation.y = 180 + cp_theta_xz;
    //     cp.rotation.x = cp_theta_yz;
    // }
    glm::vec3 CheckPoint_to_Plane(X(plane) - X(cp),Y(plane) - Y(cp),Z(plane) - Z(cp));
    glm::vec3 xz(X(plane) - X(cp),0,Z(plane) - Z(cp));
    glm::vec3 yz(0,Y(plane) - Y(cp),Z(plane) - Z(cp));
    glm::vec3 n1 = glm::normalize(CheckPoint_to_Plane),n_xz = glm::normalize(xz),n_yz = glm::normalize(yz);

    double th1 = acos(glm::dot(n1,n_xz)) * 180.0 / M_PI,th2 = acos(glm::dot(n_xz,glm::vec3(0,0,1))) * 180.0 / M_PI;
    if(isnan(th1)) th1 = 0;
    if(isnan(th2)) th2 = 0;
    cp.rotation.x = th1;
    if(Y(plane) > Y(cp)) cp.rotation.x *= -1;
    else cp.rotation.x *= 1;
    // if(Z(plane) > Z(cp)) cp.rotation.x *= 1;
    // else cp.rotation.x *= -1;
    // cp.rotation.z = cp.rotation.x;
    cp.rotation.y = th2;
    if(X(plane) > X(cp)) cp.rotation.y *= 1;
    else cp.rotation.y *= -1;
    // cout << "hello " << th1 << " " << th2 << endl;

    timer3++;
    if(timer3>=100 and ( abs(X(plane) - X(cp)) < 2000 and abs(Z(plane) - Z(cp)) < 2000))
    {
        glm::vec3 Cannon_to_Plane(X(plane) - X(cp),Y(plane) + 25.0 - Y(cp),Z(plane) - Z(cp));
        glm::vec3 xz(X(plane) - X(cp),0,Z(plane) - Z(cp));
        glm::vec3 yz(0,Y(plane) + 25.0 - Y(cp),Z(plane) - Z(cp));
        glm::vec3 n1 = glm::normalize(Cannon_to_Plane),n_xz = glm::normalize(xz),n_yz = glm::normalize(yz);

        double th1 = acos(glm::dot(n1,n_xz)) * 180.0 / M_PI,th2 = acos(glm::dot(n_xz,glm::vec3(0,0,1))) * 180.0 / M_PI;
        if(isnan(th1)) th1 = 0;
        if(isnan(th2)) th2=0;
        timer3=0;
        enemy_missile[enemy_missile_count].shoot = 1;
        enemy_missile[enemy_missile_count].set_position(X(cp),Y(cp) + 25.0,Z(cp));

        enemy_missile[enemy_missile_count].rotation.x = th1;

        if(Y(plane) > Y(enemy_missile[enemy_missile_count])) enemy_missile[enemy_missile_count].rotation.x *= -1;
        else enemy_missile[enemy_missile_count].rotation.x *= 1;

        enemy_missile[enemy_missile_count].rotation.y = th2;
        if(X(plane) > X(enemy_missile[enemy_missile_count])) enemy_missile[enemy_missile_count].rotation.y *= 1;
        else enemy_missile[enemy_missile_count].rotation.y *= -1;
        // enemy_missile[enemy_missile_count].rotation.y += 180;
        enemy_missile_count = (enemy_missile_count+1)%10;
    }
    for(int i=0;i<Missile_Num;i++)
    {
        X(enemy_missile[i]) += 20*n1.x;
        Y(enemy_missile[i]) += 20*n1.y;
        Z(enemy_missile[i]) += 20*n1.z;

    }
    // vector in direction of plane
    glm::vec3 Plane_front(-sin(plane.rotation.y * M_PI/180.0),0,-cos(plane.rotation.y * M_PI/180.0));
    // vector perpendicular to direction of plane clockwise
    glm::vec3 Plane_front_perp(cos(plane.rotation.y * M_PI/180.0),0,-sin(plane.rotation.y * M_PI/180.0));

    double arrow_theta = acos(-glm::dot(n_xz,glm::normalize(Plane_front))) * 180.0 / M_PI;
    double check = -glm::dot(n_xz,glm::normalize(Plane_front_perp));
    
    arrow.rotation.z = (check < 0) ? arrow_theta : (360.0- arrow_theta);

}

void tick_elements() {
    // ball1.tick();
    // camera_rotation_angle += 1;
    // cout << plane.position.y << endl;

    // glm::vec3 a(1.0f,1.0f,0.0f);
    // glm::vec3 b(1.0f,0.0f,0.0f);
    // glm::vec3 n1 = glm::normalize(a),n2 = glm::normalize(b);
    // cout << glm::dot(n1,n2) << endl;
    // // cout << n.x << " " << n.y << " " << n.z << endl;

    for(int i=0;i<Fuel_Num;i++)
    {
        fuel[i].tick();

        // if(abs(X(plane) - X(fuel[i])) < 50.0 and abs(Y(plane) - Y(fuel[i])) < 50.0 and abs(Z(plane) - Z(fuel[i])) < 50.0)
        // {
        //     cout << "Collide hua\n";
        //     fuel[i].set_position(rand()%20000 - 10000,rand() % 1000 + 1000,rand()%20000 - 10000);
        // }
        if(detect_collision(plane.bounding_box(),fuel[i].bounding_box()))
        {
            // cntr++;
            // cout << "hua hua hua " << cntr << endl;
            fuel_remaining = 100;
            fuel[i].set_position(rand()%20000 - 10000,rand() % 1000 + 1000,rand()%20000 - 10000);  
        }
    }
    for(int i=0;i<Rings_Num;i++)
    {
        // if(ring[i].collision==0)
        // cout << i << " " << X(plane)-X(ring[i]) << " " << Y(plane)-Y(ring[i]) << " " << endl;
        // if( abs(Z(plane) - Z(ring[i])) < 10.0 and SQ( abs(X(plane)-X(ring[i])) ) + SQ( abs(Y(plane)-Y(ring[i])) ) < SQ(ring[i].r1 - 50.0) )
        // {
        //     ring[i].collision = 1;
        // }
        if(detect_collision(plane.bounding_box(),ring[i].bounding_box()) and ring[i].collision == 0)
        {
            ring[i].collision = 1;
            Points+=10;
            cntr++;
            cout << "ring " << cntr << endl;
        }
    }
    for(int i=0;i<Missile_Num;i++)
    {
        if(detect_collision(plane.bounding_box(),enemy_missile[i].bounding_box()) and enemy_missile[i].shoot)
        {
            // cntr++;
            // cout << "enemy_missile " << cntr << endl;
            health_count--;
            enemy_missile[i].shoot = 0;
        }

        if(detect_collision(cp.bounding_box(),missile1[i].bounding_box()) and missile1[i].shoot)
        {
            // cntr++;
            // cout << "missile1 " << cntr << endl;
            cp.set_position(rand()%20000 - 10000,-500,rand()%20000 - 10000);
            Points+=50;
            missile1[i].shoot=0;

        }

        if(detect_collision(cp.bounding_box(),missile2[i].bounding_box()) and missile2[i].shoot)
        {
            // cntr++;
            // cout << "missile2 " << cntr << endl;
            cp.set_position(rand()%20000 - 10000,-500,rand()%20000 - 10000);
            Points+=50;
            missile2[i].shoot = 0;
        }
    }

    for(int i=0;i<Parachute_num;i++)
    {
        parachute[i].tick();
        if(parachute[i].collision == 1)
            parachute[i].set_position(rand()%20000 - 10000,2000,rand()%20000 - 10000);
        for(int j=0;j<Missile_Num;j++)
        {
            if(detect_collision(missile1[j].bounding_box(),parachute[i].bounding_box()) and missile1[j].shoot)
            {
            //     cntr++;
            // cout << "missile2 " << cntr << endl;
                parachute[i].collision = 1;
                Points+=20;
                missile1[j].shoot = 0;
            }
        }
    }
    for(int i=0;i<Volcano_Num;i++)
    {
        if(detect_collision(plane.bounding_box(),volcano[i].bounding_box()))
        {
            // cntr++;
            // cout << "vol " << cntr << endl; 
        }
    }
    // cout << xx << " " << yy << endl;
    // health.set_position(xx,yy);
    disp_score((Points / 100), (Points / 10) % 10, Points % 10);
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
    cp          = CheckPoint(0,-500,0,COLOR_BACKGROUND);
    // arrow       = Arrow(X(plane),Y(plane) + 100,Z(plane),COLOR_RED);
    arrow       = Arrow(0,120,0,COLOR_RED);
    for(int i=0;i<Parachute_num;i++)
        parachute[i]   = Parachute(rand()%20000 - 10000,2000,rand()%20000 - 10000,COLOR_RED);
    for(int i=0;i<Missile_Num;i++)
    {
        missile1[i] = Missile(0,0,0,20.0,50.0,1,COLOR_BACKGROUND);
        missile2[i]     = Missile(0,0,0,20.0,50.0,2,COLOR_BACKGROUND);
        enemy_missile[i] = Missile(0,0,0,10.0,40.0,3,COLOR_BACKGROUND);
    }
    for(int i=0;i<Volcano_Num;i++)
        volcano[i]     = Volcano(rand()%20000 - 10000,-500,rand()%20000 - 10000,COLOR_BACKGROUND);
    for(int i=0;i<Rings_Num;i++)
    {
        ring[i] = Rings(rand()%10000 - 5000,rand()%3400 - 600,rand()%10000 - 5000,COLOR_RED);
    }
    for(int i=0;i<Fuel_Num;i++)
    {
        fuel[i]        = Fuel(rand()%20000 - 10000,rand() % 1000 + 1000,rand()%20000 - 10000,COLOR_RED);
    }
    for(int i=0;i<50;i++)
    {
        sq[i] = Square(i*500 + 200,0,0,COLOR_BACKGROUND);
    }
    for(int i=0;i<5;i++)
    health[i] = Health(-210 + 15*i,155,COLOR_RED);
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
        // Process timersw
        // cout << t60.tick << endl;
        if (t60.processTick()) {
            // 60 fps
            // OpenGL Draw commands
            draw();
            // Swap Frame Buffer in double buffering
            reset_screen();
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

    // cout << a.x << " " << a.y << " " << a.z << endl;
    // cout << b.x << " " << b.y << " " << b.z << endl;
    return (abs(a.x - b.x) * 2 < (a.width + b.width)) &&
    (abs(a.y - b.y) * 2 < (a.height + b.height) && (abs(a.z - b.z) * 2 < (a.depth + b.depth)));
}

void reset_screen() {
    float top    = screen_center_y + HEIGHT/2 / screen_zoom;
    float bottom = screen_center_y - HEIGHT/2 / screen_zoom;
    float left   = screen_center_x - WIDTH/2 / screen_zoom;
    float right  = screen_center_x + WIDTH/2 / screen_zoom;
    // Matrices.projection = glm::ortho(left, right, bottom, top, 0.1f, 500.0f);
    Matrices.projection = glm::perspective ((GLfloat)(2.0*M_PI/3.0), (GLfloat)WIDTH / HEIGHT ,0.1f,10000.0f);
}
