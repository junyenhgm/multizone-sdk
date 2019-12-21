# multizone-sdk
MultiZone® Security Trusted Execution Environment for Andes

Hex Five's open standard technology provides hardware-enforced software-defined separation of multiple application domains, with full control over data, programs, and peripherals. Contrary to traditional solutions, MultiZone® Security is policy-driven and requires no hypervisor software or hardware support for virtualization: open source libraries, third party binaries, and legacy code can be configured in minutes to achieve unprecedented levels of safety and security. 

MultiZone® Security SDK supports the following Andes IP / boards:
 - Andes N22/AE250 - RV32ACIMU Board: Andes Corvette F1 FPGA
 - Andes N25/AE250 - RV32ACIMU Board: GoWin GW2A-55K FPGA

For Questions or feedback - send email to info 'at' hex-five.com

### Installation ###

Upload the Andes bitstream to the target board following directions from Andes / GoWin

Install the certified RISC-V toolchain for Linux - directions specific to a fresh Ubuntu 18.04 LTS, other Linux distros generally a subset
 ```
 sudo apt update
 sudo apt upgrade -y
 sudo apt install git make default-jre libftdi1-dev
 sudo ln -s /usr/lib/x86_64-linux-gnu/libmpfr.so.6 /usr/lib/x86_64-linux-gnu/libmpfr.so.4
 wget https://github.com/hex-five/multizone-sdk/releases/download/v0.1.0/riscv-gnu-toolchain-20181226.tar.xz
 tar -xvf riscv-gnu-toolchain-20181226.tar.xz
 wget https://github.com/hex-five/multizone-sdk/releases/download/v0.1.0/riscv-openocd-20181226.tar.xz
 tar -xvf riscv-openocd-20181226.tar.xz
 git clone https://github.com/hex-five/multizone-sdk
 sudo apt-get install libusb-0.1-4
 sudo apt-get install screen
```

If you have not already done so, you need to edit or create a file to place the USB devices until plugdev group so you can access them without root privileges:
```
sudo vi /etc/udev/rules.d/99-openocd.rules
```
Then place the following text in that file if it is not already there
```
# ANDES TECHNOLOGY CORPORATION - Corvette F1 [CON1]
SUBSYSTEM=="tty", ATTRS{idVendor}=="1cfc",ATTRS{idProduct}=="0000", MODE="664", GROUP="plugdev"
SUBSYSTEM=="usb", ATTR{idVendor} =="1cfc",ATTR{idProduct} =="0000", MODE="664", GROUP="plugdev"
```
Detach and re-attach the USB devices for these changes to take effect.

Add environment variables and a path to allow the Makefiles to find the toolchain

edit ~/.bashrc and ~/.profile and place the following text at the bottom of both files.
```
export RISCV=/home/<username>/riscv-gnu-toolchain-20181226
export OPENOCD=/home/<username>/riscv-openocd-20181226
export PATH="$PATH:/home/<username>/riscv-gnu-toolchain-20181226/bin"
```
Close and restart the terminal session for these changes to take effect.

### Bulid the Project for the target board ###

```
cd multizone-sdk/
make clean
make BOARD=N22
```

This will result in a multizone.bin file that is now ready for upload to the target.
Follow instructions provided by Andes / GoWin for the use of their propriertary flash burner utilities

### Operate the Demo ###

The system contains three zones:
 - Zone 1: UART Console (115200 baud, 8N1) with commands that enable the following:
   - load, store exec - issue discrete load / store / exec commands to test the boundaries of physical memory protection in Zone 1
     - invalid commands generate hardware exceptions that send a response to the user via handlers that are registered in main.c
   - send / recv messages to / from other zones
   - timer - set a soft timer in ms 
   - yield - measure the round trip time through three zones when you yield context
   - stats - complete a number of yield commands and calculate statistics on performance
   - restart - restart the console
 - Zone 2: LEDs, Buttons + Interrupts
   - Buttons are mapped to interrupts in this zone, pressing the buttons will change the pattern of the blinking led and send a message to zone 1
 - Zone 3: OWI Robot Control
   - This zone controls a robot via GPIO; if you do not have the robot then this zone simply yields for you
   - Robot commands are all issued via messages from zone 1:
     - send 3 > - unfold
     - send 3 1 - begin recursive dance
     - send 3 0 - stop recursive dance when it reaches home
     - send 3 < - fold
   - As temperature, humidity and battery life affect the robot motor speed, small adjustments can be made as follows
     - send 3 q (open claw)
     - send 3 a (close claw)
     - send 3 w (lift wrist)
     - send 3 s (drop wrist)
     - send 3 e (lift elbow)
     - send 3 d (drop elbow)
     - send 3 r (lift shoulder)
     - send 3 f (drop shoulder)
     - send 3 t (rotate base counterclockwise)
     - send 3 g (rotate base clockwise)
  
### For More Information ###

See the MultiZone Manual or visit [http://www.hex-five.com](https://www.hex-five.com)
