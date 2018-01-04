// ag3.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <stdio.h>
#define _USE_MATH_DEFINES
#include <math.h>
#include <time.h>
#include <Windows.h>

#define LOG_CONSOLE_ENABLED					1
#define LOG_FILE_ENABLED					1
#define ESTIMATE_TIME_ITERATIONS			0
#define ESTIMATE_TIME_GENERATIONS			0
#define MINIMIZE							1

#define ITERATIONS							1
#define MUTATIONS_ENABLED					1

#define DEBUG_EVALUATION                    0

FILE * logfile;
FILE * evolutionfile;
FILE * resultsfile;
FILE * avgfitnessfile;


#define LOG(x, ...)													\
{																	\
	if(LOG_CONSOLE_ENABLED) printf(x, __VA_ARGS__);					\
	if(LOG_FILE_ENABLED)fprintf(logfile, x, __VA_ARGS__);			\
}

void DisplayMatrix(int ** matrix, int n)
{
	LOG(" =====  M A T R I X  =====\n\n");
	for (int i = 0; i < n; i++)
	{
		LOG("  ");
		for (int j = 0; j < n; j++)
		{
			LOG("% 4d ", matrix[i][j]);
		}
		LOG("\n\n");
	}
	LOG(" =========================\n");
}

int Initialize_AG(int & n, int & maxGenerations, int & popSize, double & mutationProbability, int ** &matrix)
{

	FILE * input;
	fopen_s(&input, "input.txt", "r");
	
	fscanf_s(input, "%d\n", &n);

	maxGenerations = 200;
	popSize = 100;
	mutationProbability = 0.01;

	int rows = n;
	int cols = n;
	matrix = new int*[rows];
	if (rows)
	{
		matrix[0] = new int[rows * cols];
		for (int i = 1; i < rows; i++)
			matrix[i] = matrix[0] + i * cols;
	}

	int node1, node2, val;
	while (-1 != fscanf_s(input, "%d %d %d\n", &node1, &node2, &val))
	{
		matrix[node1][node2] = matrix[node2][node1] = val;
	}

	for (int i = 0; i < n; i++)
	{
		matrix[i][i] = 0;
	}
	
	return 1;
}

void GenerateRandomSolution(int * data, int n)
{
	for (int i = 0; i < n; i++)
	{
		data[i] = i;
	}

	int aux;

	for (int i = 0; i < n; i++)
	{
		int randomIndex = floor((double)rand() / RAND_MAX * n);
		aux = data[randomIndex];
		data[randomIndex] = data[i];
		data[i] = aux;
	}
}

double Evaluate(int * data, int n, int ** matrix)
{
	int total = 0;

	for (int i = 0; i < n - 1; i++)
	{
		total += matrix[data[i]][data[i + 1]];
		if(DEBUG_EVALUATION) LOG("%d to %d = %d (total %d)\n", data[i], data[i + 1], matrix[data[i]][data[i + 1]], total);
	}

	total += matrix[data[n-1]][data[0]];
    if (DEBUG_EVALUATION) LOG("%d to %d = %d (total %d) - End\n", data[n - 1], data[0], matrix[data[n - 1]][data[0]], total);

	return total;
}

double Fitness(double result)
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
	int n;									// NUMBER OF NODES
	int maxGenerations;						// MAX NUM OF GENERATIONS
	int popSize;							// SIZE OF POPULATION
	double mutationProb;					// PROBABILITY OF MUTATION
	int ** matrix = NULL;					// MATRICEA COSTURILOR

	if (!Initialize_AG(n, maxGenerations, popSize, mutationProb, matrix))
	{
		LOG("Failed to initialize!\n");
		return 0;
	}

	LOG("Initialization successful.\nNumber of nodes:%d\nPopulation size: %d\nMaximum number of generations: %d\n\n", n, popSize, maxGenerations);

	DisplayMatrix(matrix, n);

	int * data;

	data = new int[n*popSize];

	// initialize stuff

	int globalBestResult;

	// generate generation 0

	for (int i = 0; i < popSize; i++)
	{
		GenerateRandomSolution(data + i * n, n);

		int result = Evaluate(data + i * n, n, matrix);
	}

	int gen = 0;

	// evaluate generation 0
	
	for (int i = 0; i < popSize; i++)
	{
		if (i == 0)
		{
			globalBestResult = Evaluate(data + i*n, n, matrix);

			LOG("Starting best result = %d (fit. %f)\n", globalBestResult, Fitness(globalBestResult));
		}
		else
		{
			int result = Evaluate(data + i * n, n, matrix);
			if (result < globalBestResult)
			{
				globalBestResult = result;
				LOG("New global best result = %d (fit. %f)\n", globalBestResult, Fitness(globalBestResult));
			}
		}
	}

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

	while (stop)
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
				printf("Time elapsed: %.1f s\nTime remaining: %.1f s\n", timeElapsed / 1000.00, ((timeElapsed / gen)*(maxGenerations - gen)) / 1000.00);
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

	LOG("Best result: %d (fit. %f)\n", globalBestResult, Fitness(globalBestResult));

	double endTime = clock();
	LOG("Time elapsed: %.1f s\n", (endTime - startTime) / 1000.00);

	delete[n*n] matrix;

    system("pause");

	return globalBestResult;
}

int main()
{
	srand(time(NULL));

	CreateDirectory("data", NULL);
	CreateDirectory("data\\TEST", NULL);

	fopen_s(&resultsfile, "data\\TEST\\resultsfile.log", "w");
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

		int result = AG_Main();
		fprintf(resultsfile, "%d\n", result);

		fclose(logfile);
		fclose(evolutionfile);
		fclose(avgfitnessfile);
	}
	double endTime = clock();
	fprintf(resultsfile, "\nTime elapsed: %.1f s", (endTime - startTime) / 1000.00);

	fclose(resultsfile);

	return 0;
}

