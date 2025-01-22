#include "ESPNowMesh.h"

// Inicializar los callbacks estáticos
ESPNowMesh::ReceiveCallback ESPNowMesh::userReceiveCallback = nullptr;
ESPNowMesh::SentCallback ESPNowMesh::userSentCallback = nullptr;

// Constructor
ESPNowMesh::ESPNowMesh() {}

// Destructor
ESPNowMesh::~ESPNowMesh() {
    esp_now_deinit(); // Limpia ESP-NOW al destruir el objeto
}

// Inicializar ESP-NOW
bool ESPNowMesh::begin() {
    WiFi.mode(WIFI_STA); // Configurar como estación
    if (esp_now_init() != ESP_OK) {
        return false;
    }
    esp_now_register_recv_cb(onReceiveCallback);
    esp_now_register_send_cb(onSentCallback);
    return true;
}

// Añadir un peer
bool ESPNowMesh::addPeer(const std::string &peerMac) {
    esp_now_peer_info_t peerInfo = {};
    sscanf(peerMac.c_str(), "%hhx:%hhx:%hhx:%hhx:%hhx:%hhx", 
           &peerInfo.peer_addr[0], &peerInfo.peer_addr[1], &peerInfo.peer_addr[2], 
           &peerInfo.peer_addr[3], &peerInfo.peer_addr[4], &peerInfo.peer_addr[5]);
    peerInfo.channel = 0;
    peerInfo.encrypt = false;

    if (esp_now_add_peer(&peerInfo) == ESP_OK) {
        peers.push_back(peerMac); // Agrega a la lista local
        return true;
    }
    return false;
}

// Eliminar un peer
bool ESPNowMesh::removePeer(const std::string &peerMac) {
    uint8_t mac[6];
    sscanf(peerMac.c_str(), "%hhx:%hhx:%hhx:%hhx:%hhx:%hhx", 
           &mac[0], &mac[1], &mac[2], &mac[3], &mac[4], &mac[5]);
    if (esp_now_del_peer(mac) == ESP_OK) {
        peers.erase(std::remove(peers.begin(), peers.end(), peerMac), peers.end());
        return true;
    }
    return false;
}

// Enviar datos (string)
bool ESPNowMesh::send(const std::string &peerMac, const std::string &data) {
    uint8_t mac[6];
    sscanf(peerMac.c_str(), "%hhx:%hhx:%hhx:%hhx:%hhx:%hhx", 
           &mac[0], &mac[1], &mac[2], &mac[3], &mac[4], &mac[5]);
    return esp_now_send(mac, reinterpret_cast<const uint8_t *>(data.data()), data.size()) == ESP_OK;
}

// Registrar callback de recepción
void ESPNowMesh::onReceive(ReceiveCallback callback) {
    userReceiveCallback = callback;
}

// Registrar callback de envío
void ESPNowMesh::onSent(SentCallback callback) {
    userSentCallback = callback;
}

// Callback interno de recepción
void ESPNowMesh::onReceiveCallback(const uint8_t *mac, const uint8_t *data, int len) {
    if (userReceiveCallback) {
        std::string macStr = macToString(mac);
        std::vector<uint8_t> dataVec(data, data + len);
        userReceiveCallback(macStr, dataVec);
    }
}

// Callback interno de envío
void ESPNowMesh::onSentCallback(const uint8_t *mac, esp_now_send_status_t status) {
    if (userSentCallback) {
        std::string macStr = macToString(mac);
        userSentCallback(macStr, status == ESP_NOW_SEND_SUCCESS);
    }
}

// Convertir dirección MAC a string
std::string ESPNowMesh::macToString(const uint8_t *mac) {
    char buf[18];
    snprintf(buf, sizeof(buf), "%02X:%02X:%02X:%02X:%02X:%02X", 
             mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);
    return std::string(buf);
}
