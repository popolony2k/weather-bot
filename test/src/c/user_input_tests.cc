/*
 * Copyright (c) since 2025 by PopolonY2k and Leidson Campos A. Ferreira
 * 
 * This software is provided 'as-is', without any express or implied
 * warranty. In no event will the authors be held liable for any damages
 * arising from the use of this software.
 *
 * Permission is granted to anyone to use this software for any purpose,
 * including commercial applications, and to alter it and redistribute it
 * freely, subject to the following restrictions:
 *
 * 1. The origin of this software must not be misrepresented; you must not
 * claim that you wrote the original software. If you use this software
 * in a product, an acknowledgment in the product documentation would be
 * appreciated but is not required.
 * 2. Altered source versions must be plainly marked as such, and must not be
 * misrepresented as being the original software.
 * 3. This notice may not be removed or altered from any source distribution.
 */

#include "user_input_tests.h"
#include "user_input.h"

TEST(weather_bot_tests, parse_user_input_temperature_using_single_location_name) {
    
    struct UserInputResult  result;
    char   commands[6][MAX_BUFFER]  = { 
                             "/temperature Taubate", 
                             "/temperature Pindamonhangaba",
                             "/temperature Corumba",
                             "/temperature Barcelona",
                             "/temperature Tokio",
                             "/temperature Olimpia"
                           };
    char   expected[6][MAX_BUFFER] = {
                            "Taubate",
                            "Pindamonhangaba",
                            "Corumba",
                            "Barcelona",
                            "Tokio",
                            "Olimpia"
                          };

    for( int count = 0; count < 6; count++ )  {
        parse_user_input( commands[count], &result );
        EXPECT_STREQ ( expected[count], result.s_user_input );
    }
}
 
TEST(weather_bot_tests, parse_user_input_temperature_using_composed_names) {
    
    struct UserInputResult  result;
    char   commands[6][MAX_BUFFER]  = { 
                             "/temperature Sao Paulo", 
                             "/temperature Sao Jose dos Campos",
                             "/temperature Thermas dos Laranjais",
                             "/temperature Mato Grosso do Sul",
                             "/temperature New York",
                             "/temperature Santo Antonio do Pinhal"
                           };
    char   expected[6][MAX_BUFFER] = {
                            "Sao Paulo",
                            "Sao Jose dos Campos",
                            "Thermas dos Laranjais",
                            "Mato Grosso do Sul",
                            "New York",
                            "Santo Antonio do Pinhal"
                          };

    for( int count = 0; count < 6; count++ )  {
        parse_user_input( commands[count], &result );
        EXPECT_STREQ ( expected[count], result.s_user_input );
    }
}

TEST(weather_bot_tests, parse_user_input_temperature_invalid_location) {
    
    struct UserInputResult  result;
    char   command[]  = "/temperature";

    parse_user_input( command, &result );
    
    EXPECT_STREQ ( INVALID_LOCATION, result.s_user_input );
}

TEST(weather_bot_tests, parse_user_input_temperature_by_coordinate_success) {
    
    struct UserInputResult  result;
    char   commands[6][MAX_BUFFER]  = { 
                             "/temp_by_coordinate 22.15 23.1", 
                             "/temp_by_coordinate 19 2",
                             "/temp_by_coordinate 12.1 10",
                             "/temp_by_coordinate 22.10 12",
                             "/temp_by_coordinate 1 1",
                             "/temp_by_coordinate 10.1 10.20"
                           };
    float   expected[6][2] = {
                               { 22.15, 23.1 },
                               { 19.0, 2.0   },
                               { 12.1, 10.0  },
                               { 22.10, 12.0 },
                               { 1.0, 1.0    },
                               { 10.1, 10.20 }
                             };

    for( int count = 0; count < 6; count++ )  {
        parse_user_input( commands[count], &result );
        EXPECT_EQ ( expected[count][0], result.f_lat );
        EXPECT_EQ ( expected[count][1], result.f_lon );
    }
}

TEST(weather_bot_tests, parse_user_input_temperature_by_coordinate_invalid_coordinates) {
    
    struct UserInputResult  result;
    char   command[]  = "/temp_by_coordinate";

    parse_user_input( command, &result );
    
    EXPECT_STREQ ( INVALID_COORDINATES, result.s_user_input );
}

TEST(weather_bot_tests, parse_user_input_temperature_by_coordinate_invalid_coordinates_incomplete_input) {
    
    struct UserInputResult  result;
    char   command[]  = "/temp_by_coordinate 12.1";

    parse_user_input( command, &result );
    
    EXPECT_STREQ ( INVALID_COORDINATES, result.s_user_input );
}

TEST(weather_bot_tests, parse_user_input_temperature_by_coordinate_invalid_coordinates_invalid_latitude) {
    
    struct UserInputResult  result;
    char   command[]  = "/temp_by_coordinate junk 12.1";

    parse_user_input( command, &result );
    
    EXPECT_STREQ ( INVALID_COORDINATES, result.s_user_input );
}

TEST(weather_bot_tests, parse_user_input_temperature_by_coordinate_invalid_coordinates_invalid_longitude) {
    
    struct UserInputResult  result;
    char   command[]  = "/temp_by_coordinate 12.3 junk";

    parse_user_input( command, &result );
    
    EXPECT_STREQ ( INVALID_COORDINATES, result.s_user_input );
}
