#include "stdafx.h"
#include "PerlinNoise.h"


/**
 * @brief Construct a new Perlin Noise:: Perlin Noise object
 *  assigns the private members with the passed parameters
 * Perlin noise creates 2D wave-like noise which is used for terrain generation and more
 * @param _persistence
 * @param _frequency 
 * @param _amplitude 
 * @param _octaves 
 * @param _randomseed 
 */
PerlinNoise::PerlinNoise(double _persistence, double _frequency, double _amplitude, int _octaves, int _randomseed)
{
	persistence = _persistence;
	frequency = _frequency;
	amplitude = _amplitude;
	octaves = _octaves;
	randomseed = _randomseed;
}

/**
 * @brief Destroy the Perlin Noise:: Perlin Noise object
 * 
 */
PerlinNoise::~PerlinNoise()
{
}

/**
 * @brief Gtes the height of a given coordinate with the noise
 * 
 * @param x the x position
 * @param z the yposition
 * @return double the height
 */
double PerlinNoise::GetHeight(double x, double y) const
{
	return Total(x, y);
}

/**
 * @brief Gets the total of the given coordinates using perlin noise
 * 
 * @param i x position 
 * @param j z position
 * @return double the height
 */
double PerlinNoise::Total(double i, double j) const
{
	i = abs(i);
	j = abs(j);
	double t = 0.0f;
	double _amplitude = 1;
	double freq = frequency;

	for (int k = 0; k < octaves; k++)
	{
		t += GetValue(j * freq + randomseed, i * freq + randomseed) * _amplitude;
		_amplitude *= persistence;
		freq *= 2;
	}

	return t;
}

/**
 * @brief Calculates the noise 
 * 
 * @param x position of x
 * @param y position of z
 * @return double the value of the generated noise
 */
double PerlinNoise::GetValue(double x, double y) const
{
	int Xint = (int)x;
	int Yint = (int)y;
	double Xfrac = x - Xint;
	double Yfrac = y - Yint;

	//noise values
	double n01 = Noise(Xint - 1, Yint - 1);
	double n02 = Noise(Xint + 1, Yint - 1);
	double n03 = Noise(Xint - 1, Yint + 1);
	double n04 = Noise(Xint + 1, Yint + 1);
	double n05 = Noise(Xint - 1, Yint);
	double n06 = Noise(Xint + 1, Yint);
	double n07 = Noise(Xint, Yint - 1);
	double n08 = Noise(Xint, Yint + 1);
	double n09 = Noise(Xint, Yint);

	double n12 = Noise(Xint + 2, Yint - 1);
	double n14 = Noise(Xint + 2, Yint + 1);
	double n16 = Noise(Xint + 2, Yint);

	double n23 = Noise(Xint - 1, Yint + 2);
	double n24 = Noise(Xint + 1, Yint + 2);
	double n28 = Noise(Xint, Yint + 2);

	double n34 = Noise(Xint + 2, Yint + 2);

	//find the noise values of the four corners
	double x0y0 = 0.0625*(n01 + n02 + n03 + n04) + 0.125*(n05 + n06 + n07 + n08) + 0.25*(n09);
	double x1y0 = 0.0625*(n07 + n12 + n08 + n14) + 0.125*(n09 + n16 + n02 + n04) + 0.25*(n06);
	double x0y1 = 0.0625*(n05 + n06 + n23 + n24) + 0.125*(n03 + n04 + n09 + n28) + 0.25*(n08);
	double x1y1 = 0.0625*(n09 + n16 + n28 + n34) + 0.125*(n08 + n14 + n06 + n24) + 0.25*(n04);

	//interpolate between those values according to the x and y fractions
	double v1 = Interpolate(x0y0, x1y0, Xfrac); //interpolate in x direction (y)
	double v2 = Interpolate(x0y1, x1y1, Xfrac); //interpolate in x direction (y+1)
	double fin = Interpolate(v1, v2, Yfrac);  //interpolate in y direction

	return fin;
}

double PerlinNoise::Interpolate(double x, double y, double a) const
{
	double negA = 1.0 - a;
	double negASqr = negA * negA;
	double fac1 = 3.0 * (negASqr)-2.0 * (negASqr * negA);
	double aSqr = a * a;
	double fac2 = 3.0 * aSqr - 2.0 * (aSqr * a);

	return x * fac1 + y * fac2; //add the weighted factors
}

double PerlinNoise::Noise(int x, int y) const
{
	int n = x + y * 57;
	n = (n << 13) ^ n;
	int t = (n * (n * n * 15731 + 789221) + 1376312589) & 0x7fffffff;
	return 1.0 - double(t) * 0.931322574615478515625e-9;/// 1073741824.0);
}
