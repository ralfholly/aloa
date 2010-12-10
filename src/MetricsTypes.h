#ifndef MetricsTypes_h
#define MetricsTypes_h

#include <vector>
#include <map>
#include <string>
#include "File.h"
#include "Issue.h"

typedef std::map<std::string, File> FILE_MAP;
typedef std::map<int, Issue> ISSUE_MAP;
typedef std::vector<File> FILE_LIST;
typedef std::vector<Issue> ISSUE_LIST;

#endif

