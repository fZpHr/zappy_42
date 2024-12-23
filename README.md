# zappy_42
Build a network-based multiplayer game.

## Progress

```mermaid
graph TD
    subgraph "Core Infrastructure"
        A[Project Setup ✓]
        B[Base Server ✓]
        C[Socket Management ✓]
        D[Network Manager ✓]
        E[Unit Tests ✓]
    end

    subgraph "Server Features"
        F[Event Loop]
        G[Command Parser]
        H[Game Logic]
        I[Map Management]
        J[Resource System]
        K[Player Management]
    end

    subgraph "Game Elements"
        L[Teams]
        M[Inventory]
        N[Resources]
        O[Incantations]
        P[Time Management]
    end

    subgraph "Client/AI"
        Q[AI Logic]
        R[Path Finding]
        S[Resource Collection]
        T[Team Coordination]
    end

    subgraph "GUI"
        U[Graphics Engine]
        V[Map Visualization]
        W[Player Display]
        X[Resource View]
        Y[Game State Monitor]
    end

    A --> B
    B --> C
    C --> D
    D --> E
    E --> F
    F --> G
    G --> H
    H --> I
    I --> J
    J --> K
    K --> L
    L --> M
    M --> N
    N --> O
    O --> P
    P --> Q
    Q --> R
    R --> S
    S --> T
    T --> U
    U --> V
    V --> W
    W --> X
    X --> Y

    classDef done fill:#90EE90,color:#000000
    classDef doing fill:#FFB6C1,color:#000000
    classDef todo fill:#D3D3D3,color:#000000
    
    class A,B,C,D,E done
    class F doing
    class G,H,I,J,K,L,M,N,O,P,Q,R,S,T,U,V,W,X,Y todo
```

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