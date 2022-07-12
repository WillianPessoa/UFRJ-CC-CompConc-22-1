#include "substringtester.h"

#include "timer.h"

#include <cassert>
#include <chrono>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <map>
#include <memory>
#include <mutex>
#include <streambuf>
#include <string>
#include <thread>
#include <vector>

using namespace std;

namespace {
// Tests directories
const string SAMPLE_CORRECTNESS_DIRNAME = "samples-correctness";
const string SAMPLE_PERFORMANCE_DIRNAME = "samples-performance";

// Short hand for names
const map<SubstringSolverType, string> methodsName {
    { SequentialNaive, "Sequential Naive" },
    { SequentialKMP, "Sequential KMP" },
    { ConcurrentNaive, "Concurrent Naive" },
    { ConcurrentKMP, "Concurrent KMP" }
};

// To concurrent code
int lastTestLogged = 0;
std::mutex logMutex;
}

// inicializando membro estático
vector<Sample> SubstringTester::s_correctnessSamples;
vector<Sample> SubstringTester::s_performanceSamples;
vector<TestResult> SubstringTester::s_testCorrectnessResults;
vector<TestResult> SubstringTester::s_testPerformanceResults;

void SubstringTester::loadSamples(const std::string& dirname, TestType testType)
{
    vector<string> contentFiles;

    // Obtém conteúdo dos arquivos
    for (const auto& file : filesystem::directory_iterator(dirname)) {
        ifstream t(file.path());
        string str((istreambuf_iterator<char>(t)),
            istreambuf_iterator<char>());
        contentFiles.push_back(str);
    }

    // Separa o conteúdo em amostras (Samples)
    for (int i = 0, size = contentFiles.size(); i < size; i += 2) {
        assert(contentFiles[i + 1].length() >= contentFiles[i].length());
        if (testType == Correctness) {
            s_correctnessSamples.push_back(Sample(contentFiles[i + 1], contentFiles[i]));
        } else {
            s_performanceSamples.push_back(Sample(contentFiles[i + 1], contentFiles[i]));
        }
    }
}

void SubstringTester::loadCorrectnessSamples()
{
    loadSamples(SAMPLE_CORRECTNESS_DIRNAME, Correctness);
}

void SubstringTester::loadPerformanceSamples()
{
    loadSamples(SAMPLE_PERFORMANCE_DIRNAME, Performance);
}

void SubstringTester::runCorrectnessTests(const bool log)
{
    // To get elapsed time for each test
    Timer clock;

    // For each sample
    int index = 0;
    for (const auto& sample : s_correctnessSamples) {

        // For each solver type
        for (auto solverType : { SequentialNaive, SequentialKMP, ConcurrentNaive, ConcurrentKMP }) {

            // Record start time
            clock.tick();

            // Apply solver
            vector<int> ocurrences = SubstringMatcher::find_pattern(sample.text(), sample.pattern(), solverType);

            // Record end time
            clock.tock();

            // Get duration
            const double duration = clock.duration();

            // Store result
            s_testCorrectnessResults.push_back(TestResult(sample, ocurrences, duration, solverType));

            if (log && s_testCorrectnessResults.size() % 4 == 0) {
                cout << "Testcase " << setw(2) << setfill('0') << index + 1 << ":" << endl;
                int NaiveSeqIndex = s_testCorrectnessResults.size() - 4;
                int NaiveConcIndex = s_testCorrectnessResults.size() - 3;
                int KMPSeqIndex = s_testCorrectnessResults.size() - 2;
                int KMPConcIndex = s_testCorrectnessResults.size() - 1;

                for (int i = NaiveSeqIndex; i <= KMPConcIndex; ++i) {
                    logATest(s_testCorrectnessResults[i]);
                }

                vector<int> NaiveSeqIndexes = s_testCorrectnessResults[NaiveSeqIndex].ocurrences();
                vector<int> NaiveConcIndexes = s_testCorrectnessResults[NaiveConcIndex].ocurrences();
                vector<int> KMPSeqIndexes = s_testCorrectnessResults[KMPSeqIndex].ocurrences();
                vector<int> KMPConcIndexes = s_testCorrectnessResults[KMPConcIndex].ocurrences();

                cout << "Todos iguais [" << NaiveSeqIndexes.size() << "]? ";

                /*cout << "[";

                if (NaiveSeqIndexes.size() != NaiveConcIndexes.size()) {
                    cout << "NaiveConc (" << NaiveConcIndexes.size() << "), ";
                } else if (NaiveSeqIndexes.size() != KMPSeqIndexes.size()) {
                    cout << "KMPSeq (" << KMPSeqIndexes.size() << "), ";
                } else if (NaiveSeqIndexes.size() != KMPConcIndexes.size()) {
                    cout << "KMPConc (" << KMPConcIndexes.size() << ") ";
                }
                cout << "] sao diferentes" << endl;*/

                if ((NaiveSeqIndexes.size() == NaiveConcIndexes.size()) && (KMPSeqIndexes.size() == KMPConcIndexes.size()) && (NaiveConcIndexes.size() == KMPSeqIndexes.size())) {
                    cout << "SIM!" << endl;
                } else {
                    cout << "NAO!" << endl;
                }
                ++index;
            }
        }
    }
}

void SubstringTester::runPerformanceTests(const bool log)
{
    // To get elapsed time for each test
    Timer clock;

    // For each sample
    int index = 0;
    for (const auto& sample : s_performanceSamples) {

        // For each solver type
        for (auto solverType : { SequentialNaive, SequentialKMP, ConcurrentNaive, ConcurrentKMP }) {

            vector<TestResult> results;

            // 5 Times
            for (int i = 0; i < 5; ++i) {

                // Record start time
                clock.tick();

                // Apply solver
                vector<int> ocurrences = SubstringMatcher::find_pattern(sample.text(), sample.pattern(), solverType);

                // Record end time
                clock.tock();

                // Get duration
                const double duration = clock.duration();

                // Store result
                results.push_back(TestResult(sample, ocurrences, duration, solverType));
            }

            // Get best result (shorter time)
            TestResult bestResult = results[0];
            for (int i = 1; i < results.size(); ++i) {
                if (results[0].time() < bestResult.time()) {
                    bestResult = results[0];
                }
            }

            s_testPerformanceResults.push_back(bestResult);

            cout << "Terminou " << solverType << endl;
        }

        cout << "Testcase " << setw(2) << setfill('0') << index + 1 << ":" << endl;
        for (const auto& testResult : s_testPerformanceResults) {
            logATest(testResult);
        }

        ++index;
    }
}

void SubstringTester::log(const string& filename)
{
}

void SubstringTester::logATest(const TestResult& testResult)
{
    cout << "\t" << methodsName.at(testResult.solverType()) << ": " << endl;
    cout << "\t\tTime: " << testResult.time() << "ms" << endl;
    if (testResult.ocurrences().size()) {
        cout << "\t\tFounded " << testResult.ocurrences().size() << " ocurrences ";
        /*cout << "\t\tPattern founded at indexes ";
        for (const auto& ocurrence : testResult.ocurrences()) {
            cout << ocurrence << " ";
        }*/
    } else {
        cout << "\t\tPatterns not founded";
    }
    cout << endl
         << endl;
}

TestResult::TestResult(const Sample& sample,
    const std::vector<int>& ocurrences,
    const double& time,
    SubstringSolverType solverType)
    : m_sample(sample)
    , m_ocurrences(ocurrences)
    , m_time(time)
    , m_solverType(solverType)
{
}

Sample TestResult::sample() const
{
    return m_sample;
}

vector<int> TestResult::ocurrences() const
{
    return m_ocurrences;
}

double TestResult::time() const
{
    return m_time;
}

SubstringSolverType TestResult::solverType() const
{
    return m_solverType;
}
