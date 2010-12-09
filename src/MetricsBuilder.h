#ifndef MetricsBuilder_h
#define MetricsBuilder_h

#include <vector>
#include <map>
#include <string>

class File;
class Issue;

class MetricsBuilder
{
public:
    MetricsBuilder();

    int getSeverityScore() const;
    int getIssuesCount() const;
    void addSeverityScore(int score);
    void addIssuesCount(int count);
    void onNewIssue(const std::string& filename, int issueNumber);
    void buildMetricsLists();

private:
    MetricsBuilder(const MetricsBuilder& rhs);
    MetricsBuilder& operator=(const MetricsBuilder& rhs);

    typedef std::map<std::string, File> FILE_MAP;
    typedef std::map<int, Issue> ISSUE_MAP;

    int m_severityScore;
    int m_issuesCount;
    FILE_MAP m_fileMap;
    ISSUE_MAP m_issueMap;
};

#endif
