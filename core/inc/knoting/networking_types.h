#pragma once
#include <knoting/types.h>
#include <yojimbo.h>

namespace knot {
using namespace yojimbo;

constexpr int SERVER_PORT = 13189;
constexpr int CLIENT_PORT = 12646;
constexpr int MAX_CLIENTS = 6;
const char* const SERVER_ADDRESS = "127.0.0.1";
const double TICK = 1.0 / 30.0;

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

class OurMessage : public Message {
   public:
    OurMessage() : m_sequence(0), m_recentAck(0) {}

    template <typename Stream>
    bool Serialize(Stream& stream) {
        serialize_bits(stream, m_sequence, 16);
        serialize_bits(stream, m_recentAck, 16);
        return true;
    }

    uint16_t get_sequence() { return m_sequence; }
    void set_sequence(uint16_t seq) { m_sequence = seq; }

    uint16_t get_recent_ack() { return m_recentAck; }
    void set_ack(uint16_t reAck) { m_recentAck = reAck; }
    YOJIMBO_VIRTUAL_SERIALIZE_FUNCTIONS();

   protected:
    uint16_t m_sequence;
    uint16_t m_recentAck;
};

class ClientMessage : public OurMessage {
   public:
    ClientMessage() : OurMessage() {}

    uint8_t m_lookXAxis;
    uint8_t m_lookYAxis;

    uint8_t m_moveXAxis;
    uint8_t m_moveYAxis;

    bool jumpPressed;
    bool isShooting;
    bool abilityUsed;
};

class ServerMessage : public OurMessage {
   public:
    ServerMessage() : OurMessage() {}

    std::array<vec3, MAX_CLIENTS> playerPos;
    std::array<quat, MAX_CLIENTS> playerRots;
    std::array<uint16_t, MAX_CLIENTS> fsnjfs;
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
