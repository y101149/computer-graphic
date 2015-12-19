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

PolyModel background;
Vec3f g_view_dir;
Vec3f g_view_pos;

PolyModel gamepiece;///////////////////////

Vec3f inverse;
Vec3f slide;

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
    
    gluOrtho2D(0, 100, 0, 100);

    
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    
}

/**********************************************************************
 * any program initialization (set initial OpenGL state,
 **********************************************************************/
void init()
{
    string ifile = RESOURCE_DIR + string("background.d2");//load two models
    ifstream file(ifile.c_str());
    background.loadModel(file);
    
    ifile = RESOURCE_DIR + string("gamepiece.d2");
    ifstream file1(ifile.c_str());
    gamepiece.loadModel(file1);
    
    glDisable(GL_LIGHTING);
    glDisable(GL_CULL_FACE);
    background.getExteriorEdge();
    
    
    
    
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
    
    // Draw model in line mode for now
    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);//IF DELETE THIS IT WILL BECOME A SQUARE
    

    glColor3f(1.0f, 0.0f, 0.0f);
    
    // You need to implement this draw function!
    background.draw();
    //if(background.detectCollision(gamepiece.getCenter(),gamepiece.getRadius()))
    if(background.detectCollision2(gamepiece.getCenter(), gamepiece.getRadius()))
    {//the two if clause are both ok for collision detect
//        cout<<background.collisionEdge.x()<<"  "<<background.collisionEdge.y()<<"\n";
        
        glColor3f(1.0, 1.0, 0);
        float direction = gamepiece.getDirectionVector().dot(slide);
        if(direction == 0)
        {
            gamepiece.translate(inverse);
        }
        else if(direction > 0)
        {
            gamepiece.translate(inverse);
            gamepiece.translate(slide);
            if(background.detectCollision2(gamepiece.getCenter(), gamepiece.getRadius()))
            {
                gamepiece.translate(-slide);

            }
               
               
        }
        else if(direction < 0)
        {
            gamepiece.translate(inverse);
            gamepiece.translate(-slide);
            if(background.detectCollision2(gamepiece.getCenter(), gamepiece.getRadius()))
            {
                gamepiece.translate(slide);
                
            }
        }
        
        gamepiece.draw();

    }
    else
    {
        glColor3f(1.0, 0.0, 0);
        gamepiece.draw();
    }
    
    // Example to show how to write text (for debugging) to the screen:
    glColor3f(1.0f, 1.0f, 1.0f);
    drawGlutString("Assignment 1");
    
    /* since this is double buffered, swap the
     * buffers to display what just got drawn */
    glutSwapBuffers();
}

void key_press(unsigned char key, int x, int y)
{
    switch (key) {
        case ' ':
            break;
        case 'w':
            gamepiece.translate(gamepiece.getDirectionVector());
            inverse = -gamepiece.getDirectionVector();
            slide = background.collisionEdge;
            break;
        case 'a':
            gamepiece.translate(gamepiece.getHorizonVector());
            inverse = -gamepiece.getHorizonVector();
            slide = background.collisionEdge;
            break;
        case 's':
            gamepiece.translate(-gamepiece.getDirectionVector());
            inverse = gamepiece.getDirectionVector();
            slide = background.collisionEdge;
            break;
        case 'd':
            gamepiece.translate(-gamepiece.getHorizonVector());
            inverse = gamepiece.getHorizonVector();
            slide = background.collisionEdge;
            break;
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
            gamepiece.rotate(-pi/180*5);//left arrow
            break;
        case GLUT_KEY_LEFT:
            gamepiece.rotate(pi/180*5);//left arrow
            break;
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

