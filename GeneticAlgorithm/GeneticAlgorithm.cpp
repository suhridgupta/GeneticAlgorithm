#include "GeneticAlgorithm.h"

using namespace std;
using namespace ga;

	
GeneticAlgorithm::GeneticAlgorithm(int gene_length, int population_size, int number_of_decimal_digits, float rate_of_mutation) {
	srand((int)time(NULL));
	if (gene_length > 0)
		gene_len = gene_length;
	else {
		printf("WARNING: Gene length must be greater than 0.\nRounding gene length up to 1.\n");
		gene_len = 1;
	}
	if (population_size % 2 == 0 && population_size != 0)
		pop_size = population_size;
	else if (population_size == 0) {
		printf("WARNING: Population size must be greater than zero.\nRounding population size up to 2.\n");
		pop_size = 2;
	}
	else if (population_size % 2 == 1) {
		printf("WARNING: Population size must be even.\nRounding population size up by 1.\n");
		pop_size = population_size + 1;
	}
	if (number_of_decimal_digits <= gene_len)
		decimal_num = number_of_decimal_digits;
	else {
		printf("WARNING: The number of decimals must be less than or equal to the gene length\nNumber has been shortened to gene_length\n");
		decimal_num = gene_len;
	}
	if (rate_of_mutation >= 0 && rate_of_mutation <= 1)
		mutation_rate = rate_of_mutation;
	else {
		printf("WARNING: Mutation rate must be between [0-1].\nMutation rate given default value of 0.001\n");
		mutation_rate = 0.001;
	}
}

Gene* GeneticAlgorithm::initialiseGenePool(Gene gene[]) {
	for (int i = 0; i < pop_size; i++) {
		for (int j = 0; j < gene_len; j++) {
			gene[i].allele[j] = rand() % 2;
		}
		gene[i].fitness = 0.0f;
	}
	return gene;
}

Gene* GeneticAlgorithm::getFitness(Gene *gene) {
	_total_fitness = 0.0;
	for (int i = 0; i < pop_size; i++) {
		gene[i].fitness = _getFitnessValue(gene[i].allele);
		_total_fitness = _total_fitness + gene[i].fitness;
	}
	return gene;
}

Gene* GeneticAlgorithm::generateSelection(Gene *gene_pool) {
	float sum = 0;
	Gene *new_gene_pool = (Gene*)malloc(MAX_GENE_LEN * sizeof(Gene));
	sort(gene_pool, gene_pool + pop_size, [](const Gene &a, const Gene &b) {return a.fitness > b.fitness; });
	for (int i = 0; i < pop_size; i++) {
		float roulette = ((float)rand() / (float)(RAND_MAX)) * _total_fitness;
		sum = 0.0;
		for (int j = 0; j < pop_size; j++) {
			sum = sum + gene_pool[j].fitness;
			if (sum >= roulette) {
				new_gene_pool[i] = gene_pool[j];
				break;
			}
		}
	}
	return new_gene_pool;
}

Gene* GeneticAlgorithm::generateCrossover(Gene *gene_pool) {
	int visited[MAX_GENE_LEN] = { 0 }, cross_num[MAX_GENE_LEN] = { 0 };
	for (int i = 0; i < pop_size; i++) {
		if (visited[i])
			continue;
		int next_index = 0;
		do {
			next_index = (1 + i) + (rand() % (pop_size - (i + 1)));
		} while (visited[next_index]);
		visited[next_index] = 1;
		int cross_point = _getCrossoverPoint();
		for (int j = cross_point; j < gene_len; j++) {
			int t = gene_pool[i].allele[j];
			gene_pool[i].allele[j] = gene_pool[next_index].allele[j];
			gene_pool[next_index].allele[j] = t;
		}
	}
	return gene_pool;
}

Gene* GeneticAlgorithm::generateMutation(Gene *gene_pool) {
	for (int i = 0; i < pop_size; i++) {
		for (int j = 0; j < gene_len; j++) {
			float probability = ((float)rand() / (float)RAND_MAX);
			if (probability < mutation_rate) {
				gene_pool[i].allele[j] = !gene_pool[i].allele[j];
				//printf("MUTATION\n");
			}
		}
	}
	return gene_pool;
}

Gene GeneticAlgorithm::getGeneWithMaxFitness(Gene *gene_pool) {
	float max = 0;
	Gene best;
	for (int i = 0; i<pop_size; i++)
		if (gene_pool[i].fitness > max) {
			max = gene_pool[i].fitness;
			best = gene_pool[i];
		}
	return best;
}

void GeneticAlgorithm::printGenePool(Gene *gene) {
	for (int i = 0; i < pop_size; i++) {
		printf("Allele:\t");
		for (int j = 0; j < gene_len; j++) {
			printf("%d", gene[i].allele[j]);
		}
		printf("\tFitness: %f", gene[i].fitness);
		printf("\n");
	}
	printf("\n");
}

float GeneticAlgorithm::_getFitnessValue(int arr[]) {
	float num = 0;
	for (int i = 0; i < decimal_num; i++)
		num = num + arr[i] * pow(2, decimal_num - 1 - i);
	for (int i = decimal_num; i < gene_len; i++)
		num = num + arr[i] * pow(2, decimal_num - i - 1);
	return num;
}

int GeneticAlgorithm::_getCrossoverPoint() {
	return rand() % gene_len;
}