#pragma once
#include <cstdio>
#include <cstdlib>
#include <cstring>

/**!
    \brief Class representing messages being sent between client and server.
*/
class message
{
    public:
        static const size_t HEADER_LENGTH = 4; //!< Fixed header length
        static const size_t MAX_BODY_LENGTH = 256; //!< Max body lenght

        //! Constructs and empty message.
        message() : m_body_length(0) { }

        //! Copy constructor of the message.
        message(const std::string& msg) : m_body_length(msg.length())
        {
            std::memcpy(body(), msg.c_str(), body_length());
            encode_header();
        }
        
        //! Constructs the message from char[] buffer of data.
        message(const char msg[]) : m_body_length(strlen(msg))
        {
            std::memcpy(body(), msg, body_length());
            encode_header();
        }

        //! Const getter of data.
        //! \return beginning of the data.
        const char* data() const { return m_data; }
        //! Setter of data.
        //! \return beginning of the data.
        char* data() { return m_data; }

        //! Const getter of message body.
        //! \return beginning of the body.
        const char* body() const { return m_data + HEADER_LENGTH; }
        //! Setter of message body.
        //! \return beginning of the body.
        char* body() { return m_data + HEADER_LENGTH; }

        //! Gets total length of the message
        //! \return total length of the message including fixed \ref HEADER_LENGTH and variable \ref m_body_lenght
        size_t length() const { return HEADER_LENGTH + m_body_length; }

        //! Getter for body lenght.
        //! \return length of the body.
        size_t body_length() const { return m_body_length; }

        //! Setter for body length to <em>min(new_length, MAX_BODY_LENGTH)</em>.
        //! \param new_length Desired length of the body of the message.
        void body_length(size_t new_length) { m_body_length = m_body_length > MAX_BODY_LENGTH ? MAX_BODY_LENGTH : new_length; }

        //! Decodes the header and sets appropriately body lenght.
        //! \return true if succeeded, false otherwise.
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

        //! Encodes the header by the length of the body
        void encode_header()
        {
            char header[HEADER_LENGTH + 1] = "";
            std::sprintf(header, "%4zu", m_body_length);
            std::memcpy(m_data, header, HEADER_LENGTH);
        }

    private:
        char m_data[HEADER_LENGTH + MAX_BODY_LENGTH]; //!< Data of the message.
        size_t m_body_length; //!< Length of the body.
};
