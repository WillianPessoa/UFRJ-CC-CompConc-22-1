#include "sample.h"

Sample::Sample()
    : m_text()
    , m_pattern()
{

}

Sample::Sample(const std::string &text, const std::string &pattern)
    : m_text(text)
    , m_pattern(pattern)
{

}

string Sample::text() const
{
    return m_text;
}

void Sample::setText(const std::string &text)
{
    m_text = text;
}

string Sample::pattern() const
{
    return m_pattern;
}

void Sample::setPattern(const std::string &pattern)
{
    m_pattern = pattern;
}
