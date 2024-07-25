#include "lcd_screen.h"
#include "adc_key.h"
#include "icons.h"

#include "robox_audio_mux.h"
extern RoboxAudioMux mux;

/******************************************************************************
 * Play menu
 *****************************************************************************/

enum playItems {item_switch, item_audio_controls, item_settings, item_play, item_pause, item_reverse, item_forward};

typedef struct ButtonChain{
  playItems active_item;
  uint8_t button_press;
  playItems next_item;
} ButtonChain;

const ButtonChain play_item_chain[] = {
    {item_switch, GEM_KEY_UP, item_settings},
    {item_switch, GEM_KEY_DOWN, item_audio_controls},
    {item_switch, GEM_KEY_LEFT, item_forward},
    {item_switch, GEM_KEY_RIGHT, item_reverse},
    
    {item_audio_controls, GEM_KEY_UP, item_switch},
    {item_audio_controls, GEM_KEY_DOWN, item_settings},
    {item_audio_controls, GEM_KEY_LEFT, item_forward},
    {item_audio_controls, GEM_KEY_RIGHT, item_reverse},

    {item_settings, GEM_KEY_UP, item_audio_controls},
    {item_settings, GEM_KEY_DOWN, item_switch},
    {item_settings, GEM_KEY_LEFT, item_forward},
    {item_settings, GEM_KEY_RIGHT, item_reverse},

    {item_reverse, GEM_KEY_UP, item_reverse},
    {item_reverse, GEM_KEY_DOWN, item_reverse},
    {item_reverse, GEM_KEY_LEFT, item_switch},
    {item_reverse, GEM_KEY_RIGHT, item_play},

    {item_play, GEM_KEY_UP, item_pause},
    {item_play, GEM_KEY_DOWN, item_pause},
    {item_play, GEM_KEY_LEFT, item_reverse},
    {item_play, GEM_KEY_RIGHT, item_forward},

    {item_pause, GEM_KEY_UP, item_play},
    {item_pause, GEM_KEY_DOWN, item_play},
    {item_pause, GEM_KEY_LEFT, item_reverse},
    {item_pause, GEM_KEY_RIGHT, item_forward},

    {item_forward, GEM_KEY_UP, item_forward},
    {item_forward, GEM_KEY_DOWN, item_forward},
    {item_forward, GEM_KEY_LEFT, item_play},
    {item_forward, GEM_KEY_RIGHT, item_switch},
};

uint8_t length_button_chain = sizeof(play_item_chain) / sizeof(play_item_chain[0]);

playItems active_button = item_switch;

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

                if (button.button == GEM_KEY_OK) {
                    switch (active_button)
                    {
                    case item_play:
                        // mux.play();
                        break;

                    case item_pause:
                        // mux.pause();
                        break;

                    case item_switch:
                        menu->setMenuPageCurrent(menuPageSwitch);
                        menu->context.exit();
                        break;

                    case item_settings:
                        menu->setMenuPageCurrent(menuPageSettings);
                        menu->context.exit();
                        break;
                    
                    default:
                        break;
                    }
                }
                else {
                    for (uint8_t i = 0; i < length_button_chain; i++) {
                        if ((button.button == play_item_chain[i].button_press) && 
                            (active_button == play_item_chain[i].active_item)) {
                            
                            active_button = play_item_chain[i].next_item;
                            break;
                        }
                    }
                }
            }
        }
        lcd_t->fillScreen(GLCD_COLOR_CLEAR);

        // side menu
        lcd_t->drawBitmap(1, 0, icon_shuffle, 15, 15, GLCD_COLOR_SET);
        lcd_t->drawBitmap(1, 16, icon_music_note_list, 15, 15, GLCD_COLOR_SET);
        lcd_t->drawBitmap(1, 32, icon_gear, 15, 15, GLCD_COLOR_SET);

        if (active_button == item_switch) {
            lcd_t->drawRect(0, 0, 16, 16, GLCD_COLOR_SET);
        }
        if (active_button == item_audio_controls) {
            lcd_t->drawRect(0, 16, 16, 16, GLCD_COLOR_SET);
        }
        if (active_button == item_settings) {
            lcd_t->drawRect(0, 32, 16, 16, GLCD_COLOR_SET);
        }

        // central controls

        // 14 x 14
        lcd_t->drawBitmap(52, 14, (active_button == item_play) ? icon_play_circle_fill : icon_play_circle, 14, 14, GLCD_COLOR_SET);
        lcd_t->drawBitmap(52, 29, (active_button == item_pause) ? icon_pause_circle_fill : icon_pause_circle, 14, 14, GLCD_COLOR_SET);
        lcd_t->drawBitmap(34, 26, (active_button == item_reverse) ? icon_rewind_fill : icon_rewind, 14, 14, GLCD_COLOR_SET);
        lcd_t->drawBitmap(70, 26, (active_button == item_forward) ? icon_fast_forward_fill : icon_fast_forward, 14, 14, GLCD_COLOR_SET);

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
  lcd_t->clearScreen(GLCD_COLOR_CLEAR);
  menu->clearContext();

    // if (active_button == item_settings) {
    //     menu->setMenuPageCurrent(menuPageSettings);
    // }
    // if (active_button == item_switch) {
    //     menu->setMenuPageCurrent(menuPageSwitch);
    // }

  menu->reInit();
//   lcd_t->updateWholeScreen();
    menu->drawMenu();
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