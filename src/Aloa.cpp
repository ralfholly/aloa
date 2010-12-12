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

#include "Aloa.h"
#include "ClassicMetricsReporter.h"
#include "XmlMetricsReporter.h"
#include "tinyxml/tinyxml.h"

#include <iostream>
#include <vector>
#include <map>
#include <algorithm>
#include <sstream>
#include <cassert>
#include <cstring>
#include <sstream>

using namespace std;

Aloa::Aloa(int argc, const char* argv[]) : 
    m_argc(argc), 
    m_argv(argv), 
    m_lintOutputFile(""),
    m_metricsBuilder(),
    m_xmlOutputFile("") 
{
    scanCommandLine();
    parseLintOutputFile();
    MetricsReporter* reporter;
    if (m_xmlOutputFile.empty()) {
        reporter = new ClassicMetricsReporter();
    } else {
        reporter = new XmlMetricsReporter(m_xmlOutputFile);
    }
    m_metricsBuilder.buildMetricsLists(reporter);
    delete reporter;
}

int Aloa::getIssuesCount() const
{
    return m_metricsBuilder.getIssuesCount();
}

void Aloa::showVersion() const
{
    cout << COPYRIGHT;
}

void Aloa::showHelp() const
{
    showVersion();

    cout << endl
         << "This program is free software; you can redistribute it and/or" << endl
         << "modify it under the terms of the GNU General Public License" << endl
         << "as published by the Free Software Foundation; either version 2" << endl
         << "of the License, or (at your option) any later version." << endl
         << endl
         << "This program is distributed in the hope that it will be useful," << endl
         << "but WITHOUT ANY WARRANTY; without even the implied warranty of" << endl
         << "MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the" << endl
         << "GNU General Public License for more details." << endl
         << endl
         << "You should have received a copy of the GNU General Public License" << endl
         << "along with this program; if not, write to the Free Software" << endl
         << "Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA." << endl
         << endl
         << "Usage: aloa <command> [option...]" << endl
         << endl
         << "command:" << endl
         << "   -h, --help                      Shows help message" << endl
         << "   -v, --version                   Shows version information" << endl
         << "   -f <file>, --file <file>        Analyzes Lint ouput file (XML-formatted)" << endl
         << endl
         << "option:" << endl
         << "   -x <file>, --xmlout <file>      Writes output to an XML file instead of stdout" << endl
         << endl;

    exit(0);
}

const char* Aloa::getArgOption(const char* optShort, const char* optLong) const
{
    // Argument not found.
    const char* opt = NULL;

    for (int i = 1; i < m_argc; ++i) {
        // If short or long option found.
        if ((optShort != NULL && strcmp(optShort, m_argv[i]) == 0)
                || (optLong != NULL && strcmp(optLong, m_argv[i]) == 0)) {

            // Peek at next command-line argument; if it does not start with a
            // dash, it's the option of our current argument.
            if (i < m_argc - 1 && *m_argv[i + 1] != '-') {
                opt = m_argv[i + 1];
            }
            // Otherwise, we don't have an option for our current argument.
            else {
                opt = "";
            }
            break;
        }
    }

    return opt;
}

void Aloa::scanCommandLine()
{
    const char* opt = NULL;

    if (m_argc < 2 || getArgOption("-h", "--help") != NULL) {
        showHelp();
    } else if (getArgOption("-v", "--version") != NULL) {
        showVersion();
        exit(0);
    } else if ((opt = getArgOption("-f", "--file")) != NULL &&
        *opt != '\0') {
        // Great! Got a lint output file...
        m_lintOutputFile = opt;

        // Write output to XML file instead of stdout?
        if ((opt = getArgOption("-x", "--xmlout")) != NULL) {
            if (*opt == '\0') {
                showHelp();
            }
            m_xmlOutputFile = opt;
        }
    } else {
        showHelp();
    }
}

// Parses a lint output file (pFilename).
// If an lint issue is encountered, calls back on pfHandler.
void Aloa::parseLintOutputFile()
{
    TiXmlDocument doc(m_lintOutputFile.c_str());

    if (!doc.LoadFile()) {
        throwXmlParseError(&doc, doc.ErrorDesc());
    }

    TiXmlNode *root = 0;
    TiXmlElement *messageElement = 0;

    root = doc.FirstChild("doc");
    messageElement = root->FirstChildElement("message");

    while (messageElement != 0) {
        TiXmlElement *fileElement, *codeElement;

        // Get filename from 'file' element.
        fileElement = messageElement->FirstChildElement("file");
        if (fileElement == 0) {
            throwXmlParseError(fileElement, "'file' element not found");
        }
        const char *filename = fileElement->GetText();
        if (filename == 0 || *filename == '\0') {
            filename = "<unknown>";
        }

        // Get issue number from 'code' element.
        codeElement = fileElement->NextSiblingElement("code");
        if (fileElement == 0) {
            throwXmlParseError(codeElement, "'code' element not found");
        }
        const char *number = codeElement->GetText();
        if (number == 0) {
            throwXmlParseError(codeElement, "'code' value test missing");
        }

        int issueNumber = atoi(number);
        m_metricsBuilder.onNewIssue(filename, issueNumber);
        messageElement = messageElement->NextSiblingElement();
    }
}

void Aloa::throwXmlParseError(const TiXmlBase *xmlbase, const std::string &desc)
{
    std::ostringstream ost;
    ost << desc << " row: " << xmlbase->Row() << " col: " << xmlbase->Column();
    throw ParseError(ost.str());
}



