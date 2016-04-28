# GLICT

This is an old UI library I wrote for use in my game projects. At the
time, I was unhappy with all the game-oriented UI libraries that I found.
I would not encourage its use today. The largest project I used it in
was *Yet Another Tibia Client*, which itself went out of development in
2011.

More documentation can be found in [glict/README](glict/README), inside the
source code, and you can build documentation using Doxygen.

The easiest way to build and install GLICT is to use the provided
autotools-based build. So:

    ./autogen.sh
    ./configure
    make
    sudo -E make install

I'm also toying with providing <bazel.io> `BUILD` files.

- ivucica, 2016
