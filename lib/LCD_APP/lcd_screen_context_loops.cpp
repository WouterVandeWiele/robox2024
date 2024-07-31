#include "lcd_screen.h"
#include "adc_key.h"
#include "icons.h"

#include "robox_audio_mux.h"
extern RoboxAudioMux mux;

/******************************************************************************
 * Play menu
 *****************************************************************************/
uint8_t lm_current_item = 0;

enum playItems {item_switch, item_audio_controls, item_settings, item_play_pause, item_led_motor, item_reverse, item_forward};

typedef struct ButtonChain{
  playItems active_item;
  uint8_t button_press;
  playItems next_item;
} ButtonChain;

static const ButtonChain play_item_chain[] = {
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
    {item_reverse, GEM_KEY_RIGHT, item_play_pause},

    {item_play_pause, GEM_KEY_UP, item_led_motor},
    {item_play_pause, GEM_KEY_DOWN, item_led_motor},
    {item_play_pause, GEM_KEY_LEFT, item_reverse},
    {item_play_pause, GEM_KEY_RIGHT, item_forward},

    {item_led_motor, GEM_KEY_UP, item_play_pause},
    {item_led_motor, GEM_KEY_DOWN, item_play_pause},
    {item_led_motor, GEM_KEY_LEFT, item_reverse},
    {item_led_motor, GEM_KEY_RIGHT, item_forward},

    {item_forward, GEM_KEY_UP, item_forward},
    {item_forward, GEM_KEY_DOWN, item_forward},
    {item_forward, GEM_KEY_LEFT, item_play_pause},
    {item_forward, GEM_KEY_RIGHT, item_switch},
};

uint8_t length_button_chain = sizeof(play_item_chain) / sizeof(play_item_chain[0]);

playItems active_button = item_switch;

static volatile bool test_icons_active = false;

static void renderScreen() {
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

    // led - motor
    lcd_t->setFont();
    lcd_t->cp437(true);
    lcd_t->setTextWrap(false);
    if ((lm_current_item == 1) || (lm_current_item == 3)) {
        lcd_t->setCursor(53, 20);
        lcd_t->print("L");
    }
    if ((lm_current_item == 2) || (lm_current_item == 3)) {
        lcd_t->setCursor(60, 20);
        lcd_t->print("M");
    }
    if (lm_current_item == 0) {
        lcd_t->setCursor(57, 20);
        lcd_t->print("-");
    }
    if (active_button == item_led_motor) {
        lcd_t->drawRect(51, 18, 16, 11, GLCD_COLOR_SET);
    }

    // play - pause
    if (mux.audio_active() == true) {
        lcd_t->drawBitmap(52, 29, (active_button == item_play_pause) ? icon_pause_circle_fill : icon_pause_circle, 14, 14, GLCD_COLOR_SET);
    }
    else {
        lcd_t->drawBitmap(52, 29, (active_button == item_play_pause) ? icon_play_circle_fill : icon_play_circle, 14, 14, GLCD_COLOR_SET);
    }

    // rewind
    lcd_t->drawBitmap(34, 26, (active_button == item_reverse) ? icon_rewind_fill : icon_rewind, 14, 14, GLCD_COLOR_SET);
    // forward
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
    lcd_t->printf("%d%%", int(mux.get_volume() * 100));

    lcd_t->updateWholeScreen();
}

// Invoked once when the button is pressed
static void playContextEnter() {
    renderScreen();
    Serial.println("Enter play contex loop");
}

static void onOkay() {
     switch (active_button)
     {
     case item_play_pause:
         mux.audio_play_pause();
         break;

     case item_led_motor:
         lm_current_item++;
         if (lm_current_item > 3) {
             lm_current_item = 0;
         }
         switch (lm_current_item)
         {
         case 1:
             // led only
             break;

         case 2:
             // motor only
             break;

         case 3:
             // led and motor
             break;

         default:
             break;
         }
         break;

     case item_reverse:
         mux.audio_previous();
         break;

     case item_forward:
         mux.audio_next();
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

static void nextItem(uint8_t button) {
     for (uint8_t i = 0; i < length_button_chain; i++) {
         if ((button == play_item_chain[i].button_press) &&
             (active_button == play_item_chain[i].active_item)) {

             active_button = play_item_chain[i].next_item;
             break;
         }
     }
}

// Invoked every loop iteration
void playContextLoop() {
    ButtonPress button;
    if (!xQueueReceive(xQueueButtons, &(button), 100 * portTICK_PERIOD_MS)) {
        return;
    }
    if (button.long_press) {
        return;
    }

    switch (button.button) {
        case GEM_INVALIDATE:
            renderScreen();
            break;
        case GEM_KEY_OK:
            onOkay();
            break;
        default:
            nextItem(button.button);
    }

    // if (button.button == GEM_KEY_OK) {
    //     switch (active_button)
    //     {
    //     case item_play_pause:
    //         mux.audio_play_pause();
    //         break;

    //     case item_led_motor:
    //         lm_current_item++;
    //         if (lm_current_item > 3) {
    //             lm_current_item = 0;
    //         }
    //         switch (lm_current_item)
    //         {
    //         case 1:
    //             // led only
    //             break;

    //         case 2:
    //             // motor only
    //             break;

    //         case 3:
    //             // led and motor
    //             break;
            
    //         default:
    //             break;
    //         }
    //         break;

    //     case item_reverse:
    //         mux.audio_previous();
    //         break;

    //     case item_forward:
    //         mux.audio_next();
    //         break;

    //     case item_switch:
    //         menu->setMenuPageCurrent(menuPageSwitch);
    //         menu->context.exit();
    //         break;

    //     case item_settings:
    //         menu->setMenuPageCurrent(menuPageSettings);
    //         menu->context.exit();
    //         break;
        
    //     default:
    //         break;
    //     }
    // } else {
    //     for (uint8_t i = 0; i < length_button_chain; i++) {
    //         if ((button.button == play_item_chain[i].button_press) && 
    //             (active_button == play_item_chain[i].active_item)) {
                
    //             active_button = play_item_chain[i].next_item;
    //             break;
    //         }
    //     }
    // }
}

// Invoked once when the GEM_KEY_CANCEL key is pressed
static void playContextExit() {
  // Reset variables
//   previousMillis = 0;
//   currentFrame = framesCount;

  Serial.println("Exit play contex loop");


  // Draw menu back on screen and clear context
  lcd_t->clearScreen(GLCD_COLOR_CLEAR);

//     menu->reInit();
//    menu->drawMenu();
  menu->clearContext();

    // if (active_button == item_settings) {
    //     menu->setMenuPageCurrent(menuPageSettings);
    // }
    // if (active_button == item_switch) {
    //     menu->setMenuPageCurrent(menuPageSwitch);
    // }

//   menu->reInit();
//   lcd_t->updateWholeScreen();
    // menu->drawMenu();
    // menu->drawMenu();

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

void lcd_invalidate() {
    ButtonPress button = {
        .button = GEM_INVALIDATE,
        .long_press = false,
        .press_time = 0
    };
    xQueueSend(xQueueButtons, &button, 0);
}
