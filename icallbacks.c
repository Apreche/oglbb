/* icallbacks.c
 *
 * Callbacks that handle the buttons and keyboard input
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

/* BUTTON CALLBACK */
void callback( GtkWidget *widget, gpointer data ){
   
	if((gchar *)data == "z+" ){
		if( zoom < 0.11 ){
			zoom += 0.010;
		}
	}else if((gchar *)data == "zr" ){
		 zoomreset();
	}else if((gchar *)data == "z-" ){
		if( zoom > 0.02 ){
			zoom -= 0.010;
		}
	}else if((gchar *)data == "mr" ){
		movereset();
	}else if((gchar *)data == "rr" ){
		rotatereset();
	}else if((gchar *)data == "pr" ){
		panreset();
	}else if((gchar *)data == "reset" ){
		resetall();
	}else if((gchar *)data == "mx+" ){
		moveBlock( 1, 0, 0 );
	}else if((gchar *)data == "mx-" ){
		moveBlock( -1, 0, 0 );
	}else if((gchar *)data == "my+" ){
		moveBlock( 0, 1, 0 );
	}else if((gchar *)data == "my-" ){
		moveBlock( 0, -1, 0 );
	}else if((gchar *)data == "mz+" ){
		moveBlock( 0, 0, 1 );
	}else if((gchar *)data == "mz-" ){
		moveBlock( 0, 0, -1 );
	}else if((gchar *)data == "rx+" ){
		rotatex++;
	}else if((gchar *)data == "rx-" ){
		rotatex--;
	}else if((gchar *)data == "ry+" ){
		rotatey++;
	}else if((gchar *)data == "ry-" ){
		rotatey--;
	}else if((gchar *)data == "rz+" ){
		rotatez++;
	}else if((gchar *)data == "rz-" ){
		rotatez--;
	}else if((gchar *)data == "px+" ){
		panx += zoom / 2;
	}else if((gchar *)data == "px-" ){
		panx -= zoom / 2;
	}else if((gchar *)data == "py+" ){
		pany += zoom / 2;
	}else if((gchar *)data == "py-" ){
		pany -= zoom / 2;
	}else if((gchar *)data == "pz+" ){
		panz += zoom / 2;
	}else if((gchar *)data == "pz-" ){
		panz -= zoom /2;
	}else if((gchar *)data == "place" ){
		setBlock();
	}else if((gchar *)data == "delete" ){
		clearBlock();
	}
	
}
