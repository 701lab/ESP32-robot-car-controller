#include "device_web_server.h"

static void serve_root(AsyncWebServerRequest *request);

static void serve_styles(AsyncWebServerRequest *request);

static void serve_js(AsyncWebServerRequest *request);

static void serve_404(AsyncWebServerRequest *request);

char main_page[10000];
char joy_js[10000];
char styles_page[2048];
char error_page[2048];

void esp32_setup_web_server(AsyncWebServer *server)
{
    read_file_from_spiffs(main_page, 10000, "/index.html");
    read_file_from_spiffs(styles_page, 2048, "/styles.css");
    read_file_from_spiffs(joy_js, 10000, "/FlexJoystick-min.js");
    read_file_from_spiffs(error_page, 2048, "/error.html");

    start_access_point();

    server->on("/", serve_root);
    server->on("/FlexJoystick-min.js", serve_js);
    server->on("/styles.css", serve_styles);
    server->onNotFound(serve_404);

    server->begin();
}

void serve_root(AsyncWebServerRequest *request)
{
    request->send(200, "text/html", main_page);
}

void serve_styles(AsyncWebServerRequest *request)
{
    request->send(200, "text/css", styles_page);
}

void serve_js(AsyncWebServerRequest *request)
{
    request->send(200, "text/javascript", joy_js);
}

void serve_404(AsyncWebServerRequest *request)
{
    request->send(404, "text/html", error_page);
} 