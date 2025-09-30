#include "../../includes/webserv.hpp"

ServerCore::ServerCore(const Config &config) : config(config) {}

int ServerCore::init_sockets()
{
    int socket_fd;
    try
    {
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
                int client_socket;
                int addr_len = sizeof(addr);
                std::cout << "\n\n\n Waiting for clients \n\n\n";
                if ((client_socket = accept(socket_fd, (struct sockaddr *)&addr, (socklen_t *)&addr_len)) < 0)
                {
                    throw CoreError("accept()", errno, strerror(errno));
                }
                char buffer[30000] = {0};
                read(client_socket, buffer, 30000);
                printf("%s\n", buffer);
                close(client_socket);
            }
        }
    }
    catch (CoreError &error)
    {
        std::cerr << error.mesg;
        return 1;
    }
    return 0;
}