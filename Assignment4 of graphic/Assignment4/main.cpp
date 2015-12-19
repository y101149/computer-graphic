/**********************************************************************
 *
 *  CS 4554 assignment 1
 *  Sprint 2015
 *
 *  You are to read a file with triangles and vertices and display the 
 *  contents directly on the screen.
 *  The file format is:
 data number-of-vertices number-of-triangles named-point-attributes(1..n - optional)
 <all points as: x y z>
 <all triangles as: 3 point-idx1 point-idx2 point-idx3>
 <all additional attributes in form n v1, v2, .. vn>
 *  
 *
 *  A simple 2 triangle example is:
 
 data 4 2
 0.25 0.25 0.0
 0.75 0.25 0.0
 0.75 0.75 0.0
 0.25 0.75 0.0
 3 0 1 2
 3 0 2 3
 
 * Example with one attribute (not needed for first assignment):
 
 data 4 2 tex-coords1
 0.25 0.25 0.0
 0.75 0.25 0.0
 0.75 0.75 0.0
 0.25 0.75 0.0
 3 0 1 2
 3 0 2 3
 2 0.0 0.0
 2 1.0 0.0
 2 1.0 1.0
 2 0.0 1.0
 
 *  
 *  
 **********************************************************************/


#include <OpenGL/OpenGL.h>
#include <Glut/glut.h>
#include <math.h>
#include <iostream>
#include <fstream>

#include "PolyModel.h"
#include "util.h"
#include "Vec3.h"


using namespace std;
/* ascii codes for special keys */
#define ESCAPE 27

/**********************************************************************
 * Configuration
 **********************************************************************/

#define INITIAL_WIDTH 600
#define INITIAL_HEIGHT 600
#define INITIAL_X_POS 100
#define INITIAL_Y_POS 100

#define WINDOW_NAME  "Assignment 1"
#define RESOURCE_DIR "../../../../../resources/"
//#define RESOURCE_DIR "/Users/yixinyin/Desktop/xcode/Assignment1/resources/"

/**********************************************************************
 * Globals
 **********************************************************************/

PolyModel duck;
PolyModel cow;
PolyModel bishop;
PolyModel dice;
float angle = 3*M_PI/2;
Vec3f g_view_dir;
Vec3f g_view_pos;
Vec3f duckcenter;
Vec3f cowcenter;
Vec3f bishopcenter;
Vec3f update;
float changer = 0;
float rotateangle = 0;
Vec3f rotatevector;
float rotateangle1 = 0;
Vec3f rotatevector1;
int flag = 0;
Vec3f scalefactor;
Vec3f scalefactor1;
float g_rot_matrix[16];
float g_rot_matrix1[16];
float g_rot_matrix2[16];
float g_camera_rot[16];
float globalY = 0;

Vec3f updownrotate;
float angleverticle = 0;



GLuint shader1;
GLuint shader2;
GLuint shader3;
GLuint shader4;
GLuint mytexture;//for the duck
GLuint walltex;//for the wall
int shaderchooseflag = 0;
int objectchoose = 0;

/**********************************************************************
 * Set the new size of the window
 **********************************************************************/

void resize_scene(GLsizei width, GLsizei height)
{
    glViewport(0, 0, width, height);  /* reset the current viewport and
                                       * perspective transformation */
    g_window_width  = width;
    g_window_height = height;
    
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(90, (GLfloat) width / (GLfloat) height, 0.01, 1000);

    
    
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    
    
}

bool judge_camera(Vec3f camerapos)
{
    Vec3f temp;
    temp.set(-15.0, 0, 0);
    Vec3f cowlow = cow.getMinBounding() + temp;
    Vec3f cowhigh = cow.getMaxBounding() + temp;
    Vec3f bishoplow = bishop.getMinBounding();
    Vec3f bishophigh = bishop.getMaxBounding();
    Vec3f ducklow1 = duck.getMinBounding();
    Vec3f duckhigh1 = duck.getMaxBounding();
    temp.set(0.0,0.0,5.0);
    Vec3f ducklow2 = duck.getMinBounding() + temp;
    Vec3f duckhigh2 = duck.getMaxBounding() + temp;
    Vec3f roomlow;
    roomlow.set(-40, -5, -30);
    Vec3f roomhigh;
    roomhigh.set(40,20,30);
    if(!(camerapos.x() >= roomlow.x() && camerapos.z() >= roomlow.z() && camerapos.x() <= roomhigh.x() && camerapos.z() <= roomhigh.z()))
        return false;
    else if (camerapos.x() >= cowlow.x() && camerapos.z() >= cowlow.z() &&camerapos.x() <= cowhigh.x() && camerapos.z() <= cowhigh.z())
        return false;
    else if (camerapos.x() >= bishoplow.x() && camerapos.z() >= bishoplow.z() &&camerapos.x() <= bishophigh.x() && camerapos.z() <= bishophigh.z())
        return false;
    else if (camerapos.x() >= ducklow1.x() && camerapos.z() >= ducklow1.z() &&camerapos.x() <= duckhigh1.x() && camerapos.z() <= duckhigh1.z())
        return false;
    else if (camerapos.x() >= ducklow2.x() && camerapos.z() >= ducklow2.z() &&camerapos.x() <= duckhigh2.x() && camerapos.z() <= duckhigh2.z())
        return false;
    else
        return true;
    
    
}

void drawRoom(const Vec3f& min_xyz, const Vec3f& max_xyz) {
    
    glDisable(GL_CULL_FACE);
    glColor3f(1, 1, 1);
    glUseProgram(shader4);
    glBegin(GL_QUADS);
    
    glNormal3f(0.0f, 1.0f, 0.0f);//必须设置多边形的法向量，以确定哪边是正面
    
    glVertex3f(min_xyz.x(), min_xyz.y(), max_xyz.z());
    glVertex3f(max_xyz.x(), min_xyz.y(), max_xyz.z());
    glVertex3f(max_xyz.x(), min_xyz.y(), min_xyz.z());
    glVertex3f(min_xyz.x(), min_xyz.y(), min_xyz.z());
    //
    //floor
    
    glEnd();
    

    //glColor3f(1, 1, 1);
    glColor4f(1.0, 1.0, 1.0, 1.0);
    glUseProgram(shader4);
    glBegin(GL_QUADS);
    
    glNormal3f(0.0f,-1.0f, 0.0f);
    //glMultiTexCoord2f(0, 0.0f, 0.0f);
    
    
    glVertex3f(min_xyz.x(), max_xyz.y(), min_xyz.z());
    glVertex3f(max_xyz.x(), max_xyz.y(), min_xyz.z());
    glVertex3f(max_xyz.x(), max_xyz.y(), max_xyz.z());
    glVertex3f(min_xyz.x(), max_xyz.y(), max_xyz.z());
    
    
    glEnd();
    
    //walls
    glColor3f(0, 1, 1);
    glUseProgram(shader1);/////////
    glBegin(GL_QUADS);
    
    glNormal3f(0.0f, 0.0f, -1.0f);

    glTexCoord2f(0.0, 0.0);
    glVertex3f(min_xyz.x(), min_xyz.y(), max_xyz.z());
    glTexCoord2f(0.0, 5.0);
    glVertex3f(min_xyz.x(), max_xyz.y(), max_xyz.z());
    glTexCoord2f(5.0, 5.0);
    glVertex3f(max_xyz.x(), max_xyz.y(), max_xyz.z());
    glTexCoord2f(5.0, 0.0);
    glVertex3f(max_xyz.x(), min_xyz.y(), max_xyz.z());
    
    
    
    glEnd();
    
    glBegin(GL_QUADS);
    
    glNormal3f(0.0f, 0.0f, 1.0f);
    //glMultiTexCoord2f(0, 0.0f, 0.0f);
//    glTexCoord2f(0, 0);
//    glVertex3f(min_xyz.x(), min_xyz.y(), min_xyz.z());
//    glTexCoord2f(5, 0);
//    glVertex3f(max_xyz.x(), min_xyz.y(), min_xyz.z());
//    glTexCoord2f(5, 5);
//    glVertex3f(max_xyz.x(), max_xyz.y(), min_xyz.z());
//    glTexCoord2f(0, 5);
//    glVertex3f(min_xyz.x(), max_xyz.y(), min_xyz.z());
    glTexCoord2f(0, 0);
    glVertex3f(min_xyz.x(), min_xyz.y(), min_xyz.z());
    glTexCoord2f(5, 0);
    glVertex3f(max_xyz.x(), min_xyz.y(), min_xyz.z());
    glTexCoord2f(5, 5);
    glVertex3f(max_xyz.x(), max_xyz.y(), min_xyz.z());
    glTexCoord2f(0, 5);
    glVertex3f(min_xyz.x(), max_xyz.y(), min_xyz.z());
    
    
    glEnd();
    
    glBegin(GL_QUADS);
    
    glNormal3f(1.0f, 0.0f, 0.0f);
    glTexCoord2f(0, 0);
    glVertex3f(min_xyz.x(), min_xyz.y(), min_xyz.z());
    glTexCoord2f(5, 0);
    glVertex3f(min_xyz.x(), min_xyz.y(), max_xyz.z());
    glTexCoord2f(5, 5);
    glVertex3f(min_xyz.x(), max_xyz.y(), max_xyz.z());
    glTexCoord2f(0, 5);
    glVertex3f(min_xyz.x(), max_xyz.y(), min_xyz.z());

    
    glEnd();
    
    glBegin(GL_QUADS);
    
    glNormal3f(-1.0f, 0.0f, 0.0f);
    //glMultiTexCoord2f(0, 0.0f, 0.0f);
    glTexCoord2f(0, 0);
    glVertex3f(max_xyz.x(), min_xyz.y(), min_xyz.z());
    glTexCoord2f(5, 0);
    glVertex3f(max_xyz.x(), min_xyz.y(), max_xyz.z());
    glTexCoord2f(5, 5);
    glVertex3f(max_xyz.x(), max_xyz.y(), max_xyz.z());
    glTexCoord2f(0, 5);
    glVertex3f(max_xyz.x(), max_xyz.y(), min_xyz.z());
    glEnd();
    
    
    
}
/**********************************************************************
 * any program initialization (set initial OpenGL state,
 **********************************************************************/
void init()
{
   // string ifile = RESOURCE_DIR + string("car.d2");//load two models
    string ifile = RESOURCE_DIR + string("duck.d2");//load two models
    string ifile1 = RESOURCE_DIR + string("cow.d2");
    string ifile2 = RESOURCE_DIR + string("bishop.d2");
    string ifile3 = RESOURCE_DIR + string("dice.d2");


    ifstream file(ifile.c_str());
    ifstream file1(ifile1.c_str());
    ifstream file2(ifile2.c_str());
    ifstream file3(ifile3.c_str());

    duck.loadModel(file);
    cow.loadModel(file1);
    bishop.loadModel(file2);
    dice.loadModel(file3);
    
    glClearColor(0, 0, 1, 0);
    //glDisable(GL_LIGHTING);
    glDisable(GL_CULL_FACE);
    g_view_dir.set(cos(angle), 0, sin(angle));//set the view position at (0,0,1)  and direction to the origin.
    g_view_pos.set(0,0,10);
    duckcenter = duck.getCenter();
    update.set(0, 0, 0);
    scalefactor.set(1,1,1);
    scalefactor1.set(1,1,1);
    
    g_rot_matrix[0] = 1;
    g_rot_matrix[5] = 1;
    g_rot_matrix[10] = 1;
    g_rot_matrix[15] = 1;
    
    g_rot_matrix1[0] = 1;
    g_rot_matrix1[5] = 1;
    g_rot_matrix1[10] = 1;
    g_rot_matrix1[15] = 1;
    
    g_rot_matrix2[0] = 1;
    g_rot_matrix2[5] = 1;
    g_rot_matrix2[10] = 1;
    g_rot_matrix2[15] = 1;
    
    g_camera_rot[0] = 1;
    g_camera_rot[5] = 1;
    g_camera_rot[10] = 1;
    g_camera_rot[15] = 1;

    
    float specular[4] = {0.6f, 0.6f, 0.6f, 1.0f};
    float diffuse[4] = {0.6f, 0.6f, 0.6f, 1.0f};
    float ambient[4] = {0.1f, 0.1f, 0.1f, 1.0f};
    
    glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, 50.0f);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, specular);
    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, diffuse);
    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, ambient);
    // Color intensity and position of light (Ia, Is, Id)
    float light_color[4] = {1.0f, 1.0f, 1.0f, 1.0f};
    float light_color1[4] = {1.0f,0.0f,0.0f,1.0f};
    glLightfv(GL_LIGHT0, GL_DIFFUSE, light_color);
    glLightfv(GL_LIGHT0, GL_SPECULAR, light_color);
    glLightfv(GL_LIGHT0, GL_AMBIENT, light_color);
    
    glLightfv(GL_LIGHT1, GL_DIFFUSE, light_color);
    glLightfv(GL_LIGHT1, GL_SPECULAR, light_color);
    glLightfv(GL_LIGHT1, GL_AMBIENT, light_color);
    
//    glEnable(GL_LIGHTING);
//    glEnable(GL_LIGHT0);
    //glEnable(GL_LIGHT1);

    
    shader1 = createShader(RESOURCE_DIR + string("texshading"));
    shader2 = createShader(RESOURCE_DIR + string("new"));
    shader3 = createShader(RESOURCE_DIR + string("toon"));
    shader4 = createShader(RESOURCE_DIR + string("phong"));
    string texturename = RESOURCE_DIR + string("duck.png");
    mytexture = getTextureFromFile(texturename.c_str());
    
    string walltexname = RESOURCE_DIR + string("bricks.png");
    walltex = getTextureFromFile(walltexname.c_str());
    
    
    
    
    
}


/**********************************************************************
 * The main drawing functions.
 **********************************************************************/
void draw_scene(void)
{

    /* clear the screen and the depth buffer */
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    /* reset modelview matrix */
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    
    gluLookAt(g_view_pos.x(), g_view_pos.y(), g_view_pos.z(), g_view_pos.x()+ g_view_dir.x(), g_view_pos.y()+ g_view_dir.y(), g_view_pos.z()+ g_view_dir.z(), 0, 1, 0);//this should be execute at last

    float position[4] = {10.0f,8.0f,6.0f,1.0f};
    float position1[4] = {0.0f,0.0f,-4.0f,1.0f};
    glLightfv(GL_LIGHT0, GL_POSITION, position);//写在这里才能不随镜头移动
    glLightfv(GL_LIGHT1, GL_POSITION, position1);
    Vec3<float> leftbot, righttop;
    

    leftbot.set(-40, -5, -30);
    righttop.set(40,20,30);
    
    glEnable(GL_TEXTURE_2D);
    glActiveTexture(GL_TEXTURE0);
    GLint loc = glGetUniformLocation(shader1, "diffuse_tex");
    glUniform1i(loc, 0);
    glBindTexture(GL_TEXTURE_2D, walltex);
    
    //glUseProgram(shader4);
    drawRoom(leftbot, righttop);
    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);//IF DELETE THIS IT WILL BECOME A SQUARE

    if(shaderchooseflag == 0)
        glUseProgram(shader4);
    else if(shaderchooseflag == 1)
        glUseProgram(shader2);
    else
        glUseProgram(shader3);
    glPushMatrix();
    glTranslatef(-15, 0, 0);
    glColor4f(1, 0, 1, 1);
    cowcenter = cow.getCenter();
    glTranslatef(cowcenter.x(), cowcenter.y(), cowcenter.z());
    glMultMatrixf(g_rot_matrix);
    glTranslatef(-cowcenter.x(), -cowcenter.y(), -cowcenter.z());

    cow.draw();
    glPopMatrix();
    
    glPushMatrix();
    glTranslatef(0, 0, 0);
    glColor4f(0, 0, 1, 0);
    bishopcenter = bishop.getCenter();
    glTranslatef(bishopcenter.x(), bishopcenter.y(), bishopcenter.z());
    glMultMatrixf(g_rot_matrix1);
    glTranslatef(-bishopcenter.x(), -bishopcenter.y(), -bishopcenter.z());
    bishop.draw();
    glPopMatrix();
    
    
    glPushMatrix();
    //glTranslatef(-15, 0, 0);
    glColor4f(1, 1, 0, 1);
    duckcenter = duck.getCenter();
    glTranslatef(duckcenter.x(), duckcenter.y(), duckcenter.z());
    glMultMatrixf(g_rot_matrix2);
    glTranslatef(-duckcenter.x(), -duckcenter.y(), -duckcenter.z());
    duck.draw();
    glPopMatrix();
    
    glUseProgram(shader1);
    glBindTexture(GL_TEXTURE_2D, mytexture);
    glPushMatrix();
    glTranslatef(0, 0, 5);
    glColor4f(1, 1, 0,1);
    duckcenter = duck.getCenter();
    glTranslatef(duckcenter.x(), duckcenter.y(), duckcenter.z());
    glMultMatrixf(g_rot_matrix2);
    glTranslatef(-duckcenter.x(), -duckcenter.y(), -duckcenter.z());
    duck.draw();
    glPopMatrix();
   
    
//    // Example to show how to write text (for debugging) to the screen:
//    glColor3f(1.0f, 1.0f, 1.0f);
//    drawGlutString("Assignment 1");
    /* since this is double buffered, swap the
     * buffers to display what just got drawn */
    //drawGlutString(g_compile_link_log.c_str());
    glutSwapBuffers();
}

void key_press(unsigned char key, int x, int y)
{
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    
    switch (key) {
        case ' ':
            break;
        case 'w'://move viewpoint forward
        {
           g_view_pos = g_view_pos + g_view_dir*0.5;
           if(!judge_camera(g_view_pos))
               g_view_pos = g_view_pos - g_view_dir*0.5;
           break;
        }
        case 'a'://move viewpoint sideways to the left
        {
            Vec3f temp;
            temp.set(g_view_dir.z(),0,-g_view_dir.x());
            g_view_pos = g_view_pos + temp*0.5;
            //cout<<g_view_pos.x()<<" "<<g_view_pos.y()<<" "<<g_view_pos.z()<<"\n";
            if(!judge_camera(g_view_pos))
                g_view_pos = g_view_pos - temp*0.5;
            break;
        }
        case 's'://move viewpoint backward
        {
            g_view_pos = g_view_pos - g_view_dir*0.5;
            if(!judge_camera(g_view_pos))
                g_view_pos = g_view_pos + g_view_dir*0.5;
            break;
        }
        case 'd'://move viewpoint sideways to the right
        {
            Vec3f temp;
            temp.set(-g_view_dir.z(),0,g_view_dir.x());
            g_view_pos = g_view_pos + temp*0.5;
            if(!judge_camera(g_view_pos))
                g_view_pos = g_view_pos - temp*0.5;

            break;
        }
            
        case '+'://scale up
        {
            scalefactor = scalefactor * 1.1;
            //scalefactor.x() = scalefactor.x() * 2;
            break;
        }
        case '-'://scale down
        {
            scalefactor1 = scalefactor1 * 0.9;
            
            break;
        }
        case 'y':
        {//move object away from camara
//            Vec3f temp;
//            temp.set(0, 0, 1);
//            update = update - temp * 0.05;
            Vec3f temp;
            temp.set(1,1,1);
            update = update+g_view_dir*0.05;
            
            break;
        }
        case 'b'://move object toward the camera
        {
//            Vec3f temp;
//            temp.set(0, 0, 1);
//            update = update + temp * 0.05;
            Vec3f temp;
            temp.set(1,1,1);
            update = update-g_view_dir*0.05;

            break;
        }
        case 'g'://move object to the left
        {
//            Vec3f temp;
//            temp.set(1, 0, 0);
//            update = update - temp * 0.05;
            Vec3f temp;
            Vec3f g_view_verticle;
            g_view_verticle.set(g_view_dir.z(), 0, -g_view_dir.x());
            temp.set(1,1,1);
            update = update+g_view_verticle*0.05;

            break;
        }
        case 'h'://move object to the right
        {
//            Vec3f temp;
//            temp.set(1, 0, 0);
//            update = update + temp * 0.05;
            Vec3f temp;
            Vec3f g_view_verticle;
            g_view_verticle.set(-g_view_dir.z(), 0, g_view_dir.x());
            temp.set(1,1,1);
            update = update+g_view_verticle*0.05;
            break;
            
        }
        case 'i'://rotate up
        {
            

//            rotateangle = rotateangle + 1;
//            rotatevector.set(-1,0,0);
            rotateangle =  1;
            rotatevector.set(g_view_dir.z(),0,-g_view_dir.x());
            //rotatevector.set(-1,0,0);
            glRotatef(rotateangle, rotatevector.x(), rotatevector.y(), rotatevector.z());
            if(objectchoose == 1)
            {
                glMultMatrixf(g_rot_matrix);
                glGetFloatv(GL_MODELVIEW_MATRIX, g_rot_matrix);

            }
            else if(objectchoose == 2)
            {
                glMultMatrixf(g_rot_matrix1);
                glGetFloatv(GL_MODELVIEW_MATRIX, g_rot_matrix1);

            }
            else if (objectchoose == 3)
            {
                glMultMatrixf(g_rot_matrix2);
                glGetFloatv(GL_MODELVIEW_MATRIX, g_rot_matrix2);
            }
            


            
            break;

        }
        case 'j'://rotate left
        {
//            rotateangle1 = rotateangle1 + 1;
//            rotatevector1.set(0,1,0);
            rotateangle =  1;
            rotatevector.set(0,1,0);
            glRotatef(rotateangle, rotatevector.x(), rotatevector.y(), rotatevector.z());
            if(objectchoose == 1)
            {
                glMultMatrixf(g_rot_matrix);
                glGetFloatv(GL_MODELVIEW_MATRIX, g_rot_matrix);
                
            }
            else if(objectchoose == 2)
            {
                glMultMatrixf(g_rot_matrix1);
                glGetFloatv(GL_MODELVIEW_MATRIX, g_rot_matrix1);
                
            }
            else if (objectchoose == 3)
            {
                glMultMatrixf(g_rot_matrix2);
                glGetFloatv(GL_MODELVIEW_MATRIX, g_rot_matrix2);
            }


            break;
            
        }
        case 'k'://rotate right
        {
//            rotateangle1 = rotateangle1 - 1;
//            rotatevector1.set(0,1,0);
            rotateangle =  -1;
            rotatevector.set(0,1,0);
            glRotatef(rotateangle, rotatevector.x(), rotatevector.y(), rotatevector.z());
            if(objectchoose == 1)
            {
                glMultMatrixf(g_rot_matrix);
                glGetFloatv(GL_MODELVIEW_MATRIX, g_rot_matrix);
                
            }
            else if(objectchoose == 2)
            {
                glMultMatrixf(g_rot_matrix1);
                glGetFloatv(GL_MODELVIEW_MATRIX, g_rot_matrix1);
                
            }
            else if (objectchoose == 3)
            {
                glMultMatrixf(g_rot_matrix2);
                glGetFloatv(GL_MODELVIEW_MATRIX, g_rot_matrix2);
            }


            break;
        }
        case 'm'://rotate downs
        {
//            rotateangle = rotateangle - 1;
//            rotatevector.set(-1,0,0);
            rotateangle =  -1;
            //rotatevector.set(-1,0,0);
            rotatevector.set(g_view_dir.z(),0,-g_view_dir.x());
            glRotatef(rotateangle, rotatevector.x(), rotatevector.y(), rotatevector.z());
            if(objectchoose == 1)
            {
                glMultMatrixf(g_rot_matrix);
                glGetFloatv(GL_MODELVIEW_MATRIX, g_rot_matrix);
                
            }
            else if(objectchoose == 2)
            {
                glMultMatrixf(g_rot_matrix1);
                glGetFloatv(GL_MODELVIEW_MATRIX, g_rot_matrix1);
                
            }
            else if (objectchoose == 3)
            {
                glMultMatrixf(g_rot_matrix2);
                glGetFloatv(GL_MODELVIEW_MATRIX, g_rot_matrix2);
            }


            break;
        }
        case 'r':
        {
            if(shaderchooseflag == 0)
                shaderchooseflag = 1;
            else if(shaderchooseflag ==1)
                shaderchooseflag = 2;
            else if(shaderchooseflag == 2)
                shaderchooseflag = 0;
            break;
        }
        case ESCAPE: /* exit the program...normal termination. */
            exit(0);
        case '1':
        {
            objectchoose = 1;
            break;
        }
        case '2':
        {
            objectchoose = 2;
            break;
        }
        case '3':
        {
            objectchoose = 3;
            break;
        }

            
        default:
            break;
            
    }
    
    glutPostRedisplay();
}

/**********************************************************************
 * this function is called whenever the mouse is moved
 **********************************************************************/


void handle_mouse_motion(int x, int y)
{
}

/**********************************************************************
 * this function is called whenever a mouse button is pressed or released
 **********************************************************************/

void handle_mouse_click(int btn, int state, int x, int y)
{
    switch (btn) {
        case GLUT_LEFT_BUTTON:
            if (state == GLUT_DOWN) {
            }
            else {
            }
            break;
    }
    
    glutPostRedisplay();
}

void special_key(int key, int x, int y)
{
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    
    switch (key) {
        case GLUT_KEY_RIGHT: //right arrow
        {
            
            angle = angle + M_PI/180*5;
            //g_view_dir.set(cos(angle), 0, sin(angle));
            g_view_dir.set(cos(angle), globalY, sin(angle));


            
            updownrotate.set(-sin(angle), 0, cos(angle));
            
            
            break;
            

        }
        case GLUT_KEY_LEFT://left arrow
        {
            
            angle = angle - M_PI/180*5;
            //g_view_dir.set(cos(angle), 0, sin(angle));
            g_view_dir.set(cos(angle), globalY, sin(angle));

            
            updownrotate.set(-sin(angle), 0, cos(angle));

            break;
            
        }
        case GLUT_KEY_UP: //up arrow
        {
//            glRotatef(5, updownrotate.x(), updownrotate.y(), updownrotate.z());
//            glGetFloatv(GL_MODELVIEW_MATRIX, g_camera_rot);
//            for(int i = 0 ; i < 16; i++)
//                cout<<g_camera_rot[i]<<"  ";
//            cout<<"\n";
//            float xx = g_camera_rot[0]*g_view_dir.x() + g_camera_rot[4]*g_view_dir.y() + g_camera_rot[8]*g_view_dir.z();
//            float yy = g_camera_rot[1]*g_view_dir.x()+g_camera_rot[5]*g_view_dir.y() + g_camera_rot[9]*g_view_dir.z();
//            float zz = g_camera_rot[2]*g_view_dir.x()+g_camera_rot[6]*g_view_dir.y() + g_camera_rot[10]*g_view_dir.z();
//            
//            globalY = yy;
//            
//            g_view_dir.set(xx, yy, zz);

            break;
        }
        case GLUT_KEY_DOWN: //down arrow
        {
//            glRotatef(-5, updownrotate.x(), updownrotate.y(), updownrotate.z());
//            glGetFloatv(GL_MODELVIEW_MATRIX, g_camera_rot);
//            for(int i = 0 ; i < 16; i++)
//                cout<<g_camera_rot[i]<<"  ";
//            cout<<"\n";
//            float xx = g_camera_rot[0]*g_view_dir.x() + g_camera_rot[4]*g_view_dir.y() + g_camera_rot[8]*g_view_dir.z();
//            float yy = g_camera_rot[1]*g_view_dir.x()+g_camera_rot[5]*g_view_dir.y() + g_camera_rot[9]*g_view_dir.z();
//            float zz = g_camera_rot[2]*g_view_dir.x()+g_camera_rot[6]*g_view_dir.y() + g_camera_rot[10]*g_view_dir.z();
//            
//            globalY = yy;
//            
//            g_view_dir.set(xx, yy, zz);
            break;
        }
        default:
            break;
    }
    
    glutPostRedisplay();
}


int main(int argc, char * argv[])
{
    /* Initialize GLUT */
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
    glutInitWindowSize(INITIAL_WIDTH, INITIAL_HEIGHT);
    glutInitWindowPosition(INITIAL_X_POS, INITIAL_Y_POS);
    glutCreateWindow(WINDOW_NAME);
    /* Register callback functions */
    glEnable(GL_DEPTH_TEST);

   // glEnable(GL_CULL_FACE);

    
    
    glutDisplayFunc(draw_scene);
    glutReshapeFunc(resize_scene);       //Initialize the viewport when the window size changes.
    glutKeyboardFunc(key_press);         //handle when the key is pressed
    glutMouseFunc(handle_mouse_click);   //check the Mouse Button(Left, Right and Center) status(Up or Down)
    glutMotionFunc(handle_mouse_motion); //Check the Current mouse position when mouse moves
    glutSpecialFunc(special_key);        //Special Keyboard Key fuction(For Arrow button and F1 to F10 button)
    
    /* Initialize GL */
    init();
    /* Enter event processing loop */
    glutMainLoop();  
    
    return 1;
}

