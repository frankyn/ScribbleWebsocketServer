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

#include "mongo/db/geo/geojsonparser.h"
#include "third_party/s2/s2.h"
#include "third_party/s2/s2regioncoverer.h"
#include "third_party/s2/s2cell.h"
#include "third_party/s2/s2polyline.h"
#include "third_party/s2/s2polygon.h"
#include "third_party/s2/s2regioncoverer.h"

#pragma once

namespace mongo {
    // Used for passing geo data from the newCursor entry point to the S2Cursor class.
    struct GeoQueryField {
        GeoQueryField(const string& f) : field(f), cell(NULL), line(NULL), polygon(NULL) { }

        // Name of the field in the query.
        string field;
        // Only one of these should be non-NULL.  S2Region is a superclass but it only supports
        // testing against S2Cells.  We need the most specific class we can get.
        // Owned by S2Cursor.
        S2Cell *cell;
        S2Polyline *line;
        S2Polygon *polygon;
        
        // The functions below are defined in s2cursor.cpp.

        // Does this GeoQueryField intersect the provided data?  Sadly there is no common good way
        // to check this, so we do different things for all query/data pairs.
        bool intersectsPoint(const S2Cell &otherPoint);
        bool intersectsLine(const S2Polyline& otherLine);
        bool intersectsPolygon(const S2Polygon& otherPolygon);
        // One region is not NULL and this returns it.
        const S2Region& getRegion() const;
        // Delete the not NULL region.
        void free();
    };

    struct S2IndexingParams {
        // Since we take the cartesian product when we generate keys for an insert,
        // we need a cap.
        size_t maxKeysPerInsert;
        // This is really an advisory parameter that we pass to the cover generator.  The
        // finest/coarsest index level determine the required # of cells.
        int maxCellsInCovering;
        // What's the finest grained level that we'll index?  When we query for a point
        // we start at that -- we index nothing finer than this.
        int finestIndexedLevel;
        // And, what's the coarsest?  When we search in larger coverings we know we
        // can stop here -- we index nothing coarser than this.
        int coarsestIndexedLevel;

        string toString() const {
            stringstream ss;
            ss << "maxKeysPerInsert: " << maxKeysPerInsert << endl;
            ss << "maxCellsInCovering: " << maxCellsInCovering << endl;
            ss << "finestIndexedLevel: " << finestIndexedLevel << endl;
            ss << "coarsestIndexedLevel: " << coarsestIndexedLevel << endl;
            return ss.str();
        }

        void configureCoverer(S2RegionCoverer *coverer) const {
            coverer->set_min_level(coarsestIndexedLevel);
            coverer->set_max_level(finestIndexedLevel);
            // This is advisory; the two above are strict.
            coverer->set_max_cells(maxCellsInCovering);
        }
    };
}  // namespace mongo
