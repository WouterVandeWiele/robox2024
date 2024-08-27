#include <Arduino.h>
#include <tinyxml2.h>

#include <WiFi.h>
#include <HTTPClient.h>

void update_check() {

    HTTPClient http;

    // Your Domain name with URL path or IP address with path
    http.begin("https://woutervandewiele.github.io/robox2024/releases.xml");

    // Send HTTP GET request
    int httpResponseCode = http.GET();

    int version_major = 0;
    int version_minor = 0;
    int version_patch = 0;
    String text = "";
    
    if (httpResponseCode>0) {
        Serial.print("HTTP Response code: ");
        Serial.println(httpResponseCode);
        String payload = http.getString();
        Serial.println(payload);

        tinyxml2::XMLDocument xmlDocument;

        if(xmlDocument.Parse(payload.c_str())!= tinyxml2::XML_SUCCESS){
            Serial.println("Error parsing");  
            return;
        }

        tinyxml2::XMLElement * root = xmlDocument.FirstChildElement("rss");

        tinyxml2::XMLElement * version = root->FirstChildElement("channel")->LastChildElement("item")->FirstChildElement("version");

        version->QueryIntAttribute("major", &version_major);
        version->QueryIntAttribute("minor", &version_minor);
        version->QueryIntAttribute("patch", &version_patch);
        text = version->GetText();

        Serial.printf("latest version xml: (%s) %ld.%ld.%ld\n", text, version_major, version_minor, version_patch);
    }
    else {
        Serial.print("Error code: ");
        Serial.println(httpResponseCode);
    }
    // Free resources
    http.end();
}