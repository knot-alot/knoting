#pragma once
#include <knoting/types.h>
#include <yojimbo.h>

namespace knot {
using namespace yojimbo;

constexpr int SERVER_PORT = 13189;
constexpr int CLIENT_PORT = 12646;
constexpr int MAX_CLIENTS = 6;
const char* const SERVER_ADDRESS = "127.0.0.1";

inline int GetNumBitsForMessage(uint16_t sequence) {
    static int messageBitsArray[] = {1, 320, 120, 4, 256, 45, 11, 13, 101, 100, 84, 95, 203, 2, 3, 8, 512, 5, 3, 7, 50};
    const int modulus = sizeof(messageBitsArray) / sizeof(int);
    const int index = sequence % modulus;
    return messageBitsArray[index];
}

class ClientMessage : public Message {
   public:
    ClientMessage() : sequence(0) {}

    template <typename Stream>
    bool Serialize(Stream& stream) {
        serialize_bits(stream, sequence, 16);
        int numBits = GetNumBitsForMessage(sequence);
        int numWords = numBits / 32;
        uint32_t dummy = 0;
        for (int i = 0; i < numWords; ++i)
            serialize_bits(stream, dummy, 32);
        int numRemainderBits = numBits - numWords * 32;
        if (numRemainderBits > 0)
            serialize_bits(stream, dummy, numRemainderBits);
        return true;
    }

    YOJIMBO_VIRTUAL_SERIALIZE_FUNCTIONS();

   protected:
    uint16_t sequence;
};

class ServerMessage : public Message {
   public:
    ServerMessage() : sequence(0) {}

    template <typename Stream>
    bool Serialize(Stream& stream) {
        serialize_bits(stream, sequence, 16);
        int numBits = GetNumBitsForMessage(sequence);
        int numWords = numBits / 32;
        uint32_t dummy = 0;
        for (int i = 0; i < numWords; ++i)
            serialize_bits(stream, dummy, 32);
        int numRemainderBits = numBits - numWords * 32;
        if (numRemainderBits > 0)
            serialize_bits(stream, dummy, numRemainderBits);
        return true;
    }

    YOJIMBO_VIRTUAL_SERIALIZE_FUNCTIONS();

   protected:
    uint16_t sequence;
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
