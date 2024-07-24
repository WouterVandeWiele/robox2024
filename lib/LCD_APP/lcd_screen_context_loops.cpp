#include "lcd_screen.h"
#include "adc_key.h"
#include "icons.h"

/******************************************************************************
 * Play menu
 *****************************************************************************/

static volatile bool test_icons_active = false;

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
void playContextLoop() {
Serial.println("In play contex loop");

    ButtonPress button;

    if (xQueueButtons != 0) {
        if(xQueueReceive( xQueueButtons, &(button), 0)) {
            if (button.long_press == false) {

                switch (button.button)
                {
                case GEM_KEY_UP:
                    test_icons_active = true;
                    break;

                case GEM_KEY_DOWN:
                    test_icons_active = false;
                    break;

                case GEM_KEY_LEFT:
                    break;

                case GEM_KEY_RIGHT:
                    break;

                case GEM_KEY_OK:
                    // Exit animation routine if GEM_KEY_CANCEL key was pressed
                    menu->context.exit();
                    break;

                // case GEM_KEY_CANCEL:
                //     break;
                
                default:
                    break;
                }
            }
        }
        lcd_t->fillScreen(GLCD_COLOR_CLEAR);

        // side menu
        lcd_t->drawBitmap(1, 0, icon_shuffle, 15, 15, GLCD_COLOR_SET);
        lcd_t->drawBitmap(1, 16, icon_music_note_list, 15, 15, GLCD_COLOR_SET);
        lcd_t->drawBitmap(1, 32, icon_gear, 15, 15, GLCD_COLOR_SET);

        if (test_icons_active) {
            lcd_t->drawRect(0, 0, 16, 16, GLCD_COLOR_SET);
            lcd_t->drawRect(0, 16, 16, 16, GLCD_COLOR_SET);
            lcd_t->drawRect(0, 32, 16, 16, GLCD_COLOR_SET);
        }

        // central controls

        // 14 x 14
        lcd_t->drawBitmap(52, 14, test_icons_active ? icon_play_circle_fill : icon_play_circle, 14, 14, GLCD_COLOR_SET);
        lcd_t->drawBitmap(52, 29, test_icons_active ? icon_pause_circle_fill : icon_pause_circle, 14, 14, GLCD_COLOR_SET);
        lcd_t->drawBitmap(34, 26, test_icons_active ? icon_rewind_fill : icon_rewind, 14, 14, GLCD_COLOR_SET);
        lcd_t->drawBitmap(70, 26, test_icons_active ? icon_fast_forward_fill : icon_fast_forward, 14, 14, GLCD_COLOR_SET);

        // progress bar
        lcd_t->drawRect(33, 44, 52, 3, GLCD_COLOR_SET);

        // title - artist
        lcd_t->setCursor(19, 0);
        lcd_t->cp437(true);
        lcd_t->setTextWrap(false);
        lcd_t->setTextSize(1);
        lcd_t->setFont(GEM_FONT_SMALL);
        lcd_t->print("title - artist long");
        lcd_t->setCursor(19, 12);
        lcd_t->print("reaching the end");
        lcd_t->setCursor(0, 0);


        // volume level

        lcd_t->drawBitmap(74, 15, icon_volume, 10, 10, GLCD_COLOR_SET);
        lcd_t->setCursor(85, 22);
        lcd_t->print("100%");

        lcd_t->updateWholeScreen();
    }
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