#include <SimpleCLI.h>
#include <stdint.h>
#include "robox_audio_mux.h"

extern RoboxAudioMux mux;

// Can be used to implement a CLI to send debug commands
// Documentation: https://github.com/SpacehuhnTech/SimpleCLI

// Create CLI Object
SimpleCLI cli;

// Commands
Command ping;
Command help;
Command print;
Command audio_source;
Command mux_info;

// Callback function for ping command
void pingCallback(cmd* c) {
    Command cmd(c); // Create wrapper object

    Serial.println("Pong!");
}

// Callback for help message
void helpCallback(cmd *c) {
    Command cmd(c);

    Serial.println(cli.toString());
}

// Example callback to demonstrate aruments
void printCallback(cmd *c) {
    Command cmd(c);
    
    uint8_t times = atoi(cmd.getArg("n").getValue().c_str());
    String text = cmd.getArg(0).getValue();

    for (uint8_t i = 0; i < times; i++) {
        Serial.println(text);
    }
}

void sourceCallback(cmd *c) {
    Command cmd(c);
    String src = cmd.getArg(0).getValue();

    Serial.printf("'%s'\n", src);

    if (src == "ble") {
        // switch to ble
        mux.switch_to(BleSource);
    } else if (src == "web") {
        // switch to web radio
        mux.switch_to(WebRadioSource);
    } else if (src == "sd") {
        // switch to SD
        mux.switch_to(SDSource);
    }
    // else {
    //     // undefined source
    //     // mux.switch_to(NotSelectedSource);
    // }
    
}

void muxCallback(cmd *c) {
    Command cmd(c);

    Argument meta_title = cmd.getArgument("meta_title");

    if (meta_title.isSet()) {
        // const std::lock_guard<std::mutex> lock(mux.meta_mutex);
        Serial.printf("Current Title: %s\n", (mux.meta.title.c_str()));
        Serial.printf("Current Title: %s\n", mux.meta_tile);
    }
}

// Callback in case of an error
void errorCallback(cmd_error* e) {
    CommandError cmdError(e); // Create wrapper object

    Serial.print("ERROR: ");
    Serial.println(cmdError.toString());

    if (cmdError.hasCommand()) {
        Serial.print("Did you mean \"");
        Serial.print(cmdError.getCommand().toString());
        Serial.println("\"?");
    }
}

void debug_cli_setup() {
    // Print something to let us know that everything is working so far
    Serial.println(">>> Starting debug CLI.");

    // Install all callback functions
    ping = cli.addCmd("ping", pingCallback);
    ping.setDescription("Replies with \"pong\" if the ESP32 is still going through the main loop");
    
    help = cli.addCmd("help", helpCallback);
    help.setDescription("Print cli help, lists all registered commands");
    
    print = cli.addCmd("print", printCallback);
    print.setDescription("Example command demonstrating aruments, takes <text> and prints n times");
    print.addPosArg("text");
    print.addArg("n", "1");

    audio_source = cli.addCmd("source", sourceCallback);
    audio_source.setDescription("Select the audio source, takes 1 arument of: ['ble', 'web', 'sd']");
    audio_source.addPosArg("s");

    mux_info = cli.addCmd("mux", muxCallback);
    mux_info.setDescription("get information from the mux interface");
    mux_info.addFlagArg("meta_title");

    // [Optional] Check if our command was successfully added
    if (!ping) Serial.println(">Ping command not installed!");
    else Serial.println(">Ping was added to the CLI!");

    if (!help) Serial.println(">Help command not installed!");
    else Serial.println(">Help was added to the CLI!");

    if (!print) Serial.println(">Print command not installed!");
    else Serial.println(">Print was added to the CLI!");

    if (!audio_source) Serial.println(">Source command not installed!");
    else Serial.println(">Source was added to the CLI!");

    if (!mux_info) Serial.println(">Mux command not installed!");
    else Serial.println(">Mux was added to the CLI!");

    // Set error Callback
    cli.setOnError(errorCallback);

    // Start the loop
    Serial.println(">>> CLI installed, type \"help\" for all options.");
}

void debug_cli_loop() {
    // Check if user typed something into the serial monitor
    if (Serial.available()) {
        // Read out string from the serial monitor
        String input = Serial.readStringUntil('\n');

        // Echo the user input
        Serial.print("# ");
        Serial.println(input);

        // Parse the user input into the CLI
        cli.parse(input);
    }
}