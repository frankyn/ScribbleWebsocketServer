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

/**
 * This file contains tests for mongo/db/geo/geojson_parser.cpp.
 */

#include <string>
#include <sstream>

#include "mongo/db/geo/geojsonparser.h"
#include "mongo/db/json.h"
#include "mongo/unittest/unittest.h"
#include "mongo/util/assert_util.h"

#include "third_party/s2/s2.h"
#include "third_party/s2/s2polygon.h"
#include "third_party/s2/s2polyline.h"

//using std::string;
//using std::stringstream;
using mongo::GeoJSONParser;
using mongo::fromjson;

namespace {
    TEST(GeoJSONParser, isValidPoint) {
        ASSERT_TRUE(GeoJSONParser::isPoint(fromjson("{'type':'Point', 'coordinates': [40, 5]}")));
        ASSERT_TRUE(GeoJSONParser::isPoint(fromjson("{'type':'Point', 'coordinates': [-40.3, -5.0]}")));

        ASSERT_FALSE(GeoJSONParser::isPoint(fromjson("{'typo':'Point', 'coordinates': [40, -5]}")));
        ASSERT_FALSE(GeoJSONParser::isPoint(fromjson("{'type':'Point', 'coordhats': [40, -5]}")));
        ASSERT_FALSE(GeoJSONParser::isPoint(fromjson("{'type':['Point'], 'coordinates': [40, -5]}")));
        ASSERT_FALSE(GeoJSONParser::isPoint(fromjson("{'type':'Point', 'coordinates': 40}")));
        ASSERT_FALSE(GeoJSONParser::isPoint(fromjson("{'type':'Point', 'coordinates': [40, -5, 7]}")));
    }

    TEST(GeoJSONParser, isValidLineString) {
        ASSERT_TRUE(GeoJSONParser::isLineString(fromjson("{'type':'LineString', 'coordinates':[[1,2], [3,4]]}")));
        ASSERT_TRUE(GeoJSONParser::isLineString(fromjson("{'type':'LineString', 'coordinates':[[1,2], [3,4], [5,6]]}")));

        ASSERT_FALSE(GeoJSONParser::isLineString(fromjson("{'type':'LineString', 'coordinates':[[1,2]]}")));
        ASSERT_FALSE(GeoJSONParser::isLineString(fromjson("{'type':'LineString', 'coordinates':[['chicken','little']]}")));
        ASSERT_FALSE(GeoJSONParser::isLineString(fromjson("{'type':'LineString', 'coordinates':[1,2, 3, 4]}")));
        ASSERT_FALSE(GeoJSONParser::isLineString(fromjson("{'type':'LineString', 'coordinates':[[1,2, 3], [3,4, 5], [5,6]]}")));
    }

    TEST(GeoJSONParser, isValidPolygon) {
        ASSERT_TRUE(GeoJSONParser::isPolygon(fromjson("{'type':'Polygon', 'coordinates':[ [[0,0],[5,0],[5,5],[0,5],[0,0]] ]}")));
        // And one with a hole.
        ASSERT_TRUE(GeoJSONParser::isPolygon(fromjson("{'type':'Polygon', 'coordinates':[ [[0,0],[5,0],[5,5],[0,5],[0,0]],"
                                                                                        " [[1,1],[4,1],[4,4],[1,4],[1,1]] ]}")));
        // First point must be the same as the last.
        ASSERT_FALSE(GeoJSONParser::isPolygon(fromjson("{'type':'Polygon', 'coordinates':[ [[1,2],[3,4],[5,6]] ]}")));
    }

    TEST(GeoJSONParser, parsePoint) {
        S2Point point;
        GeoJSONParser::parsePoint(fromjson("{'type':'Point', 'coordinates': [40, 5]}"), &point);
        GeoJSONParser::parsePoint(fromjson("{'type':'Point', 'coordinates': [-40.3, -5.0]}"), &point);
    }

    TEST(GeoJSONParser, parseLineString) {
        S2Polyline polyline;
        GeoJSONParser::parseLineString(fromjson("{'type':'LineString', 'coordinates':[[1,2], [3,4]]}"), &polyline);
        GeoJSONParser::parseLineString(fromjson("{'type':'LineString', 'coordinates':[[1,2], [3,4], [5,6]]}"), &polyline);
    }

    TEST(GeoJSONParser, parsePolygon) {
        S2Point point;
        GeoJSONParser::parsePoint(fromjson("{'type':'Point', 'coordinates': [2, 2]}"), &point);

        S2Polygon polygonA;
        GeoJSONParser::parsePolygon(fromjson("{'type':'Polygon', 'coordinates':[ [[0,0],[5,0],[5,5],[0,5],[0,0]] ]}"), &polygonA);
        ASSERT_TRUE(polygonA.Contains(point));

        S2Polygon polygonB;
        GeoJSONParser::parsePolygon(fromjson("{'type':'Polygon', 'coordinates':[ [[0,0],[5,0],[5,5],[0,5],[0,0]],"
                                                                                        " [[1,1],[1,4],[4,4],[4,1],[1,1]] ]}"), &polygonB);
        // We removed this in the loop.
        ASSERT_FALSE(polygonB.Contains(point));

        // Now we reverse the orientations and verify that the code fixes it up (outer loop must be CCW, inner CW).
        S2Polygon polygonC;
        GeoJSONParser::parsePolygon(fromjson("{'type':'Polygon', 'coordinates':[ [[0,0],[0,5],[5,5],[5,0],[0,0]] ]}"), &polygonC);
        ASSERT_TRUE(polygonC.Contains(point));

        S2Polygon polygonD;
        GeoJSONParser::parsePolygon(fromjson("{'type':'Polygon', 'coordinates':[ [[0,0],[0,5],[5,5],[5,0],[0,0]],"
                                                                                        " [[1,1],[1,4],[4,4],[4,1],[1,1]] ]}"), &polygonD);
        // Also removed in the loop.
        ASSERT_FALSE(polygonD.Contains(point));
    }
}
