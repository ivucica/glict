/*! \mainpage GLICT Documentation
(c) 2006 OBJECT Networks

\author Ivan Vucica


\if false
This file is not actually meant to be read stand-alone, but to be compiled
into Doxygen documentation. I will however try to keep it readable both ways.
(If you try hard you may read something!)
\endif


Visit www.objectnetworks.net for more information on our projects.

\image html demoshot.jpg "GLICT in action; the UI in background is normal, the  one in front demonstrates transformations" width=50

\code
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
\endcode

\section one Introduction

GLICT stands for Graphics Library Interface Creation Toolkit. It is designed
as a drop-in library for simple building into existing applications, as well
as easy programming of additional controls into it. Every control can contain
subcontrols. We have decided to release this library under GNU Library General
Public License - the LGPL. This has been done for a simple reason - to allow
introduction of it into different closed-source products, while keeping it open
source. We, of course, encourage you to develop more free software, according
to four basic liberties as defined by Free Software Foundation.

This library provides a very simple C++ interface which uses few very simple
functions to set up the interface and to manage it.

\section two Compilation of library binary

GLICT is actively developed under Win32, but occasionally test builds are
done under GNU/Linux. 
GNU Makefile is directly in the GLICT subfolder, and is not strictly 
maintained. It produces a library "libglict.a". There's no automatic
installation.
For building under Windows, you can use Code::Blocks project file that's
included; for building library only choose the "library" target. 

To build the documentation, use the Doxyfile that's included.

\section three Usage

All the user interface controls are C++ classes derived from glictContainer
class. This class contains only basic functionalities and does not render
anything. It handles object management, clipping, and drawing subobjects. It
is intended to serve only as a desktop class.

Examples for development can be found on http://sf.net/projects/glict/ when
you click on big, green "Download" button.

We'd like to point out that this documentation file won't be maintained
much, and that we use in-code documentation. Doxygen is used to generate
documentation files, and please refer to them. (This file, readme.txt, is also
formatted for Doxygen and is set to be main page of Doxygen's documentation.)

Before using the widgets, you should set up at least a GLICT font named 
"system" (case sensitive). It is the font used through GLICT widget rendering.

Take a look at snippets.txt which contains some practical code for copypasting
into your projects (not necessarily only the first one).

\section four Development

This library is still deep in development. We encourage capable people to join
the project and assist in making this a library that's easy for implementation
everywhere.

\section five Todo

For detailed todo, please take a look at the Doxygen-generated documentation

\section six Contact

To contact me please send an e-mail to 
\image html ivucica.gif "\@gm41l.kom"
Of course, replace kom with com, and put correct name of Google's mailing service on appropriate place. I feel that privacy is important, yours just like mine.


*/