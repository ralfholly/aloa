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

#ifndef _globals_h
#define _globals_h

#include <string>
#include <vector>
#include <map>
#include <assert.h>

#include "File.h"
#include "Issue.h"

const std::string VERSION   = "4.1.0"; 
const std::string YEAR      = "2010";
const std::string COPYRIGHT = "ALOA version " + VERSION + 
    ". Copyright " + YEAR + " by Ralf Holly.\n";
const std::string COPYLEFT  =
    "This program is free software according to the GNU\n"
    "General Public License and comes WITHOUT ANY WARRANTY.\n"
    "For details, run aloa without arguments.\n";



#endif // _globals_h

