#include "IssueTable.h"

#include <cassert>

const int IssueTable::MAX_ISSUE_NUMBER;

size_t IssueTable::getIssueTableSize()
{
    return sizeof(ISSUE_TABLE) / sizeof(ISSUE_TABLE[0]);
}

int IssueTable::getSeverity(int number)
{
   const size_t tableSize = getIssueTableSize();

   for (size_t i = 0; i < tableSize; ++i) {
       if (ISSUE_TABLE[i].lowerBound <= number &&
           ISSUE_TABLE[i].upperBound >= number) {
           return ISSUE_TABLE[i].severity;
        }
   }
   // Error: issue not contained within issue table.
   assert(false);
   return -1;
}

const char* IssueTable::getIssueTableTitleByIndex(size_t i)
{
    assert(0 <= i && i <= getIssueTableSize());
    return ISSUE_TABLE[i].title;
}

int IssueTable::getIssueTableSeverityByIndex(size_t i)
{
    assert(0 <= i && i <= getIssueTableSize());
    return ISSUE_TABLE[i].severity;
}
