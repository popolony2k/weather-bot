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

#ifndef __USER_INPUT_H__
#define __USER_INPUT_H__

#define MAX_BUFFER          1024
#define INVALID_LOCATION    "invalid location"
#define INVALID_COORDINATES "invalid coordinates"
#define UNKNOWN_COMMAND     "unknown command"


#ifdef __cplusplus
extern "C" {
#endif

/**
 * User input type available.
 */
enum InputResult {
    INPUT_RESULT_LOCATION,
    INPUT_RESULT_COORDINATE,
    INPUT_RESULT_ERROR
};

/**
 * User input result data struct with parsed user input data.
 */
struct UserInputResult  {
    char             s_user_input[MAX_BUFFER];
    float            f_lat;
    float            f_lon;
    enum InputResult result;
};

void parse_user_input( char *message, struct UserInputResult *p_result );

#ifdef __cplusplus
}
#endif

#endif // __USER_INPUT_H__