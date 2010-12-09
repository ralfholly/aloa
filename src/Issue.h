#ifndef Issue_h
#define Issue_h

#include <vector>
#include <string>

// Encapsulates a lint issue (ie. warning, error)
class Issue
{
public:
    Issue(int number, int severity) :
        m_number(number),
        m_severity(severity),
        m_count(0) {}

    // Register a file with this issue
    void addFile(const std::string& filename);

    int getNumber() const       {
        return m_number;
    }
    int getSeverity() const     {
        return m_severity;
    }
    int getCount() const        {
        return m_count;
    }

private:
    friend bool operator<(const Issue& lhs, const Issue& rhs);
    int m_number;         // Lint issue number
    int m_severity;       // The severity level of this lint issue
    int m_count;          // Total number of occurrences of this lint issue
    std::vector<std::string> m_files;  // List of all files that contain this lint issue
};

inline bool operator<(const Issue& lhs, const Issue& rhs)
{
    // Sort by count, then by severity
    if (lhs.m_count == rhs.m_count)
        return lhs.m_severity > rhs.m_severity;
    return lhs.m_count > rhs.m_count;
}

#endif

