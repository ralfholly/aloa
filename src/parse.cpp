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

#include "parse.h"

#include <assert.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sstream>

#include "tinyxml/tinyxml.h"


static void throwXmlParseError(const TiXmlBase *xmlbase, const std::string &desc);

// Parses a lint output file (pFilename). 
// If an lint issue is encountered, calls back on pfHandler.
void parseLintOutputFile(const char* pFilename, LINT_ISSUE_HANDLER pfHandler) {
    assert(pFilename != 0);
    assert(pfHandler != 0);

	TiXmlDocument doc(pFilename);

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

        (*pfHandler)(filename, issueNumber);

        messageElement = messageElement->NextSiblingElement();
    }
}

static void throwXmlParseError(const TiXmlBase *xmlbase, const std::string &desc)
{
    std::ostringstream ost;
    ost << desc << " row: " << xmlbase->Row() << " col: " << xmlbase->Column();
    throw ParseError(ost.str());
}




