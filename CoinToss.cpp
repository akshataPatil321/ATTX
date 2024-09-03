#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <numeric> 
#include <iomanip>
#include <limits> 

using namespace std;

// Defining probabilities of each coin
const double PROBABILITIES[5][2] = {
    {0.20, 0.80}, // Coin A
    {0.40, 0.60}, // Coin B
    {0.50, 0.50}, // Coin C
    {0.60, 0.40}, // Coin D
    {0.80, 0.20}  // Coin E
};

/*
flipCoin simulates a coin toss of an unfair coin.
Input Parameters: 
    probHead - Head probability of a given coin
*/
bool flipCoin(double probHead) {
    return ((rand() / (double)RAND_MAX) < probHead);
}

/*  
chooseCoin generates a bestEstimatedProbability for a coin using prior 
information in form of it's totalFlips and headsCount.
Input Parameters:
    totalFlips: No. of times a given coin has been flipped.
    headsCount: No. of Head results from each flip.
*/
int chooseCoin(const vector<int>& headsCount, const vector<int>& totalFlips) {
    double bestEstimatedProb = -numeric_limits<double>::infinity();
    int bestCoin = 0;

    for (int i = 0; i < 5; ++i) {
        // Ensuring that each coin has atleast been tossed once before. 
        // If not, then assuming estimatedProb of a fair coin (0.5)
        double estimatedProb = totalFlips[i] > 0 ? (double)headsCount[i] / totalFlips[i] : 0.5;
        if (estimatedProb > bestEstimatedProb) {
            bestEstimatedProb = estimatedProb;
            bestCoin = i;
        }
    }
    return bestCoin;
}

/*
Perform One Run of 20 flips
*/
int performRun() {
    vector<int> headsCount(5, 0);  // Heads counts for each coin
    vector<int> totalFlips(5, 0);  // Total flips for each coin
    int heads = 0;
    
    int totalFlipsAllowed = 20; // Total flips allowed per run
    
    // Exploring 50% of the allowed Flips to generate a bestProbabityEstimate
    int initialFlips = (totalFlipsAllowed / 2) / 5;  
    
    // Phase 1: Initial exploration to gather data
    for (int i = 0; i < 5; ++i) {
        for (int j = 0; j < initialFlips; ++j) {
            bool isHead = flipCoin(PROBABILITIES[i][0]);
            if (isHead) headsCount[i]++;
            totalFlips[i]++;
            if (isHead) heads++;
        }
    }

    // Phase 2: Use gathered information to maximize heads
    int remainingFlips = totalFlipsAllowed - (5 * initialFlips); 
    if (remainingFlips < 0){
        throw runtime_error("Used up more flips than allowed");
    }
    for (int i = 0; i < remainingFlips; ++i) {
        int coinToFlip = chooseCoin(headsCount, totalFlips);
        bool isHead = flipCoin(PROBABILITIES[coinToFlip][0]);
        if (isHead) {
            heads++;
            headsCount[coinToFlip]++;
        }
        totalFlips[coinToFlip]++;
    }
    
    return heads;
}

/*
main executes 100 runs and computes statistics 
*/
int main() {
    srand(static_cast<unsigned int>(time(NULL))); 
    int numRuns = 100;
    vector<int> results(numRuns);
    int maxHeads = 0;
    int minHeads = 20;
    int moreThan10 = 0;
    int lessThan10 = 0;

    // Perform 100 runs
    for (int run = 0; run < numRuns; ++run) {
        int heads = performRun();
        results[run] = heads;

        if (heads > maxHeads) maxHeads = heads;
        if (heads < minHeads) minHeads = heads;
        if (heads > 10) moreThan10++;
        if (heads < 10) lessThan10++;
    }

    // Computing Statistics
    double sum = accumulate(results.begin(), results.end(), 0);
    double mean = sum / numRuns;

    // Formatting Output Results:
    cout << "Executed " << numRuns << " runs of 20 coin flips. Statistics for the runs were as follows:" << endl;
    cout << "The mean number of heads results per run was " << fixed << setprecision(2) << mean << "." << endl;
    cout << "The maximum number of heads results in a run was " << maxHeads << "." << endl;
    cout << "The minimum number of heads results in a run was " << minHeads << "." << endl;
    cout << moreThan10 << " of " << numRuns << " runs produced greater than 10 heads results." << endl;
    cout << lessThan10 << " of " << numRuns << " runs produced less than 10 heads results." << endl;

    return 0;
}