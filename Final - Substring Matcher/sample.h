#ifndef SAMPLE_H
#define SAMPLE_H

#include <string>

using std::string;

class Sample {
public:
    Sample();

    Sample(const string& text, const string& pattern);

    string text() const;

    void setText(const string& text);

    string pattern() const;

    void setPattern(const string& pattern);

private:
    string m_text;
    string m_pattern;
};

#endif // SAMPLE_H
