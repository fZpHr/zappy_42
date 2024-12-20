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
    %% Main Lib
    Lib[/"Lib.hpp<br>Central Include System"/]
    
    %% Core Layer
    subgraph CORE ["Core System"]
        Server["Server<br>● Main game loop<br>● Teams management<br>● Client connections"]
        GameEngine["GameEngine<br>● Game rules<br>● Win conditions<br>● Event handling"]
        TimeManager["TimeManager<br>● Time unit management<br>● Action scheduling<br>● Game clock"]
    end

    %% Game Layer
    subgraph GAME ["Game Logic"]
        Map["Map<br>● Grid management<br>● Resource spawning<br>● Tile information"]
        Player["Player<br>● Position & Level<br>● Inventory<br>● Actions & Stats"]
        Team["Team<br>● Team slots<br>● Team objectives<br>● Victory conditions"]
        Resources["Resources<br>● Resource types<br>● Spawn algorithms<br>● Distribution"]
        Inventory["Inventory<br>● Resource storage<br>● Item management<br>● Limits"]
    end

    %% Network Layer
    subgraph NET ["Network System"]
        Network["NetworkManager<br>● Client connections<br>● Data distribution<br>● Event handling"]
        Socket["SocketHandler<br>● Raw data transfer<br>● Connection state<br>● Buffer management"]
        Protocol["Protocol<br>● Message format<br>● Command syntax<br>● Response format"]
    end

    %% Command Layer
    subgraph CMD ["Command System"]
        CmdHandler["CommandHandler<br>● Command parsing<br>● Validation<br>● Execution"]
        Actions["Actions<br>● Movement<br>● Resource handling<br>● Elevation"]
    end

    %% Main Dependencies
    Lib --> Server
    Lib --> Network
    Lib --> Map
    
    %% Server Relations
    Server --> GameEngine
    Server --> Network
    Server --> TimeManager
    
    %% Game Relations
    GameEngine --> Map
    GameEngine --> Player
    Map --> Resources
    Player --> Team
    Player --> Inventory
    
    %% Network Relations
    Network --> Socket
    Network --> Protocol
    Network --> CmdHandler
    
    %% Command Relations
    CmdHandler --> Actions
    Actions --> Player
    Actions --> Map

    %% Styling
    classDef lib fill:#1E88E5,stroke:#000,color:#fff,stroke-width:3px
    classDef core fill:#42A5F5,stroke:#000,color:#000
    classDef game fill:#66BB6A,stroke:#000,color:#000
    classDef net fill:#FFA726,stroke:#000,color:#000
    classDef cmd fill:#EF5350,stroke:#000,color:#000
    
    class Lib lib
    class Server,GameEngine,TimeManager core
    class Map,Player,Team,Resources,Inventory game
    class Network,Socket,Protocol net
    class CmdHandler,Actions cmd
    
    %% Link styling
    linkStyle default stroke:#000,stroke-width:2px
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