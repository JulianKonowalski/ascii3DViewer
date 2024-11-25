# 3D ASCII Viewer 
Console-based 3D viewer, that uses ASCII characters to render 3D models. 
The aim of this project was to show that something like this is possible rather than making it clean, practical and efficient. Because of this, I took a couple of shortcuts that I will list down below alongside the suggestions on how to fix them in case you'd like to expand on this program.

The project was inspired by the [ASCII donut](https://www.a1k0n.net/2006/09/15/obfuscated-c-donut.html), but after completing it I realised that [autopawn](https://github.com/autopawn) made something [similar](https://github.com/autopawn/3d-ascii-viewer), but in C with additional mesh colouring features.

[Program Screenshot](screenshot.png)

# Usage
Everything is set up in the main function. You can specify the location of the file to be loaded and set the transformation and rotation vectors there. The rendering loop won't change, so it is fairly straightforward.

Resolution can be set at the top of the file with HEIGHT and WIDTH. A 2:1 resolution ratio achieves the best effect, with little to no visible stretching. If you'd like to change the FOV it is also available there and given in **degrees**, not **radians**.

# Code layout
All transforms, rotations and functions related to drawing are contained in the **graphics** namespace. It uses **Vec3**, **Triangle** and **Mesh** classes to achieve its functionality.

**Vec3** class is pretty much the backbone of the whole program, as it describes points, planes and transforms. It has its own additional **vector3** namespace with basic vector operations that were needed for this program to run properly.

**Triangle** class is used to describe 3D model faces. It's not used for anything else.

**Mesh** class represents the actual 3D model and is used as a container for **Triangle** objects representing the models faces.

# Possible Improvements
## Faster model loading
For now, the model is loaded into an std::vector, which as many may know is not a good idea from a performance standpoint. I'd suggest scanning the .obj file twice - once to determine the amount of vertices, normals and faces to preallocate needed memory, and the second time to load the data. There may also be a different solution even more efficient than that.

## Support for quads
While loading the mesh the program expects it to be made **FULLY** out of **triangles**, which is a bit of a pain. I'd suggest adding a triangulation functionality while loading the model, which shouldn't be that difficult.

## Ability to invert normals if needed
Fortunately, all Blender exports can be loaded without even looking at the normals. If loaded in the right order, vertices will always construct a triangle with the normal facing the right way. This may not be the case with all 3D programs or .obj files, so beware.

## Faster line drawing algorithm
The program uses **Bresenham's line algorithm**, which is fast but not the fastest. The effects of this optimisation won't be noticeable with smaller models but with the included 350Z model, which has almost 2 million triangles, a 10-15% improvement is a lot. I'd suggest using EFLA for the best performance.

## Faster triangle rasterisation
Another weak point is triangle rasterisation, more specifically, determining which points are a part of the rasterised triangle and must be shaded. Currently, I'm creating a bounding box around the projected triangle and scanning every line of the box to determine if the point should be shaded or not. It's okay for most triangles, but as the triangle gets wider and shorter (or the resolution is set higher), unnecessary checks increase dramatically.

# Included assets
* Cube exported from Blender
* Suzanne exported from Blender
* [Nissan 350Z](https://www.turbosquid.com/3d-models/nissan-350z-japanese-sports-coupe-with-interior-model-3d-model-2049724) made by [Bbenedict_](https://www.turbosquid.com/Search/Artists/Bbenedict_)
