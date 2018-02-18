#pragma once

#include <stdio.h>
#include <iostream>
#include <time.h>
#include <conio.h>
#include <vector>
#include <algorithm>

#define MAX_GENE_LEN 100
#define MAX_POP_SIZE 1000

namespace ga {
	struct Gene {
		int allele[MAX_GENE_LEN];
		//std::vector<int> allele = std::vector<int>(MAX_GENE_LEN);
		//std::vector<int>allele(MAX_GENE_LEN);
		float fitness;
	};
	class GeneticAlgorithm {
	public:
		int gene_len, pop_size, decimal_num;
		float mutation_rate;

		GeneticAlgorithm(int gene_length, int population_size, int number_of_decimal_digits, float rate_of_mutation);

		Gene *initialiseGenePool(Gene gene[]);

		Gene *getFitness(Gene *gene);

		Gene *generateSelection(Gene *gene_pool);

		Gene *generateCrossover(Gene *gene_pool);

		Gene *generateMutation(Gene *gene_pool);

		Gene getGeneWithMaxFitness(Gene *gene_pool);

		void printGenePool(Gene *gene);

	private:
		float _total_fitness = 0.0;

		float _getFitnessValue(int arr[]);

		int _getCrossoverPoint();

	};
}