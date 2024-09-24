#pragma once

#include "common.hpp"
#include "tsqueue.hpp"
#include "message.hpp"

namespace jnet
{
    // Connection
    // Forward declare
    template <typename T>
    class ServerInterface;

    template <typename T>
    class Connection : public std::enable_shared_from_this<Connection<T>>
    {
    public:
        // A connection is "owned" by either a server or a client, and its
        // behaviour is slightly different bewteen the two.
        enum class Owner
        {
            Server,
            Client
        };

    public:
        // Constructor: Specify Owner, connect to context, transfer the socket
        //				Provide reference to incoming message queue
        Connection(Owner parent, asio::io_context &asioContext, asio::ip::tcp::socket socket, TSQueue<OwnedMessage<T>> &queueIn)
            : m_asioContext(asioContext), m_socket(std::move(socket)), m_queueMessagesIn(queueIn)
        {
            m_ownerType = parent;

            // Construct validation check data
            if (m_ownerType == Owner::Server)
            {
                // Connection is Server -> Client, construct random data for the client
                // to transform and send back for validation
                m_handshakeOut = uint64_t(std::chrono::system_clock::now().time_since_epoch().count());

                // Pre-calculate the result for checking when the client responds
                m_handshakeCheck = scramble(m_handshakeOut);
            }
            else
            {
                // Connection is Client -> Server, so we have nothing to define,
                m_handshakeIn = 0;
                m_handshakeOut = 0;
            }
        }

        virtual ~Connection()
        {
        }

        // This ID is used system wide - its how clients will understand other clients
        // exist across the whole system.
        uint32_t getID() const
        {
            return id;
        }

    public:
        void connectToClient(jnet::ServerInterface<T> *server, uint32_t uid = 0)
        {
            if (m_ownerType == Owner::Server)
            {
                if (m_socket.is_open())
                {
                    id = uid;

                    // Was: ReadHeader();

                    // A client has attempted to connect to the server, but we wish
                    // the client to first validate itself, so first write out the
                    // handshake data to be validated
                    writeValidation();

                    // Next, issue a task to sit and wait asynchronously for precisely
                    // the validation data sent back from the client
                    readValidation(server);
                }
            }
        }

        void connectToServer(const asio::ip::tcp::resolver::results_type &endpoints)
        {
            // Only clients can connect to servers
            if (m_ownerType == Owner::Client)
            {
                // Request asio attempts to connect to an endpoint
                asio::async_connect(m_socket, endpoints,
                                    [this](std::error_code ec, asio::ip::tcp::endpoint endpoint)
                                    {
                                        if (!ec)
                                        {
                                            // Was: ReadHeader();

                                            // First thing server will do is send packet to be validated
                                            // so wait for that and respond
                                            readValidation();
                                        }
                                    });
            }
        }

        void disconnect()
        {
            if (isConnected())
                asio::post(m_asioContext, [this]()
                           { m_socket.close(); });
        }

        bool isConnected() const
        {
            return m_socket.is_open();
        }

        // Prime the connection to wait for incoming messages
        void startListening()
        {
        }

    public:
        // ASYNC - Send a message, connections are one-to-one so no need to specifiy
        // the target, for a client, the target is the server and vice versa
        void sendMessage(const Message<T> &message)
        {
            asio::post(m_asioContext,
                       [this, message]()
                       {
                           // If the queue has a message in it, then we must
                           // assume that it is in the process of asynchronously being written.
                           // Either way add the message to the queue to be output. If no messages
                           // were available to be written, then start the process of writing the
                           // message at the front of the queue.
                           bool isWritingMessage = !m_queueMessagesOut.empty();
                           m_queueMessagesOut.push_back(message);
                           if (!isWritingMessage)
                           {
                               writeHeader();
                           }
                       });
        }

    private:
        // ASYNC - Prime context to write a message header
        void writeHeader()
        {
            // If this function is called, we know the outgoing message queue must have
            // at least one message to send. So allocate a transmission buffer to hold
            // the message, and issue the work - asio, send these bytes
            asio::async_write(m_socket, asio::buffer(&m_queueMessagesOut.front().header, sizeof(MessageHeader<T>)),
                              [this](std::error_code ec, std::size_t length)
                              {
                                  // asio has now sent the bytes - if there was a problem
                                  // an error would be available...
                                  if (!ec)
                                  {
                                      // ... no error, so check if the message header just sent also
                                      // has a message body...
                                      if (m_queueMessagesOut.front().body.size() > 0)
                                      {
                                          // ...it does, so issue the task to write the body bytes
                                          writeBody();
                                      }
                                      else
                                      {
                                          // ...it didnt, so we are done with this message. Remove it from
                                          // the outgoing message queue
                                          m_queueMessagesOut.pop_front();

                                          // If the queue is not empty, there are more messages to send, so
                                          // make this happen by issuing the task to send the next header.
                                          if (!m_queueMessagesOut.empty())
                                          {
                                              writeHeader();
                                          }
                                      }
                                  }
                                  else
                                  {
                                      // ...asio failed to write the message, we could analyse why but
                                      // for now simply assume the connection has died by closing the
                                      // socket. When a future attempt to write to this client fails due
                                      // to the closed socket, it will be tidied up.
                                      std::cout << "[" << id << "] Write Header Fail.\n";
                                      m_socket.close();
                                  }
                              });
        }

        // ASYNC - Prime context to write a message body
        void writeBody()
        {
            // If this function is called, a header has just been sent, and that header
            // indicated a body existed for this message. Fill a transmission buffer
            // with the body data, and send it!
            asio::async_write(m_socket, asio::buffer(m_queueMessagesOut.front().body.data(), m_queueMessagesOut.front().body.size()),
                              [this](std::error_code ec, std::size_t length)
                              {
                                  if (!ec)
                                  {
                                      // Sending was successful, so we are done with the message
                                      // and remove it from the queue
                                      m_queueMessagesOut.pop_front();

                                      // If the queue still has messages in it, then issue the task to
                                      // send the next messages' header.
                                      if (!m_queueMessagesOut.empty())
                                      {
                                          writeHeader();
                                      }
                                  }
                                  else
                                  {
                                      // Sending failed, see WriteHeader() equivalent for description :P
                                      std::cout << "[" << id << "] Write Body Fail.\n";
                                      m_socket.close();
                                  }
                              });
        }

        // ASYNC - Prime context ready to read a message header
        void readHeader()
        {
            // If this function is called, we are expecting asio to wait until it receives
            // enough bytes to form a header of a message. We know the headers are a fixed
            // size, so allocate a transmission buffer large enough to store it. In fact,
            // we will construct the message in a "temporary" message object as it's
            // convenient to work with.
            asio::async_read(m_socket, asio::buffer(&m_messageTemporaryIn.header, sizeof(MessageHeader<T>)),
                             [this](std::error_code ec, std::size_t length)
                             {
                                 if (!ec)
                                 {
                                     // A complete message header has been read, check if this message
                                     // has a body to follow...
                                     if (m_messageTemporaryIn.header.size > 0)
                                     {
                                         // ...it does, so allocate enough space in the messages' body
                                         // vector, and issue asio with the task to read the body.
                                         m_messageTemporaryIn.body.resize(m_messageTemporaryIn.header.size);
                                         readBody();
                                     }
                                     else
                                     {
                                         // it doesn't, so add this bodyless message to the connections
                                         // incoming message queue
                                         addToIncomingMessageQueue();
                                     }
                                 }
                                 else
                                 {
                                     // Reading form the client went wrong, most likely a disconnect
                                     // has occurred. Close the socket and let the system tidy it up later.
                                     std::cout << "[" << id << "] Read Header Fail.\n";
                                     m_socket.close();
                                 }
                             });
        }

        // ASYNC - Prime context ready to read a message body
        void readBody()
        {
            // If this function is called, a header has already been read, and that header
            // request we read a body, The space for that body has already been allocated
            // in the temporary message object, so just wait for the bytes to arrive...
            asio::async_read(m_socket, asio::buffer(m_messageTemporaryIn.body.data(), m_messageTemporaryIn.body.size()),
                             [this](std::error_code ec, std::size_t length)
                             {
                                 if (!ec)
                                 {
                                     // ...and they have! The message is now complete, so add
                                     // the whole message to incoming queue
                                     addToIncomingMessageQueue();
                                 }
                                 else
                                 {
                                     // As above!
                                     std::cout << "[" << id << "] Read Body Fail.\n";
                                     m_socket.close();
                                 }
                             });
        }

        // "Encrypt" data
        uint64_t scramble(uint64_t nInput)
        {
            uint64_t out = nInput ^ 0xDEADBEEFC0DECAFE;
            out = (out & 0xF0F0F0F0F0F0F0) >> 4 | (out & 0x0F0F0F0F0F0F0F) << 4;
            return out ^ 0xC0DEFACE12345678;
        }

        // ASYNC - Used by both client and server to write validation packet
        void writeValidation()
        {
            asio::async_write(m_socket, asio::buffer(&m_handshakeOut, sizeof(uint64_t)),
                              [this](std::error_code ec, std::size_t length)
                              {
                                  if (!ec)
                                  {
                                      // Validation data sent, clients should sit and wait
                                      // for a response (or a closure)
                                      if (m_ownerType == Owner::Client)
                                      {
                                          readHeader();
                                      }
                                  }
                                  else
                                  {
                                      m_socket.close();
                                  }
                              });
        }

        void readValidation(jnet::ServerInterface<T> *server = nullptr)
        {
            asio::async_read(m_socket, asio::buffer(&m_handshakeIn, sizeof(uint64_t)),
                             [this, server](std::error_code ec, std::size_t length)
                             {
                                 if (!ec)
                                 {
                                     if (m_ownerType == Owner::Server)
                                     {
                                         // Connection is a server, so check response from client

                                         // Compare sent data to actual solution
                                         if (m_handshakeIn == m_handshakeCheck)
                                         {
                                             // Client has provided valid solution, so allow it to connect properly
                                             std::cout << "Client Validated" << std::endl;
                                             server->onClientValidated(this->shared_from_this());

                                             // Sit waiting to receive data now
                                             readHeader();
                                         }
                                         else
                                         {
                                             // Client gave incorrect data, so disconnect
                                             std::cout << "Client Disconnected (Fail Validation)" << std::endl;
                                             m_socket.close();
                                         }
                                     }
                                     else
                                     {
                                         // Connection is a client, so solve puzzle
                                         m_handshakeOut = scramble(m_handshakeIn);

                                         // Write the result
                                         writeValidation();
                                     }
                                 }
                                 else
                                 {
                                     // Some biggerfailure occured
                                     std::cout << "Client Disconnected (ReadValidation)" << std::endl;
                                     m_socket.close();
                                 }
                             });
        }

        // Once a full message is received, add it to the incoming queue
        void addToIncomingMessageQueue()
        {
            // Shove it in queue, converting it to an "owned message", by initialising
            // with the a shared pointer from this connection object
            if (m_ownerType == Owner::Server)
            {
                m_queueMessagesIn.push_back({this->shared_from_this(), m_messageTemporaryIn});
            }
            else
            {
                m_queueMessagesIn.push_back({nullptr, m_messageTemporaryIn});
            }

            // We must now prime the asio context to receive the next message. It
            // wil just sit and wait for bytes to arrive, and the message construction
            // process repeats itself. Clever huh?
            readHeader();
        }

    protected:
        // This context is shared with the whole asio instance
        asio::io_context &m_asioContext;

        // Each connection has a unique socket to a remote
        asio::ip::tcp::socket m_socket;

        // This queue holds all messages to be sent to the remote side
        // of this connection
        TSQueue<Message<T>> m_queueMessagesOut;

        // This references the incoming queue of the parent object
        TSQueue<OwnedMessage<T>> &m_queueMessagesIn;

        // Incoming messages are constructed asynchronously, so we will
        // store the part assembled message here, until it is ready
        Message<T> m_messageTemporaryIn;

        // The "owner" decides how some of the connection behaves
        Owner m_ownerType = Owner::Server;

        // Handshake Validation
        uint64_t m_handshakeOut = 0;
        uint64_t m_handshakeIn = 0;
        uint64_t m_handshakeCheck = 0;

        bool m_isValidHandshake = false;
        bool m_isConnectionEstablished = false;

        uint32_t id = 0;
    };
}