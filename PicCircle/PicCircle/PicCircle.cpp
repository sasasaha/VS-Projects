#define _SCL_SECURE_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "bitmap_image.hpp"

#define max(a,b)  (((a) > (b)) ? (a) : (b))
#define min(a,b)  (((a) < (b)) ? (a) : (b))

unsigned int Z[1000][1000][300];

#define C 20

double** ArrayCreate(int M, int N)
{
	int i;
	double** ArrayRoot;

	ArrayRoot = (double **)malloc(sizeof(double*)* M);

	for (i = 0; i < M; i++)
		ArrayRoot[i] = (double *)malloc(sizeof(double)* N);

	return ArrayRoot;
}

void ArrayDestroy(double** Array, int M)
{
	int i;

	for (i = 0; i < M; i++){
		free(Array[i]);
	};

	free(Array);
}

void main(void)
{
	double** Y; // for picture
	double** G; // for monochrome picture
	unsigned char r, g, b;
	int  H, W;
	int i, j;
	int x, x0, y1, y2, y0, R; // for m2.bmp R = 94

	bitmap_image image("m2.bmp");

	H = image.height();
	W = image.width();

	bitmap_image image1(W, H);

	Y = ArrayCreate(H, W);
	G = ArrayCreate(H, W);

	/* Start: fill mass with data from picture */
	for (i = 0; i < H; i++)
		for (j = 0; j < W; j++)
		{
			image.get_pixel(j, i, r, g, b);
			Y[i][j] = 0.3 * r + 0.59 * g + 0.11 * b;
		}
	/* End */

	/* Start: make gradient array */
	for (i = 0; i < H; i++)
		for (j = 0; j < W; j++)
		{
			if (i < H - 1 && j < W - 1)
				G[i][j] = ((sqrt(pow(Y[i + 1][j] - Y[i][j], 2) + pow(Y[i][j + 1] - Y[i][j], 2))) > C);
			else
				G[i][j] = 0;
		}
		/* End */

	/* Start: fill cicrles array with 0 */
	for (i = 0; i < H; i++)
		for (j = 0; j < W; j++)
			for (R = 80; R < 105; R++)
				Z[i][j][R] = 0;
	/* End */

	/* Start: fill circles array with data from picture */
	for (y0 = 0; y0 < H; y0++)
		for (x0 = 0; x0 < W; x0++)
			if (G[y0][x0] == 1)
				for (R = 70; R < 110; R++)
					for (x = max(0, x0 - R); x < min(W, x0 + R); x++)
					{
						y1 = y0 + (int)sqrt(R*R - pow(x - x0, 2));

						y2 = y0 - (int)sqrt(R*R - pow(x - x0, 2));

						if (y1 <= H)
							Z[y1][x][R]++;
						if (y2 >= 0)
							Z[y2][x][R]++;
					}
	/* End */

	/* Start: clear all except 'centers' of cicrles*/
	for (i = 0; i < H; i++)
		for (j = 0; j < W; j++)
			for (R = 85; R < 100; R++)
				if (Z[i][j][R] >= 10)
					Z[i][j][R] = 0;
				else
					Z[i][j][R] = 1;
	/* End */

	/* Start: draw picture 'circles' */
	for (i = 0; i < H; i++)
		for (j = 0; j < W; j++)
			for (R = 85; R < 100; R++)
				image1.set_pixel(j, i, Z[i][j][R] * 255, Z[i][j][R] * 255, Z[i][j][R] * 255);
	/* End */

	image1.save_image("output.bmp");
	
	ArrayDestroy(Y, H);
	ArrayDestroy(G, H);
}