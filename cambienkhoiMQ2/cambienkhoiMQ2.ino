
#include <LiquidCrystal_I2C.h>
const int sensorpin = 4;

// Khởi tạo màn hình LCD thông qua giao tiếp I2C
LiquidCrystal_I2C lcd(0x27, 16, 2);
boolean bit_tt=0;
void setup() {
  // Khởi động I2C
  // Khởi động màn hình LCD
  lcd.init();
  lcd.backlight();
  pinMode(sensorpin,INPUT_PULLUP);
  // Khởi động cửa sổ Serial Monitor
  Serial.begin(9600);

  // Hiển thị thông báo khởi động
  lcd.setCursor(3, 0);
  lcd.print("Trang thai");
}

void loop() {
  bit_tt = digitalRead(sensorpin);
  if (bit_tt==0)
  {
    lcd.setCursor(0,1);
    lcd.print("Co khi de chay");
  }
  else
  {
    lcd.setCursor(0,1);
    lcd.print("Khong co khi  ");
  }

}
