#ifndef IssueTable_h
#define IssueTable_h

#include <cstddef>

struct IssueTableEntry
{
    const char* title;
    int severity;
    int lowerBound;
    int upperBound;
};

const IssueTableEntry ISSUE_TABLE[] = 
{
    { "Elective note",       1, 900, 999 },
    { "Informational",       2, 700, 899 },
    { "Warning",             3, 400, 699 },
    { "Syntax error",        4,   1, 199 },
    { "PC-Lint error",     999, 200, 399 },
};

class IssueTable
{
public:
    static const int MAX_ISSUE_NUMBER = 2000;
    static size_t getIssueTableSize();
    static int getSeverity(int number);
    static int getIssueTableSeverityByIndex(size_t i);
    static const char* getIssueTableTitleByIndex(size_t i);
};

#endif

