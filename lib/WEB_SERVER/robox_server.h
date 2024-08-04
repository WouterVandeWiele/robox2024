#ifndef ROBOX_WEB_SERVER_H
#define ROBOX_WEB_SERVER_H

#include <Arduino.h>
#include <AsyncTCP.h>
#include <WiFi.h>
#include <ESPAsyncWebServer.h>

static AsyncWebServer server(80);
const char* PARAM_MESSAGE = "message";

const char* main_page = R"(
<html>
<head>
  <meta name="viewport" content="width=device-width,initial-scale=1">
  <meta name="color-scheme" content="light dark">
  <meta name="theme-color" content="#2a3140">
  <link
    rel="stylesheet"
    href="https://cdn.jsdelivr.net/npm/@picocss/pico@2/css/pico.min.css"
  >
  <script
    src="https://unpkg.com/htmx.org@2.0.1"
  ></script>

</head>
<body>
  <header>
    <div class="container">
    <a aria-label="Pico CSS homepage" data-discover="true" href="/">
    logo
    </a>
    <nav>
      <ul>
      <li class="hide-before-sm"><a aria-current="page" class="contrast" data-discover="true" href="/examples">Examples</a></li>
      <li><a class="contrast" data-discover="true" href="/docs">Docs</a></li>
      </ul>
      <ul class="icons">
      <li><a rel="noopener noreferrer" class="contrast" aria-label="GitHub repository" href="https://github.com/picocss/pico" target="_blank"></a></li>
      <li><a rel="noopener noreferrer" class="contrast" aria-label="Twitter" href="https://twitter.com/picocss" target="_blank">
      </a></li><li><a class="contrast" aria-label="Turn off dark mode" data-discover="true" href="/examples">moon</a></li>
      </ul><
    /nav>
    </div>
  </header>

</body>
</html>
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