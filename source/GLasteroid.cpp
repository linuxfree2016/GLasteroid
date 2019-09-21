//command compiler g++ console tested with Visual Studio Code
//g++ GL01Hello.cpp -o GL01Hello.exe -L"C:/MinGW/freeglut/lib" -lglu32 -lopengl32 -lfreeglut -I"C:\MinGW\freeglut\include\GL"
/*tested on visual studio code and mingw
 * To compile with -lfreeglut -lglu32 -lopengl32 and 
 */
#include <windows.h>  // for MS Windows
#include <stdio.h>      /* printf, scanf, puts, NULL */
#include <iostream>
#include <stdlib.h>     /* srand, rand */
#include <ctime> 
#include <freeglut.h>  // GLUT, include glu.h and gl.h
using namespace std;
float spin = 0.0;
GLuint texture = 0;
int w1 = 0;
int h1 = 0;
// for random color primitive polygon
static GLubyte redc,greenc,bluec;
bool prim_polygonmode = false;
bool sec_polygonmode = false;
int scale = 1;
// glut_load_image
GLuint LoadTexture( const char * filename )
{
  GLuint texture;
  int width, height;
  unsigned char * data;
  FILE * file;
  file = fopen( filename, "rb" );
  if(!file)
    std::cout<<"File not Found"<<std::endl;
  if ( file == NULL ) return 0;
  width = 1360;
  height = 768;
  data = (unsigned char *)malloc( width * height * 3 );
  //int size = fseek(file,);
  fread( data, width * height * 3, 1, file );
  fclose( file );
 for(int i = 0; i < width * height ; ++i)
{
   int index = i*3;
   unsigned char B,R;
   B = data[index];
   R = data[index+2];
   data[index] = R;
   data[index+2] = B;
}
glGenTextures( 1, &texture );
glBindTexture( GL_TEXTURE_2D, texture );
glPixelStorei( GL_UNPACK_ALIGNMENT, 1 );//Necessary for correct elements value 4 default
glTexEnvf( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE,GL_MODULATE );
glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,GL_LINEAR_MIPMAP_NEAREST );
glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER,GL_LINEAR );
glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S,GL_REPEAT );
glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T,GL_REPEAT );
gluBuild2DMipmaps( GL_TEXTURE_2D, 3, width, height,GL_RGB, GL_UNSIGNED_BYTE, data );
free( data );
return texture;
}
/* Initialize OpenGL Graphics just n this case for colors */
void initGL() {
   // Set "clearing" or background color
   glClearColor(0.0f, 0.0f, 0.0f, 1.0f); // Black and opaque
   //randomnumber color by ctime library
   srand(time(NULL));
   redc = rand()%255;
   greenc = rand()%255;
   bluec = rand()%255;
}
/* Called back when there is no other event to be handled */
void idle() {
   spin = spin + 0.075;
   if (spin > 360.0)
         spin = 0;
   glutPostRedisplay();   // Post a re-paint request to activate display()
}
/* Handler for window re-size event. Called back when the window first appears and
   whenever the window is re-sized with its new width and height */
void reshape(GLsizei width, GLsizei height) {  // GLsizei for non-negative integer
   // Compute aspect ratio of the new window
   w1 = width;
   h1 = height;
   if (height == 0) height = 1;                // To prevent divide by 0
   GLfloat aspect = (GLfloat)width / (GLfloat)height;
   // Set the viewport to cover the new window
   glViewport(0, 0, width, height);
   // Set the aspect ratio of the clipping area to match the viewport
   glMatrixMode(GL_PROJECTION);  // To operate on the Projection matrix
   glLoadIdentity();
   if (width >= height) {
     // aspect >= 1, set the height from -1 to 1, with larger width
      gluOrtho2D(-1.0 * aspect, 1.0 * aspect, -1.0, 1.0);
   } else {
      // aspect < 1, set the width to -1 to 1, with larger height
     gluOrtho2D(-1.0, 1.0, -1.0 / aspect, 1.0 / aspect);
   }
}
void orthogonalStart() 
{
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    gluOrtho2D(-w1/2, w1/2, -h1/2, h1/2);
    glMatrixMode(GL_MODELVIEW);
} 
void orthogonalEnd()
{
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);
}
void background()
{
    glBindTexture( GL_TEXTURE_2D, texture ); 
    orthogonalStart();
    glEnable(GL_POLYGON_OFFSET_FILL);
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);                                        
    glPolygonOffset(1,1);
    // texture width/height
    const int iw = 1360;
    const int ih = 768;
    glPushMatrix();
    glTranslatef( -iw/2, -ih/2, 0 );
    glBegin(GL_QUADS);
        glColor3f(1.0f, 1.0f, 1.0f); // always default color white stars, if no this line will random color same of polygon
        glTexCoord2i(0,0); glVertex2i(0, 0);
        glTexCoord2i(1,0); glVertex2i(iw, 0);
        glTexCoord2i(1,1); glVertex2i(iw, ih);
        glTexCoord2i(0,1); glVertex2i(0, ih);
    glEnd();
    glPopMatrix();
    orthogonalEnd();
}
void display() {
   glClearColor(0.0f, 0.0f, 0.0f, 1.0f); // Set background color to black and opaque
   glClear(GL_COLOR_BUFFER_BIT);// Clear the color buffer (background
   glEnable( GL_TEXTURE_2D );
   background();
   gluLookAt (0.0, 0.0, 5.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
   glDisable( GL_TEXTURE_2D );
   // A SQUARE PARAMETERS
   if (prim_polygonmode) { // draw polygon mode lines     
             scale = 2;                      
         } else {       
             scale = 1;
             sec_polygonmode = false;
         }
   if (sec_polygonmode && prim_polygonmode) { // draw polygon mode lines            
             scale = 4;           
         } 
   if (!sec_polygonmode && prim_polygonmode) {           
             scale = 2;           
         }      
   glMatrixMode(GL_MODELVIEW);
   glLoadIdentity(); 
   glPushMatrix();
   glRotatef(spin , 0., 0., 1.);
   glTranslatef(50.0, 50.0, 0);
   glTranslatef(-50.0, -50.0, 0);
   glBegin(GL_LINE_LOOP);
         glColor3ub(redc, greenc, bluec);
         glVertex2f(-0.04f/scale, 0.23f/scale);
         glVertex2f(-0.13f/scale, 0.205f/scale);
         glVertex2f(-0.285f/scale, 0.385f/scale);
         glVertex2f(-0.38f/scale, 0.105f/scale);
         glVertex2f(-0.185f/scale, -0.235f/scale);
         glVertex2f(-0.115f/scale, -0.06f/scale);
         glVertex2f(-0.035f/scale, -0.15f/scale);
         glVertex2f(-0.035f/scale, -0.05f/scale);
         glVertex2f(0.135f/scale, -0.15f/scale);
         glVertex2f(0.285f/scale, 0.15f/scale);
         glVertex2f(0.1f/scale, 0.25f/scale);
         glVertex2f(0.135f/scale, 0.55f/scale);
         glVertex2f(-0.085f/scale, 0.485f/scale);  
   glEnd(); 
   //angle += 5.0f;
   glPopMatrix();
  // glFlush();  // Render now
   glutSwapBuffers();   // Double buffered - swap the front and back buffers
}
/* Callback handler for special-key event */
void specialKeys(int key, int x, int y) {
   switch (key) {
      case GLUT_KEY_F1:    // F1: Toggle wireframe and solid polygon
         prim_polygonmode = !prim_polygonmode;         // Toggle state      
         break;
      case GLUT_KEY_F2:    // F1: Toggle wireframe and solid polygon        
           sec_polygonmode = !sec_polygonmode;         // Toggle state     
         break;   
   }
}
/* Main function: GLUT runs as a console application starting at main()  */
int main(int argc, char** argv) {
   glutInit(&argc, argv);  // Initialize GLUT
   glutInitDisplayMode(GLUT_DOUBLE);  // Enable double buffered mode
   glutInitWindowSize(1360, 768);   // Set the window's initial width & height
   glutInitWindowPosition(0, 0);
   // Position the window's initial top-left corner
   glutCreateWindow("OpenGL Setup Test"); // Create a window with the given title
   glutSpecialFunc(specialKeys); // Register callback handler for special-key event
   glutDisplayFunc(display); // Register display callback handler for window re-paint
   glutReshapeFunc(reshape);
   glutIdleFunc(idle);
  // GLuint texture;
   texture = LoadTexture( "stars.bmp" );
   initGL(); 
   glutMainLoop();// Enter the event-processing loop
   //Free our texture
   glDeleteTextures( 1, &texture );
   return 0;
}