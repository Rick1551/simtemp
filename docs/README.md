# NXP Simulated Temperature Sensor Challenge

This repository contains the implementation of the `nxp_simtemp` kernel module and user-space tools for the NXP Systems Software Engineer Candidate Challenge.

📄 Full challenge description: [docs/README.md](docs/README.md)  
📐 Design notes: [docs/DESIGN.md](docs/DESIGN.md)  
🧪 Test plan: [docs/TESTPLAN.md](docs/TESTPLAN.md)

## Structure

See [Recommended Repository Layout](docs/README.md#4-recommended-repository-layout)

## Development Environment

- OS: Ubuntu 22.04
- Kernel: 6.4.10-33.24.04.1
- Headers: Installed via `sudo apt install linux-headers-$(uname -r)`
- Editor: Visual Studio Code with C/C++ extension (Microsoft)
- IntelliSense configured via `.vscode/c_cpp_properties.json`

## Project Structure 
```
simtemp/
├─ kernel/
│  ├─ Kbuild
│  ├─ Makefile
│  ├─ nxp_simtemp.c
│  ├─ nxp_simtemp.h
│  ├─ nxp_simtemp_ioctl.h
│  └─ dts/
│     └─ nxp-simtemp.dtsi
├─ user/
│  ├─ cli/
│  │  ├─ main.cpp           
│  │  └─ requirements.txt  
│  └─ gui/                  
│     └─ qt_app.cpp   
├─ scripts/
│  ├─ build.sh
│  ├─ run_demo.sh
│  └─ lint.sh               
├─ docs/
│  ├─ README.md
│  ├─ DESIGN.md           
│  ├─ TESTPLAN.md
│  └─ AI_NOTES.md           
└─ .gitignore
```