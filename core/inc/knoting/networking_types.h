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
        this->maxPacketSize = 1500;
    }
};

class ClientMessage : public Message {
   public:
    ClientMessage() : m_sequence(0), m_recentAck(0) {
        m_lookAxis = vec2i();
        m_moveAxis = vec2i();
        jumpPressed = false;
        isShooting = false;
    }

    template <typename Stream>
    bool Serialize(Stream& stream) {
        serialize_bits(stream, m_sequence, 16);
        serialize_bits(stream, m_recentAck, 16);

        auto m_lookXAxis = (int16_t)m_lookAxis.x;
        auto m_lookYAxis = (int16_t)m_lookAxis.y;
        serialize_int(stream, m_lookXAxis, -32767, 32767);
        serialize_int(stream, m_lookYAxis, -32767, 32767);
        m_lookAxis.x = m_lookXAxis;
        m_lookAxis.y = m_lookYAxis;

        auto m_moveXAxis = (int16_t)m_moveAxis.x;
        auto m_moveYAxis = (int16_t)m_moveAxis.y;
        serialize_int(stream, m_moveXAxis, -32767, 32767);
        serialize_int(stream, m_moveYAxis, -32767, 32767);
        m_moveAxis.x = m_moveXAxis;
        m_moveAxis.y = m_moveYAxis;

        serialize_bool(stream, jumpPressed);
        serialize_bool(stream, isShooting);
        return true;
    }

    vec2i m_lookAxis;
    vec2i m_moveAxis;

    bool jumpPressed;
    bool isShooting;

    uint16_t get_sequence() { return m_sequence; }
    void set_sequence(uint16_t seq) { m_sequence = seq; }

    uint16_t get_recent_ack() { return m_recentAck; }
    void set_ack(uint16_t reAck) { m_recentAck = reAck; }
    YOJIMBO_VIRTUAL_SERIALIZE_FUNCTIONS();

   protected:
    uint16_t m_sequence;
    uint16_t m_recentAck;
};

constexpr uint16_t MAX_COLLISIONS = 20;

template <typename Stream>
bool serialize_vec3(Stream& stream, vec3& val) {
    serialize_float(stream, val.x);
    serialize_float(stream, val.y);
    serialize_float(stream, val.z);
    return true;
}

template <typename Stream>
bool serialize_quat(Stream& stream, quat& val) {
    serialize_float(stream, val.x);
    serialize_float(stream, val.y);
    serialize_float(stream, val.z);
    serialize_float(stream, val.w);
    return true;
}

class ServerMessage : public Message {
   public:
    ServerMessage() : m_sequence(0), m_recentAck(0) {
        playerPos.fill(vec3());
        playerHealth.fill(100);
    }

    template <typename Stream>
    bool Serialize(Stream& stream) {
        serialize_bits(stream, m_sequence, 16);
        serialize_bits(stream, m_recentAck, 16);
        for (vec3& pp : playerPos) {
            serialize_vec3(stream, pp);
        }
        for (quat& pr : playerRots) {
            serialize_quat(stream, pr);
        }
        for (int16_t& hp : playerHealth) {
            serialize_int(stream, hp, -100, 200);
        }
        for (vec3& pc : paintCollisions) {
            serialize_vec3(stream, pc);
        }
        return true;
    }

    std::array<vec3, MAX_CLIENTS> playerPos;
    std::array<quat, MAX_CLIENTS> playerRots;
    std::array<int16_t, MAX_CLIENTS> playerHealth;
    std::array<vec3, MAX_COLLISIONS> paintCollisions;

    uint16_t get_sequence() { return m_sequence; }
    void set_sequence(uint16_t seq) { m_sequence = seq; }

    uint16_t get_recent_ack() { return m_recentAck; }
    void set_ack(uint16_t reAck) { m_recentAck = reAck; }
    YOJIMBO_VIRTUAL_SERIALIZE_FUNCTIONS();

   protected:
    uint16_t m_sequence;
    uint16_t m_recentAck;
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
