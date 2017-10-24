#include <Wire.h>
#include <rgb_lcd.h>
//https://github.com/Seeed-Studio/Grove_LCD_RGB_Backlight
rgb_lcd lcd;

void setup(void){
  //Serial.begin(9600);
  //Serial.println("Lancement de l'application");
  lcd.begin(16, 2);
    
  lcd.setRGB(0, 0, 255);
  lcd.print("hello, world!");
  delay(1000);

}

void loop(void){
  // set the cursor to column 0, line 1
  // (note: line 1 is the second row, since counting begins with 0):
  lcd.setCursor(0, 1);
  // print the number of seconds since reset:
  lcd.print(millis()/1000);

  delay(100);
}
