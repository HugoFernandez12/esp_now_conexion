template <typename T>
bool esp_now_conexion::send(const std::string &peerMac, const T &data) {
    const uint8_t *rawData = reinterpret_cast<const uint8_t *>(&data);
    return send(peerMac, std::string(rawData, rawData + sizeof(T)));
}
