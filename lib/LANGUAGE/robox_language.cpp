// #include <Arduino.h>
// #include <map>

// #include "robox_language.h"




// vTransMap l_en{
//     {lang_no_source, _cStr("No Music")},
//     {lang_switch_ble, _cStr("Bluetooth audio")},
//     {lang_switch_web, _cStr("Internet Radio")},
//     {lang_switch_sd, _cStr("MP3-Player")},
//     {lang_menu_return, _cStr("Back")},
// };

// vTransMap l_nl{
//     {lang_no_source, _cStr("Geen Muziek")},
//     {lang_switch_ble, _cStr("Bluetooth audio")},
//     {lang_switch_web, _cStr("Internetradio")},
//     {lang_switch_sd, _cStr("MP3-Speler")},
//     {lang_menu_return, _cStr("Terug")},
// };

// void Translator::switch_language(L_Language lang) {
//     switch (lang)
//     {
//     case lang_nl:
//         current_lang_map = &l_nl;

//     case lang_en:
//     default:
//         current_lang_map = &l_en;
//         break;
//     }
// }

// https://stackoverflow.com/questions/2333728/stdmap-default-value
// const String Translator::get_text(L_Messgaes message) {
//     typename vTransMap::const_iterator it = current_lang_map->find(message);
//     if ( it == current_lang_map->end() ) {
//         return String("<?>");
//     }
//     else {
//         return it->second;
//     }
// }
