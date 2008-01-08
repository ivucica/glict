Getting the libraries is your responsibility. (If you downloaded a .devpak, it does not explicitly depend on them.)

So far you'll need GLUT or SDL. Under Windows, you'll also need GLAUX.

Please note, if you're using SVN version, examples can be built by choosing a different target in the Code::Blocks project. We have both GNU/Linux and Win32 targets.

For Windows:
To build these, create a new project. Add all files from a specific project into the project.
You'll then need to link them with the following libraries.
If you use Dev-Cpp, just copy paste the line in the project options. 
If you use Code::Blocks, add them without "-l" part into the linker options.

GLUT examples:
-lGLICT -lglut -lglut32 -lwinmm -lglaux -lgdi32 -lopengl32 -lglu32 
SDL examples:
-lGLICT -lmingw32 -lSDLmain -lSDL

For GNU/Linux:

If you're a developer here, well, you should know what you're doing by now ;)
In any case, link with:

GLUT examples:
-lGLICT -lGL -lglut
SDL exmaples:
-lGLICT -lSDL