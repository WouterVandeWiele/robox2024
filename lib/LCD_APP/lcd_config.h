
// // #include <utility>  //declarations of unique_ptr
// // #include <mutex>
// #include <GEM_adafruit_gfx.h>
// #include <KeyDetector.h>

// #include "SED1530_LCD.h"
// #include "splash.h"
// #include "general_definitions.h"
// #include "general_config.h"

// #include "SerialKey.h"

// // w (119) -> ↑
// // d (100) -> →
// // s (115) -> ↓
// // a (97) -> ←
// // q (113) -> cancel
// // e (101) -> ok
// SerialKey serialKey(/*Up=*/119, /*Right/Next=*/100,
//                     /*Down=*/115, /*Left/Prev=*/97,
//                     /*Home/Cancel=*/113, /*Select/OK=*/101);


// // Pins the buttons are connected to
// // const byte downPin = 35;
// // const byte leftPin = 36;
// // const byte rightPin = 37;
// // const byte upPin = 38;
// // const byte cancelPin = 39;
// // const byte okPin = 0;

// // // Array of Key objects that will link GEM key identifiers with dedicated pins
// // Key keys[] = {{GEM_KEY_UP, upPin}, {GEM_KEY_RIGHT, rightPin}, {GEM_KEY_DOWN, downPin}, {GEM_KEY_LEFT, leftPin}, {GEM_KEY_CANCEL, cancelPin}, {GEM_KEY_OK, okPin}};

// // // Create KeyDetector object
// // KeyDetector myKeyDetector(keys, sizeof(keys)/sizeof(Key));





// ///////////////////////////////////////////////////////////////////////////////

// // Create variables that will be editable through the menu and assign them initial values
// int number = -512;
// bool enablePrint = false;
// char name[50] = {0};

// // Create two menu item objects of class GEMItem, linked to number and enablePrint variables 
// GEMItem menuItemInt("Number:", number);
// GEMItem menuItemBool("En print:", enablePrint);
// GEMItem menuItemString("str:", name);

// // Create menu button that will trigger printData() function. It will print value of our number variable
// // to Serial monitor if enablePrint is true. We will write (define) this function later. However, we should
// // forward-declare it in order to pass to GEMItem constructor
// void printData(); // Forward declaration
// GEMItem menuItemButton("Print", printData);

// // Create menu page object of class GEMPage. Menu page holds menu items (GEMItem) and represents menu level.
// // Menu can have multiple menu pages (linked to each other) with multiple menu items each
// GEMPage menuPageMain("Main Menu");

// // Create menu object of class GEM_adafruit_gfx. Supply its constructor with reference to tft object we created earlier
// // GEM_adafruit_gfx menu(lcd_t, GEM_POINTER_ROW, GEM_ITEMS_COUNT_AUTO);
// // Which is equivalent to the following call (you can adjust parameters to better fit your screen if necessary):
// GEM_adafruit_gfx menu(lcd_t, /* menuPointerType= */ GEM_POINTER_ROW, /* menuItemsPerScreen= */ GEM_ITEMS_COUNT_AUTO, /* menuItemHeight= */ 8, /* menuPageScreenTopOffset= */ 10, /* menuValuesLeftOffset= */ 80);



// ///////////////////////////////////////////////////////////////////////////////

// void update_screen() {
//     lcd_t.updateWholeScreen();
// }


// void lcd_setup() {
    


// //   menu.setDrawMenuCallback(update_screen);
//   menu.setDrawCallback(update_screen);
//   menu.setSplash(100, 48, robox_splash);
//   menu.setSplashDelay(3000);


//   // Menu init, setup and draw
//   menu.init();
// //   update_screen();


//   // Add menu items to menu page
//   menuPageMain.addMenuItem(menuItemInt);
//   menuPageMain.addMenuItem(menuItemBool);
//   menuPageMain.addMenuItem(menuItemButton);
//   menuPageMain.addMenuItem(menuItemString);

//   // Add menu page to menu and set it as current
//   menu.setMenuPageCurrent(menuPageMain);

//   menu.drawMenu();



// }

// void lcd_test() {

//     if (menu.readyForKey()) {
//         serialKey.detect();
//         menu.registerKeyPress(serialKey.trigger);
//     }

// }


// void printData() {
//   // If enablePrint flag is set to true (checkbox on screen is checked)...
//   if (enablePrint) {
//     // ...print the number to Serial
//     Serial.print("Number is: ");
//     Serial.println(number);
//   } else {
//     Serial.println("Printing is disabled, sorry:(");
//   }
// }
