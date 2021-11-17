#include <Wire.h>              // Library for I2C communication
#include <LiquidCrystal_I2C.h> // Library for LCD

// Custom chars
/********************/

byte Speed[] = {
  B00000,
  B10100,

  B01010,
  B00101,

  B00101,
  B01010,

  B10100,
  B00000
};
byte Speed_Inverted[] {
  B11111,
  B01011,
  B10101,
  B11010,
  B11010,
  B10101,
  B01011,
  B11111
};

byte IDK[] = {
  B00000,
  B00100,
  B01010,
  B10001,
  B10001,
  B01010,
  B00100,
  B00000,
};
byte IDK_Inverted[] = {
  B11111,
  B11011,
  B10101,
  B01110,
  B01110,
  B10101,
  B11011,
  B11111,
};

byte Check[] = {
  B00000,
  B00001,
  B00011,
  B10110,
  B11100,
  B01000,
  B00000,
  B00000
};
byte Check_Inverted[] = {
  B11111,
  B11110,
  B11100,
  B01001,
  B00011,
  B10111,
  B11111,
  B11111
};

byte NOT_DEFINED[8] = {
  B01110,
  B10001,
  B00001,
  B00010,
  B00100,
  B00000,
  B00100,
  B00000
};
byte NOT_DEFINED_INVERTED[8] = {
  B10001,
  B01110,
  B11110,
  B11101,
  B11011,
  B11111,
  B11011,
  B11111
};

byte Bulb[8] = {
  B01110,
  B10001,
  B10001,
  B10001,
  B01010,
  B01010,
  B00100,
  B00000
};
/******************/

/*  TODO
      -> connect
          - LCD
          - 74HC595

          LCD // Wiring: SDA pin is connected to A4 and SCL pin to A5.
*/

int leftButtonPin = 2;
int rightButtonPin = 3;
int selectButtonPin = 4;

LiquidCrystal_I2C lcd = LiquidCrystal_I2C(0x27, 16, 2);

void setup() {
  // init pins

  pinMode(leftButtonPin, INPUT);   // LEFT
  pinMode(rightButtonPin, INPUT);  // RIGHT
  pinMode(selectButtonPin, INPUT); // SELECT

  // init display and custom chars

  // -------INIT LCD:

  lcd.init();
  lcd.backlight();
  lcd.noCursor();

  // -------INIT Chars

  lcd.createChar(0, Speed);
  lcd.createChar(1, Bulb);
  lcd.createChar(2, IDK);
  lcd.createChar(3, Check);

  // INVERTED

  lcd.createChar(4, Speed_Inverted);
  lcd.createChar(5, NOT_DEFINED_INVERTED);
  lcd.createChar(6, IDK_Inverted);
  lcd.createChar(7, Check_Inverted);

  //----------

  lcd_drawMenu();

  Serial.begin(9600);
}

int lcd_cursorPos = 0;

bool selected = false;
unsigned long millsPress;
int pressDelay = 250;

int leds_speed = 100;

void lcd_select() {
}

void lcd_update() {
  // Button Press Handler
  // - -- --- --- -- -
  // LEFT
  if (!selected) {
    if (digitalRead(leftButtonPin) == 1 && millis() - millsPress > pressDelay) {
      millsPress = millis();
      //    Serial.write("LEFT\n");

      if (lcd_cursorPos == 0) {
        lcd_cursorPos = 3;
      } else {
        lcd_cursorPos = (lcd_cursorPos - 1) % 4;
      }

      lcd_drawMenu();
    }

    // RIGHT
    if (digitalRead(rightButtonPin) == 1 && millis() - millsPress > pressDelay) {
      millsPress = millis();
      //   Serial.write("RIGHT\n");
      lcd_cursorPos = (lcd_cursorPos + 1) % 4;

      lcd_drawMenu();
    }
  }

  // Select
  if (digitalRead(selectButtonPin) == 1 && millis() - millsPress > pressDelay) {
    millsPress = millis();

    selected = !selected;

    lcd_drawMenu();
    //    Serial.write(lcd_cursorPos);
    //    Serial.write("SELECT\n");
  }

  /* --- --- --- --- --- --- --- --- --- --- --- --- ---*/
  // print information about selected option + control
  if (selected) {
    switch (lcd_cursorPos) {

//——————————————————Speed——————————————————//
	// speed of leds blinking or fading - 0%-100%

      case 0:
        Serial.write("pos = 0");

        lcd.setCursor(1, 0);
        if (digitalRead(leftButtonPin) == 1 && leds_speed != 0 && millis() - millsPress > pressDelay) {
          millsPress = millis();

          leds_speed = leds_speed - 2;
        } else if (digitalRead(rightButtonPin) == 1 && leds_speed != 100 && millis() - millsPress > pressDelay) {
          millsPress = millis();

          leds_speed = leds_speed + 2;
        }

        lcd.print("1. Speed: " + String(leds_speed) + "%");
        break;
//——————————————————Brightness——————————————————//
	
      case 1:
        break;

//——————————————————Modes——————————————————//
      case 2:
        break;

//—————————————————— IDK ——————————————————//
      case 3:
        break;
    }
  }

  /*  int c = 0;
    for (int pos = 0; pos < 16; pos += 2) {
      lcd_drawChar(pos, 1, c);
      if (c < 6) {
        c = c + 1;
      }
    }*/
}

void lcd_drawMenu() {
  int c = 0;
  unsigned long millsSel;

  // print option name based on cursor position
  if (!selected) {
    switch (lcd_cursorPos) {
      case 0:
        lcd.clear();
        lcd.setCursor(1, 0);
        lcd.print("1. Speed");
        break;

      case 1:
        lcd.clear();
        lcd.setCursor(1, 0);
        lcd.print("2. Brightness");
        break;

      case 2:
        lcd.clear();
        lcd.setCursor(1, 0);
        lcd.print("3. Modes");
        break;

      case 3:
        lcd.clear();
        lcd.setCursor(1, 0);
        lcd.print("4. Back");
        break;
    }
  }
  lcd.setCursor(0, 1);

 

  for (int pos = 0; pos < 8; pos += 2){
	if (lcd_cursorPos * 2 == pos) { // draw inverted characted if currently selected, then 
      		lcd_drawChar(pos, 1, c + 4);
      } else {
      		lcd_drawChar(pos, 1, c);
    }	
    if (c < 4) {
      c = c + 1;
    }
  }

  lcd.setCursor(14, 1);
  lcd.print(lcd_cursorPos);
}

void lcd_drawChar(int x, int y, int c) {
  // POSITION X , Y ------ custom character code 0-7
  lcd.setCursor(x, y);
  lcd.write(c);
}

int lcd_blink_interval = 250; // blinking interval;
bool lcd_cursor_inverted = true; 
unsigned long lcd_mills_blink; // time from last invert 

void lcd_cursor_blink(){
	if ( millis() - lcd_mills_blink > lcd_blink_interval){
		lcd_mills_blink = millis();
		
		if(inverted) {
			lcd_drawChar(cursorPos, 1, cursorPos + 4);	
		}else {
			lcd_drawChar(cursorPos, 1, cursorPos);
		}

		lcd_cursor_inverted = !lcd_cursor_inverted; 
	}
}

void loop() {
  lcd_update();
}
