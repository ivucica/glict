/*
    GLICT - Graphics Library Interface Creation Toolkit
    Copyright (C) 2006 OBJECT Networks

    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Library General Public
    License as published by the Free Software Foundation; either
    version 2 of the License, or (at your option) any later version.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Library General Public License for more details.

    You should have received a copy of the GNU Library General Public
    License along with this library; if not, write to the Free
    Software Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
*/


// GLICTMemory
// simple game made with GLICT
// to compile you'll need GLUT

// GLUT includes
// we use glut because it's cross platform compatible
// for mac os x, use <OpenGL/glut.h> if i remember correclty
#include <GL/glut.h>

// GLICT includes
// following includes should, in your project, actually look a
// bit different
// this project, as it is, should be placed inside the GLICT's folder so
// its includes is relative
#include "../../container.h"
#include "../../button.h"
#include "../../globals.h"
#include "../../window.h"
#include "../../panel.h"
#include "../../messagebox.h"

// some nice strings for the cards... ;)
char cardtitles[16][16] = { "Joe", "Annie", "Andrew", "the fike", "Pekay", "Yorick", "tliff", "SimOne"};
// and some colors for the cards
float cardcolor[16][3] = { { 1., 0., 0.}, { 0., 0., 1. }, {1., 1., 0.}, {1., 0., 1.},
                           {0., 1., 0.},  { .5, 0., 0. }, {0., 1., 1.}, {.5, 1., .5}};
// other game related vars
char matrix[16]; // matrix specifies where is each card
char taken[16]; // taken specifies if we've used a card in generation, so we dont generate same pair again
char solved[16]; // solved specifies if a piece has been solved
char flipd[2]; // which cards are currently flipped


int totalsolved = 0, totalopens = 0;

glictContainer desktop;
glictWindow window;
glictButton cards[16];
glictPanel pnlSolveds;
glictMessageBox* msgSuccess;
int windowhandle; // glut's window identifier

// displays stuff, glut's way of doing stuff
void display() {

    // make sure the stenciltest is off before clearing
    // in this example we're sure so we dont enforce it
    glClearColor(0.0,0.0,0.0,0.0);
    glClear(GL_COLOR_BUFFER_BIT);


    // below's the sample of glict render
    // you can practically copypaste for a simple program
    // prepare for render; set up tests
    glEnable(GL_STENCIL_TEST); // so clipping works
    glDisable(GL_DEPTH_TEST); // depth is not important to us

    // make sure we're in modelview matrix mode
    glMatrixMode(GL_MODELVIEW);
    // and reset the modelview matrix
    // that step is not important and in this example just more than unnecessary
    // so you can delete it if you want (just like the above glMatrixMode thing)
    glLoadIdentity();

    // render it up
    desktop.Paint();

    // note: glict pops every matrix it pushes
    // and undoes every transformation it does
    // so dont worry about that

    // now disable glstenciltest at the end
    // we dont put it up because we disabled it here
    // well, we disable it here because we might need it disabled... blah
    glDisable(GL_STENCIL_TEST);

    // make glut render it
    glutSwapBuffers();
}

// glut's way of telling program that window size changed
void reshape(int x, int y) {
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glViewport(0,0,x,y);
    gluOrtho2D(0,x,0,y);

    // we rotate the entire coordinate sys because we use system opposite to GL's
    // (just flipped)
    // we do that for projection matrix so modelview can be reset whenever we want
    glRotatef(180., 1., 0., 0.);
    glTranslatef(0,-y,0.0);

    glMatrixMode(GL_MODELVIEW);


    // set desktop's width and height to 100
    desktop.SetWidth(x);
    desktop.SetHeight(y);

    window.SetWidth(96*4);
    window.SetHeight(96*4);

    window.SetPos(x/2-96*2, y/2-96*2);


    glutPostRedisplay();
}

// glut's way of telling us a mouse-press event happened
void mouse(int button, int shift, int mousex, int mousey) {
    glictPos pos;
    pos.x = mousex; pos.y = mousey;

    if (shift==GLUT_DOWN) desktop.CastEvent(GLICT_MOUSEDOWN, &pos, 0);
    if (shift==GLUT_UP) desktop.CastEvent(GLICT_MOUSEUP, &pos, 0);
    glutPostRedisplay();
}



// this happens when msgbox is dismissed; serves for memory cleanup
void OnDismissSuccess (glictPos* relmousepos, glictContainer* caller) {
    delete caller;
}


// CardOnClick is executed upon click on a card with mouse
// we'll identify which card is used by pointer difference
void CardOnClick(glictPos* relmousepos, glictContainer* callerclass) {
    glictButton *button = dynamic_cast<glictButton*>(callerclass);
    int cardclicked = button - cards;
    //char c[256];
    //sprintf(c, "%d", cardclicked);


    glutSetWindowTitle("GLICTMemory");


    if (flipd[1]!=-1 ) {

        if (!solved[flipd[0]]) {
            char tmp[16];
            sprintf(tmp, "%d", flipd[0]); cards[flipd[0]].SetCaption(tmp); cards[flipd[0]].SetBGColor(0.5, 0.5, 0.5, 1.);
            sprintf(tmp, "%d", flipd[1]); cards[flipd[1]].SetCaption(tmp); cards[flipd[1]].SetBGColor(0.5, 0.5, 0.5, 1.);

        }
        flipd[0]=-1;
        flipd[1]=-1;

    }

    if (solved[cardclicked]) {
       glutSetWindowTitle("GLICTMemory - Card's been solved already.");
       return;
    }

    if (flipd[0]!=-1 && cardclicked==flipd[0]) {
        return;
    }


    button->SetCaption(cardtitles[matrix[cardclicked]]);
    button->SetBGColor(cardcolor[matrix[cardclicked]][0], cardcolor[matrix[cardclicked]][1], cardcolor[matrix[cardclicked]][2], 1.);
    if (flipd[0]==-1) flipd[0]=cardclicked; else {
        flipd[1]=cardclicked;


        totalopens++;
        if (matrix[flipd[0]] == matrix[flipd[1]]) {
            glutSetWindowTitle("GLICTMemory - Correct!");
            solved[flipd[0]] = 1;
            solved[flipd[1]] = 1;
            totalsolved++;

            if (totalsolved == 8) {
                msgSuccess = new glictMessageBox;
                msgSuccess->SetOnDismiss(OnDismissSuccess);
                msgSuccess->SetCaption("Congrats!");
                msgSuccess->SetMessage("You've solved the game!");
                msgSuccess->SetPos(32,32);
                window.AddObject(msgSuccess);

            }
        } else {
            glutSetWindowTitle("GLICTMemory - Wrong!");
        }
    }


    char solveds[256];
    sprintf(solveds, "Solved: %d\nOpens: %d", totalsolved, totalopens);
    pnlSolveds.SetCaption(solveds);
    glutPostRedisplay();
}


// game related, board generation function
void generate() {
     bool gen;
     int x,y;
     srand(time(NULL));
     for (int i=0;i<16;i++) taken[i]=false;
     for (int i=0;i<8;i++) {
         gen=false;
         while (!gen) {
               x = (int)(rand() / (float)RAND_MAX * (float)16);
               if (!taken[x]) {taken[x]=true;  matrix[x]=i; gen=true;}
         }

         gen=false;
         while (!gen) {
               x = (int)(rand() / (float)RAND_MAX * (float)16);
               if (!taken[x]) {taken[x]=true;  matrix[x]=i; gen=true;}

         }
     }
     flipd[0]=-1;
     flipd[1]=-1;
}
// MainWidgets initializes the widgets on main screen
void MainWidgets() {
    generate();
    window.SetCaption("Board");
    desktop.AddObject(&window);

    for (int i=0; i<16;i++) {

        cards[i].SetPos((i % 4) * 96, (i / 4) * 96);
        cards[i].SetWidth(96);
        cards[i].SetHeight(96);
        cards[i].SetBGColor(0.5, 0.5, 0.5, 1.);

        char tmp[3];
        sprintf(tmp,"%d", i);
        cards[i].SetCaption(tmp);
        cards[i].SetOnClick(CardOnClick);

        window.AddObject(&cards[i]);
    }

    desktop.AddObject(&pnlSolveds);
    pnlSolveds.SetCaption("Solved: 0\nOpens: 0");
    pnlSolveds.SetBGColor(0.,0.,0.,1.);
    pnlSolveds.SetWidth(100.);
}



// the main function initializes everything
int main(int argc, char** argv) {

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH | GLUT_STENCIL);
    glutInitWindowSize (640, 480);
    glutInitWindowPosition (0, 0);

    windowhandle = glutCreateWindow ("GLICTMemory");

    MainWidgets();




    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutMouseFunc(mouse);


    glutSetWindow(windowhandle);
    glutShowWindow();


    glutMainLoop();
	return 0;
}
