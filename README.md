Colorful Game Of Life
=====================

This project is one of my first attempts at OpenGL and GLSL.

Idea
----

The idea was to create a colorful version of Conway's Game of Life.

Upon launching the application, user must first prepare the board.
To do this, select a layer by pressing _1_-_4_ (corressponding to _red_, _green_, _blue_, and _aplha_ channels).
Then, with a mouse, colour the desired cells (pixels).

![Colouring the board](https://raw.github.com/rubikonx9/ColorfulGameOfLife/master/img/colour.png)

When done, press _space_ to start the game, and observe as the life evolves.
All layers evolve independently from each other.

![Live](https://raw.github.com/rubikonx9/ColorfulGameOfLife/master/img/live.png)

How it's done
-------------

The main purpose of this project was to learn some basic GLSL.

Firstly, a texture buffer object is created. It's used to transfer the data from main memory to the GPU.
It's actually used as the texture, and each change (player's colouring the board or simulation step) is first reflected in main memory, then it's transferred to the GPU, and finally it's drawn using the shaders.

All in all, this doesn't seem to be a good method in terms of performance and usability.

Dependencies
------------

* boost
* GL3W
* GLFW 2

Before issuing _make_, make sure the paths to libraries in _Makefile_ are correct.

Platforms
---------

As of now, this project was only tested in MinGW.

For other systems, probably small changes would be necessary, especially concerning the build process.

Support
-------

This project is not developed any more.

Licensing
---------

GNU GPL v2.0.
