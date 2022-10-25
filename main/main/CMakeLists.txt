idf_component_register(SRCS "main.c" "wifi_handler.c" "comms_websocket_server.c"
    INCLUDE_DIRS "include"
    EMBED_FILES ../frontend/index.html)