///////////////////////////////////////////////////////////////////////////////
// Copyright (c) 2006 - 2010 by Ralf Holly.
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

/** MetricsBuilder -- header file.
 * @file
 */

#ifndef MetricsBuilder_h
#define MetricsBuilder_h

#include "MetricsTypes.h"

class File;
class Issue;
class MetricsReporter;

/** This class collects and builds Lint metrics.
 * Whenever Aloa has fetched a Lint issue, it passes is to an instance of
 * MetricsBuilder by calling onNewIssue(). When all issues have been read, Aloa
 * calls buildMetricsLists() and passes an instance of a MetricsReporter, whose
 * job it is to present the metrics to the user.
 */
class MetricsBuilder
{
public:
    /** MetricsBuild constructor. */
    MetricsBuilder();

    /** Returns the total number of issues found. */
    int getIssuesCount() const;

    /** Informs MetricsBuilder about a new issue.
     * @param number Issue number of this issue.
     * @param filename Name of file to which issue applies.
     * @param line Line number in file to which this issue belongs.
     */ 
    void onNewIssue(int number, const std::string& filename, int line);

    /** Builds and reports metrics. 
     * @param reporter MetricsReporter that is to be used to report the computed
     * metrics.
     * @note This method shall not be called before all issues have been
     * reported via onNewIssue().
     */
    void reportMetrics(const MetricsReporter* reporter);

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
