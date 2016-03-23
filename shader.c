/* shader.c
 *
 * NVidia Cg Shader program for oglbb
 *
 * Open GL Building Blocks
 * Copyright 2004 Scott Rubin
 *
 * This file is part of oglbb.
 *
 * oglbb is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * oglbb is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with oglbb; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 */
 
#include <stdio.h>
#include <stdlib.h>

#include <GL/glut.h>

#include <Cg/cg.h>
#include <Cg/cgGL.h>

#include "oglfunc.h"

#define MAX_BRICKS 1024


int numbricks;
struct brick model[MAX_BRICKS];
float zoom = 0.05;

GLfloat lightpos[] = { 3.0, 3.0, 3.0, 0.0 };
GLfloat lightN[] = { 0.0, 0.0, 0.0, 0.0 };
GLfloat lightY[] = { 1.0, 1.0, 1.0, 0.0 };

/* Cg Variables */
static CGcontext CgCtxt = NULL;
static CGprogram CgProg = NULL;
static CGprofile CgProf = CG_PROFILE_VP20;

static CGparameter ModelViewProjParam = NULL;
static CGparameter LightAngle = NULL;
static CGparameter LightY = NULL;

float degrees = 0.0;
float ypos = 0.0;

/* COPIES CONTENTS OF FILE INTO MODEL */
void readfile( const char *filename ){
	
	struct brick retval[MAX_BRICKS];
	
	FILE *fp = fopen( filename, "r");
	
	int n=0;
	
	if( fp == NULL ){
		fprintf(stderr, "Cant open file %s", filename );
		exit(0);
	}else{
		while( fscanf( fp, "%d %d %d %d %d %d",
		&retval[n].x, &retval[n].y, &retval[n].z, 
		&retval[n].r, &retval[n].g, &retval[n].b ) != EOF ){
			n++;
		}
	}
	
	numbricks = n;
	for( int p=0; p < n; p++ ){
		model[p].x = retval[p].x;
		model[p].y = retval[p].y;
		model[p].z = retval[p].z;
		model[p].r = retval[p].r;
		model[p].g = retval[p].g;
		model[p].b = retval[p].b;
	}
}

void setCgParams(){
	
	if( ModelViewProjParam != NULL )
		cgGLSetStateMatrixParameter( ModelViewProjParam,
								CG_GL_MODELVIEW_PROJECTION_MATRIX,
								CG_GL_MATRIX_IDENTITY );
	
	if( LightAngle != NULL )
		cgGLSetParameter1f( LightAngle, degrees );
		
	if( LightY != NULL )
		cgGLSetParameter1f( LightY, ypos );
	
}

/* draw all the cubes */
void drawStuff(){

	glRotatef( 5 * degrees, 0.0, 1.0, 0.0 );
	cgGLBindProgram( CgProg );
	setCgParams();
	cgGLEnableProfile( CgProf );	
	
	glPushMatrix();

	for( int n=0; n < numbricks; n++ ){
	
		float r = (float)model[n].r;
		float g = (float)model[n].g;
		float b = (float)model[n].b;
		r = r / 255.0;
		g = g / 255.0;
		b = b / 255.0;
		float x = model[n].x * zoom;
		float y = model[n].y * zoom;
		float z = model[n].z * zoom;
		
		glColor3f( r, g, b );
		
		glBegin( GL_QUADS );
			glNormal3f( 0.0, -1.0, 0.0 );				
			glVertex3f( x, y, z );
			glVertex3f( x+zoom, y, z );
			glVertex3f( x+zoom, y, z+zoom );
			glVertex3f( x, y, z+zoom );
		glEnd();
		glBegin( GL_QUADS );
			glNormal3f( -1.0, 0.0, 0.0 );
			glVertex3f( x, y, z );
			glVertex3f( x, y, z+zoom );
			glVertex3f( x, y+zoom, z+zoom );
			glVertex3f( x, y+zoom, z );
		glEnd();
		glBegin( GL_QUADS );
			glNormal3f( 0.0, 0.0, -1.0 );
			glVertex3f( x, y, z );
			glVertex3f( x+zoom, y, z );
			glVertex3f( x+zoom, y+zoom, z );
			glVertex3f( x, y+zoom, z );
		glEnd();
		glBegin( GL_QUADS );
			glNormal3f( 1.0, 0.0, 0.0 );
			glVertex3f( x+zoom, y, z );
			glVertex3f( x+zoom, y+zoom, z );
			glVertex3f( x+zoom, y+zoom, z+zoom );
			glVertex3f( x+zoom, y, z+zoom );
		glEnd();
		glBegin( GL_QUADS );
			glNormal3f( 0.0, 0.0, 1.0 );
			glVertex3f( x, y, z+zoom );
			glVertex3f( x, y+zoom, z+zoom );
			glVertex3f( x+zoom, y+zoom, z+zoom );
			glVertex3f( x+zoom, y, z+zoom );
		glEnd();
		glBegin( GL_QUADS );
			glNormal3f( 0.0, 1.0, 0.0 );
			glVertex3f( x, y+zoom, z );
			glVertex3f( x+zoom, y+zoom, z );
			glVertex3f( x+zoom, y+zoom, z+zoom );
			glVertex3f( x, y+zoom, z+zoom );
		glEnd();
	
	}
	glPopMatrix();
	
	cgGLDisableProfile( CgProf );
	

}

//The almighty display function
void display( void ) {
	
  	/* THE BOX */
	glViewport( 0, 0, 800, 600 );
	glMatrixMode( GL_PROJECTION );
	glLoadIdentity();
	gluOrtho2D( 0, 800, 0, 600 );
	glMatrixMode( GL_MODELVIEW );
	glLoadIdentity();
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	/* ANGULAR VIEWPORT */
	glViewport( 0, 0, 800, 600 );
	glMatrixMode( GL_PROJECTION );
	glLoadIdentity();
	glOrtho( -1.0, 1.0, -1.0, 1.0, 1, 256 );
	glMatrixMode( GL_MODELVIEW );
	glLoadIdentity();
	gluLookAt( 5.0, 5.0, 5.0, 0.0, 0.0, 0.0, -1.0, 1.0, -1.0 );
	drawStuff();
	
	glutSwapBuffers();

}


void idle( void ){
	degrees += 0.00075;
	ypos += 0.0005;
	glutPostRedisplay();
}


/* Initialize glut */
void init( int argc, char *argv[] ) {

  glutInit( &argc, argv );
  glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
  glutInitWindowSize( 800, 600 );
  glutInitWindowPosition( 75, 75 );
  glutCreateWindow( "OpenGL Building Block Shader" );
  
  glutDisplayFunc( display );
  glutIdleFunc( idle );

  glClearColor( 1.0, 1.0, 1.0, 0.0 );
  glEnable( GL_DEPTH_TEST );
  
}

void CgInit(){
	CgCtxt = cgCreateContext();
	CgProg = cgCreateProgramFromFile( CgCtxt, CG_SOURCE, "shader.cg",
											CgProf, NULL, NULL );
	if( CgProg != NULL ){
		
		cgGLLoadProgram( CgProg );
		
		ModelViewProjParam = cgGetNamedParameter( CgProg, "ModelViewProj" );
		
		LightAngle = cgGetNamedParameter( CgProg, "LA" );
		
		LightY = cgGetNamedParameter( CgProg, "LY" );
	}
}

/* THE MAIN */
int main( int argc, char** argv ) {
	
	if( argc == 2 ){
		char *a = argv[1];
		readfile( a );
	}else{
		printf( "Usage: shader <filename>\n" );
		exit(0);
	}
	
	init(argc, argv);
	CgInit();
	glutMainLoop();
	
	cgDestroyProgram( CgProg );
	cgDestroyContext( CgCtxt );

	return 0;
}
