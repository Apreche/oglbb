/* blocker.c
 *
 * The MAIN file.
 * Contains gtk+ and gtkglext code.
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
 
#ifdef G_OS_WIN32
#define WIN32_LENA_AND_MEAN 1
#include <windows.h>
#endif
 
#include "callbacks.c"
#include "icallbacks.c"

/* PULLDOWN MENU STRUCTURE*/
GtkItemFactoryEntry menu_items[] = {
  { "/_File",         NULL,         NULL,           0, "<Branch>" },
  { "/File/_New",     "<control>N", file_new,    0, "<StockItem>", GTK_STOCK_NEW },
  { "/File/_Open",    "<control>O", open_file,    0, "<StockItem>", GTK_STOCK_OPEN },
  { "/File/_Save",    "<control>S", save_file,    0, "<StockItem>", GTK_STOCK_SAVE },
  { "/File/_Quit",    "<CTRL>Q",    gtk_main_quit, 0, "<StockItem>", GTK_STOCK_QUIT },
};

/* NUMBER OF MENU ITEMS */
gint nmenu_items = sizeof (menu_items) / sizeof (menu_items[0]);

/* MENU WIDGET MAKER */
GtkWidget *get_menubar_menu( GtkWidget  *window ){

  GtkItemFactory *item_factory;
  GtkAccelGroup *accel_group;
  accel_group = gtk_accel_group_new();
  item_factory = gtk_item_factory_new( GTK_TYPE_MENU_BAR, "<main>", 
  										accel_group );
  gtk_item_factory_create_items( item_factory, nmenu_items, menu_items, NULL );
  gtk_window_add_accel_group( GTK_WINDOW (window), accel_group );
  return gtk_item_factory_get_widget( item_factory, "<main>" );
}

/* BUTTON MAKER FUNCTION */
void drawbutton( char *tx, char *sg, GtkWidget *btn, GtkWidget *tb, 
					GtkWidget *dar, int a, int b, int c, int d ){
	
	btn = gtk_button_new_with_mnemonic( tx );
	g_signal_connect( G_OBJECT( btn ), "clicked",
		      G_CALLBACK( callback ), (gpointer)sg );
	g_signal_connect_after( G_OBJECT( btn ), "clicked", 
				G_CALLBACK(refresh), (gpointer)dar );
	gtk_table_attach_defaults( GTK_TABLE( tb ), btn, a, b, c, d );
}

/* THE MAIN */
int main( int argc, char *argv[] ){

	/* GTK "OBJECTS" */
    GtkWidget *window;	//the main window
    GtkWidget *button;	//buttons
	GtkWidget *toggle;	//toggle buttons
    GtkWidget *big_vbox;//seperate the menu from other stuffs
	GtkWidget *hbox;	//seperate tool buttons and drawing area
	GtkWidget *vbox; 	//seperate button area
	GtkWidget *table;	//button area
	GtkWidget *label;	//labels
	GtkWidget *darea;   //drawing area
	GtkWidget *menubar; //pulldown menus
	
	/* OPENGL "OBJECTS" */
	GdkGLConfig *glconfig;
	gint major, minor;
	
	/* GTK INITIALIZATION */
    gtk_init( &argc, &argv );
	gtk_gl_init( &argc, &argv );

	/* OPENGL CONFIGURATION */
	gdk_gl_query_version( &major, &minor );
	glconfig = gdk_gl_config_new_by_mode( GDK_GL_MODE_RGB |
										  GDK_GL_MODE_DEPTH |
										  GDK_GL_MODE_DOUBLE );	
	if( glconfig == NULL ){
		glconfig = gdk_gl_config_new_by_mode( GDK_GL_MODE_RGB |
											  GDK_GL_MODE_DEPTH );
		if( glconfig == NULL ){
			g_print( "ERROR, NO OPENGL\n" );
		}
	}
	
	/* THE MAIN WINDOW */
    window = gtk_window_new( GTK_WINDOW_TOPLEVEL );
    gtk_window_set_title( GTK_WINDOW( window ), "OpenGL Building Blocks" );
	/*auto redraw window (for opengl)*/
	gtk_container_set_reallocate_redraws( GTK_CONTAINER( window ), TRUE);
    g_signal_connect( G_OBJECT( window ), "delete_event",
		      G_CALLBACK( delete_event ), NULL);
    gtk_container_set_border_width( GTK_CONTAINER( window ), 10 );
	gtk_window_set_resizable( GTK_WINDOW( window), FALSE );
	
	/* PULLDOWN MENU */
    big_vbox  = gtk_vbox_new( FALSE, 1 );
	gtk_container_set_border_width( GTK_CONTAINER( big_vbox ), 1 );
	gtk_container_add( GTK_CONTAINER (window), big_vbox );
	menubar = get_menubar_menu( window );
	gtk_box_pack_start( GTK_BOX( big_vbox ), menubar, FALSE, TRUE, 0 );
	
	/* PACKING AREAS */
	hbox = gtk_hbox_new( FALSE, 0 );
    gtk_box_pack_start( GTK_BOX( big_vbox ), hbox, FALSE, FALSE, 0 );
	vbox = gtk_vbox_new( FALSE, 0 );
	gtk_box_pack_start( GTK_BOX( hbox ), vbox, FALSE, FALSE, 10 );
	
	/* OPENGL DRAWING AREA */
	darea = gtk_drawing_area_new();
	gtk_widget_set_size_request( darea, 800, 600 );
	gtk_widget_set_gl_capability( darea, glconfig, NULL, TRUE,
								  GDK_GL_RGBA_TYPE);
	g_signal_connect_after( G_OBJECT(darea), "realize",
							G_CALLBACK(realize), NULL );
	g_signal_connect( G_OBJECT(darea), "configure_event",
					  G_CALLBACK(configure_event), NULL );
	g_signal_connect( G_OBJECT(darea), "expose_event",
					  G_CALLBACK(expose_event), NULL );
		
	/* BUTTON TABLE */
	table = gtk_table_new( 17, 3, FALSE );
	gtk_box_pack_start( GTK_BOX( vbox ), table, FALSE, FALSE, 0 );
	
	/* LABELS */
	label = gtk_label_new( "Move" );
	gtk_misc_set_alignment( GTK_MISC( label ), 0, 1 );
	gtk_table_attach_defaults( GTK_TABLE( table ), label, 0, 2, 0, 1);
	gtk_widget_show( label );
	
	label = gtk_label_new( "Rotate" );
	gtk_misc_set_alignment( GTK_MISC( label ), 0, 1 );
	gtk_table_attach_defaults( GTK_TABLE( table ), label, 0, 2, 3, 4);
	gtk_widget_show( label );
	
	label = gtk_label_new( "Pan" );
	gtk_misc_set_alignment( GTK_MISC( label ), 0, 1 );
	gtk_table_attach_defaults( GTK_TABLE( table ), label, 0, 3, 6, 7);
	gtk_widget_show( label );
	
	label = gtk_label_new( "Zoom" );
	gtk_misc_set_alignment( GTK_MISC( label ), 0, 0 );
	gtk_table_attach_defaults( GTK_TABLE( table ), label, 0, 3, 9, 10);
	gtk_widget_show( label );
	
	label = gtk_label_new("");
	gtk_table_attach_defaults( GTK_TABLE( table ), label, 0, 3, 13, 14 );
	gtk_widget_show( label );
	
	label = gtk_label_new("Toggles");
	gtk_misc_set_alignment( GTK_MISC( label ), 0, 0 );
	gtk_table_attach_defaults( GTK_TABLE( table ), label, 0, 3, 11, 12);
	gtk_widget_show(label);
	
	/* MOVEMENT BUTTONS */
	drawbutton( "R", "mr", button, table, darea, 2, 3, 0, 1 );
	drawbutton( "X+", "mx+", button, table, darea, 0, 1, 1, 2 );
	drawbutton( "Y+", "my+", button, table, darea, 1, 2, 1, 2 );
	drawbutton( "Z+", "mz+", button, table, darea, 2, 3, 1, 2 );
	drawbutton( "X-", "mx-", button, table, darea, 0, 1, 2, 3 );
	drawbutton( "Y-", "my-", button, table, darea, 1, 2, 2, 3 );
	drawbutton( "Z-", "mz-", button, table, darea, 2, 3, 2 ,3 );
	
	/* ROTATION BUTTONS */
	drawbutton( "R", "rr", button, table, darea, 2, 3, 3, 4 );
	drawbutton( "X+", "rx+", button, table, darea, 0, 1, 4, 5 );
	drawbutton( "Y+", "ry+", button, table, darea, 1, 2, 4, 5 );
	drawbutton( "Z+", "rz+", button, table, darea, 2, 3, 4, 5 );
	drawbutton( "X-", "rx-", button, table, darea, 0, 1, 5, 6);
	drawbutton( "Y-", "ry-", button, table, darea, 1, 2, 5, 6 );
	drawbutton( "Z-", "rz-", button, table, darea, 2, 3, 5, 6 );
	
	/* PANNING BUTTONS */	
	drawbutton( "R", "pr", button, table, darea, 2, 3, 6, 7 );
	drawbutton( "X+", "px+", button, table, darea, 0, 1, 7, 8 );
	drawbutton( "Y+", "py+", button, table, darea, 1, 2, 7, 8 );
	drawbutton( "Z+", "pz+", button, table, darea, 2, 3, 7, 8 );
	drawbutton( "X-", "px-", button, table, darea, 0, 1, 8, 9 );
	drawbutton( "Y-", "py-", button, table, darea, 1, 2, 8, 9 );
	drawbutton( "Z-", "pz-", button, table, darea, 2, 3, 8, 9 );
		
	/* ZOOMING BUTTONS */
	drawbutton( "ZR", "zr", button, table, darea, 1, 2, 10, 11 );
	drawbutton( "Z+", "z+", button, table, darea, 0, 1, 10, 11 );
	drawbutton( "Z-", "z-", button, table, darea, 2, 3, 10, 11 );
	
	/* BIG BUTTONS */
	drawbutton( "Place _Block", "place", button, table, darea, 0, 3, 15, 16);
	drawbutton( "Delete Block", "delete", button, table, darea, 0, 3, 16, 17 );
	drawbutton( "Reset All", "reset", button, table, darea, 0, 3, 17, 18 );
	
	/* COLOR CHOOSING BUTTON */
	button = gtk_button_new_with_label( "Change Color" );
	g_signal_connect (G_OBJECT (button), "clicked",
		      G_CALLBACK (colorselect), (gpointer) "color");
	gtk_table_attach_defaults( GTK_TABLE (table), button, 0, 3, 14, 15 );
	gtk_widget_show( button );
	
	/* TOGGLE BUTTONS */	
	toggle = gtk_toggle_button_new_with_label("G");
	gtk_table_attach_defaults( GTK_TABLE( table ), toggle, 0, 1, 12, 13);
	gtk_widget_show( toggle );
	g_signal_connect (GTK_OBJECT (toggle), "toggled",
                     G_CALLBACK (toggle_button_callback), "G");
	g_signal_connect_after( G_OBJECT( toggle ), "toggled", 
				G_CALLBACK(refresh), (gpointer)darea );
	
	toggle = gtk_toggle_button_new_with_label("O");
	gtk_table_attach_defaults( GTK_TABLE( table ), toggle, 1, 2, 12, 13);
	gtk_widget_show( toggle );
	g_signal_connect (GTK_OBJECT (toggle), "toggled",
                     G_CALLBACK (toggle_button_callback), "O" );
	g_signal_connect_after( G_OBJECT( toggle ), "toggled", 
				G_CALLBACK(refresh), (gpointer)darea );
	
	toggle = gtk_toggle_button_new_with_label("L");
	gtk_table_attach_defaults( GTK_TABLE( table ), toggle, 2, 3, 12, 13);
	gtk_widget_show( toggle );
	g_signal_connect (GTK_OBJECT (toggle), "toggled",
                     G_CALLBACK (toggle_button_callback), "L");
	g_signal_connect_after( G_OBJECT( toggle ), "toggled", 
				G_CALLBACK(refresh), (gpointer)darea );
	
	gtk_box_pack_end( GTK_BOX( hbox ), darea, FALSE, FALSE, 0 ); 
	gtk_widget_show_all( window );
    gtk_main();

    return 0;
}
