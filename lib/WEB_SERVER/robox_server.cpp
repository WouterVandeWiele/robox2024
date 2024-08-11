#include <Arduino.h>
#include <vector>
#include <AsyncTCP.h>
#include <WiFi.h>
#include <ESPAsyncWebServer.h>

#include "robox_server_static.h"

static AsyncWebServer server(80);
const char* PARAM_MESSAGE = "message";
const char* STATIC_QUERY_PARAM = "r";


static std::vector<String> station_list{ "http://link1", "http://link2", "http://link3"}; 

String station_table() {
    String response = R"x(<table class="striped"><thead><tr><th scope="col">#</th><th scope="col">url</th><th scope="col">Action</th></tr></thead><tbody hx-confirm="Are you sure?" hx-target="closest tr">)x";
    uint8_t count = 0;

    String delete_button1 = R"x(<button class="btn danger" hx-delete="/station_list?id=)x";
    String delete_button2 = R"x(" hx-target="#station_table" hx-swap="innerHTML">Delete</button>)x";
    for (auto & element : station_list) {
      response += "<tr><td>" + String(++count) + "</td><td>" + element + "</td><td>" + delete_button1 + String(count) + delete_button2 + "</td></tr>";
    }
    response += "</tbody></table>";

    return response;
}



void notFound(AsyncWebServerRequest* request) {
  request->send(404, "text/plain", "Not found");
}

void server_setup() {

  server.on("/", HTTP_GET, [](AsyncWebServerRequest* request) {
    request->send(200, "text/html", static_root_html);
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

  server.on("/static", HTTP_GET, [](AsyncWebServerRequest* request) {
    AsyncWebServerResponse *response;
    String query_param;
    String message;

    if (request->hasParam(STATIC_QUERY_PARAM)) {
      query_param = request->getParam(STATIC_QUERY_PARAM)->value();

      if (strcasecmp(query_param.c_str(), "pico") == 0) {
        response = request->beginResponse(200, "text/css", static_pico_css);
      }
      else if (strcasecmp(query_param.c_str(), "htmx") == 0) {
        response = request->beginResponse(200, "text/javascript", static_htmx);
      }
      else if (strcasecmp(query_param.c_str(), "robox-logo2.svg") == 0) {
        response = request->beginResponse(200, "image/svg+xml", static_robox_logo);
      }
      else if (strcasecmp(query_param.c_str(), "minimal-theme-switcher.js") == 0) {
        response = request->beginResponse(200, "text/javascript", static_minimal_theme_switcher);
      }
      else {
        response = request->beginResponse(200, "text/plain", "Unknown static endpoint");
      }

    } else {
      response = request->beginResponse(200, "text/pain", "No message sent");
    }
    request->send(response);
  });

  server.on("/station_list", HTTP_GET, [](AsyncWebServerRequest* request) {

    request->send(200, "text/html", station_table().c_str());
  });

  server.on("/station_list", HTTP_DELETE, [](AsyncWebServerRequest* request) {
    long query_param;

    if (request->hasParam("id")) {
      query_param = request->getParam("id")->value().toInt();

      Serial.printf("deleting: %d\n", query_param);
      station_list.erase(station_list.begin() + query_param -1);

      for (auto & element : station_list) {
        Serial.printf("- %s\n", element);
      }

    }
    request->send(200, "text/html", station_table().c_str());
  });

  server.on("/station_list", HTTP_POST, [](AsyncWebServerRequest* request) {
    String query_param;

    if (request->hasParam("url", true)) {
      query_param = request->getParam("url", true)->value();
      Serial.println(query_param);

      station_list.push_back(query_param);

      Serial.printf("adding: %s\n", query_param);
      for (auto & element : station_list) {
        Serial.printf("- %s\n", element);
      }
    }
    request->send(200, "text/html", station_table().c_str());
  });

  server.on("/station_list_edit", HTTP_POST, [](AsyncWebServerRequest* request) {
    String query_param1 = "";
    long query_param2 = 0;

    if (request->hasParam("url_edit", true)) {
      query_param1 = request->getParam("url_edit", true)->value();
    } 
    if (request->hasParam("url_id", true)) {
      query_param2 = request->getParam("url_id")->value().toInt();
    }
    Serial.printf("url: %s, id: %d\n", query_param1, query_param2);

    if (query_param1 && query_param2) {
      station_list.at(query_param2 -10) = query_param1;

      // Serial.printf("adding: %s\n", query_param);
      for (auto & element : station_list) {
        Serial.printf("- %s\n", element);
      }
    }
    request->send(200, "text/html", station_table().c_str());
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