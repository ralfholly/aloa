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

/** ALOA application object -- implementation file. 
 * @file
 */

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
    m_lintOutputFiles(),
    m_metricsBuilder(),
    m_xmlOutputFile(""),
    m_reporter(NULL)
{
    scanCommandLine();
    parseLintOutputFile();
    if (m_xmlOutputFile.empty()) {
        m_reporter = new ClassicMetricsReporter;
    } else {
        m_reporter = new XmlMetricsReporter(m_xmlOutputFile);
    }
    m_metricsBuilder.reportMetrics(m_reporter);
}

Aloa::~Aloa() 
{
    delete m_reporter;
    m_argv = NULL;
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
        << "   -h, --help          Shows help message" << endl
        << "   -v, --version       Shows version information" << endl
        << "   -f, --file <file>   Analyzes Lint ouput file (XML-formatted)" << endl
        << "                       Can be specified multiple times to support multiple lint output files." << endl
        << endl
        << "option:" << endl
        << "   -x, --xmlout <file> Writes output to an XML file instead of stdout" << endl
        << endl;

    exit(0);
}

const char* Aloa::getArgOptionFromIndex(const char* optShort, const char* optLong, int* index) const
{
    // Argument not found.
    const char* opt = NULL;
    if (index != NULL && *index > 0) {

        for (int i = *index; i < m_argc; ++i) {
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
                // If called again, start search at this index
                *index = i + 1;
                break;
            }
        }
    }
    
    return opt;
}

void Aloa::scanCommandLine()
{
    const char* opt = NULL;

    int startIndex = 1;

    if (m_argc < 2 || getArgOptionFromIndex("-h", "--help", &startIndex) != NULL) {
        showHelp();
    } else if (getArgOptionFromIndex("-v", "--version", &startIndex) != NULL) {
        showVersion();
        exit(0);
    } else {
        // Read in all lint output files.
        while ((opt = getArgOptionFromIndex("-f", "--file", &startIndex)) != NULL && *opt != '\0') { 
            m_lintOutputFiles.push_back(string(opt));
        }

        if (m_lintOutputFiles.empty()) {
            showHelp();
        }

        // Write output to XML file instead of stdout?
        startIndex = 1;
        if ((opt = getArgOptionFromIndex("-x", "--xmlout", &startIndex)) != NULL) {
            if (*opt == '\0') {
                showHelp();
            }
            m_xmlOutputFile = opt;
        }
    }
}

// Parses a lint output file (pFilename).
// If an lint issue is encountered, calls back on pfHandler.
void Aloa::parseLintOutputFile()
{
    vector<string>::const_iterator iter = m_lintOutputFiles.begin();
    for (; iter != m_lintOutputFiles.end(); ++iter) {
        const char* lintOutputFile = iter->c_str();
        TiXmlDocument doc(lintOutputFile);

        if (!doc.LoadFile()) {
            throwXmlParseError(lintOutputFile, &doc, doc.ErrorDesc());
        }

        TiXmlNode* root = doc.FirstChild("doc");
        TiXmlElement* messageElement = root->FirstChildElement("message");

        while (messageElement != 0) {
            TiXmlElement* fileElement, *codeElement;

            // Get filename from 'file' element.
            fileElement = messageElement->FirstChildElement("file");
            if (fileElement == 0) {
                throwXmlParseError(lintOutputFile, root, "'file' element not found");
            }
            const char* filename = fileElement->GetText();
            if (filename == 0 || *filename == '\0') {
                filename = "<unknown>";
            }

            // Get issue number from 'code' element.
            codeElement = fileElement->NextSiblingElement("code");
            if (codeElement == 0) {
                throwXmlParseError(lintOutputFile, fileElement, "'code' element not found");
            }
            const char* number = codeElement->GetText();
            if (number == 0) {
                throwXmlParseError(lintOutputFile, codeElement, "'code' value missing");
            }

            // Get line number from 'line' element.
            codeElement = fileElement->NextSiblingElement("line");
            if (codeElement == 0) {
                throwXmlParseError(lintOutputFile, fileElement, "'line' element not found");
            }
            const char* line = codeElement->GetText();
            if (line == 0) {
                throwXmlParseError(lintOutputFile, codeElement, "'line' value missing");
            }

            m_metricsBuilder.onNewIssue(atoi(number), filename, atoi(line));
            messageElement = messageElement->NextSiblingElement();
        }
    }
}

void Aloa::throwXmlParseError(const char* filename, const TiXmlBase* xmlbase, const std::string& desc) const
{
    assert(filename != NULL);
    assert(xmlbase != NULL);
    std::ostringstream ost;
    ost << "File: " << filename << ", " << desc << " row: " << xmlbase->Row() << " col: " << xmlbase->Column();
    throw ParseError(ost.str());
}



