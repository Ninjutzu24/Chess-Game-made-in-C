🔴###Proiect: Joc de Sah

🛠 **Cerințe pentru rulare**
        -Windows 10/11 cu WSL2 activat
        -Distribuție Ubuntu instalată în WSL
        -SDL2 și SDL2_image instalate
        -make și gcc instalate

🔧**Pasii ce trebuie urmati pentru a instala WSL2 (Windows Subsystem for Linux)**
    1. *Deschide PowerShell ca administrator și rulează următoarele comenzi:*
          dism.exe /online /enable-feature /featurename:Microsoft-Windows-Subsystem-Linux /all /norestart
          dism.exe /online /enable-feature /featurename:VirtualMachinePlatform /all /norestart
          
    2.*Repornește calculatorul*
    3.*Instalează Ubuntu cu comanda:*
        wsl --install -d Ubuntu

▶️ **Cum rulăm proiectul**
        *Pentru a rula proiectul trebuie sa tastam in terminal comenzile:*
                 -wsl
                 -cd sah
                 -make
                 -./Joc-Sah

ℹ️ **Informații suplimentare**
        *Daca va lipseste make-ul si nu il aveti instalat, folositi comenzile de mai jos, pentru a-l instala:*
                sudo apt install make
        *Dacă lipsesc SDL2 sau SDL2_image:*
                sudo apt install libsdl2-dev libsdl2-image-dev

 


