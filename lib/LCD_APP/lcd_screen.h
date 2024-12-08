#ifndef LCD_SCREEN_H
#define LCD_SCREEN_H

#include "SED1530_LCD.h"
#include "robox_io.h"
#include "general_definitions.h"
#include "general_config.h"

#include <GEM_adafruit_gfx.h>

extern SED1530_LCD* lcd_t;
extern GEM_adafruit_gfx* menu;

extern GEMPage menuPageSwitch;
extern GEMPage menuAudioControl;
extern GEMPage menuPageSettings;

#define GEM_INVALIDATE (0x80)

// robox LCD architecture [docs/lcd_overview.excalidraw.png]

#if defined(LCD_RUN_THREADED)

// class LCD_Threaded: public SED1530_LCD {

//     public:
//         #if !defined(IO_EXPANDER)
//             LCD_Threaded(uint8_t A0, uint8_t RW, uint8_t EN, uint8_t *DATA): SED1530_LCD(A0, RW, EN, DATA) {};
//         #elif defined(IO_EXPANDER)
//             LCD_Threaded(RoboxIoExpander *io): SED1530_LCD(io) {};
//         #endif

//         void updateWholeScreen();

//         void taskUpdateWholeScreen();
// };

#endif

typedef enum {
    INVALIDATE_ALL,
    INVALIDATE_VOLUME,
} LcdInvalidate;

/**
 * @brief Requests that the LCD display should be redrawn.
 */
void lcd_invalidate(LcdInvalidate invalidate);
void lcd_go_to_play_menu();

class RoboxLcdScreen: public IoInterface {
    public:
        #if defined(IO_EXPANDER)
            RoboxLcdScreen(RoboxIoExpander* io);
        #else
            RoboxLcdScreen(uint8_t A0, uint8_t RW, uint8_t EN, uint8_t *DATA);
        #endif
        // void lcd_gfx_test();

        void power_up();
        void power_down();

        // static method that returns the IO expander config expected by this component
        static ExpanderConfig io_config();

        // interrupt method to be triggered by the io-expander
        void io_interrupt_observer(std::vector<uint8_t>& data) override;

        void init_lcd();
        void deinit_lcd();

        // void lcd_menu_loop();


    private:

        #if defined(IO_EXPANDER)
            RoboxIoExpander* io;
        #else
            uint8_t A0;
            uint8_t RW;
            uint8_t EN,;
            uint8_t *DATA;
        #endif

    
        // GEM_adafruit_gfx* menu;
    
};

#endif  // LCD_SCREEN_H
