#include <boost/asio.hpp>

#include <iostream>
#include <thread>
#include <string>
#include <chrono>

std::string user_input;
std::string network_output;
std::mutex input_mutex;
std::condition_variable input_cv;

void input_thread()
{
    while (true)
    {
        std::cout << "Введите текст:" << std::endl;
        std::getline(std::cin, user_input);
        network_output = user_input;
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
        {
            std::unique_lock<std::mutex> lock(input_mutex);
            input_cv.wait(lock);
        }

        do
        {
            try
            {
                // Подключение к серверу и отправка текста
                socket.connect(endpoint);
                boost::asio::write(socket, boost::asio::buffer(network_output));
                break;
            }
            catch (const std::exception &e)
            {
                std::cout << "Не удалось отправить данные: " << e.what() << std::endl;
                std::cout << "Переподключение произойдёт через 5 секунд..." << std::endl;
                std::this_thread::sleep_for(std::chrono::seconds(5));
            }
        } while (true);
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
