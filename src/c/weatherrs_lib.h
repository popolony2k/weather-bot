#include <stdarg.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

typedef struct WeatherRequest {
  char *api_key;
  char *place_name;
  float lat;
  float lon;
} WeatherRequest;

typedef struct WeatherResponse {
  char *temperature;
  char *error;
} WeatherResponse;

void request_weather(struct WeatherRequest request, struct WeatherResponse response);
