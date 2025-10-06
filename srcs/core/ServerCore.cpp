#include "../../includes/webserv.hpp"

ServerCore::ServerCore(const Config &config) : config(config) {}

void ServerCore::init_sockets()
{
    int socket_fd;
    for (size_t i = 0; i < config.pairs.size(); i++)
    {
        if (listen_sockets.find(config.pairs[i]) == listen_sockets.end())
        {
            socket_fd = socket(AF_INET, SOCK_STREAM, 0);
            if (socket_fd < 0)
            {
                throw CoreError("socket()", errno, strerror(errno));
            }
            int opt_val = 1;
            if (setsockopt(socket_fd, SOL_SOCKET, SO_REUSEADDR, &opt_val, sizeof(opt_val)) < 0)
            {
                throw CoreError("setsockopt()", errno, strerror(errno));
            }
            struct sockaddr_in addr;
            addr.sin_family = AF_INET;
            addr.sin_port = htons(config.pairs[i].second);
            addr.sin_addr.s_addr = inet_addr(config.pairs[i].first.c_str());
            if (bind(socket_fd, (struct sockaddr *)&addr, sizeof(addr)) < 0)
            {
                throw CoreError("bind()", errno, strerror(errno));
            }
            if (listen(socket_fd, SOMAXCONN) < 0)
            {
                throw CoreError("listen()", errno, strerror(errno));
            }
            listen_sockets.insert(std::make_pair(config.pairs[i], socket_fd));
            std::cout << "Listening on " << config.pairs[i].first
                      << ":" << config.pairs[i].second
                      << " (fd=" << socket_fd << ")" << std::endl;
            // int client_socket;
            // int addr_len = sizeof(addr);
            // std::cout << "\n\n\n Waiting for clients \n\n\n";
            // if ((client_socket = accept(socket_fd, (struct sockaddr *)&addr, (socklen_t *)&addr_len)) < 0)
            // {
            //     throw CoreError("accept()", errno, strerror(errno));
            // }
            // char buffer[30000] = {0};
            // read(client_socket, buffer, 30000);
            // printf("%s\n", buffer);
            // close(client_socket);
        }
    }
}

const Server &ServerCore::findserver(std::string ip, int port)
{
    int try_count = 2;
    while (try_count > 0)
    {
        for (size_t i = 0; i < config.servers.size(); i++)
        {
            for (size_t j = 0; j < config.servers[i].directives.size(); j++)
            {
                if (config.servers[i].directives[j].name == "listen")
                {
                    if (config.servers[i].directives[j].args[0] == ip &&
                        atoi(config.servers[i].directives[j].args[1].c_str()) == port)
                    {
                        return config.servers[i];
                    }
                }
            }
        }
        ip = "0.0.0.0";
        try_count--;
    }
    throw CoreError("findserver()", 1, "No server configured for IP " + ip + " and port " + intTo_String(port));
}