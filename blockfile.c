/* blockfile.c
 *
 * Functions for reading and writing default oglbb file format
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
#include "oglfunc.h"

/* ACTUALLY WRITES A FILE */
int writefile( const char *filename, struct brick b[], int bs ){
	
	int retval = 0;
	FILE *fp = fopen(filename, "w");
	
	if(fp == NULL) {
		fprintf(stderr, "Can't open output file %s", filename);
		fflush( stderr );
		retval++;
	}else{
		for( int x=0; x < bs; x++ ){
			fprintf( fp, "%d %d %d %d %d %d\n", 
					 b[x].x, b[x].y, b[x].z, b[x].r, b[x].g, b[x].b );
		}
		fflush( fp );
	return retval;	
	}
}

/* COPIES CONTENTS OF FILE INTO MODEL */
void readfile( const char *filename ){
	
	struct brick retval[MAX_BRICKS];
	
	FILE *fp = fopen( filename, "r");
	
	int n=0;
	
	if( fp == NULL ){
		fprintf(stderr, "Cant open file %s", filename );
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

/* CALLS readfile() */
void opencall( GtkWidget *w, GtkFileSelection *fn ){
	const gchar *filename;
	filename = gtk_file_selection_get_filename( GTK_FILE_SELECTION( fn ) );
	readfile( filename );
}

/* CALLS writefile */
void savecall( GtkWidget *w, GtkFileSelection *fn ){
	const gchar *filename;
	filename = gtk_file_selection_get_filename( GTK_FILE_SELECTION( fn ) );
	writefile( filename, model, numbricks );
}

/* The callback when they push the open menu item */
void open_file( GtkWidget *w, gpointer data )
{
	GtkWidget *fileget;
	fileget = gtk_file_selection_new( "Open File" );
	
	g_signal_connect( G_OBJECT(fileget), "destroy",
						G_CALLBACK(gtk_widget_destroy), NULL);
	
	g_signal_connect( G_OBJECT(GTK_FILE_SELECTION(fileget)->ok_button),
						"clicked", G_CALLBACK( opencall ), (gpointer)fileget);
	
	g_signal_connect_swapped (GTK_OBJECT (GTK_FILE_SELECTION (fileget)->ok_button),
                             "clicked",
                             G_CALLBACK (gtk_widget_destroy), 
                             (gpointer) fileget); 
	
	g_signal_connect_swapped (G_OBJECT (GTK_FILE_SELECTION (fileget)->cancel_button),
	                      "clicked", G_CALLBACK (gtk_widget_destroy),
			      G_OBJECT (fileget));
	gtk_widget_show( fileget );
}

/* The callback when they push the save menu item */
void save_file( GtkWidget *w, gpointer data )
{
	GtkWidget *fileget;
	fileget = gtk_file_selection_new( "Save File As" );
	
	g_signal_connect( G_OBJECT(fileget), "destroy",
						G_CALLBACK(gtk_widget_destroy), NULL);
	
	g_signal_connect( G_OBJECT(GTK_FILE_SELECTION(fileget)->ok_button),
						"clicked", G_CALLBACK( savecall), (gpointer)fileget);
	
	g_signal_connect_swapped (GTK_OBJECT (GTK_FILE_SELECTION (fileget)->ok_button),
                             "clicked",
                             G_CALLBACK (gtk_widget_destroy), 
                             (gpointer) fileget); 
	
	g_signal_connect_swapped (G_OBJECT (GTK_FILE_SELECTION (fileget)->cancel_button),
	                      "clicked", G_CALLBACK (gtk_widget_destroy),
			      G_OBJECT (fileget));
	gtk_widget_show( fileget );
}
