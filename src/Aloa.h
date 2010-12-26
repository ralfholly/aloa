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

/** ALOA application object -- header file. 
 * @file
 */

#ifndef Aloa_h
#define Aloa_h

#include "MetricsBuilder.h"

#include <string>

/** ALOA version number. */
const std::string VERSION   = "4.1.0"; 
const std::string YEAR      = "2006 - 2010";
const std::string COPYRIGHT = "ALOA version " + VERSION + 
    ". Copyright " + YEAR + " by Ralf Holly.\n";
const std::string COPYLEFT  =
    "This program is free software according to the GNU\n"
    "General Public License and comes WITHOUT ANY WARRANTY.\n"
    "For details, run aloa without arguments.\n";

class TiXmlBase;

/** Parser error exception.
 * An instance of this class is thrown if the ALOA parser encounters an error.
 */
class ParseError
{
public:
    ParseError(const std::string &msg) : m_msg(msg) { }
    const std::string& getMessage() const { return m_msg; }
    virtual ~ParseError() { }
private:
    std::string m_msg;
};


/** ALOA application object.
 * This class is responsible for driving and implementing the ALOA
 * functionality.
 * After checking the command-line arguments, it parses the given Lint output
 * and hands over every issue to the MetricsBuilder. Once the metrics have been
 * collected, the results are reported via a subclass of MetricsReporter.
 */
class Aloa
{
public:
    /** Aloa constructor.
     * @param argc Number of command-line arguments.
     * @param argv Vector of command-line arguments.
     * @note The command-line arguments are exactly as received by the main()
     * entry point routine.
     */
    Aloa(int argc, const char* argv[]);

    /** Returns the total number of issues found in the Lint output file. */
    int getIssuesCount() const;

private:
    Aloa();
    Aloa(const Aloa& rhs);
    Aloa& operator=(const Aloa& rhs);

    void showVersion() const;
    void showHelp() const;
    const char* getArgOption(const char* optShort, const char* optLong) const;
    void scanCommandLine();
    void parseLintOutputFile();
    void throwXmlParseError(const TiXmlBase *xmlbase, const std::string &desc);

    int m_argc;
    const char** m_argv;
    std::string m_lintOutputFile;
    MetricsBuilder m_metricsBuilder;
    std::string m_xmlOutputFile;
};

#endif
