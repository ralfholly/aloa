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

/** IssueTable -- header file. 
 * @file
 */

#ifndef IssueTable_h
#define IssueTable_h

#include <cstddef>

/** Encapsulates a table of PC-Lint issue categories. */
class IssueTable
{
public:
    /** Virtual issue number space, start. */
    static const int VIRTUAL_ISSUE_LOWER_BOUND = 1000000;
    /** Virtual issue number space, end. */
    static const int VIRTUAL_ISSUE_UPPER_BOUND = 1999999;
    /** Common serverity of all virtual issues.. */
    static const int VIRTUAL_ISSUE_SEVERITY = 1;

    /** Returns size of this table.
     * @return Number of table rows. 
     */
    static size_t getIssueTableSize();

    /** Looks-up severity by number. 
     * @param number Issue number.
     * @return Severity of given number.
     */
    static int getSeverity(int number);

    /** Returns severity of a given table row.
     * @param i table row.
     * @return Severety of given row.
     */
    static int getIssueTableSeverityByIndex(size_t i);

    /** Returns category title of a given table row.
     * @param i table row.
     * @return Category title of given row.
     */
    static const char* getIssueTableTitleByIndex(size_t i);

private:
    struct IssueTableEntry
    {
        const char* title;
        int severity;
        int lowerBound;
        int upperBound;
    };
    static const IssueTableEntry ISSUE_TABLE[]; 
};

#endif

