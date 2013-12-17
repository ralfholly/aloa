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

/** MisraParser -- header file.
 * @file
 */

#ifndef MisraParser_h
#define MisraParser_h

#include "MisraStringIssue.h"

#include <map>
#include <string>


class MisraParser {
public:

    MisraParser();
    ~MisraParser() throw();
    int parseMisraRule(const char* text, const char**  after);
    const MISRA_STRING_ISSUE_LIST* getMisraStringIssueList() const;

private:
    typedef std::map<std::string, int> MISRA_ISSUE_BY_STRING_MAP;

    MisraParser(const MisraParser& rhs);
    MisraParser& operator=(const MisraParser&);

    int m_nextVirtualIssueNumber;
    MISRA_ISSUE_BY_STRING_MAP m_misraIssuebyStringMap;
    MISRA_STRING_ISSUE_LIST m_misraStringIssueList;
};

#endif

