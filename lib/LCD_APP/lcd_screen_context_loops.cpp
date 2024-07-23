#include "lcd_screen.h"
#include "icons.h"

/******************************************************************************
 * Play menu
 *****************************************************************************/

// Invoked once when the button is pressed
static void playContextEnter() {
  // Clear sreen
  lcd_t->fillScreen(GLCD_COLOR_CLEAR);
  // Draw initial frame for the case of manual navigation ("Manual" tempo preset)
//   if (interval == 0) {
//     drawFrame(true);
//   }
  Serial.println("Enter play contex loop");
}

// Invoked every loop iteration
static void playContextLoop() {
Serial.println("In play contex loop");
  if (false) {
    // Exit animation routine if GEM_KEY_CANCEL key was pressed
    menu->context.exit();
  } else {

    lcd_t->fillScreen(GLCD_COLOR_CLEAR);

    // side menu
    lcd_t->drawBitmap(1, 0, icon_shuffle, 15, 15, GLCD_COLOR_SET);
    lcd_t->drawBitmap(1, 16, icon_music_note_list, 15, 15, GLCD_COLOR_SET);
    lcd_t->drawBitmap(1, 32, icon_gear, 15, 15, GLCD_COLOR_SET);

    // central controls
    lcd_t->drawBitmap(51, 10, icon_play_circle, 15, 15, GLCD_COLOR_SET);
    lcd_t->drawBitmap(51, 27, icon_pause_circle, 15, 15, GLCD_COLOR_SET);
    lcd_t->drawBitmap(33, 22, icon_rewind, 15, 15, GLCD_COLOR_SET);
    lcd_t->drawBitmap(70, 22, icon_fast_forward, 15, 15, GLCD_COLOR_SET);

    // progress bar
    lcd_t->drawRect(33, 43, 52, 4, GLCD_COLOR_SET);

    // title - artist
    lcd_t->setCursor(19, 0);
    lcd_t->cp437(true);
    lcd_t->setTextWrap(false);
    lcd_t->setTextSize(1);
    lcd_t->setFont(GEM_FONT_SMALL);
    lcd_t->print("title - artist");


    lcd_t->updateWholeScreen();

    // delay(10000);
  }
    // menu->context.exit();
}

// Invoked once when the GEM_KEY_CANCEL key is pressed
static void playContextExit() {
  // Reset variables
//   previousMillis = 0;
//   currentFrame = framesCount;

  Serial.println("Exit play contex loop");


  // Draw menu back on screen and clear context
  menu->reInit();
  menu->drawMenu();
  menu->clearContext();
}

// Setup context
void playLoop() {
  menu->context.loop = playContextLoop;
  menu->context.enter = playContextEnter;
  menu->context.exit = playContextExit;
  menu->context.allowExit = false; // Setting to false will require manual exit from the loop
  menu->context.enter();
  menu->context.loop();
}