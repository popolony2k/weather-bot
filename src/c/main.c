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

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>
#include <unistd.h>
#include <telebot.h>
#include "weatherrs_lib.h"
#include "user_input.h"

#define SIZE_OF_ARRAY(array) (sizeof(array) / sizeof(array[0]))


/**
 * Main application.
 * @param argv[1] open meteo api key;
 * @param argv[2] telegram bot token key;
 */
int main(int argc, char *argv[])
{
    telebot_handler_t      handle;
    telebot_user_t         me;
    struct WeatherRequest  req;
    struct WeatherResponse resp;
    char                   *telegram_bot_token;

    printf("Welcome to Echobot\n");

    if( argc < 3 )  {
        perror( "Invalid number of arguments. 1st argument open-meteo key, 2nd argument Telegram bot token." );
        return EXIT_FAILURE;
    }

    req.api_key = argv[1];
    telegram_bot_token = argv[2];

    // Allocate buffer needed to receive data from Rust
    resp.error       = malloc( sizeof( char ) * MAX_BUFFER );
    resp.temperature = malloc( sizeof( char ) * MAX_BUFFER );
    resp.error[0]    = 0;
    
    if (telebot_create(&handle, telegram_bot_token) != TELEBOT_ERROR_NONE)
    {
        printf("Telebot create failed\n");
        return EXIT_FAILURE;
    }

    if (telebot_get_me(handle, &me) != TELEBOT_ERROR_NONE)
    {
        printf("Failed to get bot information\n");
        telebot_destroy(handle);
        return EXIT_FAILURE;
    }

    printf("ID: %d\n", me.id);
    printf("First Name: %s\n", me.first_name);
    printf("User Name: %s\n", me.username);

    telebot_put_me(&me);

    int index, count, offset = -1;
    telebot_error_e ret;
    telebot_message_t message;
    telebot_update_type_e update_types[] = {TELEBOT_UPDATE_TYPE_MESSAGE};

    while (1)
    {
        telebot_update_t *updates;

        ret = telebot_get_updates(handle, offset, 20, 0, update_types, 0, &updates, &count);
        
        if (ret != TELEBOT_ERROR_NONE)
            continue;

        printf("Number of updates: %d\n", count);
        
        for (index = 0; index < count; index++)
        {
            message = updates[index].message;
            
            if (message.text)
            {
                struct UserInputResult  result;
                char                    msg[MAX_BUFFER*2];

                printf("%s: %s \n", message.from->first_name, message.text);

                parse_user_input( message.text, &result );

                switch ( result.result )  {
                    case INPUT_RESULT_LOCATION:
                        req.place_name = result.s_user_input;
                        break;

                    case INPUT_RESULT_COORDINATE:
                        req.lat = result.f_lat;
                        req.lon = result.f_lon;
                        break;

                    case INPUT_RESULT_ERROR:
                        sprintf( msg, "Got an error due user input error: %s", result.s_user_input );
                        break;
                }

                // Format output
                if( result.result != INPUT_RESULT_ERROR )  {
                    resp.error[0] = '\0';
                    request_weather( req, resp );

                    if( strlen(resp.error) > 0 )  {
                        sprintf( msg, "Got an error due: %s", resp.error );
                    }
                    else  {
                        if( result.result == INPUT_RESULT_LOCATION )
                            sprintf( msg, "(%s) temperature: %s", req.place_name, resp.temperature );
                        else
                            sprintf( msg, "(%f, %f) temperature: %s", req.lat, req.lon, resp.temperature );
                    }
                }

                ret = telebot_send_message(handle, message.chat->id, msg, "HTML", false, false, updates[index].message.message_id, "");

                if (ret != TELEBOT_ERROR_NONE)
                {
                    printf("Failed to send message: %d \n", ret);
                }
            }
            offset = updates[index].update_id + 1;
        }
        telebot_put_updates(updates, count);

        sleep(1);
    }

    telebot_destroy(handle);

    free( resp.error );
    free( resp.temperature );

    return EXIT_SUCCESS;
}