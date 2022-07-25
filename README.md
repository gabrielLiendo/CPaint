# CPaint
CPaint is a simple drawing tool that was made as a first project for Introduction to Computer Graphics. With the application we can draw 6 types of primitives (Lines, Rectangles, Circles, Ellipses, Triangles, and Bezier Curves) and we can modify the figures's border and filler colors, overall shape and position. Afterwards, we can save the scene to load it at a later time and continue with our work.

For rendering we have two options (by harware and by software), the hardware mode uses the old OpenGL primitives while the software mode exploits the CPU and renders the figures pixel by pixel. To implement this last mode we use the Mid-Point Algorithm for the Line, the Circle and the Ellipse shapes, and the Casteljau's algorithm for the Bezier Curve.

The source code was written with C++, the user interface was integrated with ImGui, and the management of file loading and saving by using window dialogs was done with TinyDialogFile.

## Use Guide
To draw and edit the scene we can use the user interface or the keyboard, a scene with the UI displayed looks like this:

![CPaint with UI](https://user-images.githubusercontent.com/35168412/180677578-2b47ee6f-2709-41a9-8347-06300e4e9ea8.png)

On the other hand, we can collapse the UI with the 'I' key and only use the keyboard, we can find a list with all the keyboard shortcuts by pressing the 'H' key or by pressing the 'Help->Shortcuts' tab on the UI: 

![CPaint without UI](https://user-images.githubusercontent.com/35168412/180834476-28ce214f-a318-4534-ad4b-806051b124dd.png)

If you have trouble drawing a figure you can head over the help marker beside the 'Shape' section in the UI to find how to start and end each shape.

## Use and Compiling
This project was developed using Visual Studio 2022, to build it yourself open the .sln file and compile it using the x84 configuration. If you just want to use the application, just open the .exe file included in the main directory.

## Current Known Issues

- Given that the keyboards keys from 1 to 6 are assigned to change the shape of the figure to draw, it's not possible to enter a color value by hand (RGB, HSV or Hex) in the color editors.

- In the bezier curve after imputting more than 13 control points, the curve starts to look jagged, depending of the positions of said control points.

- It's not possible to resize the bounding boxes of the triangles and the bezier curves.

- If you bring the executable outside of the main directory you can still use the program, but the font of the interface will revert back to the deafult ImGui font. which is rather unpleasant.
