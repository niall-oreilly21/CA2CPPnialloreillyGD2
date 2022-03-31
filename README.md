# CA2CPPnialloreillyGD2

## Aim
The aim of this project is to develop an image manipulation application in C++. This application
should allow a user to load a Portable Pixmap Format ( .ppm ) image file and apply a range of
manipulations to the image. A feature to save the manipulated image should also be incorporated.
To assist you in the development of the application, a Basic GUI window has been created and
will be made available to you on GitHub. You may use this application to see real-time changes
to the image based on your own image manipulation code. This is a limited GUI with very basic
functionality that will allow you to see the results of applying your manipulations to the image.

## Portable Pixmap File format (.ppm)
The ppm file format is a very basic image format that can be used to store image data. The format
contains only enough information to allow the image to be rendered. The format of the PPM file is as
follows:
1. A file type identifier for identifying the file type. This is the two characters "P6".
2. Whitespace (blanks, TABs, CRs, LFs), usually a blank line.
3. A width, formatted as ASCII characters in decimal. (e.g. 640)
4. Whitespace. (Usually a space)
5. A height, again in ASCII decimal. (e.g. 480)
6. Whitespace. Usually a newline character.
7. The maximum color value (Maxval), again in ASCII decimal. Must be less than
65536 and more than zero.
8. A single whitespace character (usually a newline).
9. A raster (table) of Height number of rows, in order from top to bottom.
Each row consists of Width number of pixels, in order from left to right.
Each pixel is a triplet of red, green, and blue samples, in that order.
Each sample is represented in pure binary by either 1 or 2 bytes. If the Maxval is less
than 256, it is 1 byte. Otherwise, it is 2 bytes. The most significant byte is first.
In this project, only one byte per colour is used.
PPM files can be generated using Adobe Photoshop or the Gimp App.

## System Requirements
**_In your application you will be required to implement the following features:_**
1. **Load and display image:** Should allow a ppm file to be loaded into the application, you may
   assume that the “File Type Identifier” will be “P6” and each channel is 1 byte in size. If the
   file does not match this the load function should return false and display an informative
   message.
 
2. **Greyscale:** Should change the image to greyscale 

3. **Save current Image to file:** After an image has been modified an option should be available to save the
   image as a ppm file using the format described above. 

4. 
*  **Flip Horizontal:** Should flip the image along the centre Y-axis
*  **Flip Vertical:**   Should flip the image along the centre X-axis   
   
5. 
*  **Filter Red:**      Should show only the red channel of the image.
*  **Filter Green:**    Should show only the green channel of the image.
*  **Filter Blue:**     Should show only the blue channel of the image.

6. **Additional Function 1:**  
7. **Additional Function 2:**  
8. **Additional Function 3:**  
 
10. **Gamma Encoding:** Conversion from float to int for colour channels. 
11. **Other Advanced feature:**  

