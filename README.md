# zappy_42
Build a network-based multiplayer game.

## Project Structure
```
zappy_42/
├── CMakeLists.txt
├── include/
│   ├── core/
│   │   ├── Server.hpp
│   │   └── Client.hpp
│   ├── network/
│   │   ├── NetworkManager.hpp
│   │   └── SocketHandler.hpp
│   ├── utils/
│   │   ├── Logger.hpp
│   │   └── Exception.hpp
│   └── Lib.hpp
├── src/
│   ├── core/
│   │   ├── Server.cpp
│   │   └── Client.cpp
│   ├── network/
│   │   ├── NetworkManager.cpp
│   │   └── SocketHandler.cpp
│   ├── utils/
│   │   ├── Logger.cpp
│   │   └── Exception.cpp
│   └── main.cpp
└── tests/
    ├── core/
    │   ├── ServerTest.cpp
    │   └── ClientTest.cpp
    └── network/
        └── NetworkTest.cpp
```

## Project Architecture
```mermaid
graph TD
    subgraph "Implementation Steps"
        E1["1. Core Components"] --> E2["2. Map & Resources"]
        E2 --> E3["3. Network Layer"]
        E3 --> E4["4. Game Logic"]
        E4 --> E5["5. Command System"]
        E5 --> E6["6. Tests & Polish"]
    end

    subgraph "Core Components"
        Server[["Server Class<br>- Main loop<br>- Component coordination"]]
        GameEngine[["GameEngine<br>- Game state<br>- Rules<br>- Updates"]]
        Server --> GameEngine
    end

    subgraph "Map System"
        Map[["Map Class<br>- Grid management<br>- Resource distribution"]]
        Tile[["Tile Class<br>- Resources<br>- Players present"]]
        Resource[["Resource System<br>- Types<br>- Quantities"]]
        Map --> Tile
        Tile --> Resource
    end

    subgraph "Network Layer"
        NetworkManager[["NetworkManager<br>- Connection handling<br>- Message distribution"]]
        ClientConnection[["ClientConnection<br>- Socket management<br>- Message queues"]]
        Protocol[["Protocol Handler<br>- Message formatting<br>- Command parsing"]]
        NetworkManager --> ClientConnection
        NetworkManager --> Protocol
    end

    subgraph "Game Logic"
        Player[["Player Class<br>- Position<br>- Inventory<br>- Level"]]
        Team[["Team Class<br>- Player groups<br>- Team limits"]]
        Actions[["Action System<br>- Movement<br>- Collection<br>- Elevation"]]
        Player --> Team
        Player --> Actions
    end

    subgraph "Command System"
        CommandParser[["Command Parser<br>- Command validation<br>- Execution"]]
        Commands[["Commands<br>- Movement<br>- Interaction<br>- Information"]]
        CommandParser --> Commands
    end

    subgraph "Tests & Tools"
        UnitTests[["Unit Tests<br>- Component testing<br>- Integration tests"]]
        Logger[["Logger System<br>- Debug info<br>- Error tracking"]]
        Config[["Configuration<br>- Game parameters<br>- Network settings"]]
    end

    %% Connections between components
    Server --> NetworkManager
    GameEngine --> Map
    GameEngine --> Player
    NetworkManager --> CommandParser
    Player --> Map

    %% Priority levels
    style E1 fill:#f9f,stroke:#333,stroke-width:4px
    style E2 fill:#ff9,stroke:#333,stroke-width:3px
    style E3 fill:#9f9,stroke:#333,stroke-width:3px
    style E4 fill:#99f,stroke:#333,stroke-width:2px
    style E5 fill:#f99,stroke:#333,stroke-width:2px
    style E6 fill:#9ff,stroke:#333,stroke-width:1px

    %% Critical path highlighting
    style Server stroke:#f00,stroke-width:3px
    style GameEngine stroke:#f00,stroke-width:3px
    style Map stroke:#f00,stroke-width:3px
    style NetworkManager stroke:#f00,stroke-width:3px
```

## Building the Project
```bash
mkdir build && cd build
cmake ..
make
```

## Running Tests
```bash
./tests/zappy_tests
```

## Running the Server
```bash
./zappy_server <port>
```