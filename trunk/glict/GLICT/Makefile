# GNU/Linux makefile for GLICT
# GNU LGPLv2 (or any later version at your opinion) applies to this file as 
# well.
CPPFLAGS = -I../
CXX = g++

SRCS = button.cpp  container.cpp  fonts.cpp  globals.cpp   messagebox.cpp  panel.cpp  scrollbar.cpp  textbox.cpp  window.cpp skinner.cpp list.cpp progressbar.cpp
OBJS = button.o     fonts.o    messagebox.o  textbox.o  container.o  globals.o  panel.o       window.o scrollbar.o  skinner.o list.o progressbar.o
all: libGLICT.a

libGLICT.a: $(OBJS)
	ar cru libGLICT.a $(OBJS)
	ranlib libGLICT.a
	
	
#g++ $(OBJS) -o libglict.a  -lGL 
clean:
	rm $(OBJS) libGLICT.a

