# LightControlCLI
Controlling all the lights in my room from the command line.

## How to use
### Preqrequisites
 * A C-Compiler (for example GCC)
 * A POSIX compliant operating system (e.g. everything but windows)
 
### Compiling
The programm can be compiled with one simple command:
```
cc -Wall -o Lightstrip main.c RadioControlProtocolC/rc_lib.c
```

### Usage
Run the generated `Lightstrip` file with four arguments (each argument is the information sent in one channel),
if you want to use a different IP-Address (the default is `192.168.2.200`) add the address as the fifth argument:
```
./Lightstrip 0 1023 0 0 192.168.0.100
```
The first channel represents the command, see the firmware repo for more information. The following channels represent the 
color (in RGB order, each color represented by a 10bit value, which means the value is in [0,1024)).
