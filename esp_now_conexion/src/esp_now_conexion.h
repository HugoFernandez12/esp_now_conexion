#ifndef ESPNOW_MESH_H
#define ESPNOW_MESH_H

#include <esp_now.h>
#include <WiFi.h>
#include <vector>
#include <string>
#include <functional>

class esp_now_conexion {
public:
    using ReceiveCallback = std::function<void(const std::string &mac, const std::vector<uint8_t> &data)>;
    using SentCallback = std::function<void(const std::string &mac, bool success)>;

    esp_now_conexion();
    ~esp_now_conexion();

    // Inicializar ESP-NOW
    bool begin();

    // Añadir un peer (nodo de la red)
    bool addPeer(const std::string &peerMac);

    // Eliminar un peer
    bool removePeer(const std::string &peerMac);

    // Métodos para enviar datos
    bool send(const std::string &peerMac, const std::string &data);
    template <typename T>
    bool send(const std::string &peerMac, const T &data);

    // Registrar callbacks
    void onReceive(ReceiveCallback callback);
    void onSent(SentCallback callback);

private:
    // Callbacks internos
    static void onReceiveCallback(const uint8_t *mac, const uint8_t *data, int len);
    static void onSentCallback(const uint8_t *mac, esp_now_send_status_t status);

    // Convertir dirección MAC a string
    static std::string macToString(const uint8_t *mac);

    // Contenedores STL
    std::vector<std::string> peers;

    // Funciones de usuario
    static ReceiveCallback userReceiveCallback;
    static SentCallback userSentCallback;
};

#include "esp_now_conexion.tpp" // Implementación de métodos template

#endif
