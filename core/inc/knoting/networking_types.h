#pragma once
#include <knoting/types.h>
#include <yojimbo.h>
#include <cstdlib>
namespace knot {
using namespace yojimbo;

constexpr int SERVER_PORT = 13189;
constexpr int CLIENT_PORT = 12646;
constexpr int MAX_CLIENTS = 6;
const double TICK = 1.0 / 64.0;

constexpr int clientTimeout = 5;
constexpr int maxPacketSize = 1500;

struct GameConfig : public ClientServerConfig {
    GameConfig() {
        ClientServerConfig();
        this->numChannels = 2;
        this->channel[0].type = yojimbo::CHANNEL_TYPE_RELIABLE_ORDERED;
        this->channel[1].type = yojimbo::CHANNEL_TYPE_UNRELIABLE_UNORDERED;
        this->timeout = clientTimeout;
        this->networkSimulator = false;
        this->maxPacketSize = maxPacketSize;
    }
};

template <typename Stream>
bool serialize_vec2i(Stream& stream, vec2i& val) {
    auto valX = (int16_t)val.x;
    auto valY = (int16_t)val.y;
    serialize_int(stream, valX, -1, 1);
    serialize_int(stream, valY, -1, 1);
    val.x = valX;
    val.y = valY;
    return true;
}

class ClientMessage : public Message {
   public:
    ClientMessage()
        : m_sequence(0),
          m_recentAck(0),
          m_lookAxis(vec2i()),
          m_moveAxis(vec2i()),
          jumpPressed(false),
          isShooting(false) {}

    template <typename Stream>
    bool Serialize(Stream& stream) {
        serialize_bits(stream, m_sequence, 16);
        serialize_bits(stream, m_recentAck, 16);

        serialize_vec2i(stream, m_lookAxis);
        serialize_vec2i(stream, m_moveAxis);
        serialize_bool(stream, jumpPressed);
        serialize_bool(stream, isShooting);
        return true;
    }

    vec2i m_lookAxis;
    vec2i m_moveAxis;

    bool jumpPressed = false;
    bool isShooting = false;

    uint16_t m_sequence;
    uint16_t m_recentAck;

    YOJIMBO_VIRTUAL_SERIALIZE_FUNCTIONS();
};

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

constexpr uint16_t MAX_COLLISIONS = 20;

class ServerMessage : public Message {
   public:
    ServerMessage() : m_sequence(0), m_recentAck(0) {
        playerPos.fill(vec3(0));
        playerRots.fill(quat(1, 0, 0, 0));
        playerHealth.fill(100);
        paintCollisions.fill(vec3(0, 0, 0));
    }

    template <typename Stream>
    bool Serialize(Stream& stream) {
        serialize_bits(stream, m_sequence, 16);
        serialize_bits(stream, m_recentAck, 16);
        serialize_bits(stream, m_clientNum, 16);
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

    uint16_t m_clientNum = 10;
    std::array<vec3, MAX_CLIENTS> playerPos;
    std::array<quat, MAX_CLIENTS> playerRots;
    std::array<int16_t, MAX_CLIENTS> playerHealth;
    std::array<vec3, MAX_COLLISIONS> paintCollisions;

    uint16_t m_sequence;
    uint16_t m_recentAck;

    YOJIMBO_VIRTUAL_SERIALIZE_FUNCTIONS();
};

enum class NetworkMessageTypes { ClientMessage, ServerMessage, LAST };

YOJIMBO_MESSAGE_FACTORY_START(GameMessageFactory, static_cast<size_t>(NetworkMessageTypes::LAST));
YOJIMBO_DECLARE_MESSAGE_TYPE(static_cast<size_t>(NetworkMessageTypes::ClientMessage), ClientMessage);
YOJIMBO_DECLARE_MESSAGE_TYPE(static_cast<size_t>(NetworkMessageTypes::ServerMessage), ServerMessage);
YOJIMBO_MESSAGE_FACTORY_FINISH();

class GameAdapter : public Adapter {
   public:
    explicit GameAdapter(Server* server = nullptr) : m_server(server) {}
    MessageFactory* CreateMessageFactory(Allocator& allocator) {
        return YOJIMBO_NEW(allocator, GameMessageFactory, allocator);
    }

   private:
    std::shared_ptr<Server> m_server;
};
static GameAdapter gameAdapter;
}  // namespace knot
