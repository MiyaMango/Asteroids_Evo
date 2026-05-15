#ifndef EVO_H
#define EVO_H

#include <vector>
#include "bot.hpp"

class Evo{
private:
    std::vector<double> last_best_scores;
    std::vector<std::pair<std::vector<double>,float>> pop;
    int current_k;
    int best_pos;
    int genome_size;
    int pop_size;
    float population_average_score;

    std::vector<double> gen_mutation(std::vector<double> original, double mut_percent, int max_mut_genes, int min_mut_genes);
    double mean();
    double variance();
    std::vector<double> crossover(std::vector<double> genoma_pai1, std::vector<double> genoma_pai2, int gen_size);
    int k_tournament_selection(int k);


public:

    // constructor -----------------------------------------------------------------------------
    // notes: the population is saved as a vector of pairs, and each pairs has a vector of doubles (the genome) and a float (the score obtained in the last simulation)
    Evo(int population_size, std::vector<std::pair<std::vector<double>,float>> population, int gsize);
    ~Evo();

    // getters e setters -----------------------------------------------------------------------
    std::vector<double> get_best_genes();
    int get_k();
    float get_population_average_score();

    // methods ---------------------------------------------------------------------------------

    void SaveBestGenScore(int gen, double score);

    int updateK(int pop_size);

    void set_best_pos(int pos);

    void find_best_gene();

    std::vector<std::vector<double>> repopulation();

};

#endif