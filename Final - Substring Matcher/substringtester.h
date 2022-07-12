#ifndef SUBSTRINGTESTER_H
#define SUBSTRINGTESTER_H

#include <map>
#include <string>
#include <vector>

#include "sample.h"
#include "substringmatcher.h"

using std::pair;
using std::string;
using std::vector;

// Foward declaration
class TestResult;

enum TestType {
    Correctness,
    Performance
};

class SubstringTester {
public:
    static void loadCorrectnessSamples();

    static void loadPerformanceSamples();

    static void runCorrectnessTests(const bool log = false);

    static void runPerformanceTests(const bool log = false);

    static void log(const std::string& filename);

    static void logATest(const TestResult& testResult);

private:
    SubstringTester() { }

    static void loadSamples(const string& dirname, TestType testType);

private:
    static vector<Sample> s_correctnessSamples;
    static vector<Sample> s_performanceSamples;
    static vector<TestResult> s_testCorrectnessResults;
    static vector<TestResult> s_testPerformanceResults;
};

/**
 * @brief The TestResult class is a shord hand to store pattern search result.
 */
class TestResult {
public:
    TestResult(const Sample& sample,
        const vector<int>& ocurrences,
        const double& time,
        SubstringSolverType solverType);

    Sample sample() const;

    vector<int> ocurrences() const;

    double time() const;

    SubstringSolverType solverType() const;

private:
    Sample m_sample;
    vector<int> m_ocurrences;
    double m_time;
    SubstringSolverType m_solverType;
};

#endif // SUBSTRINGTESTER_H
