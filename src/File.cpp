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

/** File -- implementation file.
 * @file
 */

#include "File.h"
#include "IssueTable.h"

#include <cassert>

const int File::UNUSED_ISSUE_NUMBER; 

void File::addIssue(int number, int line) 
{
    m_issues.push_back(FileIssue(number, line));
    int severity = IssueTable::getSeverity(number);
    if (   m_severestIssueNumber == UNUSED_ISSUE_NUMBER
            || severity > IssueTable::getSeverity(m_severestIssueNumber) ) {
        m_severestIssueNumber = number;
    }
    m_severityScore += severity;
}

