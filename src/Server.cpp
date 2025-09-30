
#include "Server.hpp"
#include <fcntl.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>
#include <iostream>

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
    std::cout << "Server::stop" << std::endl;
    if (server_thread.joinable())
        server_thread.join();
    std::cout << "server_thread stopped" << std::endl;
    if (data_thread.joinable())
        data_thread.join();
    std::cout << "data_thread stopped" << std::endl;
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
        perror("socket failed");
        fatal_error = true;
    }
    else
    {
        // Set server_fd to non-blocking
        int flags = fcntl(server_fd, F_GETFL, 0);
        if (flags == -1 || fcntl(server_fd, F_SETFL, flags | O_NONBLOCK) == -1) {
            perror("fcntl O_NONBLOCK");
            close(server_fd);
            fatal_error = true;
        }
    }
    std::cout << "Socket server created" << std::endl;
    if (!fatal_error && setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt)))
    {
        perror("setsockopt");
        close(server_fd);
        fatal_error = true;
    }
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);
    if (!fatal_error && bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0)
    {
        perror("bind failed");
        close(server_fd);
        fatal_error = true;
    }
    if (!fatal_error && listen(server_fd, 3) < 0)
    {
        perror("listen");
        close(server_fd);
        fatal_error = true;
    }
    if (!fatal_error)
    {
        std::cout << "Socket server started on port " << PORT << std::endl;
    }

    while (running && !fatal_error)
    {
        if ((client_fd = accept(server_fd, (struct sockaddr *)&address, (socklen_t *)&addrlen)) < 0)
        {
            continue;
        }
        std::cout << "Client connected." << std::endl;
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
        std::cout << "Client disconnected." << std::endl;
        close(client_fd);
    }
    std::cout << "Server stopped." << std::endl;
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
