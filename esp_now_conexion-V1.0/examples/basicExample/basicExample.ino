#include <ESPNowMesh.h>

ESPNowMesh mesh;

void onReceive(const std::string &mac, const std::vector<uint8_t> &data) {
    Serial.printf("Mensaje recibido de %s: %s\n", mac.c_str(), std::string(data.begin(), data.end()).c_str());
}

void onSent(const std::string &mac, bool success) {
    Serial.printf("Mensaje enviado a %s: %s\n", mac.c_str(), success ? "Éxito" : "Fallo");
}

void setup() {
    Serial.begin(115200);
    if (!mesh.begin()) {
        Serial.println("Error iniciando ESP-NOW");
        return;
    }

    mesh.onReceive(onReceive);
    mesh.onSent(onSent);

    std::string peerMac = "24:6F:28:AB:CD:EF";
    mesh.addPeer(peerMac);

    mesh.send(peerMac, "Hola, ESP-NOW!");
    mesh.send(peerMac, 42); // Enviar un entero
}

void loop() {}
