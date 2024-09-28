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


const String row_edit1 = R"x(<tr class='editing'>
    <td>)x";
const String row_edit2 = R"x(</td>
    <td><input name='url' value=")x";
const String row_edit3 = R"x("></td>
    <td>
        <button hx-get="/station_list" hx-target="#station_table">
            Cancel
        </button>
        <button hx-put="/station_list?id=)x";
const String row_edit4 = R"x(" hx-include="closest tr">
            Save
        </button>
    </td>
</tr>)x";

const String delete_button1 = R"x(<button class="contrast"
        hx-delete="/station_list?id=)x";
const String delete_button2 = R"x("
        hx-trigger="edit"
        onClick="let editing = document.querySelector('.editing')
        if(editing) {
            document.getElementById('edit_warning').setAttribute('open', '')
        } else {
            document.getElementById('delete_warning').setAttribute('open', '');
            document.getElementById('delete_button').setAttribute('hx-vals', 'js:{id: )x";
const String delete_button3 = R"x(}');
        }
        ">Delete</button>)x";
const String edit_button1 = R"x(<button class="btn danger"
        hx-get="/url_edit?id=)x";
const String edit_button2 = R"x("
        hx-trigger="edit"
        onClick="let editing = document.querySelector('.editing')
        if(editing) {
            document.getElementById('edit_warning').setAttribute('open', '')
        } else {
        htmx.trigger(this, 'edit')
        }">Edit</button>)x";

const String table1 = R"x(<table class="striped" id="station_table">
    <thead>
        <tr><th scope="col">#</th><th scope="col">url</th><th scope="col">Action</th></tr>
    </thead>
    <tbody hx-swap="outerHTML" hx-target="closest tr">)x";
const String table2 = R"x(    </tbody>
</table>)x";

String station_table() {
    String response = table1;
    uint8_t count = 0;

    for (auto & element : station_list) {
      response += "<tr>";
      response += "<td>" + String(++count) + "</td>";
      response += "<td>" + element + "</td>";
      response += "<td>" + delete_button1 + String(count) + delete_button2 + String(count) + delete_button3 + edit_button1 + String(count) + edit_button2 + "</td>";
      response += "</tr>";
    }
    response += table2;

    return response;
}


void notFound(AsyncWebServerRequest* request) {
  request->send(404, "text/plain", "Not found");
}

void server_setup() {

  server.on("/", HTTP_GET, [](AsyncWebServerRequest* request) {
    request->send(200, "text/html", static_root_html);
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
    AsyncWebServerResponse *response = request->beginResponse(200, "text/html", station_table().c_str());
    response->addHeader("HX-Location","{\"path\":\"/station_list\", \"target\":\"#station_table\"}");
    request->send(response);
  });

  server.on("/station_list", HTTP_PUT, [](AsyncWebServerRequest* request) {
    String url = "";
    long id = -1;

    //List all parameters (Compatibility)
    // int args = request->args();
    // for(int i=0;i<args;i++){
    //   Serial.printf("ARG[%s]: %s\n", request->argName(i).c_str(), request->arg(i).c_str());
    // }

    if (request->hasArg("url")) {
      url = request->arg("url");
      Serial.printf("UPH: %s\n", url.c_str());
    }
    
    // if (request->hasParam("url")) {
    //   url = request->getParam("url")->value();
    //   Serial.printf("UP: %s\n", url.c_str());
    // }

    if (request->hasParam("id")) {
      id = request->getParam("id")->value().toInt();
      Serial.printf("UI: %ld\n", id);
    }

    if (request->hasArg("url") && request->hasParam("id")) {
      Serial.printf("update: %ld - %s\n", url, url.c_str());
      station_list[id-1] = url;
    }


    AsyncWebServerResponse *response = request->beginResponse(200, "text/html", station_table().c_str());
    response->addHeader("HX-Location","{\"path\":\"/station_list\", \"target\":\"#station_table\"}");
    request->send(response);

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
    // AsyncWebServerResponse *response = request->beginResponse(200, "text/html", station_table().c_str());
    AsyncWebServerResponse *response = request->beginResponse(200);
    response->addHeader("HX-Location","{\"path\":\"/station_list\", \"target\":\"#station_table\"}");
    request->send(response);
  });

  server.on("/url_edit", HTTP_GET, [](AsyncWebServerRequest* request) {

    long id;
    String r = "";
    String url = "";

    if (request->hasParam("id")) {
      id = request->getParam("id")->value().toInt();
      url = station_list.at(id-1);
      r = row_edit1 + String(id) + row_edit2 + url + row_edit3 + String(id) + row_edit4;
    }

    Serial.printf("editing %d\n", id);

    request->send(200, "text/html", r.c_str());
  });

  server.onNotFound(notFound);
}

void server_start() {
  server.begin();
}

void server_end() {
    server.end();
}