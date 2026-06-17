<<<<<<< HEAD
#include <vector>
#include "evolution.hpp"

using namespace std;
// constructor -----------------------------------------------------------------------------
Evo::Evo(int population_size, vector<pair<vector<double>,float>> population, int gsize){
    last_best_scores = {0,0,0,0,0};
    current_k = 2;
    population_average_score = 0;
    pop_size = population_size;
    pop = population;
    genome_size = gsize;
    find_best_gene();
}

Evo::~Evo() {}

// getters e setters -----------------------------------------------------------------------

vector<double> Evo::get_best_genes(){
    return(pop[best_pos].first);
}

int Evo::get_k(){
    return(current_k);
}

float Evo::get_population_average_score(){
    return population_average_score;
}

// methods ---------------------------------------------------------------------------------

void Evo::SaveBestGenScore(int gen, double score){
    int position = gen % 5;

    last_best_scores[position] = score;

}

void Evo::find_best_gene(){
    int curr_best = 0;
    for(int i=0; i<pop_size; i++){
         if(pop[i].second > pop[curr_best].second) curr_best = i;
    }

    best_pos = curr_best;
}

int Evo::updateK(int pop_size){
    double var = variance();
    double mean_best = mean();
    double stddev = sqrt(var);
    double VAR_THRESHOLD = stddev/(mean_best + 1e-9);

    if(VAR_THRESHOLD < 0.1) {
        // stagnated
        current_k = current_k / 2;
        if(current_k < 2) current_k = 2;
    } else {
        // evolving
        current_k = current_k * 2;
        if(current_k > pop_size) current_k = pop_size;
    }
    current_k = 2;
    return current_k;
}

void Evo::set_best_pos(int pos){
    best_pos = pos;
}

vector<vector<double>> Evo::repopulation(){

    vector<vector<double>> new_genomes;
    new_genomes.reserve(pop_size);

    vector<double> melhor_de_todos = get_best_genes();
    new_genomes.push_back(melhor_de_todos);

    for(int i = 0; i < pop_size-1; i++){
        // 1. Selection
        int pos_pai1 = k_tournament_selection(current_k);
        int pos_pai2 = k_tournament_selection(current_k);

        vector<double> pai1 = pop[pos_pai1].first;
        vector<double> pai2 = pop[pos_pai2].first;

        // 2. Crossover & Mutation (Creates Child DNA)
        vector<double> child_dna = crossover(pai1, pai2, genome_size);

        // 3. Store only the genome
        new_genomes.push_back(child_dna);
    }

    return new_genomes;
}

vector<double> Evo::gen_mutation(vector<double> original, double mut_percent, int max_mut_genes, int min_mut_genes){
    int muted_gen_num = GetRandomValue(min_mut_genes, max_mut_genes);

    vector<double> new_genoma = original;
    vector<bool> control(original.size(), false);

    for(int i = 0; i < muted_gen_num; i++){
        // Busca uma casa do genoma aleatoria para mutar, se a casa ja tiver sido mutada antes,
        // sorteia casas aleatorias até achar uma que não foi modificada
        int pos_gene = GetRandomValue(0, original.size() - 1);
        while(control[pos_gene] == 1){
            pos_gene = GetRandomValue(0, original.size() - 1);
        }
        // marca control na posicao com 1 para nao acessar mais aquela casa
        control[pos_gene] = 1;

        double mutation = random_double_sign(get_random_double(0, original[pos_gene]*mut_percent));

        while(mutation <= -1*original[pos_gene]){
            mutation = random_double_sign(get_random_double(0, original[pos_gene]*mut_percent));
        }

        new_genoma[pos_gene] = original[pos_gene] + mutation;
    }

    return(new_genoma);

}

double Evo::mean(){
    double mean = 0;
    for(double x : last_best_scores){
        mean += x;
    }
    mean /= last_best_scores.size();
    return mean;
}

double Evo::variance(){
    double mean_best = mean();
    double variance = 0;
    for(double x : last_best_scores){
        variance += (x - mean_best)*(x - mean_best);
    }
    variance /= last_best_scores.size();

    return variance;

    }

vector<double> Evo::crossover(vector<double> genoma_pai1, vector<double> genoma_pai2, int gen_size){
    vector<double> genoma_out(gen_size);

    //O cruzamento é uma média aritmética
    for(int i = 0; i < gen_size; i++){
        genoma_out[i] = (genoma_pai1[i] + genoma_pai2[i])/2;
    }

    genoma_out = gen_mutation(genoma_out, 0.25, gen_size, 1);

    return(genoma_out);
}

int Evo::k_tournament_selection(int k){
    int best = GetRandomValue(0, pop.size() - 1);

    for(int i = 1; i < k; i++){
        int challenger = GetRandomValue(0, pop.size() - 1);
        if(pop[challenger].second > pop[best].second) {
            best = challenger;
        }
    }
    return best;
}
=======
#include <vector>
#include "evolution.hpp"

using namespace std;
// constructor -----------------------------------------------------------------------------
Evo::Evo(int population_size, vector<pair<vector<double>,float>> population, int gsize){
    last_best_scores = {0,0,0,0,0};
    current_k = 2;
    population_average_score = 0;
    pop_size = population_size;
    pop = population;
    genome_size = gsize;
    find_best_gene();
}

Evo::~Evo() {}

// getters e setters -----------------------------------------------------------------------

vector<double> Evo::get_best_genes(){
    return(pop[best_pos].first);
}

int Evo::get_k(){
    return(current_k);
}

float Evo::get_population_average_score(){
    return population_average_score;
}

// methods ---------------------------------------------------------------------------------

void Evo::SaveBestGenScore(int gen, double score){
    int position = gen % 5;

    last_best_scores[position] = score;

}

void Evo::find_best_gene(){
    int curr_best = 0;
    for(int i=0; i<pop_size; i++){
         if(pop[i].second > pop[curr_best].second) curr_best = i;
    }

    best_pos = curr_best;
}

int Evo::updateK(int pop_size){
    double var = variance();
    double mean_best = mean();
    double stddev = sqrt(var);
    double VAR_THRESHOLD = stddev/(mean_best + 1e-9);

    if(VAR_THRESHOLD < 0.1) {
        // stagnated
        current_k = current_k / 2;
        if(current_k < 2) current_k = 2;
    } else {
        // evolving
        current_k = current_k * 2;
        if(current_k > pop_size) current_k = pop_size;
    }
    current_k = 2;
    return current_k;
}

void Evo::set_best_pos(int pos){
    best_pos = pos;
}

vector<vector<double>> Evo::repopulation(){

    vector<vector<double>> new_genomes;
    new_genomes.reserve(pop_size);

    vector<double> melhor_de_todos = get_best_genes();
    new_genomes.push_back(melhor_de_todos);

    for(int i = 0; i < pop_size-1; i++){
        // 1. Selection
        int pos_pai1 = k_tournament_selection(current_k);
        int pos_pai2 = k_tournament_selection(current_k);

        vector<double> pai1 = pop[pos_pai1].first;
        vector<double> pai2 = pop[pos_pai2].first;

        // 2. Crossover & Mutation (Creates Child DNA)
        vector<double> child_dna = crossover(pai1, pai2, genome_size);

        // 3. Store only the genome
        new_genomes.push_back(child_dna);
    }

    return new_genomes;
}

vector<double> Evo::gen_mutation(vector<double> original, double mut_percent, int max_mut_genes, int min_mut_genes){
    int muted_gen_num = GetRandomValue(min_mut_genes, max_mut_genes);

    vector<double> new_genoma = original;
    vector<bool> control(original.size(), false);

    for(int i = 0; i < muted_gen_num; i++){
        // Busca uma casa do genoma aleatoria para mutar, se a casa ja tiver sido mutada antes,
        // sorteia casas aleatorias até achar uma que não foi modificada
        int pos_gene = GetRandomValue(0, original.size() - 1);
        while(control[pos_gene] == 1){
            pos_gene = GetRandomValue(0, original.size() - 1);
        }
        // marca control na posicao com 1 para nao acessar mais aquela casa
        control[pos_gene] = 1;

        double mutation = random_double_sign(get_random_double(0, original[pos_gene]*mut_percent));

        while(mutation <= -1*original[pos_gene]){
            mutation = random_double_sign(get_random_double(0, original[pos_gene]*mut_percent));
        }

        new_genoma[pos_gene] = original[pos_gene] + mutation;
    }

    return(new_genoma);

}

double Evo::mean(){
    double mean = 0;
    for(double x : last_best_scores){
        mean += x;
    }
    mean /= last_best_scores.size();
    return mean;
}

double Evo::variance(){
    double mean_best = mean();
    double variance = 0;
    for(double x : last_best_scores){
        variance += (x - mean_best)*(x - mean_best);
    }
    variance /= last_best_scores.size();

    return variance;

    }

vector<double> Evo::crossover(vector<double> genoma_pai1, vector<double> genoma_pai2, int gen_size){
    vector<double> genoma_out(gen_size);

    //O cruzamento é uma média aritmética
    for(int i = 0; i < gen_size; i++){
        genoma_out[i] = (genoma_pai1[i] + genoma_pai2[i])/2;
    }

    genoma_out = gen_mutation(genoma_out, 0.25, gen_size, 1);

    return(genoma_out);
}

int Evo::k_tournament_selection(int k){
    int best = GetRandomValue(0, pop.size() - 1);

    for(int i = 1; i < k; i++){
        int challenger = GetRandomValue(0, pop.size() - 1);
        if(pop[challenger].second > pop[best].second) {
            best = challenger;
        }
    }
    return best;
}
>>>>>>> 325110fb93a5c8689592507857878177653534ad
