#pragma once

#include "common.hpp"
#include "tsqueue.hpp"
#include "message.hpp"
#include "connection.hpp"

namespace jnet
{
    // Server
    template <typename T>
    class ServerInterface
    {
    public:
        // Create a server, ready to listen on specified port
        ServerInterface(uint16_t port)
            : m_asioAcceptor(m_asioContext, asio::ip::tcp::endpoint(asio::ip::tcp::v4(), port))
        {
        }

        virtual ~ServerInterface()
        {
            // May as well try and tidy up
            stop();
        }

        // Starts the server!
        bool start()
        {
            try
            {
                // Issue a task to the asio context - This is important
                // as it will prime the context with "work", and stop it
                // from exiting immediately. Since this is a server, we
                // want it primed ready to handle clients trying to
                // connect.
                waitForClientConnection();

                // Launch the asio context in its own thread
                m_threadContext = std::thread([this]()
                                              { m_asioContext.run(); });
            }
            catch (std::exception &e)
            {
                // Something prohibited the server from listening
                std::cerr << "[SERVER] Exception: " << e.what() << std::endl;
                return false;
            }

            Logger::log(Logger::LogType::Info, "server", "Server started!");
            return true;
        }

        // Stops the server!
        void stop()
        {
            // Request the context to close
            m_asioContext.stop();

            // Tidy up the context thread
            if (m_threadContext.joinable())
            {
                m_threadContext.join();
            }

            Logger::log(Logger::LogType::Info, "server", "Clearing deque connections");
            m_dequeConnections.clear(); // <--- THIS LITTLE LINE, CAUSED ME SO MUCH TROUBLE. know this...
                                        // if omitted, the server will crash upon deconstruction

            // Inform someone, anybody, if they care...
            Logger::log(Logger::LogType::Info, "server", "Server stopped!");
        }

        // ASYNC - Instruct asio to wait for connection
        void waitForClientConnection()
        {
            // Prime context with an instruction to wait until a socket connects. This
            // is the purpose of an "acceptor" object. It will provide a unique socket
            // for each incoming connection attempt
            m_asioAcceptor.async_accept(
                [this](std::error_code ec, asio::ip::tcp::socket socket)
                {
                    // Triggered by incoming connection request
                    if (!ec)
                    {
                        // Display some useful(?) information
                        Logger::log(Logger::LogType::Info, "server", "New connection: " + socket.remote_endpoint().address().to_string());

                        // Create a new connection to handle this client
                        std::shared_ptr<Connection<T>> newConnection =
                            std::make_shared<Connection<T>>(Connection<T>::Owner::Server,
                                                            m_asioContext, std::move(socket), m_queueMessagesIn);

                        // Give the user server a chance to deny connection
                        if (onClientConnect(newConnection))
                        {
                            // Connection allowed, so add to container of new connections
                            m_dequeConnections.push_back(std::move(newConnection));

                            // And very important! Issue a task to the connection's
                            // asio context to sit and wait for bytes to arrive!
                            m_dequeConnections.back()->connectToClient(this, IDCounter++);

                            Logger::log(Logger::LogType::Info, "server", "Connection approved: " + std::to_string(m_dequeConnections.back()->getID()));
                        }
                        else
                        {
                            Logger::log(Logger::LogType::Info, "server", "A connection was denied");

                            // Connection will go out of scope with no pending tasks, so will
                            // get destroyed automagically due to the wonder of smart pointers
                        }
                    }
                    else
                    {
                        // Error has occurred during acceptance
                        Logger::log(Logger::LogType::Info, "server", "Connection error: " + ec.message());
                    }

                    // Prime the asio context with more work - again simply wait for
                    // another connection...
                    waitForClientConnection();
                });
        }

        // Send a message to a specific client
        void sendMessageToClient(std::shared_ptr<Connection<T>> clientConnection, const Message<T> &message)
        {
            // Check client is legitimate...
            if (clientConnection && clientConnection->isConnected())
            {
                // ...and post the message via the connection
                clientConnection->sendMessage(message); // ORIGINALLY: clientConnection->sendMessageToServer(message)
            }
            else
            {
                // If we cant communicate with client then we may as
                // well remove the client - let the server know, it may
                // be tracking it somehow
                onClientDisconnect(clientConnection);

                // Off you go now, bye bye!
                clientConnection.reset();

                // Then physically remove it from the container
                m_dequeConnections.erase(
                    std::remove(m_dequeConnections.begin(), m_dequeConnections.end(), clientConnection), m_dequeConnections.end());
            }
        }

        // Send message to all clients
        void sendMessageToAllClients(const Message<T> &msg, std::shared_ptr<Connection<T>> ignoreClient = nullptr)
        {
            bool doesInvalidClientExist = false;

            // Iterate through all clients in container
            for (auto &clientConnection : m_dequeConnections)
            {
                // Check client is connected...
                if (clientConnection && clientConnection->isConnected())
                {
                    // ..it is!
                    if (clientConnection != ignoreClient)
                    {
                        clientConnection->sendMessage(msg);
                    }
                }
                else
                {
                    // The client couldnt be contacted, so assume it has
                    // disconnected.
                    onClientDisconnect(clientConnection);
                    clientConnection.reset();

                    // Set this flag to then remove dead clients from container
                    doesInvalidClientExist = true;
                }
            }

            // Remove dead clients, all in one go - this way, we dont invalidate the
            // container as we iterated through it.
            if (doesInvalidClientExist)
            {
                m_dequeConnections.erase(
                    std::remove(m_dequeConnections.begin(), m_dequeConnections.end(), nullptr), m_dequeConnections.end());
            }
        }

        // Force server to respond to incoming messages
        void update(size_t maxMessages = -1, bool shouldWait = false)
        {
            if (shouldWait)
            {
                m_queueMessagesIn.wait();
            }

            // Process as many messages as you can up to the value
            // specified
            size_t messageCount = 0;
            while (messageCount < maxMessages && !m_queueMessagesIn.empty())
            {
                // Grab the front message
                auto message = m_queueMessagesIn.pop_front();

                // Pass to message handler
                onMessage(message.remote, message.message);

                messageCount++;
            }
        }

    public:
        // This server class should override thse functions to implement
        // customised functionality

        // Called when a client connects, you can veto the connection by returning false
        virtual bool onClientConnect(std::shared_ptr<Connection<T>> client)
        {
            return false;
        }

        // Called when a client is validated
        virtual void onClientValidated(std::shared_ptr<Connection<T>> client)
        {
        }

        // Called when a client appears to have disconnected
        virtual void onClientDisconnect(std::shared_ptr<Connection<T>> client)
        {
        }

        // Called when a message arrives
        virtual void onMessage(std::shared_ptr<Connection<T>> client, Message<T> &message)
        {
        }

    protected:
        // Thread Safe Queue for incoming message packets
        TSQueue<OwnedMessage<T>> m_queueMessagesIn;

        // Container of active validated connections
        std::deque<std::shared_ptr<Connection<T>>> m_dequeConnections;

        // Order of declaration is important - it is also the order of initialisation
        asio::io_context m_asioContext;
        std::thread m_threadContext;

        // These things need an asio context
        asio::ip::tcp::acceptor m_asioAcceptor; // Handles new incoming connection attempts...

        // Clients will be identified in the "wider system" via an ID
        uint32_t IDCounter = 10000;
    };
}