#include <iostream>
#include <vector>

#include "substringmatcher.h"
#include "substringtester.h"

using namespace std;

// Função auxiliar para realizar testes
void make_test(const int testNumber, const string& text, const string& pattern)
{
    cout << "Naive " << testNumber << ": " << endl;
    const auto Naiveindexes = SubstringMatcher::find_pattern(text, pattern, SequentialNaive);
    for (const auto& i : Naiveindexes) {
        cout << "Padrao encontrado no indice " << i << "." << endl;
    }
    cout << endl;

    cout << "KMP " << testNumber << ": " << endl;
    const auto KMPindexes = SubstringMatcher::find_pattern(text, pattern, SequentialKMP);
    for (const auto& i : KMPindexes) {
        cout << "Padrao encontrado no indice " << i << "." << endl;
    }
    cout << endl;

    cout << "Test " << testNumber;
    if (Naiveindexes == KMPindexes) {
        cout << " OK!" << endl;
    } else {
        cout << " FAILED!" << endl;
    }
}

void make_another_test(const string& text, const string& pattern)
{
    cout << "Naive Sequential (";
    const auto NaiveSeqIndexes = SubstringMatcher::find_pattern(text, pattern, SequentialNaive);
    cout << NaiveSeqIndexes.size() << ") " << endl;
    for (const auto& i : NaiveSeqIndexes) {
        cout << "Padrao encontrado no indice " << i << "." << endl;
    }
    cout << endl;

    cout << "Naive Concurrent (";
    const auto NaiveConcIndexes = SubstringMatcher::find_pattern(text, pattern, ConcurrentNaive);
    cout << NaiveConcIndexes.size() << ") " << endl;
    for (const auto& i : NaiveConcIndexes) {
        cout << "Padrao encontrado no indice " << i << "." << endl;
    }
    cout << endl;

    cout << "KMP Sequential (";
    const auto KMPSeqIndexes = SubstringMatcher::find_pattern(text, pattern, SequentialKMP);
    cout << KMPSeqIndexes.size() << ") " << endl;
    for (const auto& i : KMPSeqIndexes) {
        cout << "Padrao encontrado no indice " << i << "." << endl;
    }
    cout << endl;

    cout << "KMP Concurrent (";
    const auto KMPConcIndexes = SubstringMatcher::find_pattern(text, pattern, ConcurrentKMP);
    cout << KMPConcIndexes.size() << ") " << endl;
    for (const auto& i : KMPConcIndexes) {
        cout << "Padrao encontrado no indice " << i << "." << endl;
    }
    cout << endl;

    cout << "Todos iguais? ";

    cout << "[";
    if (NaiveSeqIndexes != NaiveConcIndexes) {
        cout << "NaiveConc, ";
    } else if (NaiveSeqIndexes != KMPSeqIndexes) {
        cout << "KMPSeq, ";
    } else if (NaiveSeqIndexes != KMPConcIndexes) {
        cout << "KMPConc";
    }
    cout << "] sao diferentes" << endl;

    if ((NaiveSeqIndexes == NaiveConcIndexes) && (KMPSeqIndexes == KMPConcIndexes) && (NaiveConcIndexes == KMPSeqIndexes)) {
        cout << "SIM!" << endl;
    } else {
        cout << "NAO!" << endl;
    }
}

int main()
{
    //make_another_test("AABCAABAAABAACAADAABAABAAABCAABAAABCAABAAABAACAADAABAABAAABCAABAAABCAABAAABAACAADAABAABAAABCAABAAABCAABAAABAACAADAABAABAAABCAABAAABCAABAAABAACAADAABAABAAABCAABAAABCAABAAABAACAADAABAABAAABCAABAAABCAABAAABAACAADAABAABAAABCAABAAABCAABAAABAACAADAABAABAAABCAABAAABCAABAAABAACAADAABAABAAABCAABAAABCAABAAABAACAADAABAABAAABCAABAAABCAABAAABAACAADAABAABAAABCAABAAABCAABAAABAACAADAABAABAAABCAABA",
    //    "AABAAABAACAADAABAABAAABCAABAAABCAABAAABAACAADAABAABAAABCA");

    // Carrega arquivos localizados no diretório "samples-correctness" junto do executável
    //SubstringTester::loadCorrectnessSamples();

    // Executa os testes de corretude
    //SubstringTester::runCorrectnessTests(true);

    // Carrega os arquivos localizados no diretório "samples-performance" junto do executável
    SubstringTester::loadPerformanceSamples();

    // Executa os testes de performance
    SubstringTester::runPerformanceTests(true);

    // Gera o arquivo de log
    SubstringTester::log("");

    return 0;
}
