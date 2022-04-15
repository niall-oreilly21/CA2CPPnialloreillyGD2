//
// Created by floodd on 23/03/2022.
//
#include <iostream>
#include <fstream>
#include <iomanip>
#include <cstring>
#include "Image.h"
#include <bitset>
#include <fci.h>

using namespace std;

bool Image::load(string filename)
{
    ifstream ifs;
    ifs.open(filename, std::ios::binary|std::ios::in); // need to spec. binary & input mode for Windows users
    try {
        if (ifs.fail()) { throw("Can't open input file"); }
        string header;
        int w, h, b;
        ifs >> header;
        if (strcmp(header.c_str(), "P6") != 0) throw("Can't read input file");
        ifs >> w >> h >> b;
        this->w = w;
        this->h = h;
        this->pixels = new Rgb[w * h];  // this will throw an exception if bad_alloc
        ifs.ignore(256, '\n'); // skip empty lines if necessary until we get to the binary data

        unsigned char pixel_buffer[3]; // buffer to store one pixel, with three channels red,green and blue

        // read each pixel one by one and convert bytes to floats
        for (int i = 0; i < (w * h); i++)
        {
            ifs.read(reinterpret_cast<char *>(pixel_buffer), 3);
            this->pixels[i].r = pixel_buffer[0]; // / 255.f;
            this->pixels[i].g = pixel_buffer[1]; // / 255.f;
            this->pixels[i].b = pixel_buffer[2]; // / 255.f;
        }

        ifs.close();
    }
    catch (const char *err) {
        fprintf(stderr, "%s\n", err);
        ifs.close();
    }

    return true;
}

bool Image::loadRaw(string filename)
{
    std::ifstream ifs;
    ifs.open(filename, std::ios::binary|std::ios::in);

    try
    {
        if(ifs.fail())
        {
            throw("Can't open input file.");
        }

       ifs >> w >> h;

//        this->w = width;
//        this->h = height;
        this->pixels = new Rgb[w * h];
        ifs.ignore(256, '\n');

        for (int i = 0; i < (w * h); i++)
        {
            float r,g,b;
            ifs >> r >> g >> b;

            this->pixels[i].r = r * 255;
            this->pixels[i].g = g * 255;
            this->pixels[i].b = b * 255;
        }

        ifs.close();
    }

    catch(const char* err)
    {
        fprintf(stderr, "%s\n", err);
        ifs.close();
    }
    return true;
}

bool Image::savePPM(string filename)
{
    if (this->w == 0 || this->h == 0) { fprintf(stderr, "Can't save an empty image\n"); return false; }
    std::ofstream ofs;
    try {
        ofs.open(filename, std::ios::binary|std::ios::out); // need to spec. binary mode for Windows users
        if (ofs.fail()) throw("Can't open output file");
        ofs << "P6\n" << this->w << " " <<  this->h << "\n255\n";

        unsigned char r, g, b;
        // loop over each pixel in the image, clamp and convert to byte format
        for (int i = 0; i <  this->w *  this->h; i++)
        {
            ofs.write(reinterpret_cast<char *>(&this->pixels[i].r),1);    // write 1 byte;
            ofs.write(reinterpret_cast<char *>(&this->pixels[i].g),1);    // write 1 byte;
            ofs.write(reinterpret_cast<char *>(&this->pixels[i].b),1);    // write 1 byte;
        }
        ofs.close();
    }
    catch (const char *err)
    {
        fprintf(stderr, "%s\n", err);
        ofs.close();
    }
    return true;
}


void Image::filterRed()
{
    for (int i = 0; i < getImageSize(); i++)
    {
        pixels[i].g = 0;
        pixels[i].b = 0;
    }

}
void Image::filterGreen()
{
    for (int i = 0; i < getImageSize(); i++)
    {
        pixels[i].r = 0;
        pixels[i].b = 0;
    }
}
void Image::filterBlue()
{
    for (int i = 0; i < getImageSize(); i++)
    {
        pixels[i].r = 0;
        pixels[i].g = 0;
    }
}
void Image::greyScale()
{
    for (int i = 0; i < getImageSize(); i++)
    {
        float averageValue = (pixels[i].g + pixels[i].b + pixels[i].b) /3;

        pixels[i].g = averageValue;
        pixels[i].b = averageValue;
        pixels[i].r = averageValue;
    }
}

void Image::flipVertically()
{
    for(int row = 0; row < h / 2; row++)
    {
        int swapRow = (h - 1) - row;

        for(int column = 0; column < w; column++)
        {
            Rgb temp = pixels[(row * w) + column];
            pixels[(row * w) + column] = pixels[(swapRow * w) + column];
            pixels[(swapRow * w) + column] = temp;
        }
    }
}

void Image::flipHorizontal()
{
    for(int row = 0; row < h; row++)
    {
        for(int column = 0; column < w / 2; column++)
        {
            int swapColumn = (w - 1) - column;

            Rgb temp = pixels[(row * w) + column];
            pixels[(row * w) + column] = pixels[(row * w) + swapColumn];
            pixels[(row * w) + swapColumn] = temp;
        }
    }
}

void Image :: blurImage()
{
    //ref https://stackoverflow.com/questions/30427918/apply-blur-on-bmp-pixel-rgb-array
    //ref https://www.scratchapixel.com/lessons/digital-imaging/simple-image-manipulations/bookeh-effect
    //ref https://localcoder.org/bluring-an-image-in-c-c

    double blurValue = 0.11;

    for(int row = 1; row < h - 1; row++)
    {
        for(int column = 1; column < w - 1; column++)
        {
            int averageValueR = 0;
            int averageValueG = 0;
            int averageValueB = 0;

            for(int matrixRow = row - 1; matrixRow <= row + 1; matrixRow++)
            {
                for (int matrixColumn = column - 1; matrixColumn <= column + 1; matrixColumn++)
                {
                    averageValueR = averageValueR + (blurValue * pixels[(matrixRow * w) + matrixColumn].r);
                    averageValueG = averageValueG + (blurValue * pixels[(matrixRow * w) + matrixColumn].g);
                    averageValueB = averageValueB + (blurValue * pixels[(matrixRow * w) + matrixColumn].b);

                }
            }

            pixels[(row * w) + column].r = averageValueR;
            pixels[(row * w) + column].g = averageValueG;
            pixels[(row * w) + column].b = averageValueB;
        }
    }
}

void Image::AdditionalFunction1()
{
    blurImage();
}

void Image::AdditionalFunction2()
{

}

void Image::AdditionalFunction3()
{
}

/* Functions used by the GUI - DO NOT MODIFY */
int Image::getWidth()
{
    return w;
}

int Image::getHeight()
{
    return h;
}

int Image:: getImageSize()
{
    return w * h;
}

Rgb* Image::getImage()
{
    return pixels;
}

