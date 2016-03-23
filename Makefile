blocker:
	gcc blocker.c -o oglbb -std=c99 `pkg-config --cflags --libs gtkglext-1.0`
	gcc shader.c -o shader -std=c99 -lGL -lGLU -lglut -lCg -lCgGL
