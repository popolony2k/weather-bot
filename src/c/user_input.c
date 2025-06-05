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

#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include "user_input.h"

#define __TEMPERATURE_CMD               "/temperature"
#define __TEMPERATIRE_BY_COORDINATE_CMD "/temp_by_coordinate"


// Local function prototypes
enum InputResult __get_command( char *command );
void __parse_location( char *command, struct UserInputResult *p_result );


/**
 * Get the input result type by command passed by user.
 * Can be improved to use jump tables etc (for performance).
 * @param command The command to check;
 */
enum InputResult __get_command( char *command )  {

    if( strstr( command, __TEMPERATURE_CMD ) )
        return INPUT_RESULT_LOCATION;
    
    if( strstr( command, __TEMPERATIRE_BY_COORDINATE_CMD ) )
        return INPUT_RESULT_COORDINATE;

    return INPUT_RESULT_ERROR;
}

/**
 * Parse the location token.
 * @param command The command that will be parsed;
 * @param p_result The address of the @see UserInputResult struct which
 * location will be updated;
 */
void __parse_location( char *command, struct UserInputResult *p_result )  {

    char* token = strtok(command, " ");
                    
    if( token ) {
        p_result -> s_user_input[0] = '\0';
        token = strtok( NULL, " " );

        if( !token )  {
            strcpy( p_result -> s_user_input, INVALID_LOCATION );
            p_result -> result = INPUT_RESULT_ERROR;       
        }

        while( token )  {
            strcat( p_result -> s_user_input, token );
            token = strtok( NULL, " " );

            if( token )
                strcat( p_result -> s_user_input, " " );
        }

        p_result -> result = INPUT_RESULT_LOCATION;

        return;
    }

    strcpy( p_result -> s_user_input, INVALID_LOCATION );
    p_result -> result = INPUT_RESULT_ERROR;
}

/**
 * Parse the coordinate token.
 * @param command The command that will be parsed;
 * @param p_result The address of the @see UserInputResult struct which
 * coordinate will be updated;
 */
void __parse_coordinate( char *command, struct UserInputResult *p_result )  {
    
    char* token = strtok( command, " " );
                    
    if( token ) {
        char    *s_lat = NULL;
        char    *s_lon = NULL;

        if( ( token = strtok( NULL, " " ) ) )
            s_lat = token;

        if( ( token = strtok( NULL, " " ) ) )
            s_lon = token;                       

        if( s_lat && s_lon )  {
            char* end_ptr = NULL;

            p_result -> f_lat = strtof( s_lat, &end_ptr );

            if ( end_ptr != ( s_lat + strlen( s_lat ) ) )  {
                strcpy( p_result -> s_user_input, INVALID_COORDINATES );
                p_result -> result = INPUT_RESULT_ERROR;
                return;
            }

            p_result -> f_lon = strtof( s_lon, &end_ptr );

            if ( end_ptr != ( s_lon + strlen( s_lon ) ) )  {
                strcpy( p_result -> s_user_input, INVALID_COORDINATES );
                p_result -> result = INPUT_RESULT_ERROR;
                return;
            }

            p_result -> result = INPUT_RESULT_COORDINATE;
        }
        else  {
            p_result -> f_lat = 0.0;
            p_result -> f_lon = 0.0;
            strcpy( p_result -> s_user_input, INVALID_COORDINATES );
            p_result -> result = INPUT_RESULT_ERROR;
        }
    }
    else  {
        p_result -> f_lat = 0.0;
        p_result -> f_lon = 0.0;
        strcpy( p_result -> s_user_input, INVALID_COORDINATES );
        p_result -> result = INPUT_RESULT_ERROR;
    }
}

/**
 * Parse the message returning all parsed data into a @see UserInputResult.
 * @param message The message to parse;
 * @param p_result The @see UserInputResult data struct pointer that will 
 * receive parsed data;
 */
void parse_user_input( char *message, struct UserInputResult *p_result )  {
    struct UserInputResult   input;
    enum InputResult         result = __get_command( message );

    switch( result )  {
        case INPUT_RESULT_LOCATION:
            __parse_location(message, p_result);
            break;

        case INPUT_RESULT_COORDINATE:
            __parse_coordinate(message, p_result);
            break;

        case INPUT_RESULT_ERROR:
            strcpy( p_result -> s_user_input, UNKNOWN_COMMAND );
            p_result -> result = INPUT_RESULT_ERROR;
            break;
    }
}