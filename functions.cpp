#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#include <cmath>
#include "functions.h"

using namespace std;

// TODO Write this function
int energy(const Pixel *const*image, int col, int row, int width, int height)
{
  int Rx;
  int Gx;
  int Bx;
  int Ry;
  int Gy;
  int By;
  
 
  if (height == 1 || width == 1)
  {
    return 0;
  }

  if (width == 0 || height == 0)
  {
    throw std::invalid_argument("height or width cannot be 0");
  }


  if ((col == 0 || col == width - 1) && (row == 0 || row == height - 1)) // if corner pixel
  {
    Rx = image[abs(col-1)][row].r - image[width - (col + 1)][row].r;
    Gx = image[abs(col-1)][row].g - image[width - (col + 1)][row].g;
    Bx = image[abs(col-1)][row].b - image[width - (col + 1)][row].b;
    Ry = image[col][abs(row-1)].r - image[col][height - (row + 1)].r;
    Gy = image[col][abs(row-1)].g - image[col][height - (row + 1)].g;
    By = image[col][abs(row-1)].b - image[col][height - (row + 1)].b;
  }
  else if ((col == 0 || col == width - 1) && (row != 0 || row != height - 1)) // if not corner pixel but still border column pixel
  {
    Rx = image[abs(col-1)][row].r - image[width - (col + 1)][row].r;
    Gx = image[abs(col-1)][row].g - image[width - (col + 1)][row].g;
    Bx = image[abs(col-1)][row].b - image[width - (col + 1)][row].b;
    Ry = image[col][row-1].r - image[col][row + 1].r;
    Gy = image[col][row-1].g - image[col][row + 1].g;
    By = image[col][row-1].b - image[col][row + 1].b;
  }
  else if ((col != 0 || col != width -1) && (row == 0 || row == height -1 )) // if not corner pixel but still border row pixel
  {
    Rx = image[col-1][row].r - image[col + 1][row].r;
    Gx = image[col-1][row].g - image[col + 1][row].g;
    Bx = image[col-1][row].b - image[col + 1][row].b;
    Ry = image[col][abs(row-1)].r - image[col][height - (row + 1)].r;
    Gy = image[col][abs(row-1)].g - image[col][height - (row + 1)].g;
    By = image[col][abs(row-1)].b - image[col][height - (row + 1)].b;
  }
  else // for any non-border pixel
  {
    Rx = image[col-1][row].r - image[col + 1][row].r;
    Gx = image[col-1][row].g - image[col + 1][row].g;
    Bx = image[col-1][row].b - image[col + 1][row].b;
    Ry = image[col][row-1].r - image[col][row + 1].r;
    Gy = image[col][row-1].g - image[col][row + 1].g;
    By = image[col][row-1].b - image[col][row + 1].b;
  }
  return (Rx*Rx) + (Bx * Bx) + (Gx * Gx) + (Ry*Ry) + (Gy * Gy) + (By * By); 
}
// TODO Write this function
int getVerticalSeam(const Pixel *const*image, int start_col, int width, int height, int* seam)
{
  int energyLeft = 0;
  int energyMiddle = 0;
  int energyRight = 0;
  int seamEnergy = 0;

  seam[0] = start_col;

  for (int i = 1; i < height; i++)
  {
    if (start_col-1 < 0)
    {
      energyLeft = 10000000;
    }
    else
    {
      energyLeft = energy(image, start_col-1, i, width, height);
    }
    if (start_col+1 > width-1)
    {
      energyRight = 10000000;
    }
    else
    {
      energyRight = energy(image, start_col+1, i, width, height);
    }
    energyMiddle = energy(image, start_col, i, width, height);
    
    if ((energyLeft < energyMiddle) && (energyLeft < energyRight))
    {
      start_col = start_col - 1;
    }
    else if ((energyMiddle < energyRight) && (energyMiddle < energyLeft))
    {
      start_col = start_col;
    }
    else if ((energyRight < energyLeft) && (energyRight < energyMiddle))
    {
      start_col = start_col + 1;
    }
    else if (energyMiddle == energyLeft)
    {
      start_col = start_col;
    }
    else if (energyMiddle == energyRight)
    {
      start_col = start_col;
    }
    else if (energyLeft == energyRight)
    {
      start_col = start_col + 1;
    }
      
    seam[i] = start_col;
  }
  for (int j = 0; j < height; j++)
  {
    seamEnergy += energy(image,seam[j],j,width,height);
  }
  return seamEnergy;
}

// TODO Write this function
void removeVerticalSeam(Pixel **image, int& width, int height, int *verticalSeam)
{
  for (int i = 0; i < height; ++i)
  {
    for (int j = verticalSeam[i]; j < width - 1; ++j)
    {
      image[j][i] = image[j+1][i];
    }
  }
  width -= 1;
}


int getHorizontalSeam(const Pixel *const*image, int start_row, int width, int height, int* seam)
{
  if (image == nullptr){ ; }
  if (start_row == 0) { ; }
  if (width == 0) { ; }
  if (height == 0) { ; }
  if (seam == nullptr) { ; }
  return 0;
}


void removeHorizontalSeam(Pixel **image, int width, int& height, int *horizontalSeam)
{
  if (image == nullptr) { ; }
  if (width == 0) { ; }
  if (height == 0) { ; }
  if (horizontalSeam == nullptr) { ; }
}

int *findMinVerticalSeam(const Pixel *const*image, int width, int height)
{
  // initialize minSeam and minDistance to seam starting at first col (index 0)
  int *minSeam = new int[height]{0};
  int minDist = getVerticalSeam(image, 0, width, height, minSeam);

  int *candidateSeam = new int[height]{0};
  int candidateDistance = -1; // invalid distance

  // start at second col (index 1) since we initialized with first col (index 0)
  for (int col = 1; col < width; ++col)
  {
    candidateDistance = getVerticalSeam(image, col, width, height, candidateSeam);

    if (candidateDistance < minDist)
    { // new min
      //  swap min & candidate
      minDist = candidateDistance;
      int* temp = candidateSeam;
      candidateSeam = minSeam;
      minSeam = temp;
    }
  }

  // clean up 
  delete [] candidateSeam;

  return minSeam;
}

int *findMinHorizontalSeam(const Pixel *const*image, int width, int height)
{
  // initialize minSeam and minDistance to seam starting at first row (index 0)
  int *minSeam = new int[width]{0};
  int minDistance = getHorizontalSeam(image, 0, width, height, minSeam);

  int *candidateSeam = new int[width]{0};
  int candidateDistance = -1; // invalid distance

  // start at second row (index 1) since we initialized with first row (index 0)
  for (int row = 1; row < height; ++row)
  {
    candidateDistance = getHorizontalSeam(image, row, width, height, candidateSeam);

    if (candidateDistance < minDistance)
    { // new minimum
      //  swap min and candidate seams
      minDistance = candidateDistance;
      int* temp = candidateSeam;
      candidateSeam = minSeam;
      minSeam = temp;
    }
  }

    // clean up 
  delete [] candidateSeam;

  return minSeam;
}

Pixel **createImage(int width, int height)
{
  cout << "Start createImage... " << endl;

  // Create a one dimensional array on the heap of pointers to Pixels
  //    that has width elements (i.e. the number of columns)
  Pixel **image = new Pixel *[width] {}; // initializes to nullptr

  for (int col = 0; col < width; ++col)
  { // loop through each column
    // assign that column to a one dimensional array on the heap of Pixels
    //  that has height elements (i.e. the number of rows)
    try
    {
      image[col] = new Pixel[height];
    }
    catch (std::bad_alloc &e)
    {
      // clean up already allocated arrays
      for (int i = 0; i < col; ++i)
      {
        delete[] image[i];
      }
      delete[] image;
      // rethrow
      throw e;
    }
  }

  // initialize cells
  // cout << "Initializing cells..." << endl;
  for (int row = 0; row < height; ++row)
  {
    for (int col = 0; col < width; ++col)
    {
      // cout << "(" << col << ", " << row << ")" << endl;
      image[col][row] = {0, 0, 0};
    }
  }
  cout << "End createImage... " << endl;
  return image;
}

void deleteImage(Pixel **image, int width)
{
  cout << "Start deleteImage..." << endl;
  // avoid memory leak by deleting the array
  for (int i = 0; i < width; ++i)
  {
    delete[] image[i];
  }
  delete[] image;
  image = nullptr;
  cout << "End deleteImage..." << endl;
}

bool isValidColor(int colorVal)
{
  if (colorVal < 0 || colorVal > 255)
  {
    return false;
  }
  return true;
}

Pixel ** loadImage(string filename, int &width, int &height)
{
  cout << "Start loadImage..." << endl;
  // remove
  ifstream ifs(filename);
  if (!ifs.is_open())
  {
    throw std::invalid_argument("Failed to open input file (" + filename + ")");
  }

  string type;
  ifs >> type; // should be P3
  if (toupper(type.at(0)) != 'P' || type.at(1) != '3')
  {
    throw std::domain_error("Not PPM type P3 (" + type + ")");
  }
  ifs >> width;
  // cout << "w and h: " << w << " " << h << endl;
  if (ifs.fail())
  {
    throw std::domain_error("Read non-integer value for width");
  }
  if (width <= 0)
  {
    ostringstream oss;
    oss << "Width in file must be greater than 0 (" << width << ")";
    throw std::domain_error(oss.str());
  }

  ifs >> height;
  if (ifs.fail())
  {
    cout << "Read non-integer value for height" << endl;
  }
  if (height <= 0)
  {
    ostringstream oss;
    oss << "Height in file must be greater than 0 (" << height << ")";
    throw std::domain_error(oss.str());
  }

  int colorMax = 0;
  ifs >> colorMax;
  if (ifs.fail())
  {
    throw std::domain_error("Read non-integer value for max color value");
  }
  if (colorMax != 255)
  {
    ostringstream oss;
    oss << "Max color value must be 255 (" << colorMax << ")";
    throw std::domain_error(oss.str());
  }

  // load image throws exceptions but we will let them pass through
  Pixel **image = createImage(width, height);

  for (int row = 0; row < height; ++row)
  {
    for (int col = 0; col < width; ++col)
    {
      // cout << "Pixel(" << col << ", " << row << ")" << endl;
      ifs >> image[col][row].r;
      if (ifs.fail() && !ifs.eof())
      { // bad input that is not caused by being at the end of the file
        throw std::domain_error("Read non-integer value for red");
      }
      if (!isValidColor(image[col][row].r))
      {
        ostringstream oss;
        oss << "Invalid color value for red (" << image[col][row].r << ")";
        throw std::domain_error(oss.str());
      }

      ifs >> image[col][row].g;
      if (ifs.fail() && !ifs.eof())
      { // bad input that is not caused by being at the end of the file
        throw std::domain_error("Read non-integer value for green");
      }
      if (!isValidColor(image[col][row].r))
      {
        ostringstream oss;
        oss << "Invalid color value for green (" << image[col][row].r + ")";
        throw std::domain_error(oss.str());
      }

      ifs >> image[col][row].b;
      if (ifs.fail() && !ifs.eof())
      { // bad input that is not caused by being at the end of the file
        throw std::domain_error("Read non-integer value for blue");
      }
      if (!isValidColor(image[col][row].r))
      {
        ostringstream oss;
        oss << "Invalid color value for blue (" << image[col][row].r + ")";
        throw std::domain_error(oss.str());
      }
    }
  }
  cout << "End loadImage..." << endl;
  return image;
}

void outputImage(string filename, const Pixel *const *image, int width, int height)
{
  cout << "Start outputImage..." << endl;
  // remove code
  // declare/define and open output file stream with filename
  ofstream ofs(filename);
  // ensure file is open
  if (!ofs.is_open())
  {
    throw std::invalid_argument("Error: failed to open output file - " + filename);
  }
  ofs << "P3" << endl;
  ofs << width << " " << height << endl;
  ofs << 255 << endl;
  for (int row = 0; row < height; ++row)
  {
    for (int col = 0; col < width; ++col)
    {
      ofs << image[col][row].r << " ";
      ofs << image[col][row].g << " ";
      ofs << image[col][row].b << " ";
    }
    ofs << endl;
  }
  cout << "End outputImage..." << endl;
}