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

#ifndef _parse_h
#define _parse_h

#include <string>

//lint --e{1712} No default ctor needed
class ParseError {
public:
	ParseError(const std::string &msg) : m_msg(msg) {
	}

	const std::string& getMessage() const {
		return m_msg;
	}

    virtual ~ParseError() {
    }
private:
    std::string m_msg;
};

typedef void (*LINT_ISSUE_HANDLER)(const char* pFilename, int number);
void parseLintOutputFile(const char* pFilename, LINT_ISSUE_HANDLER pfHandler);


#endif //_parse_h
