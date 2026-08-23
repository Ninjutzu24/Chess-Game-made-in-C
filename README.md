🔴 ### Project: Chess Game

♟️ **About the Project**

        This project is a graphical chess game developed in the C programming language.

        The purpose of the project is to implement a functional chess game with
        graphical rendering, user interaction and game-specific logic.

        The game uses SDL2 for graphics and window management, while SDL2_image
        is used for handling image assets.

        The project is structured into multiple modules in order to separate
        the game logic, graphics, chessboard management, piece handling and
        user interaction.

🎮 **Implemented Features**

        - Graphical chessboard rendering

        - Graphical chess piece rendering

        - Mouse interaction

        - Chessboard management

        - Piece management

        - Move handling

        - Pawn promotion

        - Special game functionality

        - Fog / visual effects

        - AI-related functionality

        - Modular project architecture

💻 **Programming Language and Technologies Used**

        - **C** – main programming language

        - **SDL2** – graphics, window management and user interaction

        - **SDL2_image** – loading and displaying image assets

        - **GCC** – project compilation

        - **Make / Makefile** – build automation

        - **WSL2** – Linux environment used on Windows

        - **Ubuntu** – Linux distribution used for development and execution

🛠 **Requirements to Run the Project**

        - Windows 10/11 with WSL2 enabled

        - Ubuntu distribution installed in WSL

        - SDL2 and SDL2_image installed

        - make and gcc installed

🔧 **Installation**

        **1. Enable WSL2**

        Open PowerShell as Administrator and run the following commands:

              dism.exe /online /enable-feature /featurename:Microsoft-Windows-Subsystem-Linux /all /norestart

              dism.exe /online /enable-feature /featurename:VirtualMachinePlatform /all /norestart


        **2. Restart your computer**


        **3. Install Ubuntu**

              wsl --install -d Ubuntu


        **4. Install the required dependencies**

              sudo apt update

              sudo apt install make gcc libsdl2-dev libsdl2-image-dev

▶️ **How to Run the Project**

        Open the terminal and navigate to the project directory:

              cd sah

        Compile the project:

              make

        Run the game:

              ./Joc-Sah
