///////////////////////////////////////////////////////////////////////////////
// Copyright (c) 2010 by Ralf Holly.
//
// This program is free software; you can redistribute it and/or
// modify it under the terms of the GNU General Public License
// as published by the Free Software Foundation; either version 2
// of the License, or (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software
// Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
///////////////////////////////////////////////////////////////////////////////

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
    void onNewIssue(const std::string& filename, int issueNumber, int line);
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
