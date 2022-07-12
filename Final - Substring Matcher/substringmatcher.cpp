#include "substringmatcher.h"

#include <algorithm>
#include <cassert>
#include <iostream>
#include <mutex>
#include <thread>

using std::cout;
using std::endl;
using std::mutex;
using std::sort;
using std::string;
using std::thread;
using std::vector;

// Global (in this file) variables
namespace {
const auto processor_count = std::thread::hardware_concurrency();

/************************ NAIVE *************************/
int indexNaive = -1;
vector<int> naiveConcurrentOcurrences;

/************************ KMP *************************/
const int N_THREADS = 8;
vector<int> KMPConcurrentIndexes;

// SuportData
vector<int> partsPos;
vector<int> partsEndPos;
vector<int> partNPos;

//Suport Data to connections
vector<int> conPartsPos;
vector<int> conPartsEndPos;
vector<int> conPartNPos;

int iKMP = -1;
int jKMP = -1;

mutex indexAccessMutex;
}

namespace PatternSearch::Naive {

/**
 * @brief Solução sequencial do algoritmo inocente (força bruta) para detecção de padrões.
 * @param text Texto a ser verificado.
 * @param pattern Padrão de verificação.
 * @return Vetor contendo índices das ocorrências.
 */
vector<int> sequentialSearch(const string& text, const string& pattern)
{
    const int M = pattern.length();
    const int N = text.length();

    vector<int> matchedIndexes;

    /* A loop to slide pat[] one by one */
    for (int i = 0; i <= N - M; i++) {
        int j;

        /* For current index i, check for pattern match */
        for (j = 0; j < M; j++) {
            if (text[i + j] != pattern[j]) {
                break;
            }
        }

        if (j == M) { // if pat[0...M-1] = txt[i, i+1, ...i+M-1] {
            matchedIndexes.push_back(i);
        }
    }

    return matchedIndexes;
}

vector<int> concurrentSearchAux(const string& text, const string& pattern)
{
    const int M = pattern.length();
    const int N = text.length();

    vector<int> matchedIndexes;

    // Primeiro índice para esta thread (thread safe)
    indexAccessMutex.lock();
    int i = indexNaive;
    indexAccessMutex.unlock();

    /* A loop to slide pat[] one by one */
    for (; i <= N - M;) {

        // Próximo índice para esta thread (thread safe)
        indexAccessMutex.lock();
        i = ++indexNaive;
        //std::cout << "Thread " << std::this_thread::get_id() << ". (" << i << ")" << std::endl;
        indexAccessMutex.unlock();

        int j;

        /* For current index i, check for pattern match */
        for (j = 0; j < M; j++) {
            if (text[i + j] != pattern[j]) {
                break;
            }
        }

        if (j == M) { // if pat[0...M-1] = txt[i, i+1, ...i+M-1] {
            matchedIndexes.push_back(i);
        }
    }

    return matchedIndexes;
}

vector<int> concurrentSearch(const string& text, const string& pattern)
{
    vector<thread> threads;
    for (int i = 0; i < N_THREADS; ++i) {
        // Pass by reference here, make sure the object lifetime is correct
        threads.emplace_back([&]() {
            const auto ocurrences = concurrentSearchAux(text, pattern);
            naiveConcurrentOcurrences.insert(naiveConcurrentOcurrences.end(),
                ocurrences.begin(),
                ocurrences.end());
        });
    }

    for (auto& t : threads) {
        t.join();
    }

    // Reseta após threads executarem o serviço
    indexNaive = -1;
    sort(naiveConcurrentOcurrences.begin(), naiveConcurrentOcurrences.end());
    vector<int> occurrences = naiveConcurrentOcurrences;
    naiveConcurrentOcurrences.clear();
    return occurrences;
}

} // namespace PatternSearch::Naive

namespace PatternSearch::KMP {

vector<int> KMPPrefix(const string& pattern)
{
    // length of the previous longest prefix suffix
    int previousLPSSize = 0;

    vector<int> lps(pattern.length());
    lps[0] = 0; // Primeiro sempre é 0

    // the loop calculates lps[i] for i = 1 to M-1
    for (int i = 1; i < pattern.length(); /**/) {

        // Ok
        if (pattern[i] == pattern[previousLPSSize]) {
            previousLPSSize++;
            lps[i] = previousLPSSize;
            i++;
        }

        else { // (pattern[i] != pattern[len])
            if (previousLPSSize == 0) {
                lps[i] = 0;
                ++i;
            } else {
                previousLPSSize = lps[previousLPSSize - 1];
                // i não é incrementado aqui
            }
        }
    }
    return lps;
}

/**
 * @brief Solução sequencial do algoritmo KMP para detecção de padrões.
 * @param text Texto a ser verificado.
 * @param pattern Padrão de verificação.
 * @return Vetor contendo índices das ocorrências.
 */
vector<int> KMP(const string& text, const string& pattern)
{
    // Vetor dos índices das ocorrências
    vector<int> matchIndexes;

    // Vetor para longest prefix suffix
    vector<int> lps = KMPPrefix(pattern);

    for (int i = 0, j = 0; i < text.length(); /**/) {

        // Ocorrência não completa
        if (pattern[j] == text[i]) {
            j++;
            i++;
        }

        // Ocorrência completa encontrada
        if (j == pattern.length()) {
            matchIndexes.push_back(i - j);
            j = lps[j - 1];
        }

        // Incosistência encontrada
        else if (i < text.length() && pattern[j] != text[i]) {
            // Se não voltou para o começo, pegue o próximo J
            if (j != 0) {
                j = lps[j - 1];
            }
            // Voltou para o começo, segue para o próximo na string amior
            else {
                i = i + 1;
            }
        }
    }

    return matchIndexes;
}

vector<int> parallelKMP(const string& text, const string& pattern)
{
    // Separating indexes
    vector<thread> partThreads;

    // Checking dimension restriction
    // The length of (text / n_threads) should be lesser of pattern length
    // If not, use less threads
    int n_threads = N_THREADS;
    while (pattern.length() > (text.length() / n_threads)) {
        --n_threads;
    }

    if (n_threads < N_THREADS) {
        cout << "The number of threads was reduced to " << n_threads << " because pattern size restriction." << endl;
    }

    for (int i = 0; i < n_threads; ++i) {
        partsPos.push_back(i * (text.length() / n_threads));
        partsEndPos.push_back(((text.length() / n_threads) * ((i + 1)) - 1));
        partNPos.push_back(text.length() / n_threads);
    }

    // Dividing string in parts of n_threads
    for (int i = 0; i < n_threads; ++i) {
        // Pass by reference here, make sure the object lifetime is correct
        partThreads.emplace_back([&]() {
            indexAccessMutex.lock();
            int i = ++iKMP;
            indexAccessMutex.unlock();

            const auto ocurrences = KMP(string(text.substr(partsPos[i], partNPos[i])), pattern);

            indexAccessMutex.lock();
            KMPConcurrentIndexes.insert(KMPConcurrentIndexes.end(),
                ocurrences.begin(),
                ocurrences.end());
            indexAccessMutex.unlock();
        });
    }

    for (auto& t : partThreads) {
        t.join();
    }

    // Searching in conection points
    // Connection points is the join of:
    //    n-1 elements from end of part1
    //    n-1 elements from start of part2
    //
    // where n is (pattern size)
    const int n_1 = pattern.length() - 1;

    // Separating indexes
    for (int j = 0; j < n_threads - 1; ++j) {
        conPartsPos.push_back(partsEndPos[j] - n_1 + 1); // a
        conPartsEndPos.push_back(partsPos[j + 1] + n_1 - 1); // b
        conPartNPos.push_back(conPartsEndPos[j] - conPartsPos[j] + 1); // c
    }

    vector<thread> connectionthreads;
    for (int j = 0; j < n_threads - 1; ++j) {
        //KMP()
        // Pass by reference here, make sure the object lifetime is correct
        connectionthreads.emplace_back([&]() {
            indexAccessMutex.lock();
            int j = ++jKMP;
            indexAccessMutex.unlock();

            auto ocurrences = KMP(string(text.substr(conPartsPos[j], conPartNPos[j])), pattern);

            // Fixing indexes in tmp to match with text string
            for (int k = 0; k < ocurrences.size(); ++k) {
                ocurrences[k] = conPartsPos[j] + ocurrences[k];
            }

            indexAccessMutex.lock();

            // Inserting in global vector
            KMPConcurrentIndexes.insert(KMPConcurrentIndexes.end(),
                ocurrences.begin(),
                ocurrences.end());
            indexAccessMutex.unlock();
        });
    }

    for (auto& t : connectionthreads) {
        t.join();
    }

    // Reseta após threads executarem o serviço
    vector<int> occurrences = KMPConcurrentIndexes;
    KMPConcurrentIndexes.clear();

    partsPos.clear();
    partsEndPos.clear();
    partNPos.clear();

    conPartsPos.clear();
    conPartsPos.clear();
    conPartsPos.clear();

    iKMP = -1;
    jKMP = -1;
    return occurrences;
}

}

std::vector<int> SubstringMatcher::find_pattern(
    const std::string& text,
    const std::string& pattern,
    const SubstringSolverType& solverType)
{
    std::vector<int> ocurrences = { -1 };

    switch (solverType) {
    case SequentialNaive:
        ocurrences = PatternSearch::Naive::sequentialSearch(text, pattern);
        break;
    case ConcurrentNaive:
        ocurrences = PatternSearch::Naive::concurrentSearch(text, pattern);
        break;
    case SequentialKMP:
        ocurrences = PatternSearch::KMP::KMP(text, pattern);
        break;
    case ConcurrentKMP:
        ocurrences = PatternSearch::KMP::parallelKMP(text, pattern);
        break;
    default:
        break;
    }

    return ocurrences;
}
