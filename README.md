# fetched
This project just got started and it's still in infancy. The pretty fonts and the colorful images are a work in progress.

fetchme will hopefully become a simple program written in C to get information about your system on Linux and display it in manner worthy of r/unixporn. 

## Usage
The command to run the program:

`fetched`

After you clone the repository, move into the fetchme folder and run the command: 

`sudo make install`

This will create two binaries in the /usr/bin directory, namely the fetchme and asciify binaries and a folder for fetchme in /usr/share.

You can now run the command from anywhere and if you want to uninstall the program, move once again into the repository folder and run the command:

`sudo make uninstall`

If you have made changes to the source code and would like to compile it, but you don't want it to be a global command, run:

`make`

You'll need the package `stb` which may be named differently on different distributions. Look for something along the lines of `stb` or `libstb` or similar.

## Roadmap
- More extensive information about the system.
    - [x] Disk usage 
    - [x] Desktop Environment
    - [x] Ram usage
    - [x] Shell
    - [x] Number of packages
    - Overview of the basic commands for the package manager
    - [x] GPU
    - [x] CPU
    - Locale
    - Theme
    - Icon Theme
    - [x] Terminal
    - [x] Session Type
    - [x]Bluetooth Devices
    - [x] Uptime
- Formatting the output.
- Optimizing for speed.
- Customization options.
- Better image retrieval logic.
- Output that changes according to the size of the terminal window.
