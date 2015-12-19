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

PolyModel car;
float angle = 3*pi/2;
Vec3f g_view_dir;
Vec3f g_view_pos;

Vec3f carcenter;
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
    gluPerspective(120, (GLfloat) width / (GLfloat) height, 0.01, 100);

    
    //glOrtho(-3, 3, -3, 3, -3, 3);
    
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    


    

    
}

/**********************************************************************
 * any program initialization (set initial OpenGL state,
 **********************************************************************/
void init()
{
    string ifile = RESOURCE_DIR + string("car.d2");//load two models
    ifstream file(ifile.c_str());
    car.loadModel(file);
    glClearColor(0, 0, 1, 0);
    glDisable(GL_LIGHTING);
    glDisable(GL_CULL_FACE);
    g_view_dir.set(cos(angle), 0, sin(angle));//set the view position at (0,0,1)  and direction to the origin.
    g_view_pos.set(0,0,1);
    carcenter = car.getCenter();
    update.set(0, 0, 0);
    scalefactor.set(1,1,1);
    scalefactor1.set(1,1,1);
    
    g_rot_matrix[0] = 1;
    g_rot_matrix[5] = 1;
    g_rot_matrix[10] = 1;
    g_rot_matrix[15] = 1;

    
    
}


/**********************************************************************
 * The main drawing functions.
 **********************************************************************/
void draw_scene(void)
{

    /* clear the screen and the depth buffer */
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
//    cout<<car.getCenter().x()<<" "<<car.getCenter().y()<<" "<<car.getCenter().z();
    /* reset modelview matrix */
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    
    gluLookAt(g_view_pos.x(), g_view_pos.y(), g_view_pos.z(), g_view_pos.x()+ g_view_dir.x(), g_view_pos.y()+ g_view_dir.y(), g_view_pos.z()+ g_view_dir.z(), 0, 1, 0);//this should be execute at last

    

    glTranslatef(update.x(), update.y(), update.z());//translate function
    
    //glTranslated(1, 0, 0);
    Vec3f center = car.getCenter();//rotate up and down
//    glTranslatef(center.x(), center.y(), center.z());
//    glRotatef(rotateangle, rotatevector.x(), rotatevector.y(), rotatevector.z());
//    glTranslatef(-center.x(), -center.y(), -center.z());
//
//
//    
//    glTranslatef(center.x(), center.y(), center.z());//rotate left and right
//    glRotatef(rotateangle1, rotatevector1.x(), rotatevector1.y(), rotatevector1.z());
//    glTranslatef(-center.x(), -center.y(), -center.z());
    glMultMatrixf(g_rot_matrix);

    glTranslatef(center.x(), center.y(), center.z());//scale
    glScalef(scalefactor.x(), scalefactor.y(), scalefactor.z());
    glTranslatef(-center.x(), -center.y(), -center.z());
    
    glTranslatef(center.x(), center.y(), center.z());//scale
    glScalef(scalefactor1.x(), scalefactor1.y(), scalefactor1.z());
    glTranslatef(-center.x(), -center.y(), -center.z());


    // Draw model in line mode for now
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);//IF DELETE THIS IT WILL BECOME A SQUARE
    glColor3f(1.0f, 0.0f, 0.0f);
    // You need to implement this draw function!
    car.draw();
    // Example to show how to write text (for debugging) to the screen:
    glColor3f(1.0f, 1.0f, 1.0f);
    drawGlutString("Assignment 1");
    /* since this is double buffered, swap the
     * buffers to display what just got drawn */
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
           g_view_pos = g_view_pos + g_view_dir*0.05;
           break;
        }
        case 'a'://move viewpoint sideways to the left
        {
            Vec3f temp;
            temp.set(g_view_dir.z(),0,-g_view_dir.x());
            g_view_pos = g_view_pos + temp*0.05;
            //cout<<g_view_pos.x()<<" "<<g_view_pos.y()<<" "<<g_view_pos.z()<<"\n";
            break;
        }
        case 's'://move viewpoint backward
        {
            g_view_pos = g_view_pos - g_view_dir*0.05;
            break;
        }
        case 'd'://move viewpoint sideways to the right
        {
            Vec3f temp;
            temp.set(-g_view_dir.z(),0,g_view_dir.x());
            g_view_pos = g_view_pos + temp*0.05;
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
            glMultMatrixf(g_rot_matrix);//每次调用该函数，初始化modelview，所以开始modelmatrix都是单位矩阵，然后乘以累计矩阵和rotate矩阵栈顶变成新的帅矩阵，将此时的modelmatrix存到累计矩阵中即可
            
            glGetFloatv(GL_MODELVIEW_MATRIX, g_rot_matrix);


            
            break;

        }
        case 'j'://rotate left
        {
//            rotateangle1 = rotateangle1 + 1;
//            rotatevector1.set(0,1,0);
            rotateangle =  1;
            rotatevector.set(0,1,0);
            glRotatef(rotateangle, rotatevector.x(), rotatevector.y(), rotatevector.z());
            glMultMatrixf(g_rot_matrix);
            glGetFloatv(GL_MODELVIEW_MATRIX, g_rot_matrix);


            break;
            
        }
        case 'k'://rotate right
        {
//            rotateangle1 = rotateangle1 - 1;
//            rotatevector1.set(0,1,0);
            rotateangle =  -1;
            rotatevector.set(0,1,0);
            glRotatef(rotateangle, rotatevector.x(), rotatevector.y(), rotatevector.z());
            glMultMatrixf(g_rot_matrix);
            glGetFloatv(GL_MODELVIEW_MATRIX, g_rot_matrix);


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
            glMultMatrixf(g_rot_matrix);
            glGetFloatv(GL_MODELVIEW_MATRIX, g_rot_matrix);


            break;
        }
        case ESCAPE: /* exit the program...normal termination. */
            exit(0);
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
    switch (key) {
        case GLUT_KEY_RIGHT: //right arrow
        {
            
            angle = angle + pi/180;
            if(angle > 2*pi)
                angle = angle - 2*pi;//make it work over 360 degree
            g_view_dir.set(cos(angle), 0, sin(angle));
            break;
            

        }
        case GLUT_KEY_LEFT://left arrow
        {
            
            angle = angle - pi/180;
            if(angle < 0)
                angle = 2*pi - angle;
            g_view_dir.set(cos(angle), 0, sin(angle));
            break;
            
        }
        case GLUT_KEY_UP: //up arrow
            break;
        case GLUT_KEY_DOWN: //down arrow
            break;
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

