#ifndef SUBSTRINGMATCHER_H
#define SUBSTRINGMATCHER_H

#include <string>
#include <vector>

enum SubstringSolverType {
    SequentialNaive,
    ConcurrentNaive,
    SequentialKMP,
    ConcurrentKMP
};

class SubstringMatcher {
public:
    static std::vector<int> find_pattern(const std::string& text,
        const std::string& pattern,
        const SubstringSolverType& solverType = SequentialNaive);

private:
    SubstringMatcher() {};
};

#endif // SUBSTRINGMATCHER_H
