#include <SimpleCLI.h>

// Create CLI Object
SimpleCLI cli;

// Commands
Command ping;
Command help;

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
    help = cli.addCmd("help", helpCallback);

    // [Optional] Check if our command was successfully added
    if (!ping) Serial.println(">Ping command not installed!");
    else Serial.println(">Ping was added to the CLI!");

    if (!help) Serial.println(">Help command not installed!");
    else Serial.println(">Help was added to the CLI!");

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