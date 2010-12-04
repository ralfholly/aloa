///////////////////////////////////////////////////////////////////////////////
// ALOA - A Lint Output Analyzer
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

#include <iostream>
#include <vector>
#include <map>
#include <algorithm>
#include <sstream>
#include <cassert>
#include <cstring>

#include "parse.h"
#include "globals.h"
#include "report.h"

using namespace std;

static void showVersion()
{
    cout << COPYRIGHT;
}

static void showHelp()
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
         << "Usage: aloa [command]" << endl
         << endl
         << "   -h, --help                      Shows help message" << endl
         << "   -v, --version                   Shows version information" << endl
         << "   -f <file>, --file <file>        Analyzes Lint ouput file (XML-formatted)" << endl
         << "   -x <file>, --xmlout <file>      Writes output to an XML file instead of stdout" << endl
         << endl;

    exit(0);
}

static const char* getArgOption(int argc, const char* argv[], const char* optShort,
        const char* optLong)
{
    // Argument not found.
    const char* opt = NULL;

    for (int i = 1; i < argc; ++i) {
        // If short or long option found.
        if ((optShort != NULL && strcmp(optShort, argv[i]) == 0)
                || (optLong != NULL && strcmp(optLong, argv[i]) == 0)) {

            // Peek at next command-line argument; if it does not start with a
            // dash, it's the option of our current argument.
            if (i < argc - 1 && *argv[i + 1] != '-') {
                opt = argv[i + 1];
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

static void scanCommandLine(int argc, const char* argv[])
{
    if (argc < 2 || getArgOption(argc, argv, "-h", "--help") != NULL) {
        showHelp();
    } else if (getArgOption(argc, argv, "-v", "--version") != NULL) {
        showVersion();
        exit(0);
    } else if ((gpLintOutputFile = getArgOption(argc, argv, "-f", "--file")) != NULL) {
        // Great! Got a lint output file...
    } else {
        showHelp();
    }
}

static void onNewIssueHandler(const char* pFilename, int number)
{
    int severity = getSeverity(number);

    // Update global metrics
    ++gIssuesCount;
    gSeverityScore += severity;

    // Obtain file object
    string filename(pFilename);
    File* pFile = 0;
    FILE_MAP::iterator iterFile = gFileMap.find(filename);

    // If unknown filename, create new file object
    if (iterFile == gFileMap.end()) {
        pFile = new File(filename);
        bool wasInserted = gFileMap.insert(make_pair(filename, pFile)).second;
        assert(wasInserted);
        // If known filename, retrieve existing file object
    } else {
        pFile = (*iterFile).second;
    }

    // Obtain issue object
    Issue* pIssue = 0;
    ISSUE_MAP::iterator iterIssue = gIssueMap.find(number);

    // If unknown issue, create new issue object
    if (iterIssue == gIssueMap.end()) {
        pIssue = new Issue(number, severity);
        bool wasInserted = gIssueMap.insert(make_pair(number, pIssue)).second;
        assert(wasInserted);
        // If known issue, retrieve existing issue object
    } else {
        pIssue = (*iterIssue).second;
    }

    // Update file/issue metrics
    pFile->addIssue(pIssue);
    pIssue->addFile(pFile);
}

static void buildMetricsLists()
{
    // Create sorted file list
    FILE_MAP::iterator iterFile = gFileMap.begin();
    for (; iterFile != gFileMap.end(); ++iterFile) {
        gFileList.push_back(*(*iterFile).second);
    }
    sort(gFileList.begin(), gFileList.end());

    // Create sorted issue list
    ISSUE_MAP::iterator iterIssue = gIssueMap.begin();
    for (; iterIssue != gIssueMap.end(); ++iterIssue) {
        gIssueList.push_back(*(*iterIssue).second);
    }
    sort(gIssueList.begin(), gIssueList.end());
}

int main(int argc, const char* argv[])
{
    try {
        initGlobals();
        scanCommandLine(argc, argv);
        parseLintOutputFile(gpLintOutputFile, &onNewIssueHandler);
        buildMetricsLists();
        reportMetrics();
    } catch (const ParseError& e) {
        reportFatalError("Parse error: " + e.getMessage());
    } catch (...) {
        reportFatalError("Unspecified fatal error");
    }

    return gIssuesCount == 0 ?
           EXIT_SUCCESS :
           EXIT_FAILURE;
}
