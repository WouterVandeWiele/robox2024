#ifndef ROBOX_WEB_SERVER_H
#define ROBOX_WEB_SERVER_H

#include <Arduino.h>
#include <AsyncTCP.h>
#include <WiFi.h>
#include <ESPAsyncWebServer.h>

static AsyncWebServer server(80);
const char* PARAM_MESSAGE = "message";

const char* main_page = R"(
<body>
    <script src="https://unpkg.com/mithril/mithril.js"></script>
    <script>
    var root = document.body

    m.render(root, [
        m("main", [
            m("h1", {class: "title"}, "My first app"),
            m("button", "A button"),
        ])
    ])
    </script>
</body>
)";


void notFound(AsyncWebServerRequest* request) {
  request->send(404, "text/plain", "Not found");
}

void server_setup() {

  server.on("/", HTTP_GET, [](AsyncWebServerRequest* request) {
    request->send(200, "text/html", main_page);
  });

  // Send a GET request to <IP>/get?message=<message>
  server.on("/get", HTTP_GET, [](AsyncWebServerRequest* request) {
    String message;
    if (request->hasParam(PARAM_MESSAGE)) {
      message = request->getParam(PARAM_MESSAGE)->value();
    } else {
      message = "No message sent";
    }
    request->send(200, "text/plain", "Hello, GET: " + message);
  });

  // Send a POST request to <IP>/post with a form field message set to <message>
  server.on("/post", HTTP_POST, [](AsyncWebServerRequest* request) {
    String message;
    if (request->hasParam(PARAM_MESSAGE, true)) {
      message = request->getParam(PARAM_MESSAGE, true)->value();
    } else {
      message = "No message sent";
    }
    request->send(200, "text/plain", "Hello, POST: " + message);
  });

  server.onNotFound(notFound);
}

void server_start() {
  server.begin();
}

void server_end() {
    server.end();
}

#endif      // ROBOX_WEB_SERVER_H