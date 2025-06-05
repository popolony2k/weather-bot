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

extern crate open_meteo_api;

use std::error::Error;
use std::str;
use std::ffi::CStr;
use std::ffi::CString;
use std::ffi::c_float;
use std::os::raw::c_char;
use open_meteo_api::query::OpenMeteo;
use open_meteo_api::models::TimeZone;

/*
 * Weather resquest data struct.
 */
#[repr(C)]
pub struct WeatherRequest  {
    api_key : *mut c_char, 
    place_name : *mut c_char, 
    lat: c_float, 
    lon : c_float
}

/*
 * Weather response data struct.
 * All pointer are managed on C side; 
 */
#[repr(C)]
pub struct WeatherResponse {
    temperature : *mut c_char,
    error: *mut c_char,
}

/*
 * Function to interop from C to Rust.
 * Request data from open meteo service.
 * @param request The data struct with request data information;
 * @param response The response received from open meteo service;
 */
#[no_mangle]
#[tokio::main]
pub async extern "C" fn request_weather(request: WeatherRequest, response: WeatherResponse) {
    let error:*mut c_char = response.error;
    let ret = get_weather( request, response ).await;

    match ret {
        Ok(_resp) => {},    
        Err(err) =>
            unsafe {
                let str_err: CString = CString::new(err.to_string()).expect("CString::new failed");
                copy_string(error, str_err);
            }
    };
}

/*
 * Request data from open meteo service.
 * @param request The data struct with request data information;
 * @param response The response received from open meteo service;
 * For more info see open-meteo.com/en/docs
 * sign up to get a free api key here https://geocode.maps.co/
 */
async fn get_weather(request: WeatherRequest, response: WeatherResponse) -> Result<WeatherResponse, Box<dyn Error>> {
    let open_meteo: OpenMeteo;
    let place_name: &str = c_char_to_str(request.place_name);

     if !place_name.is_empty()  {
        let api_key: &str = c_char_to_str(request.api_key); 

        open_meteo = OpenMeteo::new() 
                .location(place_name, api_key).await?; // add location
    } else  {
        open_meteo = OpenMeteo::new() 
                .coordinates(request.lat, request.lon)?; // add location
    }

    let data = open_meteo
                                .forecast_days(10)?  // add forecast data
                                .current_weather()?  // add current weather data
                                .past_days(10)? // add past days data
                                .time_zone(TimeZone::EuropeLondon)? // set time zone for using .daily()
                                .hourly()? // add hourly weather data
                                .daily()? // add daily weather data
                                .query()
                                .await?;
    let temperature = data.current_weather.unwrap().temperature;
    let str_temp: CString = CString::new(temperature.to_string()).expect("CString::new failed");
 
    unsafe {
        copy_string(response.temperature, str_temp);
    }

    Ok(response)
}

/*
 * Internal support functions to help interop between C and Rust 
 */

/**
 * Copy Rust string to C string.
 * @param ptr_dest A valid ponter to receive rust buffer string data;
 * The ptr_dest should be a valid pointer to the buffer of required size
 * @param str_src The source string whose data will be copied;
 */
/// # Safety
unsafe fn copy_string(ptr_dest: *mut c_char, str_src: CString) {
    let bytes = str_src.as_bytes();
    let len = bytes.len();

    std::ptr::copy(str_src.as_bytes().as_ptr().cast(), ptr_dest, len);
    std::ptr::write(ptr_dest.offset(len as isize) as *mut u8, 0u8);
}

/**
 * Copy C string to Rust string.
 * @param c_dest A valid ponter to a C string;
 * @return a str Rust string;
 */
fn c_char_to_str(c_str: *const c_char) -> &'static str {
    unsafe {
        let c_str = CStr::from_ptr(c_str);
        return c_str.to_str().unwrap();
    }
}