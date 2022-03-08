#pragma once
#include <knoting/types.h>
#include <yojimbo.h>

namespace knot {
using namespace yojimbo;

constexpr int SERVER_PORT = 13189;
constexpr int CLIENT_PORT = 12646;
constexpr int MAX_CLIENTS = 6;
const char* const SERVER_ADDRESS = "127.0.0.1";

struct m_clientServerConfig : public ClientServerConfig {
    m_clientServerConfig() {
        ClientServerConfig();
        this->numChannels = 2;
        this->channel[0].type = yojimbo::CHANNEL_TYPE_RELIABLE_ORDERED;
        this->channel[1].type = yojimbo::CHANNEL_TYPE_UNRELIABLE_UNORDERED;
        this->timeout = 5;
        this->networkSimulator = false;
    }
};

class ClientMessage : public Message {
   public:
    ClientMessage() : sequence(0) {}

    template <typename Stream>
    bool Serialize(Stream& stream) {
        serialize_bits(stream, sequence, 16);
        return true;
    }

    uint16_t getSequence() { return sequence; }
    YOJIMBO_VIRTUAL_SERIALIZE_FUNCTIONS();

   protected:
    uint16_t sequence;
};

class ServerMessage : public Message {
   public:
    ServerMessage() : sequence(0) {}
    ~ServerMessage() {}
    template <typename Stream>
    bool Serialize(Stream& stream) {
        serialize_bits(stream, sequence, 16);
        for (int i = 0; i < 15; ++i) {
            serialize_string(stream, letter, 8);
        }
        return true;
    }

    uint16_t getSequence() { return sequence; }
    void setSequence(uint16_t seq) { sequence = seq; }
    YOJIMBO_VIRTUAL_SERIALIZE_FUNCTIONS();

   protected:
    uint16_t sequence;
    char* letter = "a";
};

enum MessageTypes { CLIENT_MESSAGE, SERVER_MESSAGE, NUM_MESSAGE_TYPES };

YOJIMBO_MESSAGE_FACTORY_START(ClientMessageFactory, NUM_MESSAGE_TYPES);
YOJIMBO_DECLARE_MESSAGE_TYPE(CLIENT_MESSAGE, ClientMessage);
YOJIMBO_DECLARE_MESSAGE_TYPE(SERVER_MESSAGE, ServerMessage);
YOJIMBO_MESSAGE_FACTORY_FINISH();

class ClientAdapter : public Adapter {
   public:
    MessageFactory* CreateMessageFactory(Allocator& allocator) {
        return YOJIMBO_NEW(allocator, ClientMessageFactory, allocator);
    }
};
static ClientAdapter clientAdapter;
}  // namespace knot
