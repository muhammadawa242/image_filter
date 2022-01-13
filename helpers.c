#include "helpers.h"
#include <math.h>

// Prototype
BYTE average(BYTE sum, int denominator);


// Convert image to grayscale
void grayscale(int height, int width, RGBTRIPLE image[height][width])
{
    // A function iterating through 2D array and taking average of each pixel
    int i;
    int j;
    for (i = 0; i < height; i++)
    {
        for (j = 0; j < width; j++)
        {
            BYTE blue = image[i][j].rgbtBlue;
            BYTE green = image[i][j].rgbtGreen;
            BYTE red = image[i][j].rgbtRed;
            
            // Average of this pixel's fields
            BYTE avg = average((red + blue + green),3);

            image[i][j].rgbtBlue = avg;
            image[i][j].rgbtGreen = avg;
            image[i][j].rgbtRed = avg;
        }
    }
}

// Reflect image horizontally
void reflect(int height, int width, RGBTRIPLE image[height][width])
{
    int i, j;
    RGBTRIPLE tmp;
    int start = 0;
    int end = width;
    for (i = 0; i < height; i++)
    {
        for (j = 0; j < width / 2; j++)
        {
            // Swap the first and last pixel of the row
            tmp = image[i][start];
            image[i][start] = image[i][end];
            image[i][end] = tmp;

            // The next indexes to be swaped
            start++;
            end--;
        }
    }
}

// Blur image
void blur(int height, int width, RGBTRIPLE image[height][width])
{
    int i, j, h, k;
    for (i = 0; i < height; i++)
    {
        for (j = 0; j < width; j++)
        {
            
            RGBTRIPLE sum;
            sum.rgbtBlue = 0;
            int Bdenominator = 0;
            
            sum.rgbtGreen = 0;
            int Gdenominator = 0;

            sum.rgbtRed = 0;
            int Rdenominator = 0;

            // Iterate a total of 9 times to touch each pixel of the 3 by 3 grid
            for (h = i - 1; h < i + 2; h++)
            {
                for (k = j -1; k < j + 2; k++)
                {
                    if (h < height && h > -1 && k < width && k > -1)
                    {
                        sum.rgbtBlue += image[h][k].rgbtBlue;
                        sum.rgbtGreen += image[h][k].rgbtGreen;
                        sum.rgbtRed += image[h][k].rgbtRed;
                        if (sum.rgbtGreen != 0)
                            Gdenominator++;
                        if (sum.rgbtBlue != 0)
                            Bdenominator++;
                        if (sum.rgbtRed != 0)
                            Rdenominator++;
                    }
                }
            }

            // Store average values into the pixel's fields
            image[i][j].rgbtBlue = average(sum.rgbtBlue, Bdenominator);
            image[i][j].rgbtGreen = average(sum.rgbtGreen, Gdenominator);
            image[i][j].rgbtRed = average(sum.rgbtRed, Rdenominator);
        }
    }
}

// Detect edges
void edges(int height, int width, RGBTRIPLE image[height][width])
{
    int i, j, h, k, number;
    RGBTRIPLE Gx;
    Gx.rgbtRed = 0;
    Gx.rgbtBlue = 0;
    Gx.rgbtGreen = 0;
    RGBTRIPLE Gy;
    Gy.rgbtRed = 0;
    Gy.rgbtBlue = 0;
    Gy.rgbtGreen = 0;

    for (i = 0; i < height; i++)
    {
        for (j = 0; j < width; j++)
        {
            
            // Iterate a total of 9 times to touch each pixel of the 3 by 3 grid
            int x_matrix[3][3] = {-1, 0, 1, -2, 0, 2, -1, 0, 1};
            int y_matrix[3][3] = {-1, -2, -1, 0, 0, 0, 1, 2, 1};
            int m = 0;
            int n = 0;
            for (h = i - 1; h < i + 2; h++,m++)
            {
                for (k = j -1; k < j + 2; k++,n++)
                {
                    if (h < height && h >= 0 && k < width && k >= 0)
                    {
                        Gx.rgbtRed += x_matrix[m][n] * image[h][k].rgbtRed;
                        Gy.rgbtRed += y_matrix[m][n] * image[h][k].rgbtRed;
                        Gx.rgbtBlue += x_matrix[m][n] * image[h][k].rgbtBlue;
                        Gy.rgbtBlue += y_matrix[m][n] * image[h][k].rgbtBlue;
                        Gx.rgbtGreen += x_matrix[m][n] * image[h][k].rgbtGreen;
                        Gy.rgbtGreen += y_matrix[m][n] * image[h][k].rgbtGreen;
                    }
                }
            }
            // Taking Square of Gx and Gy
            Gx.rgbtRed *= Gx.rgbtRed;
            Gy.rgbtRed *= Gy.rgbtRed;
            Gx.rgbtBlue *= Gx.rgbtBlue;
            Gy.rgbtBlue *= Gy.rgbtBlue;
            Gx.rgbtGreen *= Gx.rgbtGreen;
            Gy.rgbtGreen *= Gy.rgbtGreen;
            
            // Change the fields of this image[i][j] pixel
            image[i][j].rgbtBlue = sqrt(Gx.rgbtBlue + Gy.rgbtBlue);
            image[i][j].rgbtGreen = sqrt(Gx.rgbtGreen + Gy.rgbtGreen);
            image[i][j].rgbtRed = sqrt(Gx.rgbtRed + Gy.rgbtRed);
        }
    }
}

// Average function
BYTE average(BYTE sum, int denominator)
{
    return round(sum/denominator);
}