# ALOA -- A Lint Output Analyzer.
ALOA generates useful metrics from PC-Lint output which are helpful for policy fine-tuning and prioritizing of refactoring activities. It's a GNU C++ remake of the original [ALOA tool](http://www.drdobbs.com/184401810).

## History
```
4.0.0  2010-12-04   NEW   Initial release.
4.1.0  2010-12-26   NEW   XML output, major refactoring, Doxygen documentation,
                          make test, make todo, make doxy.
4.1.1  2011-01-16   FIX   Lint issues with numbers >= 1000 didn't work.
4.2.0  2011-01-28   NEW   PC-Lint support ('make lint' etc.)
                    NEW   Cleanup of lint issues
                    FIX   Minor bugfixes (exit code was not 0 if no lint issues
                          were found, handling of malformed lint output files.)
                    NEW   'make deploy'
4.2.1  2011-10-09   NEW   Support for release builds via 'make TARGET=RELEASE'.
                    NEW   tools/gcclint: support for Cygwin.
                    NEW   Minor Lint policy improvements.
4.2.2  2013-08-08   FIX   Updated version info
4.3.0  2013-11-25   NEW   Support for multiple lint output files (-f may appear
                          more than once).
4.4.0  2013-12-17   CNG   Correction to 4.3.0: -f may not appear more than once
                          anymore. Any additional lint output files must come
                          directly after first lint output file.
                    NEW   Support for fine-grained reports of MISRA issues (-m)
                          through so-called "virtual issues".
4.5.0  2013-12-30   NEW   Merged gcclint and g++lint. Now there is only gcclint
                          and it supports both, C and C++.
4.6.0  2014-03-22   NEW   gcclint: Suppression of issues coming from 
                          lint_cmac.h/ lint_cppmac.h.
                          Added build target COVERAGE to Makefile:
                          make TARGET=COVERAGE
                          Added 'gcov_report' and 'lcov_report' targets to 
                          Makefile.
4.7.0  2014-11-08   NEW   Upgraded gcclint to support PC-lint versions >= 9.00L
4.8.0  2015-02-14   NEW   Upgraded gcclint to automatically mark gcc include file
                          directories as library header directories (+libdir)
4.8.1  2015-11-26   FIX   Report error if lint output file has a root element
                          different to <doc>.
                          Help text now stresses that lint output XML file must
                          be created via env-xml.lnt.
4.8.2  2017-07-04   FIX   When XML output is selected (--xmlout) no legend of
                          virtual MISRA issues was appended to the output XML
                          file (Thanks, Peut!)
4.9.0  2019-03-29   NEW   Support for PC-lint Plus (additional messages and
                          message types).
                          Note option --misra only applicable for pre PC-lint
                          Plus editions of PC-lint (i. e. PC-lint 9.x).
```
## Usage

* Do a PC-Lint run and use Gimpel's env-xml.lnt file to produce XML-formatted
  output.
* Redirect standard output to a file, e. g. output.xml.
* Run ALOA and provide the output file as an argument, eg. `aloa -f output.xml`

For further information consult the ALOA help, eg.` aloa --help`

## Subfolders
legacy/ 
    Contains the original version of ALOA for Win32 as well as the
    corresponding DDJ article.

tools/
    Contains various PC-Lint-related utilities; see tools/README for details.
