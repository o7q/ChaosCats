#pragma once

#include "common.hpp"

namespace net
{
    template <typename T>
    class ClientInterface
    {
    public:
        ClientInterface()
        {
        }

        virtual ~ClientInterface()
        {
            // If the client is destroyed, always try and disconnect from server
            disconnect();
        }

    public:
        // Connect to server with hostname/ip-address and port
        bool connect(const std::string &address, const uint16_t port)
        {
            try
            {
                // Resolve hostname/ip-address into tangiable physical address
                asio::ip::tcp::resolver resolver(m_context);
                asio::ip::tcp::resolver::results_type endpoints = resolver.resolve(address, std::to_string(port));

                // Create connection
                m_connection = std::make_unique<Connection<T>>(Connection<T>::Owner::Client, m_context, asio::ip::tcp::socket(m_context), m_queueMessagesIn);

                // Tell the connection object to connect to server
                m_connection->connectToServer(endpoints);

                // Start Context Thread
                threadContext = std::thread([this]()
                                            { m_context.run(); });
            }
            catch (std::exception &e)
            {
                Logger::log(Logger::LogType::Error, "net-client", "Exception: " + std::string(e.what()));
                return false;
            }
            return true;
        }

        // Disconnect from server
        void disconnect()
        {
            // If connection exists, and it's connected then...
            if (isConnectedToServer())
            {
                // ...disconnect from server gracefully
                m_connection->disconnect();
            }

            // Either way, we're also done with the asio context...
            m_context.stop();
            // ...and its thread
            if (threadContext.joinable())
            {
                threadContext.join();
            }

            // Destroy the connection object
            m_connection.release();
        }

        // Check if client is actually connected to a server
        bool isConnectedToServer()
        {
            if (m_connection)
            {
                return m_connection->isConnected();
            }
            else
            {
                return false;
            }
        }

    public:
        // Send message to server
        void sendMessageToServer(const Message<T> &message)
        {
            if (isConnectedToServer())
            {
                m_connection->sendMessage(message);
            }
        }

        // Retrieve queue of messages from server
        TSQueue<OwnedMessage<T>> &incoming()
        {
            return m_queueMessagesIn;
        }

    protected:
        // asio context handles the data transfer...
        asio::io_context m_context;
        // ...but needs a thread of its own to execute its work commands
        std::thread threadContext;
        // The client has a single instance of a "connection" object, which handles data transfer
        std::unique_ptr<Connection<T>> m_connection;

    private:
        // This is the thread safe queue of incoming messages from server
        TSQueue<OwnedMessage<T>> m_queueMessagesIn;
    };
}