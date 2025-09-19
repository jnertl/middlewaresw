
#include <iostream>
#include <thread>
#include <chrono>
#include <cstdlib>
#include <mutex>
#include <atomic>
#include <cstring>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>
#include "Engine.h"
#include "engine_data.pb.h"

int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cerr << "Usage: " << argv[0] << " <UpdateIntervalMs>" << std::endl;
        return 1;
    }
    int updateIntervalMs = std::atoi(argv[1]);
    if (updateIntervalMs <= 0) {
        std::cerr << "UpdateIntervalMs must be a positive integer." << std::endl;
        return 1;
    }
    EngineImpl engine;
    std::mutex data_mutex;
    int latest_rpm = 0;
    int latest_temperature = 0;
    std::atomic<bool> running{true};

    // Socket server thread
    std::thread server_thread([&]() {
        int server_fd, client_fd;
        struct sockaddr_in address;
        int opt = 1;
        int addrlen = sizeof(address);
        const int PORT = 5555;

        if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
            perror("socket failed");
            return;
        }
        if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt))) {
            perror("setsockopt");
            close(server_fd);
            return;
        }
        address.sin_family = AF_INET;
        address.sin_addr.s_addr = INADDR_ANY;
        address.sin_port = htons(PORT);
        if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
            perror("bind failed");
            close(server_fd);
            return;
        }
        if (listen(server_fd, 3) < 0) {
            perror("listen");
            close(server_fd);
            return;
        }
        std::cout << "Socket server started on port " << PORT << std::endl;
        while (running) {
            if ((client_fd = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen)) < 0) {
                if (running) perror("accept");
                continue;
            }
            std::cout << "Client connected." << std::endl;
            char buffer[64];
            ssize_t valread;
            while ((valread = read(client_fd, buffer, sizeof(buffer)-1)) > 0) {
                buffer[valread] = '\0';
                // On any request, send the latest values as protobuf
                int rpm, temp, oil_pressure;
                {
                    std::lock_guard<std::mutex> lock(data_mutex);
                    rpm = latest_rpm;
                    temp = latest_temperature;
                    oil_pressure = engine.getOilPressure();
                }
                EngineData msg;
                msg.set_rpm(rpm);
                msg.set_temperature(temp);
                msg.set_oil_pressure(oil_pressure);
                std::string out;
                msg.SerializeToString(&out);
                // Send size prefix (uint32_t, network byte order)
                uint32_t size = htonl(static_cast<uint32_t>(out.size()));
                send(client_fd, &size, sizeof(size), 0);
                send(client_fd, out.data(), out.size(), 0);
            }
            std::cout << "Client disconnected." << std::endl;
            close(client_fd);
        }
        close(server_fd);
    });

    // Main loop
    while (true) {
        int rpm = engine.getRpm();
        int temperature = engine.getTemperature();
        int oil_pressure = engine.getOilPressure();
        {
            std::lock_guard<std::mutex> lock(data_mutex);
            latest_rpm = rpm;
            latest_temperature = temperature;
        }
        std::cout << "Engine RPM:[" << rpm << "], Temperature:[" << temperature << "], Oil Pressure:[" << oil_pressure << "] psi" << std::endl;
        std::this_thread::sleep_for(std::chrono::milliseconds(updateIntervalMs));
    }

    running = false;
    if (server_thread.joinable()) server_thread.join();
    return 0;
}
