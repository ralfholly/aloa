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

#include "XmlMetricsReporter.h"
#include "tinyxml/tinyxml.h"
#include "IssueTable.h"

#include <sstream>
#include <iostream>

using namespace std;

template<typename T>
TiXmlElement* NewXmlElementWithValue(const char* elemName,
        T elemValue, TiXmlElement* parent)
{
    stringstream ss;
    ss << elemValue;
    TiXmlElement* elem = new TiXmlElement(elemName);
    parent->LinkEndChild(elem);
    TiXmlText* telem = new TiXmlText(ss.str().c_str());
    elem->LinkEndChild(telem);
    return elem;
}

XmlMetricsReporter::XmlMetricsReporter(const string& xmlfile) :
    MetricsReporter(),
    m_xmlfile(xmlfile)
{
}

void XmlMetricsReporter::reportMetrics(int totalSeverityScore, int totalIssuesCount,
        const FILE_MAP& fileMap, const ISSUE_MAP& issueMap, const FILE_LIST& fileList, const ISSUE_LIST& issueList)
{
    // same as write_simple_doc1 but add each node
    // as early as possible into the tree.

    TiXmlDocument doc;
    TiXmlDeclaration* decl = new TiXmlDeclaration("1.0", "", "");
    doc.LinkEndChild(decl);

    TiXmlElement* root = new TiXmlElement("Aloa");
    doc.LinkEndChild(root);

    (void)NewXmlElementWithValue("TotalIssuesCount", totalIssuesCount, root);
    (void)NewXmlElementWithValue("TotalSeverityScore", totalSeverityScore, root);

    printFileList(fileList, root);
    printIssueList(issueList, root);

    if (!doc.SaveFile(m_xmlfile.c_str())) {
        cerr << "Cannot write to " << m_xmlfile << endl;
        exit(EXIT_FAILURE);
    }
}

void XmlMetricsReporter::printFileList(const FILE_LIST& fileList, TiXmlElement* root)
{
    TiXmlElement* flist = new TiXmlElement("FileList");
    root->LinkEndChild(flist);

    int pos = 0;
    for (FILE_LIST::const_iterator iter = fileList.begin(); 
        iter != fileList.end();
        ++iter) {
        ++pos;

        TiXmlElement* file = new TiXmlElement("File");
        flist->LinkEndChild(file);

        int severestIssueNr = (*iter).getSeverestIssueNumber();
        int maxSeverity = IssueTable::getSeverity(severestIssueNr);

        (void)NewXmlElementWithValue("Name", iter->getFilename(), file);
        (void)NewXmlElementWithValue("Rank", pos, file);
        (void)NewXmlElementWithValue("Score", iter->getSeverityScore(), file);
        (void)NewXmlElementWithValue("Issues", iter->getIssueCount(), file);
        (void)NewXmlElementWithValue("MMsg", severestIssueNr, file);
        (void)NewXmlElementWithValue("MSev", maxSeverity, file);
    }
}

void XmlMetricsReporter::printIssueList(const ISSUE_LIST& issueList, TiXmlElement* root)
{
    TiXmlElement* ilist = new TiXmlElement("IssueList");
    root->LinkEndChild(ilist);

    int pos = 0;
    for (ISSUE_LIST::const_iterator iter = issueList.begin(); 
        iter != issueList.end();
        ++iter) {
        ++pos;

        TiXmlElement* issue = new TiXmlElement("Issue");
        ilist->LinkEndChild(issue);

        (void)NewXmlElementWithValue("Rank", pos, issue);
        (void)NewXmlElementWithValue("Msg", iter->getNumber(), issue);
        (void)NewXmlElementWithValue("Sev", iter->getSeverity(), issue);
        (void)NewXmlElementWithValue("Count", iter->getCount(), issue);
    }
}

