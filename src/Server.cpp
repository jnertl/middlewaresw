
#include "Server.hpp"
#include <fcntl.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>
#include <iostream>
#include <spdlog/spdlog.h>

Server::Server() : updateIntervalMs(200), latest_rpm(0), latest_temperature(0), latest_oil_pressure(0), running(true) {}

// Mutex to protect shared engine data
std::mutex data_mutex;

void Server::start(int updateIntervalMs)
{
    this->updateIntervalMs = updateIntervalMs;
    server_thread = std::thread(&Server::run, this);
    data_thread = std::thread(&Server::updateDataLoop, this);
}

void Server::stop()
{
    running = false;
    spdlog::info("Server::stop");
    if (server_thread.joinable())
        server_thread.join();
    spdlog::info("server_thread stopped");
    if (data_thread.joinable())
        data_thread.join();
    spdlog::info("data_thread stopped");
}

int Server::getLatestRpm()
{
    std::lock_guard<std::mutex> lock(data_mutex);
    return latest_rpm;
}

int Server::getLatestTemperature()
{
    std::lock_guard<std::mutex> lock(data_mutex);
    return latest_temperature;
}

int Server::getLatestOilPressure()
{
    std::lock_guard<std::mutex> lock(data_mutex);
    return latest_oil_pressure;
}

void Server::run()
{
    int server_fd, client_fd;
    struct sockaddr_in address;
    int opt = 1;
    int addrlen = sizeof(address);
    const int PORT = 5555;

    bool fatal_error = false;
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        spdlog::error("socket failed");
        fatal_error = true;
    }
    else
    {
        // Set server_fd to non-blocking
        int flags = fcntl(server_fd, F_GETFL, 0);
        if (flags == -1 || fcntl(server_fd, F_SETFL, flags | O_NONBLOCK) == -1) {
            spdlog::error("fcntl O_NONBLOCK");
            close(server_fd);
            fatal_error = true;
        }
    }
    spdlog::info("Socket server created");
    if (!fatal_error && setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt)))
    {
        spdlog::error("setsockopt");
        close(server_fd);
        fatal_error = true;
    }
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);
    if (!fatal_error && bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0)
    {
        spdlog::error("bind failed");
        close(server_fd);
        fatal_error = true;
    }
    if (!fatal_error && listen(server_fd, 3) < 0)
    {
        spdlog::error("listen failed");
        close(server_fd);
        fatal_error = true;
    }
    if (!fatal_error)
    {
        spdlog::info("Socket server started on port {}", PORT);
    }

    while (running && !fatal_error)
    {
        if ((client_fd = accept(server_fd, (struct sockaddr *)&address, (socklen_t *)&addrlen)) < 0)
        {
            continue;
        }
        spdlog::info("Client connected.");
        char buffer[64];
        ssize_t valread;
        while ((valread = read(client_fd, buffer, sizeof(buffer) - 1)) > 0)
        {
            buffer[valread] = '\0';
            {
                std::lock_guard<std::mutex> lock(data_mutex);
                EngineData msg;
                msg.set_rpm(latest_rpm);
                msg.set_temperature(latest_temperature);
                msg.set_oil_pressure(latest_oil_pressure);
                std::string out;
                msg.SerializeToString(&out);
                uint32_t size = htonl(static_cast<uint32_t>(out.size()));
                send(client_fd, &size, sizeof(size), 0);
                send(client_fd, out.data(), out.size(), 0);
            }
        }
        spdlog::info("Client disconnected.");
        close(client_fd);
    }
    spdlog::info("Server stopped.");
    close(server_fd);
}

void Server::updateDataLoop()
{
    while (running)
    {
        {
            std::lock_guard<std::mutex> lock(data_mutex);
            latest_rpm = engine.getRpm();
            latest_temperature = engine.getTemperature();
            latest_oil_pressure = engine.getOilPressure();
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(updateIntervalMs));
    }
}
