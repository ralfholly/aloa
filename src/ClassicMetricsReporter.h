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

#ifndef ClassicMetricsReporter_h
#define ClassicMetricsReporter_h

/** ClassicMetricsReporter -- header file. 
 * @file
 */

#include "MetricsReporter.h"

/** This class is a concrete subclass of MetricsReporter that writes metrics to
 * standard output. */
class ClassicMetricsReporter : public MetricsReporter
{
public:
    virtual void reportMetrics(int totalSeverityScore, int totalIssuesCount,
        const FILE_MAP& fileMap, const ISSUE_MAP& issueMap,
        const FILE_LIST& fileList, const ISSUE_LIST& issueList) const;

private:
    void printFileList(const FILE_LIST& fileList) const;
    void printIssueList(const ISSUE_LIST& issueList) const;
    void printFooter() const;
};

#endif

