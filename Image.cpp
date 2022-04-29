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
#include <cmath>
#define MAX_RGB_VALUE 255
#define KERNEL_SIZE 9

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

        this->pixels = new Rgb[w * h];
        ifs.ignore(256, '\n');

        for (int i = 0; i < (w * h); i++)
        {
            float r, g, b;
            ifs >> r >> g >> b;

            this->pixels[i].r = r * MAX_RGB_VALUE;
            this->pixels[i].g = g * MAX_RGB_VALUE;
            this->pixels[i].b = b * MAX_RGB_VALUE;
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

void Image :: boxBlurFilter()
{
    //ref https://stackoverflow.com/questions/30427918/apply-blur-on-bmp-pixel-rgb-array
    //ref https://www.scratchapixel.com/lessons/digital-imaging/simple-image-manipulations/bookeh-effect
    //ref https://localcoder.org/bluring-an-image-in-c-c

    double blurValue = 0.111;

    https://www.bitcoininsider.org/article/70964/computer-vision-busy-developers-convolutions
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

void Image :: invertFilter()
{
    //ref https://dyclassroom.com/image-processing-project/how-to-convert-a-color-image-into-negative

    for (int i = 0; i < getImageSize(); i++)
    {
        pixels[i].g = MAX_RGB_VALUE - pixels[i].g;
        pixels[i].b = MAX_RGB_VALUE - pixels[i].b;
        pixels[i].r = MAX_RGB_VALUE - pixels[i].r;
    }
}

void Image :: sepiaFilter()
{
    //ref https://github.com/abhijitnathwani/image-processing/blob/master/image_colortosepia.c

    for (int i = 0; i < getImageSize(); i++)
    {
            int currentPixelR = pixels[i].r;
            int currentPixelG = pixels[i].g;
            int currentPixelB = pixels[i].b;

            int sepiaR = (currentPixelR * 0.393) + (currentPixelG * 0.769) + (currentPixelB * 0.189);
            int sepiaG = (currentPixelR * 0.349) + (currentPixelG * 0.686) + (currentPixelB * 0.168);
            int sepiaB = (currentPixelR * 0.272) + (currentPixelG * 0.534) + (currentPixelB * 0.131);

            if(sepiaR > MAX_RGB_VALUE)
            {
                sepiaR = MAX_RGB_VALUE;
            }

            if(sepiaG > MAX_RGB_VALUE)
            {
                sepiaG = MAX_RGB_VALUE;
            }

            if(sepiaB > MAX_RGB_VALUE)
            {
                sepiaB = MAX_RGB_VALUE;
            }

            pixels[i].r = sepiaR;
            pixels[i].g = sepiaG;
            pixels[i].b = sepiaB;
    }
}

void Image::additionalFunction1()
{
    boxBlurFilter();
}

void Image::additionalFunction2()
{
    invertFilter();
}

void Image::additionalFunction3()
{
    sepiaFilter();
}

void Image::gaussianBlur()
{
    //https://quincemedia.com/2017/10/04/9-types-of-blur-effects-in-after-effects/
    //https://medium.com/@rlahiri/gaussian-blurring-and-its-importance-in-image-processing-4be8915b85ec
    /*
     * The Gaussian Blur is similar to the box blur above, except it uses a convolution matrix.
     *
     */
    int blurConvolutionMatrix[9] = {1, 2, 1,
                                    2, 4, 2,
                                    1, 2, 1};

    int convolutionAmount = 0;
    for (int j = 0; j < KERNEL_SIZE; j++)
    {
        convolutionAmount += blurConvolutionMatrix[j];
    }

    Rgb* convolutionPixels = nullptr;

    for (int i = w + 1; i < getImageSize() - w; i++)
    {


        convolutionPixels = getConvolutionPixels(i);

        int convolvedPixelR = 0;
        int convolvedPixelG = 0;
        int convolvedPixelB = 0;

        // apply the convolution for each of red, green and blue
        for (int j = 0; j < KERNEL_SIZE; j++)
        {
            convolvedPixelR += convolutionPixels[j].r * blurConvolutionMatrix[j];
            convolvedPixelG += convolutionPixels[j].g * blurConvolutionMatrix[j];
            convolvedPixelB += convolutionPixels[j].b * blurConvolutionMatrix[j];
        }

        pixels[i].r = convolvedPixelR / convolutionAmount;
        pixels[i].g = convolvedPixelG / convolutionAmount;
        pixels[i].b = convolvedPixelB / convolutionAmount;

    }

    delete[] convolutionPixels;
}

void Image::embossFilter()
{
    //ref https://en.wikipedia.org/wiki/Kernel_(image_processing)#:~:text=In%20image%20processing%2C%20a%20kernel,the%20kernel%20and%20an%20image.
    //ref https://developer.apple.com/documentation/accelerate/blurring_an_image

    /*
     * The following array of 9 numbers is a convolution matrix or mask used to emboss a image.
     * This is accomplished by doing a convolution(transforming an image) between a kernel and an image.
     */
    int embossConvolutionMatrix[9] = {0,0,0,
                                      0,2,-1,
                                      0,-1,0};


    Rgb* convolutionPixels = nullptr;

    for(int i = w + 1; i < getImageSize() - w; i++)
    {
        convolutionPixels = getConvolutionPixels(i);

        // do the convolution
        int convolvedPixelR = 0;
        int convolvedPixelG = 0;
        int convolvedPixelB = 0;

        for (int j = 0; j < KERNEL_SIZE; j++)
        {
            convolvedPixelR += (convolutionPixels[j].r * embossConvolutionMatrix[j]);
            convolvedPixelG += (convolutionPixels[j].g * embossConvolutionMatrix[j]);
            convolvedPixelB += (convolutionPixels[j].b * embossConvolutionMatrix[j]);
        }

        pixels[i].r = convolvedPixelR + 127;
        pixels[i].g = convolvedPixelG + 127;
        pixels[i].b = convolvedPixelB + 127;

    }
    delete[] convolutionPixels;;
}

Rgb* Image :: getConvolutionPixels(const int& currentPixel)
{
    Rgb* convolutionPixels = new Rgb[KERNEL_SIZE];

    //Gets the current pixel and its eight surrounding pixel values from the original image
    convolutionPixels[0] = Rgb(pixels[currentPixel - w - 1].r, pixels[currentPixel - w - 1].g, pixels[currentPixel - w - 1].b);
    convolutionPixels[1] = Rgb(pixels[currentPixel - w].r, pixels[currentPixel - w].g, pixels[currentPixel - w].b);
    convolutionPixels[2] = Rgb(pixels[currentPixel - w + 1].r, pixels[currentPixel - w + 1].g, pixels[currentPixel - w + 1].b);
    convolutionPixels[3] = Rgb(pixels[currentPixel - 1].r, pixels[currentPixel - 1].g, pixels[currentPixel - 1].b);
    convolutionPixels[4] = Rgb(pixels[currentPixel].r, pixels[currentPixel].g, pixels[currentPixel].b);
    convolutionPixels[5] = Rgb(pixels[currentPixel + 1].r, pixels[currentPixel + 1].g, pixels[currentPixel + 1].b);
    convolutionPixels[6] = Rgb(pixels[currentPixel + w - 1].r, pixels[currentPixel + w - 1].g, pixels[currentPixel + w - 1].b);
    convolutionPixels[7] = Rgb(pixels[currentPixel + w].r, pixels[currentPixel + w].g, pixels[currentPixel + w].b);
    convolutionPixels[8] = Rgb(pixels[currentPixel + w + 1].r, pixels[currentPixel + w + 1].g, pixels[currentPixel + w + 1].b);

    return convolutionPixels;
}

void Image::gammaEncoding()
{
    //ref https://www.cambridgeincolour.com/tutorials/gamma-correction.htm

    /*
     * Use for RAW files in particular which use linear gamma.
     * The Standard encoding gamma is 1/2.2
     */
    for(int i = 0; i < getImageSize(); i++)
    {
        float r,g,b;

        r = static_cast<float>(pixels[i].r) / MAX_RGB_VALUE;
        g = static_cast<float>(pixels[i].g) / MAX_RGB_VALUE;
        b = static_cast<float>(pixels[i].b) / MAX_RGB_VALUE;

        unsigned int newR = MAX_RGB_VALUE * pow(r, 1.0 / 2.2);
        pixels[i].r = newR;

        unsigned int newG = MAX_RGB_VALUE * pow(g, 1.0 / 2.2);
        pixels[i].g = newG;

        unsigned int newB = MAX_RGB_VALUE * pow(b, 1.0 / 2.2);
        pixels[i].b = newB;
    }
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


