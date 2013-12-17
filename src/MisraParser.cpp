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

/** MisraParser -- implementation file.
 * @file
 */

#include "MisraParser.h"
#include "IssueTable.h"
#include <cstring>

using namespace std;

MisraParser::MisraParser() : 
    m_nextVirtualIssueNumber(IssueTable::VIRTUAL_ISSUE_LOWER_BOUND),
    m_misraIssuebyStringMap(),
    m_misraStringIssueList()
    { }

MisraParser::~MisraParser() throw()
{
}

static const char* findCharInString(const char* str, char c, const char* strEnd)
{
    for (;;) {
        if (str >= strEnd) {
            return NULL;
        }
        if (*str == c) {
            break;
        }
        ++str;
    }

    return str;
}

int MisraParser::parseMisraRule(const char* text, const char**  after) 
{
    const char* DIRECT_MISRA_START_TAG = "Violates MISRA ";
    const char DIRECT_MISRA_END_CHAR = ',';
    const char* REFERRED_MISRA_START_TAG = "[MISRA ";
    const char REFERRED_MISRA_END_CHAR = ']';

    if (text == NULL) {
        return -1;
    }

    const char* const textEnd = text + strlen(text);

    string misraRule("");

    // First, check if it is a 'direct' MISRA message (that is, 1960 or 960...).
    if (strstr(text, DIRECT_MISRA_START_TAG) == text) {
        // If found.
        const char* p =  text + strlen(DIRECT_MISRA_START_TAG);
        p = findCharInString(p, DIRECT_MISRA_END_CHAR, textEnd);
        // There is only one MISRA rule referred; do not search further.
        *after = NULL;
        if (p != NULL) {
            misraRule = string(text, static_cast<size_t>(p - text));
        }

    // Otherwise check if a regular message refers to a MISRA message.
    } else {
        // If referred start tag found.
        const char* start = strstr(text, REFERRED_MISRA_START_TAG);
        if (start != NULL) {
            // If found.
            const char* p =  start + strlen(REFERRED_MISRA_START_TAG);
            p = findCharInString(p, REFERRED_MISRA_END_CHAR, textEnd);
            if (p != NULL) {
                ++p;
                // There might be more referred MISRA messages.
                *after = p;
                misraRule = string(start, static_cast<size_t>(p - start));
            }
        }
    }

    // If a MISRA rule has been found in a message.
    if (!misraRule.empty()) {
        MISRA_ISSUE_BY_STRING_MAP::iterator iter = m_misraIssuebyStringMap.find(misraRule);
        // If this particular MISRA rule has been registered already.
        if (iter != m_misraIssuebyStringMap.end()) {
            return (*iter).second;
        // Otherwise, register it.
        } else {
            m_misraIssuebyStringMap[misraRule] = m_nextVirtualIssueNumber;
            MISRA_STRING_ISSUE stringIssue = { misraRule, m_nextVirtualIssueNumber };
            m_misraStringIssueList.push_back(stringIssue);
            return m_nextVirtualIssueNumber++;
        }
    }

    // Not found.
    return -1;
}

const MISRA_STRING_ISSUE_LIST* MisraParser::getMisraStringIssueList() const 
{
    return &m_misraStringIssueList;
}

