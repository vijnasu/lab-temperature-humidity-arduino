#include <Arduino.h>
#include "th02.hpp"
#include "upm_utilities.h"
#include "jhd1313m1.h"
int main();

int
main() {
  
  // Set the subplatform for the shield
  mraa_add_subplatform(MRAA_GROVEPI, "0");

  // Create the temperature & humidity sensor object 
  upm::TH02 sensor;

  // initialize the LCD and check for initialization
  jhd1313m1_context lcd = jhd1313m1_init(0, 0x3e, 0x62);

  if (!lcd) {
    printf("jhd1313m1_i2c_init() failed\n");
    return 1;
  }
  
  // set the LCD parameters
  char string1[20];
  char string2[20];
  uint8_t rgb[7][3] = {
            {0xd1, 0x00, 0x00},
            {0xff, 0x66, 0x22},
            {0xff, 0xda, 0x21},
            {0x33, 0xdd, 0x00},
            {0x11, 0x33, 0xcc},
            {0x22, 0x00, 0x66},
            {0x33, 0x00, 0x44}};
        
  // Read the temperature and humidity printing both the Celsius and
  // equivalent Fahrenheit temperature and Relative Humidity, waiting two seconds between readings
  while (1) {
      float celsius = sensor.getTemperature();
      float fahrenheit = (celsius * 9.0 / 5.0 + 32.0);
      float humidity = sensor.getHumidity();
      printf("%2.3f Celsius, or %2.3f Fahrenheit\n", celsius, fahrenheit);
      printf("%2.3f%% Relative Humidity\n", humidity);
      
      snprintf(string1, sizeof(string1), "Temperature:");
      snprintf(string2, sizeof(string2), "%2.1f%cF %2.1f%cC", fahrenheit, 223, celsius, 223);
      // Alternate rows on the LCD
      jhd1313m1_set_cursor(lcd, 0, 0);
      jhd1313m1_write(lcd, string1, strlen(string1));
      jhd1313m1_set_cursor(lcd, 1, 0);
      jhd1313m1_write(lcd, string2, strlen(string2));
      // Change the color
      uint8_t r = rgb[(int)fahrenheit%7][0];
      uint8_t g = rgb[(int)fahrenheit%7][1];
      uint8_t b = rgb[(int)fahrenheit%7][2];
      jhd1313m1_set_color(lcd, r, g, b);
      upm_delay(2);
      jhd1313m1_clear(lcd);
      
      snprintf(string1, sizeof(string1), "Humidity:");
      snprintf(string2, sizeof(string2), "%2.1f%%", humidity);
      // Alternate rows on the LCD
      jhd1313m1_set_cursor(lcd, 0, 0);
      jhd1313m1_write(lcd, string1, strlen(string1));
      jhd1313m1_set_cursor(lcd, 1, 0);
      jhd1313m1_write(lcd, string2, strlen(string2));
      upm_delay(2);
      jhd1313m1_clear(lcd);
  }

  return 0;
}
