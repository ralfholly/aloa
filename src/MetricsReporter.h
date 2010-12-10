#ifndef MetricsReporter_h
#define MetricsReporter_h

#include "MetricsTypes.h"

class MetricsReporter
{
public:
    virtual void reportMetrics(int totalSeverityScore, int totalIssuesCount,
        const FILE_MAP& fileMap, const ISSUE_MAP& issueMap,
        const FILE_LIST& fileList, const ISSUE_LIST& issueList) = 0;

    virtual ~MetricsReporter() { }

};

#endif


