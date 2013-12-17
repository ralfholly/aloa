///////////////////////////////////////////////////////////////////////////////
// Copyright (c) 2006 - 2013 by Ralf Holly.
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

/** XmlMetricsReporter -- header file. 
 * @file
 */

#ifndef XmlMetricsReporter_h
#define XmlMetricsReporter_h

#include "MetricsReporter.h"
#include "MisraStringIssue.h"
#include <string>

class TiXmlElement;

/** This class is a concrete subclass of MetricsReporter that writes metrics to
 * as an XML file. */
class XmlMetricsReporter : public MetricsReporter
{
public:
    /** Constructor.
     * @param xmlfile Name of the XML file to which the ouput is written to. */
    XmlMetricsReporter(const std::string& xmlfile);
    virtual void reportMetrics(int totalSeverityScore, int totalIssuesCount,
        const FILE_MAP& fileMap, const ISSUE_MAP& issueMap,
        const FILE_LIST& fileList, const ISSUE_LIST& issueList, const MISRA_STRING_ISSUE_LIST* misraStringIssueList) const;

    /** Destructor. */
    virtual ~XmlMetricsReporter() throw() { }

private:
    void writeFileList(const FILE_LIST& fileList, TiXmlElement* root) const;
    void writeIssueList(const ISSUE_LIST& issueList, TiXmlElement* root) const;
    void writeMisraStringIssueList(const MISRA_STRING_ISSUE_LIST& misraStringIssueList, TiXmlElement* root) const;

    std::string m_xmlfile;
};

#endif

