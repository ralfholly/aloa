#ifndef ClassicMetricsReporter_h
#define ClassicMetricsReporter_h

#include "MetricsReporter.h"

class ClassicMetricsReporter : public MetricsReporter
{
public:
    virtual void reportMetrics(int totalSeverityScore, int totalIssuesCount,
        const FILE_MAP& fileMap, const ISSUE_MAP& issueMap,
        const FILE_LIST& fileList, const ISSUE_LIST& issueList);

private:
    void printFileList(const FILE_LIST& fileList);
    void printIssueList(const ISSUE_LIST& issueList);
    void printFooter();
};

#endif

