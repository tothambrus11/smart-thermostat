#include "local_server/wifi_settings.h"
#include "ESPAsyncWebServer.h"
#include "globals.h"
#include "storage.h"

String processor(const String &var) {
    if (var == "wifi_ssid") {
        return String(storedData.connectedWifi.ssid).substring(0, storedData.connectedWifi.ssidLength);
    }
    if (var == "wifi_pass") {
        return String(storedData.connectedWifi.password).substring(0, storedData.connectedWifi.passwordLength);
    }
    if (var == "ap_ssid") {
        return String(storedData.wifiAP.ssid).substring(0, storedData.wifiAP.ssidLength);
    }
    if (var == "ap_pass") {
        return String(storedData.wifiAP.password).substring(0, storedData.wifiAP.passwordLength);
    }
    return "";
}

const char index_html[] PROGMEM = "<!DOCTYPE html>\n"
                                  "<html lang=\"en\">\n"
                                  "<head>\n"
                                  "    <meta charset=\"UTF-8\">\n"
                                  "    <meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\">\n"
                                  "    <title>Wifi Settings</title>\n"
                                  "    <style>\n"
                                  "        * {\n"
                                  "            font-family: \"Century Gothic\", Arial, sans-serif;\n"
                                  "            font-weight: 100;\n"
                                  "        }\n"
                                  "\n"
                                  "        body {\n"
                                  "            display: flex;\n"
                                  "            justify-content: center;\n"
                                  "            align-items: center;\n"
                                  "        }\n"
                                  "\n"
                                  "        html, body {\n"
                                  "            min-height: 100vh;\n"
                                  "            overflow-y: auto;\n"
                                  "            margin: 0;\n"
                                  "        }\n"
                                  "\n"
                                  "        h1 {\n"
                                  "            margin: 0;\n"
                                  "        }\n"
                                  "\n"
                                  "        .big-cont {\n"
                                  "            padding: 32px;\n"
                                  "            margin: 32px;\n"
                                  "            display: grid;\n"
                                  "            gap: 16px;\n"
                                  "            max-width: 450px;\n"
                                  "            box-shadow: 0 4px 4px 0 #00000040;\n"
                                  "            background-color: #F8F8F8;\n"
                                  "        }\n"
                                  "\n"
                                  "        input {\n"
                                  "            display: block;\n"
                                  "            height: 40px;\n"
                                  "            border: 0;\n"
                                  "            background-color: #ededed;\n"
                                  "            font-size: 16px;\n"
                                  "            padding: 0 8px;\n"
                                  "            box-sizing: border-box;\n"
                                  "            outline: none;\n"
                                  "        }\n"
                                  "\n"
                                  "        button {\n"
                                  "            padding: 10px;\n"
                                  "            background-color: #015694;\n"
                                  "            color: white;\n"
                                  "            border: 0;\n"
                                  "            outline: none;\n"
                                  "            font-size: 16px;\n"
                                  "        }\n"
                                  "\n"
                                  "        button:hover {\n"
                                  "            filter: brightness(.9);\n"
                                  "        }\n"
                                  "    </style>\n"
                                  "\n"
                                  "</head>\n"
                                  "<body>\n"
                                  "<div class=\"big-cont\">\n"
                                  "    <h1>Wifi beállítása</h1>\n"
                                  "    <input id=\"wifi_ssid\" type=\"text\" value=\"%wifi_ssid%\" placeholder=\"SSID\">\n"
                                  "    <input id=\"wifi_pass\" type=\"password\" value=\"%wifi_pass%\" placeholder=\"Jelszó\">\n"
                                  "    <h1>Hozzáférési pont beállítása</h1>\n"
                                  "    <input id=\"ap_ssid\" type=\"text\" value=\"%ap_ssid%\" placeholder=\"SSID\">\n"
                                  "    <input id=\"ap_pass\" type=\"password\" value=\"%ap_pass%\" placeholder=\"Jelszó\">\n"
                                  "    <button onclick=\"submit()\">Mentés</button>\n"
                                  "</div>\n"
                                  "<script>\n"
                                  "    async function postData(url, data) {\n"
                                  "        const response = await fetch(url, {\n"
                                  "            method: 'POST',\n"
                                  "            mode: 'cors',\n"
                                  "            cache: 'no-cache',\n"
                                  "            body: data\n"
                                  "        });\n"
                                  "        return response.text();\n"
                                  "    }\n"
                                  "\n"
                                  "    function submit() {\n"
                                  "        let data = new FormData();\n"
                                  "\n"
                                  "        data.append(\"wifi_ssid\", document.getElementById(\"wifi_ssid\").value);\n"
                                  "        data.append(\"wifi_pass\", document.getElementById(\"wifi_pass\").value);\n"
                                  "        data.append(\"ap_ssid\", document.getElementById(\"ap_ssid\").value);\n"
                                  "        data.append(\"ap_pass\", document.getElementById(\"ap_pass\").value);\n"
                                  "\n"
                                  "        postData(\"/save-wifi-settings\", data).catch(e => {\n"
                                  "            console.log(e);\n"
                                  "            alert(\"Failed saving\");\n"
                                  "        });\n"
                                  "    }\n"
                                  "</script>\n"
                                  "</body>\n"
                                  "</html>";

void routeWifiSettings() {
    localServer.on("/", HTTP_GET, [](AsyncWebServerRequest *request) {
        request->send_P(200, "text/html", index_html, processor);
    });
}

void routeSaveWifiSettings() {
    localServer.on("/save-wifi-settings", HTTP_POST, [](AsyncWebServerRequest *request) {
        size_t params = request->params();
        for (size_t i = 0; i < params; i++) {
            AsyncWebParameter *p = request->getParam(i);
            if (p->name() == "wifi_ssid") {
                p->value().toCharArray(storedData.connectedWifi.ssid, p->value().length() + 1);
                storedData.connectedWifi.ssidLength = p->value().length();
            } else if (p->name() == "wifi_pass") {
                p->value().toCharArray(storedData.connectedWifi.password, p->value().length() + 1);
                storedData.connectedWifi.passwordLength = p->value().length();
            } else if (p->name() == "ap_ssid") {
                p->value().toCharArray(storedData.wifiAP.ssid, p->value().length() + 1);
                storedData.wifiAP.ssidLength = p->value().length();
            } else if (p->name() == "ap_pass") {
                p->value().toCharArray(storedData.wifiAP.password, p->value().length() + 1);
                storedData.wifiAP.passwordLength = p->value().length();
            }
        }

        saveData();

        request->send(200, "text/html", "ok");
    });
}