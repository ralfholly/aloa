///////////////////////////////////////////////////////////////////////////////
// ALOA - A Lint Output Analyzer
// Copyright (c) 2010 by Ralf Holly.
// Written by Ralf Holly (rholly@pera-software.com)
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
#include <assert.h>

#include "parse.h"
#include "globals.h"
#include "report.h"

using namespace std;


static void showHelp() {
    cerr << endl
         << COPYRIGHT
         << endl
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
         << "Usage: aloa [filename]" << endl
         << endl
         << "filename - Lint output file that contains all the lint issues of a project" 
         << endl;
    exit(EXIT_FAILURE);
}

static void scanCommandLine(int argc, const char* const argv[]) {
    if (argc < 2)
        showHelp();    
    // Command line parameter 1 is name of the lint output file
    gpLintOutputFile = argv[1];   
}

static void onNewIssueHandler(const char* pFilename, int number) {
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

static void buildMetricsLists() {
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

int main(int argc, const char* const argv[]) {
    static const int EXIT_ALOA_ISSUES = -1;

    try {
        initGlobals();
        scanCommandLine(argc, argv);
        parseLintOutputFile(gpLintOutputFile, &onNewIssueHandler);
        buildMetricsLists();
        reportMetrics();
    } catch (const ParseFileNotFoundError& e) {
        reportFatalError("Cannot access " + e.getFilename());
    } catch (const ParseMalformedLineError& e) {
        ostringstream s;
        s << "Malformed file: " << e.getFilename() << ", line: " << e.getLineNo();
        reportFatalError(s.str());
    } catch (...) {
        reportFatalError("Unspecified fatal error");        
    }

	return gIssuesCount == 0 ?
        EXIT_SUCCESS :
        EXIT_ALOA_ISSUES;
}
