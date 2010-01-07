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

static const char ATTRIBUTE_VALUE_SEPARATOR = '=';

static int compareXMLAttribute(const char* pLine, const char* pAttribute, const char* p);
static void getXMLValue(const char* pLineEnd, const char* p, char* pValue);
static char* getXMLAttributeValue(const char* pLine, const char *pAttribute, char *pValue);

// Parses a lint output file (pFilename). 
// If an lint issue is encountered, calls back on pfHandler.
void parseLintOutputFile(const char* pFilename, LINT_ISSUE_HANDLER pfHandler) {
    assert(pFilename != 0);
    assert(pfHandler != 0);

    static char buffer[1024 + 1];
    static char currFilename[512 + 1];
    static char currNumber[100 + 1]; 
   
    int line = 0;
    int number = 0;

    // Open lint output file
    FILE* pFile = fopen(pFilename, "r");

    if (pFile == 0)
        throw ParseFileNotFoundError(pFilename);
    
    // Read next line
    while (fgets(buffer, sizeof(buffer), pFile) != 0) {
        ++line;
        // If new issue found
        if (strstr(buffer, "<issue")) {
            if (    (getXMLAttributeValue(buffer, "file", currFilename))
                 && (getXMLAttributeValue(buffer, "number", currNumber))
                 && ((number = atoi(currNumber)) > 0) ) {
                (*pfHandler)(currFilename, number); 
            } else
                throw ParseMalformedLineError(pFilename, line);
        }
    } 
}

static inline bool isXMLValueDelimiter(char c) {
    return c == '\'' || c == '\"';
}

static int compareXMLAttribute(const char* pLine, const char* pAttribute, const char* p) {
    assert(p > pLine);
    assert(*p != ATTRIBUTE_VALUE_SEPARATOR);

    // Eat whitespace
    while ((p > pLine) && isspace(*p))
        --p;
    // Go left until beginning of attribute name
    while ((p > pLine) && isalnum(*(p - 1)))
        --p;
    // Compare attribute with given attribute
    while (*pAttribute != 0) {
        if (*(p++) != *(pAttribute++))
            // Given attribute not found
            return 0;
    }

    // Given attribute found
    return 1;
}

static void getXMLValue(const char* pLineEnd, const char* p, char* pValue) {
    // Go to beginning of value
    while (((p < pLineEnd) && (isspace(*p) || isXMLValueDelimiter(*p))))
        ++p;

    const char valueDelim = *(p - 1);

    // Copy value to destination buffer
    while (p < pLineEnd && *p != valueDelim)
        *(pValue++) = *(p++);

    // Terminate value string in destination buffer
    *pValue = '\0';
}

static char* getXMLAttributeValue(const char* pLine, const char *pAttribute, char *pValue) {
    const char* pLineEnd = pLine + strlen(pLine);
    const char* p;

    assert(pLine != 0 && pAttribute != 0 && pValue != 0);
    assert(*pLine != 0 && *pAttribute != 0);

    // Scan trough given line
    for (p = pLine; p < pLineEnd; ++p) {

        // If given attribute has been found
        if (   *p == ATTRIBUTE_VALUE_SEPARATOR  
            && compareXMLAttribute(pLine, pAttribute, p - 1)) {
            // Return value that belongs to given attribute
            getXMLValue(pLineEnd, p + 1, pValue);
            return pValue;
        }
    }

    // Given attribute not found
    return 0;
}

