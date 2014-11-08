///////////////////////////////////////////////////////////////////////////////
// Copyright (c) 2006 - 2014 by Ralf Holly.
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
#include "IssueTable.h"

#include "tinyxml/tinyxml.h"

#include <iostream>
#include <vector>
#include <map>
#include <algorithm>
#include <sstream>
#include <cassert>
#include <cstring>
#include <sstream>
#include <iomanip>

using namespace std;

Aloa::Aloa(int argc, const char* argv[]) :
    m_argc(argc),
    m_argv(argv),
    m_lintOutputFiles(),
    m_misraEnabled(false),
    m_metricsBuilder(m_misraEnabled),
    m_xmlOutputFile(""),
    m_reporter(NULL),
    m_misraParser(NULL)
{
    scanCommandLine();
    if (m_misraEnabled) {
        m_misraParser = new MisraParser();
    }
    parseLintOutputFile();
    if (m_xmlOutputFile.empty()) {
        m_reporter = new ClassicMetricsReporter;
    } else {
        m_reporter = new XmlMetricsReporter(m_xmlOutputFile);
    }

    m_metricsBuilder.reportMetrics(m_reporter, m_misraParser);
}

Aloa::~Aloa() 
{
    delete m_reporter;
    delete m_misraParser;
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
        << "   -h, --help             Shows help message" << endl
        << "   -v, --version          Shows version information" << endl
        << "   -f, --file <file>...   Analyzes Lint ouput file(s) (XML-formatted)" << endl
        << endl
        << "option:" << endl
        << "   -x, --xmlout <file>    Writes output to an XML file instead of stdout" << endl
        << "   -m, --misra            Support for MISRA rules" << endl
        << endl;

    exit(0);
}

const char* Aloa::getArgOption(const char* optShort, const char* optLong) const
{
    static int nextArg = 0;

    // Option not found.
    const char* opt = NULL;

    // If in resume with next option value.
    if (optShort == NULL && optLong == NULL) {
        if (0 < nextArg && nextArg < m_argc) {
            if (*m_argv[nextArg] != '-') {
                return m_argv[nextArg++];
            }
        }

    // If start search from beginning.
    } else {
        int i;
        for (i = 1; i < m_argc; ++i) {
            // If short or long option found.
            if ((optShort != NULL && strcmp(optShort, m_argv[i]) == 0)
                || (optLong != NULL && strcmp(optLong, m_argv[i]) == 0)) {

                // Peek at next command-line argument; if it does not start with a
                // dash, it's the option value of our current option.
                if (i < m_argc - 1 && *m_argv[i + 1] != '-') {
                    opt = m_argv[i + 1];
                    // Potentially, there are more option values.
                    nextArg = i + 2;
                }
                // Otherwise, we don't have an option value for our current
                // option.
                else {
                    opt = "";
                }
                break;
            }
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
    } else {
        // Read in all lint output files.
        opt = getArgOption("-f", "--file");
        while (opt != NULL && *opt != '\0') {
            m_lintOutputFiles.push_back(string(opt));
            opt = getArgOption(NULL, NULL);
        }
        
        if (m_lintOutputFiles.empty()) {
            showHelp();
        }

        // Write output to XML file instead of stdout?
        if ((opt = getArgOption("-x", "--xmlout")) != NULL) {
            if (*opt == '\0') {
                showHelp();
            }
            m_xmlOutputFile = opt;
        }

        // Enable support for MISRA rules.
        m_misraEnabled = (getArgOption("-m", "--misra") != NULL) ? true : false;
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
            TiXmlElement *fileElement, *codeElement, *descElement;

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

            bool alreadyHandled = false;

            if (m_misraEnabled) {
                // Get description from 'desc' element.
                descElement = fileElement->NextSiblingElement("desc");
                if (descElement == 0) {
                    throwXmlParseError(lintOutputFile, fileElement, "'desc' element not found");
                }
                const char* desc = descElement->GetText();
                if (desc == 0) {
                    throwXmlParseError(lintOutputFile, descElement, "'desc' value missing");
                }

                const char* next;
                int virtualIssue;
                while ((virtualIssue = m_misraParser->parseMisraRule(desc, &next)) != -1) {
                    m_metricsBuilder.onNewIssue(virtualIssue, filename, atoi(line));
                    alreadyHandled = true;
                    desc = next;
                }
            }

            if (!alreadyHandled) {
                m_metricsBuilder.onNewIssue(atoi(number), filename, atoi(line));
            }

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



