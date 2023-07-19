#include "../SFML-2.5.1/include/SFML/Network.hpp"

namespace pacetStates {
    enum pacetStates {
        disconnect,         // client send to host
        PlayersAmount,      // to connecting client
        PlayerConnect,      // host sent to clients when someone connected
        PlayerDisconnect,   // host sent to clients when someone disconnected
        Labyrinth,          // for LabirintData
        PlayerPos,          // send from client to host than host sending others clients
        SetPos,             // for set pos of player, because PlayerPos just say positions of others players
        ChatEvent,
        Shooting,
    };
};

////////////////////////////////////////////////////////////
// Class
////////////////////////////////////////////////////////////

class Client : public sf::TcpSocket, public sf::Vector2f {
public:
    int number;
};

////////////////////////////////////////////////////////////
// Realization
////////////////////////////////////////////////////////////

