# Blur
Utilizes breadth first search in order to blur a .ppm image

This code reads an image and maps it into a map of vectors which themselves are pairs of pixels.
A node (which we will call megaNode) exists outside of the vector who is then assigned ALL pixels as neighbors. 
First the pixels are paired into a vector which will be recalled to assign a pixel neighbors.

We do a breadth first search on megaNode which looks at all pixel neighbors, and assigns all pixels "distances"
This allows us to reassign color values based on how close a pixel is to a neighbor with color.
