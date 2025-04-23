#pragma once

namespace narc_core
{
    template <typename... Args>
    class Event
    {
    public:
        using HandlerFunc = std::function<void(Args...)>;

        class Subscription
        {
        public:
            Subscription(size_t id, Event* event) : m_id(id), m_event(event) {}
            ~Subscription()
            {
                if (m_event)
                {
                    m_event->unsubscribe(m_id);
                }
            }
            Subscription(const Subscription&) = delete;
            Subscription& operator=(const Subscription&) = delete;

            Subscription(Subscription&& other) noexcept : m_id(other.m_id), m_event(other.m_event)
            {
                other.m_event = nullptr;
            }

            Subscription& operator=(Subscription&& other) noexcept
            {
                if (this != &other)
                {
                    unsubscribe();
                    m_id = other.m_id;
                    m_event = other.m_event;
                    other.m_event = nullptr;
                }
                return *this;
            }

            void unsubscribe()
            {
                if (m_event)
                {
                    m_event->unsubscribe(m_id);
                    m_event = nullptr;
                }
            }

        private:
            size_t m_id;
            Event* m_event;
        };

        Subscription subscribe(HandlerFunc&& handler)
        {
            size_t id = m_nextId++;
            m_handlers.emplace(id, std::move(handler));
            return Subscription(id, this);
        }

        void trigger(Args... args) const
        {
            for (const auto& [id, handler] : m_handlers)
            {
                handler(args...);
            }
        }

    private:
        void unsubscribe(size_t id)
        {
            m_handlers.erase(id);
        }

        std::unordered_map<size_t, HandlerFunc> m_handlers;
        std::atomic<size_t> m_nextId{ 0 };
    };
}