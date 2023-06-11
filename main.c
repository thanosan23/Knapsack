#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int** generate_pool(int n, int pool_size) {
    int** pool = malloc(pool_size * sizeof(int*));
    for(int i = 0; i < pool_size; i++) {
        pool[i] = (int*) malloc(n * sizeof(int));
    }
    for(int i = 0; i < pool_size; i++) {
        for(int j = 0; j < n; j++) {
            pool[i][j] = rand() % 2;
        }
    }
    return pool;
}

int fitness(int* genome, int max_weight, int* weights, int* values, int genome_length) {
    int total_weight = 0;
    int total_val = 0;
    for(int i = 0; i < genome_length; i++) {
        if(genome[i] == 1) {
            total_weight += weights[i];
            total_val += values[i];
        }
    }
    return (total_weight > max_weight ? 0 : total_val);
}

int compare( const void* a, const void* b)
{
   int int_a = *((int*) a);
   int int_b = *((int*) b);

   return (int_a < int_b) - (int_a > int_b);
}

int* crossover(int* genome1, int* genome2, int n) {
    int idx = rand() % (n);
    int *genome = malloc(n * sizeof(int));
    for(int i = 0; i < n; i++) {
        if(i < idx) {
            genome[i] = genome1[i];
        } else {
            genome[i] = genome2[i];
        }
    }
    return genome;
}

int* mutate(int* genome_in, int n) {
    int idx = rand() % (n+1);
    int *genome = malloc(n * sizeof(int));
    genome[idx] = 1 - genome_in[idx];
    return genome;
}

int main() {
    srand(time(NULL));

    int weights[4] = { 3, 4, 6, 5 };
    int values[4] = { 2, 3, 1, 4 };
    int max_weight = 8;

    int pool_length = 100;
    int top_n = 50;
    int n = 4;

    // generate a pool of genomes
    int** pool = generate_pool(n, pool_length);

    int* current_best;
    for(int gen = 0; gen < 50; gen++) {
        // calculate scores of the genomes and get the best genomes
        int scores[pool_length];
        for(int i = 0; i < pool_length; i++) {
            int score = fitness(pool[i], max_weight, weights, values, n);
            scores[i] = score;
        }
        qsort(scores, pool_length, sizeof(int), compare);

        int top[top_n];
        for(int i = 0; i < top_n; i++) {
            int val = scores[i];
            for(int j = 0; j < pool_length; j++) {
                if(fitness(pool[j], max_weight, weights, values, n) == val) top[i] = j;
            }
        }
        printf("Gen: %d, Fitness: %d\n", gen+1, fitness(pool[top[0]], max_weight, weights, values, n));

        // perform crossover and mutation on the genomes for the next generation
        current_best = pool[top[0]];
        int** new_pool = malloc(pool_length * sizeof(int*));
        for(int i = 0; i < pool_length; i++) {
            new_pool[i] = (int*) malloc(4 * sizeof(int));
        }
        for(int i = 0; i < pool_length; i++) {
            int idx1 = rand() % (top_n+1);
            int idx2 = rand() % (top_n+1);
            new_pool[i] = crossover(pool[idx1], pool[idx2], n);
            if(rand() % 6 == 1) { // 1/6 chance of mutating a genome
                new_pool[i] = mutate(new_pool[i], n);
            }
        }
        pool = new_pool;
    }

    // Show the solution
    printf("Solution: ");
    for(int i = 0; i < n; i++) {
        if(current_best[i] == 1) printf("%d ", i+1);
    }
    printf("\n");

    return 0;
}
