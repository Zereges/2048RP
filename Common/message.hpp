#pragma once
#include <cstdio>
#include <cstdlib>
#include <cstring>

class message
{
    public:
        static const size_t HEADER_LENGTH = 4;
        static const size_t MAX_BODY_LENGTH = 256;

        message() : m_body_length(0) { }
        message(const std::string& msg) : m_body_length(msg.length())
        {
            std::memcpy(body(), msg.c_str(), body_length());
            encode_header();
        }
        message(const char msg[]) : m_body_length(strlen(msg))
        {
            std::memcpy(body(), msg, body_length());
            encode_header();
        }

        const char* data() const { return m_data; }
        char* data() { return m_data; }

        const char* body() const { return m_data + HEADER_LENGTH; }
        char* body() { return m_data + HEADER_LENGTH; }

        size_t length() const { return HEADER_LENGTH + m_body_length; }
        size_t body_length() const { return m_body_length; }
        void body_length(size_t new_length) { m_body_length = m_body_length > MAX_BODY_LENGTH ? MAX_BODY_LENGTH : new_length; }

        bool decode_header()
        {
            char header[HEADER_LENGTH + 1] = "";
            std::strncat(header, m_data, HEADER_LENGTH);
            m_body_length = std::atoi(header);
            if (m_body_length > MAX_BODY_LENGTH)
            {
                m_body_length = 0;
                return false;
            }
            return true;
        }

        void encode_header()
        {
            char header[HEADER_LENGTH + 1] = "";
            std::sprintf(header, "%4zu", m_body_length);
            std::memcpy(m_data, header, HEADER_LENGTH);
        }

    private:
        char m_data[HEADER_LENGTH + MAX_BODY_LENGTH];
        size_t m_body_length;
};
