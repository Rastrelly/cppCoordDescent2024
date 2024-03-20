#include <iostream>
#include <vector>

float func(float a, float b, float c, float x, float y)
{
	return a*pow(x, 2) + b * pow(y, 2) + c * sin(0.01*a*x * y) + a*b;
}

std::vector<std::vector<float>> dataMatrix = {};

float ox1, ox2, oy1, oy2;
float dx, dy;
float fa, fb, fc;

int opCount = 0;

int main()
{
	//source data
	ox1 = -4.0f;
	ox2 = 4.0f;
	oy1 = -4.0f;
	oy2 = 4.0f;
	dx = 0.001f; dy = 0.001f;
	fa = 10.0f; fb = 20.0f; fc = -6.0f;

	//fill matrix
	//1 prep
	std::cout << "Generating data...\n";

	int ly = trunc((oy2 - oy1) / dy) + 1;
	int lx = trunc((ox2 - ox1) / dx) + 1;

	dataMatrix.resize(ly);
	for (int i=0;i<ly;i++)
	dataMatrix[i].resize(lx);

	//2 calc input data
	for (int j = 0; j < ly; j++)
	{
		float cy = oy1 + j * dy;
		for (int i = 0; i < lx; i++)
		{
			float cx = ox1 + i * dx;			
			dataMatrix[j][i] = func(fa, fb, fc, cx, cy);
		}
	}

	//3 run coord descent [numeric]
	std::cout << "Numeric approach\n";
	int cxId = 0, dxId = 0, sxId=0;
	int cyId = 0, dyId = 0, syId=0;
	int axis = 0; //0 - x, 1 - y
	int minXId = 0, minYId = 0;
	float minX, minY;

	int nx = dataMatrix[0].size();
	int ny = dataMatrix.size();

	int counter = 0;

	bool stop = false;
	do
	{
		counter++;
		minX = dataMatrix[cyId][0];
		sxId = minXId;
		minXId = 0;
		for (int i = 0; i < nx; i++)
		{
			if (dataMatrix[cyId][i] < minX)
			{
				minX = dataMatrix[cyId][i];
				minXId = i;
				dxId = minXId - sxId;
			}
			opCount++;
		}
		axis++;
		cxId = minXId;

		minY= dataMatrix[0][cxId];
		syId = minYId;
		minYId = 0;
		for (int j = 0; j < ny; j++)
		{
			if (dataMatrix[j][cxId] < minY)
			{
				minY = dataMatrix[j][cxId];
				minYId = j;
				dyId = minYId - syId;
			}
			opCount++;
		}
		axis = 0;
		cyId = minYId;


		if ((dxId == 0) && (dyId == 0)) stop = true;

		std::cout << "Iteration " << counter << ". MinX = " << minX << "; minY = " << minY << "\n";
		
	}
	while (!stop);

	std::cout << "Done in " << counter << "iterations, " << opCount << "ops. Result = " << (minX + minY) / 2.0f << "\n";

	//4 run coord descent [analytical]
	std::cout << "Semianalytical approach\n";
	float acx = ox1;
	float acy = oy1;
	float cminX = acx;
	float cminY = acy;
	
	float aminX = func(fa,fb,fc,acx,acy);
	float aminY = func(fa, fb, fc, acx, acy);
	float lminX = aminX;
	float lminY = aminY;

	float tresh = 0.01;

	counter = 0;
	opCount = 0;

	do {
		counter++;
		stop = false;
		acx = ox1;
		acy = oy1;
		lminX = aminX;
		lminY = aminY;
		while (acx <= ox2)
		{
			float tx = func(fa, fb, fc, acx, cminY);
			if (tx < aminX)
			{
				cminX = acx;
				aminX = tx;
			}
			acx += dx;
			opCount++;
		}
		while (acy < oy2)
		{
			float ty = func(fa, fb, fc, cminX, acy);
			if (ty < aminY)
			{
				cminY = acy;
				aminY = ty;
			}
			acy += dy;
			opCount++;
		}
		if ((abs(aminX - lminX) < tresh) && (abs(aminY - lminY) < tresh))
		{
			stop = true;
		}
	} while (!stop);
	std::cout << "Semianalytical solution: minX = " << aminX << "; minY = " << aminY << "\n";
	std::cout << "Done in " << counter << "iterations, " << opCount << "ops. Result = " << (minX + minY) / 2.0f << "\n";

	//find raw min
	std::cout << "Raw approach\n";
	opCount = 0;
	float min = dataMatrix[0][0];
	for (int i=0;i<nx;i++)
		for (int j = 0; j < ny; j++)
		{
			if (dataMatrix[j][i] < min) min = dataMatrix[j][i];
			opCount++;
		}
	std::cout << "Raw minimum = " << min << ". Found in "<<opCount<<" ops\n";

}