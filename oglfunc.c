/* oglfunc.c
 *
 * OpenGL functions
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
#include <GL/gl.h>
#include <GL/glu.h>
#include <glib.h>
#include <gdk/gdk.h>
#include <gtk/gtk.h>
#include <gtk/gtkgl.h>
#include "oglfunc.h"

#define MAX_BRICKS 1024

/* OPENGL DATA */
float zoom;
struct brick model[MAX_BRICKS];
int numbricks;
struct brick current;
float rotatex, rotatey, rotatez;
float panx, pany, panz;
int grid;
int origin;

/* LIGHTS DEFINED */
int light = 1;
GLfloat lightpos[] = { 3.0, 3.0, 3.0, 0.0 };
GLfloat lightN[] = { 0.0, 0.0, 0.0, 0.0 };
GLfloat lightY[] = { 1.0, 1.0, 1.0, 0.0 };

/* CHEAP HACK TO ERASE ALL BRICKS */
/* However, it does work and does perform well */
void eraseall(){
	numbricks = 0;
}	

/* Put current at origin */
void movereset(){
	current.x = 0;
	current.y = 0;
	current.z = 0;
}

/* Undo all rotattions */
void rotatereset(){
	rotatex = 0;
	rotatey = 0;
	rotatez = 0;
}

/* Undo all panning */
void panreset(){
	panx = 0;
	pany = 0;
	panz = 0;
}

/* Set zoom to default */
void zoomreset(){
	zoom = 0.05;
}

/* RESET EVERYTHING! */
void resetall(){
	movereset();
	rotatereset();
	panreset();
	zoomreset();
}

/* moves the selector brick */
void moveBlock( int x, int y, int z ){
	current.x += x;
	current.y += y;
	current.z += z;
}

/* delete selected brick */
void clearBlock() {

	int brick = -1;
	for( int n=0; n < numbricks; n++ ){
		if( model[n].x == current.x &&
			model[n].y == current.y &&
			model[n].z == current.z ){
					brick = n;
		}
	}
	if( brick != -1 ){
		for( brick; brick < numbricks-1; brick++ ){
			model[brick] = model[brick+1];
		}
		numbricks--;
	}
}


/* places brick of appropriate color at selected space */
void setBlock(){
	int replace=0;
	if( numbricks < MAX_BRICKS ){
		for( int n=0; n < numbricks; n++ ){
			if( model[n].x == current.x &&
				model[n].y == current.y &&
				model[n].z == current.z ){
					model[n] = current;
					replace = 1;
			}
		}
		if( replace == 0 ){
			model[numbricks] = current;
			numbricks++;
		}
	}else{
		printf( "OUT OF BRICKS\n" );
	}
}

/* Draws the lights */
void drawLight(){
	glLightfv( GL_LIGHT0, GL_AMBIENT, lightY );
	glLightfv( GL_LIGHT0, GL_DIFFUSE, lightN );
	glLightfv( GL_LIGHT0, GL_SPECULAR, lightN );
	glLightfv( GL_LIGHT0, GL_POSITION, lightpos );
	
	glLightfv( GL_LIGHT1, GL_AMBIENT, lightN);
	glLightfv( GL_LIGHT1, GL_DIFFUSE, lightY );
	glLightfv( GL_LIGHT1, GL_SPECULAR, lightN );
	glLightfv( GL_LIGHT1, GL_POSITION, lightpos );
	
	glLightfv( GL_LIGHT2, GL_AMBIENT, lightN );
	glLightfv( GL_LIGHT2, GL_DIFFUSE, lightN );
	glLightfv( GL_LIGHT2, GL_SPECULAR, lightY );
	glLightfv( GL_LIGHT2, GL_POSITION, lightpos );
}

/* Draws a single cube */
void drawCube( struct brick b ){
	glPushMatrix();
		glColor3ub( b.r, b.g, b.b );
		glTranslatef( zoom * b.x, zoom *  b.y, zoom * b.z );
		gdk_gl_draw_cube( TRUE, zoom );
	glPopMatrix();
}

/* DRAW THE GRID */
void drawGrid(){
	glPushMatrix();
	glColor3ub( 255, 255, 255 );
	glBegin( GL_LINES );
		for( float x = 0; x < 1; x += zoom ){
			glVertex3f( x, 0, 0 );
			glVertex3f( x, 0, 1 );
		}
		for( float z = 0; z < 1; z += zoom ){
			glVertex3f( 0, 0, z );
			glVertex3f( 1, 0, z );
		}
	glEnd();
	glPopMatrix();
				
}

/* DRAW THE ORIGIN AXES */
void drawOrigin(){
	glPushMatrix();
	glColor3ub( 255, 0, 0 );
		glBegin( GL_LINES );
			glVertex3i( 0, 0, 0 );
			glVertex3i( 1, 0, 0 );
		glEnd();
		glColor3ub( 0, 255, 0 );
		glBegin( GL_LINES );
			glVertex3i( 0, 0, 0 );
			glVertex3i( 0, 1, 0 );
		glEnd();
		glColor3ub( 0, 0, 255 );
		glBegin( GL_LINES );
			glVertex3i( 0, 0, 0 );
			glVertex3i( 0, 0, 1 );
		glEnd();
	glPopMatrix();
}

/* draw all the cubes in the model, then the selector brick */
void drawStuff(){
	glTranslatef( panx, pany, panz );
	glPushMatrix();
	
	/* ROTATE */
	glRotatef( rotatex, 1.0, 0.0, 0.0 );
	glRotatef( rotatey, 0.0, 1.0, 0.0 );
	glRotatef( rotatez, 0.0, 0.0, 1.0 );
	
	/*Draw the selector block, always ambiently lighted */
	glDisable( GL_LIGHTING );
	if( origin == 0 ){
		drawOrigin();
	}
	if( grid == 0 ){
		drawGrid();
	}
	glPushMatrix();
		glColor3ub( 255, 255, 255 );
		glTranslatef( zoom * current.x, zoom * current.y, zoom * current.z );
		gdk_gl_draw_cube( FALSE, zoom );
		glColor3ub( current.r, current.g, current.b );
		gdk_gl_draw_cube( TRUE, zoom );
	glPopMatrix();
	glEnable( GL_LIGHTING );
	
	/* Draw all the other blocks in the model */
	glPushMatrix();
		for( int x=0; x < numbricks; x++ ){
			drawCube( model[x] );
		}
	glPopMatrix();
	glPopMatrix();
}

/* ALMIGHTY DISPLAY FUNCTION!!! */
void display(){

	/* TURN OFF THAT LIGHT! */
	glDisable( GL_LIGHTING );
	
	glViewport( 0, 0, 800, 600 );
	glMatrixMode( GL_PROJECTION );
	glLoadIdentity();
	gluOrtho2D( 0, 800, 0, 600 );
	glMatrixMode( GL_MODELVIEW );
	glLoadIdentity();
	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
	glColor3ub( 255, 255, 255 );
	
	/* seperate the viewports with lines */
	glColor3ub( 255, 255, 255 );
	glBegin( GL_LINES );
		glVertex2i( 600, 0 );
		glVertex2i( 600, 600 );
	glEnd();
	glBegin( GL_LINES );
		glVertex2i( 600, 200 );
		glVertex2i( 800, 200 );
	glEnd();
	glBegin( GL_LINES );
		glVertex2i( 600, 400 );
		glVertex2i( 800, 400 );
	glEnd();
	
	/* LET THERE BE LIGHT! */
	glEnable( GL_LIGHTING );
	
	if( light == 0 ){
		glEnable( GL_LIGHT0 );
		glDisable( GL_LIGHT1 );
		glDisable( GL_LIGHT2 );
	}else{
		glDisable( GL_LIGHT0 );
		glEnable( GL_LIGHT1 );
		glEnable( GL_LIGHT2 );
	}
	drawLight();
	
	/* FRONT VIEWPORT */
	glViewport( 600, 400, 200, 200 );
	glMatrixMode( GL_PROJECTION );
	glLoadIdentity();
	glOrtho( -1.0, 1.0, -1.0, 1.0, 1, 256 );
	glMatrixMode( GL_MODELVIEW );
	glLoadIdentity();
	gluLookAt( 0.5, 0.5, 3.0, 0.5, 0.5, 0.5, 0.0, 1.0, 0.0 );
	drawStuff();

	/* TOP VIEWPORT */
	glViewport( 600, 200, 200, 200 );
	glMatrixMode( GL_PROJECTION );
	glLoadIdentity();
	glOrtho( -1.0, 1.0, -1.0, 1.0, 1, 256 );
	glMatrixMode( GL_MODELVIEW );
	glLoadIdentity();
	gluLookAt( 0.5, 3.0, 0.5, 0.5, 0.5, 0.5, 0.0, 0.0, -1.0 );
	drawStuff();
	
	/* SIDE VIEWPORT */
	glViewport( 600, 0, 200, 200 );
	glMatrixMode( GL_PROJECTION );
	glLoadIdentity();
	glOrtho( -1.0, 1.0, -1.0, 1.0, 1, 256 );
	glMatrixMode( GL_MODELVIEW );
	glLoadIdentity();
	gluLookAt( 3.0, 0.5, 0.5, 0.5, 0.5, 0.5, 0.0, 1.0, 0.0 );
	drawStuff();

	/* ANGULAR VIEWPORT (the big one) */
	glViewport( 0, 0, 600, 600 );
	glMatrixMode( GL_PROJECTION );
	glLoadIdentity();
	glOrtho( -1.0, 1.0, -1.0, 1.0, 1, 256 );
	glMatrixMode( GL_MODELVIEW );
	glLoadIdentity();
	gluLookAt( 5.0, 5.0, 5.0, 0.0, 0.0, 0.0, -1.0, 1.0, -1.0 );
	drawStuff();

}
