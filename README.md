# CPaint
CPaint is a simple drawing tool that was made as a first project for Introduction to Computer Graphics. With the application we can draw 6 types of primitives (Lines, Rectangles, Circles, Ellipses, Triangles, and Bezier Curves) and we can modify the figures's border and filler colors, overall shape and position, after a drawing session we can save the scene to load it at a later time and continue with our work. To do all this we can use the user interface or the keyboard, a list with all the keyboard shortcuts can be displayed by pressing the 'H' key or by pressing the 'Help->Shortcuts' tab on the UI.

![CPaint with UI](https://ibb.co/d0vrDfy)

For the rendering we have two modes (by harware and by software), the hardware mode uses the old OpenGL primitives while the software mode exploits the CPU and renders the figures pixel by pixel. To implement this last mode we use the Mid-Point Algorithm for the Line, the Circle and the Ellipse shapes, and the Casteljau's algorithm for the Bezier Curve.

The source code was written with C++, the user interface was integrated with ImGui, and the management of file loading and saving by using window dialogs was done with TinyDialogFile.

## Use and Compiling
This project was developed using Visual Studio 2020, to build it yourself open the .sln file and compile it using the x84 configuration. If you just want to use the applicatton, just open the .exe file included in the main directory.

## Current Issues

- Given that the keyboards keys from 1 to 6 are assigned to change the shape of the figure to draw, it's not possible to enter a color value by hand (RGB, HSV or Hex) in the color editors.

- In the bezier curve after imputting more than 15 control points the curve starts to look jagged.
