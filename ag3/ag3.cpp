// ag3.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <stdio.h>
#define _USE_MATH_DEFINES
#include <math.h>
#include <time.h>
#include <Windows.h>

#define LOG_CONSOLE_ENABLED					0
#define LOG_FILE_ENABLED					1
#define ESTIMATE_TIME_ITERATIONS			0
#define ESTIMATE_TIME_GENERATIONS			1
#define MINIMIZE							1
#define USE_DEFAULT_INTERVAL				1

#define ITERATIONS							1
#define MUTATIONS_ENABLED					1

FILE * logfile;
FILE * evolutionfile;
FILE * resultsfile;
FILE * avgfitnessfile;


#define LOG(x, ...)													\
{																	\
	if(LOG_CONSOLE_ENABLED) printf(x, __VA_ARGS__);					\
	if(LOG_FILE_ENABLED)fprintf(logfile, x, __VA_ARGS__);			\
}

int Initialize(double & a, double & b, int & d, int & nParams, int & nTries, int & popSize, double & mutationProbability)
{
	a = 2;
	b = 3;
	d = 4;
	nParams = 5;
	nTries = 10000;
	popSize = 1000;
	mutationProbability = 0.01;

	return 1;
}


double Evaluate(char * data, int n, int pow2n, int nParams, double a, double b)
{

	return -1;
}


double Fitness(double result, int nParams)
{
	double fMin = 0.00;
	if (result != 0.00)
		return (1.00 / (result + fMin));
	return (1.00 / (result + 0.000001 + fMin));
}


double DifferencePercent(double v1, double v2)
{
	double difference = fabs(v1 - v2);
	return difference / (fabs(v1 + v2) / 2.00);
}

double AG_Main()
{
	double a, b;							// INTERVAL
	int d;									// PRECISION
	int nParams;							// NUMBER OF PARAMETERS
	int nTries;								// NUMBER OF TRIES
	int popSize;							// SIZE OF POPULATION
	double mutationProb;					// PROBABILITY OF MUTATION

	if (!Initialize(a, b, d, nParams, nTries, popSize, mutationProb))
	{
		LOG("Failed to initialize!\n");
		return 0;
	}

	LOG("Initialization successful.\nInterval: %f,%f\nPrecision: %d\nNumber of parameters: %d\nNumber of tries: %d\n\n", a, b, d, nParams, nTries);

	// initialize stuff

	double globalBestResult;

	// generate generation 0

	int gen = 0;

	// evaluate generation 0

	gen++;

	bool stop = false;

	int goodCrosses;
	int badCrosses;
	int neutralCrosses;

	int goodMutations;
	int badMutations;

	double currentBest;

	double startTime = clock();
	double lastTime = startTime;

	while (!stop)
	{
		LOG("\n >>> GENERATION #%d\n", gen);
		if (ESTIMATE_TIME_GENERATIONS)
		{
			double time = clock();
			if (time - lastTime >= 1000)
			{
				system("cls");
				lastTime = time;
				double timeElapsed = time - startTime;
				printf("\n[%06d]\n", gen);
				printf("Time elapsed: %.1f s\nTime remaining: %.1f s\n", timeElapsed / 1000.00, ((timeElapsed / gen)*(nTries - gen)) / 1000.00);
			}
		}


		// do selection

		goodCrosses = 0;
		badCrosses = 0;
		neutralCrosses = 0;

		// do crosses

		LOG("Crosses results:\n Good crosses: %d\n Bad crosses: %d\n Neutral crosses: %d\n", goodCrosses, badCrosses, neutralCrosses);

		if (MUTATIONS_ENABLED)
		{

			goodCrosses = 0;
			badCrosses = 0;
			neutralCrosses = 0;

			// do mutations

			LOG("Crosses results:\n Good crosses: %d\n Bad crosses: %d\n Neutral crosses: %d\n", goodCrosses, badCrosses, neutralCrosses);

		}

		// evaluate generation + stopping condition

		gen++;
	}

	LOG("Best result: %f\n", globalBestResult);

	double endTime = clock();
	LOG("Time elapsed: %.1f s", (endTime - startTime) / 1000.00);

	return 0;
}

int main()
{
	srand(time(NULL));

	CreateDirectory("data", NULL);
	CreateDirectory("data\\TEST", NULL);

	fopen_s(&resultsfile, "data\\TEST\\resultsfile.out", "w");
	double result;
	double startTime = clock();
	double lastTime = startTime;

	for (int iteration = 0; iteration < ITERATIONS; iteration++)
	{
		if (ESTIMATE_TIME_ITERATIONS)
		{
			double time = clock();
			if (time - lastTime >= 1000)
			{
				system("cls");
				lastTime = time;
				double timeElapsed = time - startTime;
				printf("\n[%06d]\n", iteration);
				printf("Time elapsed: %.1f s\nTime remaining: %.1f s\n", timeElapsed / 1000.00, ((timeElapsed / iteration)*(ITERATIONS - iteration)) / 1000.00);
			}
		}

		char * logfilename = new char[MAX_PATH];
		char * evolutionfilename = new char[MAX_PATH];
		char * avgfitnessfilename = new char[MAX_PATH];

		snprintf(logfilename, MAX_PATH, "data\\TEST\\%03d_Log.log", iteration);
		snprintf(evolutionfilename, MAX_PATH, "data\\TEST\\%03d_Evolution.log", iteration);
		snprintf(avgfitnessfilename, MAX_PATH, "data\\TEST\\%03d_Fitness.log", iteration);

		fopen_s(&logfile, logfilename, "w");
		fopen_s(&evolutionfile, evolutionfilename, "w");
		fopen_s(&avgfitnessfile, avgfitnessfilename, "w");

		double result = AG_Main();
		fprintf(resultsfile, "%f\n", result);

		fclose(logfile);
		fclose(evolutionfile);
		fclose(avgfitnessfile);
	}
	double endTime = clock();
	fprintf(resultsfile, "\nTime elapsed: %.1f s", (endTime - startTime) / 1000.00);

	fclose(resultsfile);

	return 0;
}

