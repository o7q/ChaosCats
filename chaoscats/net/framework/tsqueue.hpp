#pragma once

#include "common.hpp"

namespace net
{
    template <typename T>
    class TSQueue
    {
    public:
        TSQueue() = default;
        TSQueue(const TSQueue<T> &) = delete;
        virtual ~TSQueue() { clear(); }

    public:
        // Returns and maintains item at front of Queue
        const T &front()
        {
            std::scoped_lock lock(muxQueue);
            return dequeQueue.front();
        }

        // Returns and maintains item at back of Queue
        const T &back()
        {
            std::scoped_lock lock(muxQueue);
            return dequeQueue.back();
        }

        // Removes and returns item from front of Queue
        T pop_front()
        {
            std::scoped_lock lock(muxQueue);
            auto t = std::move(dequeQueue.front());
            dequeQueue.pop_front();
            return t;
        }

        // Removes and returns item from back of Queue
        T pop_back()
        {
            std::scoped_lock lock(muxQueue);
            auto t = std::move(dequeQueue.back());
            dequeQueue.pop_back();
            return t;
        }

        // Adds an item to back of Queue
        void push_back(const T &item)
        {
            std::scoped_lock lock(muxQueue);
            dequeQueue.emplace_back(std::move(item));

            std::unique_lock<std::mutex> ul(muxBlocking);
            cvBlocking.notify_one();
        }

        // Adds an item to front of Queue
        void push_front(const T &item)
        {
            std::scoped_lock lock(muxQueue);
            dequeQueue.emplace_front(std::move(item));

            std::unique_lock<std::mutex> ul(muxBlocking);
            cvBlocking.notify_one();
        }

        // Returns true if Queue has no items
        bool empty()
        {
            std::scoped_lock lock(muxQueue);
            return dequeQueue.empty();
        }

        // Returns number of items in Queue
        size_t count()
        {
            std::scoped_lock lock(muxQueue);
            return dequeQueue.size();
        }

        // Clears Queue
        void clear()
        {
            std::scoped_lock lock(muxQueue);
            dequeQueue.clear();
        }

        void wait()
        {
            while (empty())
            {
                std::unique_lock<std::mutex> ul(muxBlocking);
                cvBlocking.wait(ul);
            }
        }

    protected:
        std::mutex muxQueue;
        std::deque<T> dequeQueue;
        std::condition_variable cvBlocking;
        std::mutex muxBlocking;
    };
}