# GNU/Linux makefile for GLICT
# GNU GPLv2 (or any later version at your opinion) applies to this file as well.
CPPFLAGS = -I../

SRCS = button.cpp  container.cpp  fonts.cpp  globals.cpp  glut-helper.cpp  messagebox.cpp  panel.cpp  scrollbar.cpp  textbox.cpp  window.cpp
OBJS = button.o     fonts.o    messagebox.o  textbox.o  container.o  globals.o  panel.o       window.o scrollbar.o 
all: libglict.a

libglict.a: $(OBJS)
	ar -r libglict.a $(OBJS)
	
	
#g++ $(OBJS) -o libglict.a  -lGL 
clear:
	rm *.o libglict.a

