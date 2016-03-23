/* callbacks.c
 *
 * The callback functions
 * Contains callback functions for gtk+ widgets.
 *
 * Open GL Building Blocks
 * Copyright 2004 Scott Rubin
 * 
 * Portions of this code were borrowed from the following sources:
 * examples in gtkglext cvs repository
 * gtk-2.0 tutorial
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

#include <stdlib.h>
#include <gtk/gtk.h>
#include <unistd.h>
#include "oglfunc.c"
#include "blockfile.c"

/* MENU CALLBACK FUNCTIONS */
void file_new( GtkWidget *w, gpointer data ){
  eraseall();
  resetall();
}

/* EXIT CALLBACK */
gboolean delete_event( GtkWidget *widget, GdkEvent  *event, gpointer data ){
    gtk_main_quit();
    return FALSE;
}

/* REFRESH THE DRAWING AREA */
void refresh( GtkButton *button, gpointer data ) {
     gtk_widget_queue_draw( (GtkWidget *)data );
}

/* TOGGLE BUTTON CALLBACK*/
void toggle_button_callback (GtkWidget *widget, gpointer data)
{
    if (gtk_toggle_button_get_active (GTK_TOGGLE_BUTTON (widget))) 
    {
        /* If control reaches here, the toggle button is down */
		if( (gchar *)data == "L" ){
			light = 0;
		}else if( (gchar *)data == "G" ){
			grid = 0;
		}else if( (gchar *)data == "O" ){
			origin = 1;
		}
					
    } else {
        /* If control reaches here, the toggle button is up */
    	if( (gchar *)data == "L" ){
			light = 1;
		}else if( (gchar *)data == "G" ){
			grid = 1;
		}else if( (gchar *)data == "O" ){
			origin = 0;
		}
	}
}

/* REALIZE CALLBACK FUNCTION */
void realize ( GtkWidget *widget, gpointer data ){

	GdkGLContext *glcontext = gtk_widget_get_gl_context( widget );
	GdkGLDrawable *gldrawable = gtk_widget_get_gl_drawable( widget );

	if ( !gdk_gl_drawable_gl_begin( gldrawable, glcontext ) ){
		return;
	}
	
	/* BEGIN OPENGL CODE */
	
	resetall();
	numbricks = 0;
	grid = 1;
	glClearColor( 0.5, 0.5, 0.5, 0.0 );
	glEnable( GL_POLYGON_SMOOTH );
    glEnable( GL_DEPTH_TEST );
	glEnable( GL_COLOR_MATERIAL );
	glShadeModel( GL_SMOOTH );
	glHint( GL_LINE_SMOOTH_HINT, GL_NICEST );
	glHint( GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST );
	glHint( GL_POINT_SMOOTH_HINT, GL_NICEST );
	glHint( GL_POLYGON_SMOOTH_HINT, GL_NICEST );

	/* END OPENGL CODE */
	
	gdk_gl_drawable_gl_end( gldrawable );
}

/* CONFIGURE CALLBACK FUNCTION */
gboolean configure_event( GtkWidget *widget, GdkEventConfigure *event,
                 				 gpointer data ){
	GdkGLContext *glcontext = gtk_widget_get_gl_context( widget );
	GdkGLDrawable *gldrawable = gtk_widget_get_gl_drawable( widget );

	if( !gdk_gl_drawable_gl_begin( gldrawable, glcontext ) ){
		return FALSE;
	}
	
	/* START OPENGL CODE */
	
	/* END OPENGL CODE */
	
	gdk_gl_drawable_gl_end( gldrawable );
	return TRUE;
}

/* EXPOSE CALLBACK FUNCTION */
gboolean expose_event( GtkWidget *widget, GdkEventExpose *event,
							  gpointer data ){
	
	//g_print( "EXPOSE\n" );
	GdkGLContext *glcontext = gtk_widget_get_gl_context( widget );
	GdkGLDrawable *gldrawable = gtk_widget_get_gl_drawable( widget );

	if( !gdk_gl_drawable_gl_begin( gldrawable, glcontext ) ){
    	return FALSE;
	}
	
	/* START OPENGL CODE */
		
		/* see oglfunc.c */
		display();
	
	/* END OPENGL CODE */
  
	if( gdk_gl_drawable_is_double_buffered( gldrawable ) ){
		gdk_gl_drawable_swap_buffers( gldrawable );
	}else{
		glFlush();
	}
	gdk_gl_drawable_gl_end( gldrawable );
	return TRUE;
}
 
/* COLOR SELECTION CALLBACK */
gboolean colorselect( GtkWidget *widget, GdkEvent *event, 
					gpointer client_data )
					{
  GdkColor color;
  GtkWidget *colorseldlg = NULL;
  gint response;
  GtkColorSelection *colorsel;

      if (colorseldlg == NULL){
        colorseldlg = gtk_color_selection_dialog_new ("Select new color");
	  }
      colorsel = GTK_COLOR_SELECTION (GTK_COLOR_SELECTION_DIALOG (colorseldlg)->colorsel);

      gtk_color_selection_set_previous_color (colorsel, &color);
      gtk_color_selection_set_current_color (colorsel, &color);
      gtk_color_selection_set_has_palette (colorsel, TRUE);

      response = gtk_dialog_run (GTK_DIALOG (colorseldlg));

      if (response == GTK_RESPONSE_OK){
        gtk_color_selection_get_current_color (colorsel, &color);
		float r = color.red;
		float g = color.green;
		float b = color.blue;
		r = r / 65535.0;
      	g = g / 65535.0;
		b = b / 65535.0;
		r *= 255.0;
		g *= 255.0;
		b *= 255.0;
		current.r = (int)r;
		current.g = (int)g;
		current.b = (int)b;
	  }
	
	gtk_widget_hide (colorseldlg);
	return TRUE;
}
