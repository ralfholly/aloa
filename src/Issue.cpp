
#include "Issue.h"
#include <cassert>

using namespace std;

void Issue::addFile(const string& filename) 
{
    m_files.push_back(filename);
    ++m_count;
}
