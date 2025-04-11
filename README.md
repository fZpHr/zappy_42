# zappy_42
Build a network-based multiplayer game.

## Progress

```mermaid
graph TD
    subgraph Mandatory[Mandatory Part]
        subgraph Constraints[Key Constraints]
            C1[Single Process]
            C2[No Threads]
            C3[10 Max Commands]
            C4[TCP Protocol]
            C5[Time Unit - t]
        end

        subgraph Server[Server Core]
            S1[Arguments Parser ✓]
            S2[Socket Handler ✓]
            S3[Event Loop ✓]
            S4[Command Buffer ✓]
            S5[Time Management]
        end

        subgraph Game[Game Logic]
            G1[World Generation ✓]
            G2[Team Management ✓]
            G3[Resource System ✓]
            G4[Life System]
            G5[Egg System]
        end

        subgraph Commands[Commands t/unit]
            CM1[Movement - 7/t]
            CM2[Vision - 7/t]
            CM3[Objects - 7/t]
            CM4[Actions - 7/t]
            CM5[Special]
        end

        subgraph Protocol[Communication]
            P1[Welcome ✓]
            P2[Commands ✓]
            P3[Responses]
            P4[Messages]
        end

        subgraph Clients[Clients]
            AI1[Team Connection ✓]
            AI2[Commands ✓]
            AI3[Behavior]
            GUI1[2D Map]
            GUI2[Progress]
        end
    end

    subgraph Bonus[Bonus Part]
        B1[3D Client]
        B2[Admin Shell]
        B3[SSL Security]
        B4[Map Resize]
        B5[Sound System]
    end

    Constraints --> Server
    Server --> Game
    Game --> Commands
    Commands --> Protocol
    Protocol --> Clients
    Clients --> Bonus

    classDef done fill:#90EE90,color:#000000
    classDef doing fill:#FFB6C1,color:#000000
    classDef todo fill:#D3D3D3,color:#000000
    
    class S1,S2,S3 done
    class S4 doing
    class S5,G1,G2,G3,G4,G5,CM1,CM2,CM3,CM4,CM5,P1,P2,P3,P4,AI1,AI2,AI3,GUI1,GUI2,B1,B2,B3,B4,B5 todo
```

## Project Structure
```
zappy_42/
├── Makefile
├── AI
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
│   │   └── Logger.cpp
│   └── main.cpp
└── tests/
    ├── core_tests/
    │   ├── ServerTest.cpp
    │   └── ClientTest.cpp
    ├── network_tests/
    │  ├── NetworkManagerTest.cpp
    |  └── SocketHandlerTest.cpp
    ├── utils_tests/
    │   ├── LoggerTest.cpp
    |   └── ExceptionTest.cpp
    └── mainTest.cpp
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
make
```

## Running Tests
```bash
make tests
```

## Running the Server
```bash
./server <port>
```