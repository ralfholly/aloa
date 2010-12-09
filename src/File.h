#ifndef File_h
#define File_h

#include <vector>
#include <string>

// Encapsulates a source code file with possibly many lint issues
class File
{
public:
    File(const std::string& filename) :
        m_filename(filename),
        m_severityScore(0),
        m_severestIssueNumber(UNUSED_ISSUE_NUMBER) {}

    // Registers a lint issue with this file
    void addIssue(int issueNumber);

    const std::string& getFilename() const   {
        return m_filename;
    }
    int getSeverityScore() const             {
        return m_severityScore;
    }
    int getSeverestIssueNumber() const       {
        return m_severestIssueNumber;
    }
    unsigned int getIssueCount() const       {
        return m_issues.size();
    }

private:
    static const int UNUSED_ISSUE_NUMBER = 0;  // Sentinel marker value

    friend bool operator<(const File& lhs, const File& rhs);
    std::string m_filename;         // The name of this source code module
    int m_severityScore;            // The accumulated severity score
    int m_severestIssueNumber;      // The issue number with the highest severity
    std::vector<int> m_issues;      // A list of all Lint issues contained in this file
};

inline bool operator<(const File& lhs, const File&  rhs)
{
    // Sort by severity score, then filename
    if (lhs.m_severityScore == rhs.m_severityScore)
        return lhs.m_filename < rhs.m_filename;
    return lhs.m_severityScore > rhs.m_severityScore;
}

#endif

