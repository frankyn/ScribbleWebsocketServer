/**
*    Copyright (C) 2012 10gen Inc.
*
*    This program is free software: you can redistribute it and/or  modify
*    it under the terms of the GNU Affero General Public License, version 3,
*    as published by the Free Software Foundation.
*
*    This program is distributed in the hope that it will be useful,
*    but WITHOUT ANY WARRANTY; without even the implied warranty of
*    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
*    GNU Affero General Public License for more details.
*
*    You should have received a copy of the GNU Affero General Public License
*    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef UTIL_VERSION_HEADER
#define UTIL_VERSION_HEADER

#include <string>

#include "mongo/base/string_data.h"

namespace mongo {
    struct BSONArray;

    // mongo version
    extern const char versionString[];
    extern const BSONArray versionArray;
    std::string mongodVersion();
    int versionCmp(StringData rhs, StringData lhs); // like strcmp

    const char * gitVersion();
    void printGitVersion();

    std::string sysInfo();
    void printSysInfo();

    void show_warnings();

}  // namespace mongo

#endif  // UTIL_VERSION_HEADER
