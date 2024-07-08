# fetchme
This project just got started and it's still in infancy. The pretty fonts and the colorful images are a work in progress.

fetchme will hopefully become a simple program written in C to get information about your system on Linux and display it in manner worthy of r/unixporn. 

## Usage
You can either just run the command './fetchme' from the clone directory or you can run 'sudo make install' to have it as a system wide command.
If you have modified the code and would like to just compile without putting it into /usr/bin (making it a global command), just run 'make'

NOTE: The system wide version is as of yet not functional, it won't cause any big problems but the image of your distribution won't be displayed either.

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
    - Bluetooth Devices
    - [x] Uptime
- Formatting the output.
- Optimizing for speed.
- Customization options.
- Better image retrieval logic.
- Output that changes according to the size of the terminal window.
