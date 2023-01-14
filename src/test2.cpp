#include <boost/asio.hpp>

#include <iostream>
#include <thread>
#include <string>

std::string user_input;
std::string server_output;
std::mutex input_mutex;
std::condition_variable input_cv;

void input_thread()
{
    while (true)
    {
        std::cout << "Enter text: ";
        std::getline(std::cin, user_input);
        server_output = user_input;
        input_cv.notify_one();
    }
}

void network_thread()
{
    boost::asio::io_context io_context;
    boost::asio::ip::tcp::socket socket(io_context);
    boost::asio::ip::tcp::endpoint endpoint(boost::asio::ip::address::from_string("127.0.0.1"), 8080);

    while (true)
    {
        // Connect to the server and send the user input
        socket.connect(endpoint);
        boost::asio::write(socket, boost::asio::buffer(server_output));
    }
}

int main()
{
    std::thread input_t(input_thread);
    std::thread network_t(network_thread);

    input_t.join();
    network_t.join();

    return 0;
}
