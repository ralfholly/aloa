#ifndef MetricsBuilder_h
#define MetricsBuilder_h

#include "MetricsTypes.h"

class File;
class Issue;
class MetricsReporter;

class MetricsBuilder
{
public:
    MetricsBuilder();

    int getSeverityScore() const;
    int getIssuesCount() const;
    void addSeverityScore(int score);
    void addIssuesCount(int count);
    void onNewIssue(const std::string& filename, int issueNumber);
    void buildMetricsLists(MetricsReporter* reporter);

private:
    MetricsBuilder(const MetricsBuilder& rhs);
    MetricsBuilder& operator=(const MetricsBuilder& rhs);

    int m_severityScore;
    int m_issuesCount;

    FILE_MAP m_fileMap;
    ISSUE_MAP m_issueMap;
    FILE_LIST m_fileList;
    ISSUE_LIST m_issueList;
};

#endif
