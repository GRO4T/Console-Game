/* Copyright 2024 Damian Kolaska */
#include <string.h>

#include <algorithm>
#include <asio.hpp>
#include <fstream>
#include <list>
#include <vector>

#include "assets.h"
#include "config.h"
#include "game.h"
#include "menu.h"
#include "window.h"

// TODO(GRO4T): This should be read from the assets file.
const std::vector<std::string> kMapNames = {"Plain Flat", "Hills", "Valley", "Platformer"};

using namespace ascii_combat;  // NOLINT

int main(int, char* argv[]) {
    const auto mode = std::string(argv[1]);

    if ("server" == mode) {
        const auto port = std::stoi(argv[2]);

        asio::error_code ec;
        asio::io_context context;

        asio::ip::tcp::acceptor acceptor(context,
                                         asio::ip::tcp::endpoint(asio::ip::tcp::v4(), port));
        asio::ip::tcp::socket socket(context);
        acceptor.accept(socket);

        std::cout << "Accepted connection" << std::endl;

        char data[1024];
        socket.read_some(asio::buffer(data), ec);

        std::cout << "Received msg: " << data << std::endl;
    } else if ("client" == mode) {
        const auto server_addr = std::string(argv[2]);
        const auto server_port = std::stoi(argv[3]);

        asio::error_code ec;
        asio::io_context context;

        asio::ip::tcp::endpoint endpoint(asio::ip::make_address(server_addr, ec), server_port);

        asio::ip::tcp::socket socket(context);

        socket.connect(endpoint, ec);

        if (!ec) {
            std::cout << "connected" << std::endl;
        } else {
            std::cout << "failed to connect to address:\n" << ec.message() << std::endl;
        }

        std::string msg = "Hello Asio";
        socket.write_some(asio::buffer(msg.data(), msg.size() + 1), ec);
    }

    std::string buffer;
    std::cin >> buffer;

    Window window(kWindowHeight, kWindowWidth, kWindowTopLeftX, kWindowTopLeftY, kWindowPadding);

    std::string game_mode = "";
    do {
        game_mode = Menu(window, {kPlayerVsAIMode, kLocalMultiplayerMode, kMultiplayerMode, kExit})
                        .GetChoice();

        if (kLocalMultiplayerMode == game_mode) {
            std::string map_name = Menu(window, kMapNames).GetChoice();
            std::size_t map_id =
                std::find(kMapNames.begin(), kMapNames.end(), map_name) - kMapNames.begin();
            auto map = Assets::Instance().GetMaps()[map_id];
            Game game(window, map);
            game.Run();
        }
    } while (kExit != game_mode);

    return 0;
}
