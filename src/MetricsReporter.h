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

/** MetricsReporter -- header file.
 * @file
 */

#ifndef MetricsReporter_h
#define MetricsReporter_h

#include "MetricsTypes.h"

/** This class defines an abstract interface for concrete metrics reporting
 * subclasses.
 */
class MetricsReporter
{
public:
    /** Reports given metrics. 
     * @param totalSeverityScore Total severity score.
     * @param totalIssuesCount Total number of issues found.
     * @param fileMap Filename vs. File object map.
     * @param issueMap Issue number vs. Issue object map.
     * @param fileList List of all File objects.
     * @param issueList List of all Issue objects.
     */
    virtual void reportMetrics(int totalSeverityScore, int totalIssuesCount,
        const FILE_MAP& fileMap, const ISSUE_MAP& issueMap,
        const FILE_LIST& fileList, const ISSUE_LIST& issueList) const = 0;

    /** Destructor. */
    virtual ~MetricsReporter() throw() { }
};

#endif


