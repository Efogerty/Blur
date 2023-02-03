#include <iostream>
#include <cstdio>
#include <cmath>
#include <vector>
#include <map>
#include <queue>
#include<unordered_map>
#include "graphics.h"
using namespace std;

//Alows us to assign values to each pixel of the ppm file
struct Pixel {
  unsigned char r, g, b;
};

Pixel *image;
int width, height;

Pixel white = { 255, 255, 255 };
Pixel black = { 0, 0, 0 };

Pixel &get_pixel(int x, int y)
{
  return image[y*width + x];
}

//opens a file of a given name (paw.ppm later)
void read_image(const char *filename)
{
  FILE *fp = fopen (filename, "r");
  int dummy = fscanf (fp, "P6\n%d %d\n255%*c", &width, &height);
  image = new Pixel[width * height];
  dummy = fread (image, width*height, sizeof(Pixel), fp);
  fclose (fp);
}

//creates a new file of a given name with the size of our earlier struct
void write_image(const char *filename)
{
  FILE *fp = fopen (filename, "w");
  fprintf (fp, "P6\n%d %d\n255\n", width, height);
  int dummy = fwrite (image, width*height, sizeof(Pixel), fp);
  fclose (fp);
}

//allow a map of nodes containing pixels
bool operator== (Pixel &a, Pixel &b) {
      return a.r == b.r && a.g == b.g && a.b == b.b;
}
typedef pair<int,int> Node;
Node megaNode = make_pair(-1,-1);
vector<pair<int,int> >allPixels;
map<Node,vector<Node> >neighbors;
map<Node,int> distances;
int unused = width*height;

//adds every pixel into a 2d array
void allpixels()
{
  for(int i=0;i<width;i++)
  {
    for(int j=0;j<height;j++)
    {
      allPixels.push_back(make_pair(i,j));
    }
  }
}

//calculates every pixel's four neighbors and puts it to a list of all neighbors of all pixels
void findNeighbors()
{
  Node temp;
  for(int i=0;i<width;i++)
  {
    for(int j=0;j<height;j++)
    {
      temp = make_pair(i,j);
      if(i!=0)
      {
        Node add = make_pair(i-1,j);
        neighbors[temp].push_back(add);
      }
      if(j!=0)
      {
        Node add = make_pair(i,j-1);
        neighbors[temp].push_back(add);
      }
      if(i!=width)
      {
        Node add = make_pair(i+1,j);
        neighbors[temp].push_back(add);
      }
      if(j!=height)
      {
        Node add = make_pair(i,j+1);
        neighbors[temp].push_back(add);
      }
      if(get_pixel(temp.first,temp.second)==white)
      {
        neighbors[megaNode].push_back(temp);
      }
    }
  }
}

//a source node is neighbors to all pixels. This visits all neighbors to calculate what pixels should become
void bfs(Node source)
{
  for(Node &a : allPixels) distances[a] = unused;
    distances[source]=0;
    queue<Node>to_visit;
    to_visit.push(source);

    //once every pixel has been given base distance values, it traverses the all neighbors array to update distances and assign color values
    while(!to_visit.empty())
    {
      Node temp  = to_visit.front();
      to_visit.pop();
      for(Node b : neighbors[temp])
      if (distances[b] == unused)
      {
        distances[b] = 1+distances[temp];
        to_visit.push(b);
        get_pixel(b.first,b.second).r = 255*pow(0.9,(distances[b]-1));
        get_pixel(b.first,b.second).g = 255*pow(0.9,(distances[b]-1));
        get_pixel(b.first,b.second).b = 255*pow(0.9,(distances[b]-1));
      }
    }
  }

//creates pixels, gives them neighbors, breadth first searches
void calculate_blur(void)
{
  allpixels();
  findNeighbors();
  bfs(megaNode);
}

int main(void)
{
  read_image("paw.ppm");
  calculate_blur();
  write_image("paw2.ppm");
}
