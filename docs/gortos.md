HARDWARE ======================================================================

MSP430FR2153 -------------------------
POWER
    VCC (+3.3v)
    GND
PROG & DEBUG
    RST / SBWTDIO
    SBWTCK
GPIO
    P1.0 - LED_HEARTBEAT
    P1.1 - LED_TEST0
    P1.2 - 
    P1.3 - 
    P2.0 - LEDBAR_BIT0
    P2.1 - LEDBAR_BIT1
    P2.2 - LEDBAR_BIT2
    P2.3 - LEDBAR_BIT3
    P2.4 - LEDBAR_BIT4
    P2.5 - LEDBAR_BIT5
    P2.6 - LEDBAR_BIT6
    P2.7 - LEDBAR_BIT7
    P3.0 - 
    P3.1 - 
    P3.2 - 
    P3.3 - 
    P3.4 - 
    P3.5 - 
    P3.6 - 
    P3.7 - 
    P4.0 - LEDBAR_BIT8
    P4.1 - LEDBAR_BIT9
    P4.4 - LED_TEST2
    P4.5 - LED_TEST1
    P5.0 - SWITCH0
    P5.1 - SWITCH1
UART
    P4.2 - UCA1RXD
    P4.3 - UCA1TXD
I2C
    P4.6 - UCB1SDA
    P4.7 - UCB1SCL
SPI
    P1.4 - UCA0MISO
    P1.5 - UCA0MOSI
    P1.6 - UCA0CLK
    P1.7 - UCA0STE
ADC
    ?
---------------------------------------

Embedded System:
- Bypass capacitors
- LED bar (10)
- Real-Time Clock
    - 3.3 V battery
- 32KB RAM (?)
- LCD Display



===============================================================================

GORTOS KERNEL =================================================================
Drivers -------------------------------
uart.h - UART serial interface
    uart_init
    uart_tx
    uart_rx

i2c.h - I2C serial interface
    i2c_init
    i2c_write
    i2c_read

spi.h - SPI serial interface
    spi_init

led.h - LED configuration (w)
    led_init

switch.h - Switch configuration (r)
    switch_init
    
rtc.h - Real-Time Clock (rw)
    rtc_init
    rtc_start
    rtc_stop
    rtc_get
    rtc_get_str
    rtc_get_strtime
    rtc_get_strdate
    
    ~ Format: "07-24-2026 21:22:05" ~
    

mem.h - Cyprus 32KB RAM (rw)
    mem_init
    mem_write
    mem_read

lcd.h - LCD Display (rw)
    lcd_init

dial.h - Dial Type (r)
	dty_init
    
---------------------------------------

sys.h - System
    get_cur_time

gfs.h - Gort Filesystem
    make_file
    del_file

gio.h - Gort I/O
    GIN
    GOUT
    helloworld
    hellogort

init.h - System initialization --------
    init 
    
---------------------------------------

main.c - Gort Kernel!
    int main() {
        init();
        
        gsh();
        
        while(1) {}
        return 0;
	}

===============================================================================

System Variables ----------------------
gfs_table?

gtime
guptime
guid
guser
gwd
gpath
gin
gout

arg0?
arg1?
arg2?
arg3?
---------------------------------------

File Format ---------------------------
	Name (16 B)
	Mode Bits (1 B)
		00 - Normal
		01 - Directory
		10 - Stream
		11 - 
		000 - Read, Write, Execute
	
	Size in bytes (1 B)
	N: Data
	D: Pointers to other files
	S: (2 B) Pointer to Stream
---------------------------------------

Filesystem ----------------------------
/bin
	/bin/...
/dev
	/dev/uart
/home
	/home/gort
/sys
	/sys/log.txt
    /sys/...
/tmp

v2: ---------------
/boot
	/boot/gorb
	/boot/gorb.cfg
/etc
	/etc/hosts
	/etc/passwd
/proc
---------------------------------------



=== SOFTWARE ==================================================================
Advanced ------------------------------
gsh.c - Gort shell
	const char prompt = '$';
	char cmd_buf = [];
	char c;
	while c != 'EOF':
		print prompt to $stdout
		while c != '\n':
			read char into cmd_buf from $stdin
			print char to $stdout
		locate file path of $arg0:
			check $path
			check $dir
			print "command not found"
	
gim.c - Gort text editor
	open $1

v2:
gorb.c - Gort bootloader?
gcc.c - Gort C compiler?
ld.c - Linker?
make.c - Make?
gdb.c - Gort debugger?
gar.c - Gort archiver?
gapt.c - Gort advanced package tool?
git.c - Git?
gort.c - Gort?
---------------------------------------

cat
clear
cd
cp
echo
eep
gim
gort
gsh
ls
mkdir
mv
rm
time

v2:
bc.c - Print the size of a file path $1 to stdout
*cat.c - Print the contents of a file path $1 to stdout
*clear.c - Clear stdout
*cd.c - Change DIR to path $1
chmod.c - Change the mode bits of a file path $1
*cp.c - Copy a file path $1 to another specified path $2
curl.c - 
diff.c - Compare the contents of two file paths $1 and $2 byte by byte
du.c - Print disk usage to stout
*echo.c - Print $1 to stdout
*eep.c - Set a timer for $1 seconds and wait
env.c - Print the current environment variables to stdout
find.c - 
fsck.c - 
fdisk.c - 
grep.c - 
head.c - Print only the first N bytes of a file path $1
id.c - 
kill.c - 
ln.c - 
*ls.c - List the contents of a directory path $1 to stdout
lsmod.c - 
man.c - 
*mkdir.c - Make a directory with path $1
mkfs.c - 
mount.c - 
*mv.c - Copy and delete a file path $1 to another specified path $2
netcat.c - 
ping.c - 
ps.c - 
pwd.c - Print the current DIR to stdout
reboot.c - Reboot system
*rm.c - Delete a file or directory path $1
rmmod.c - 
shutdown.c - Shutdown system
sort.c - 
ss.c - 
ssh.c - 
sudo.c - 
sysctl.c - Configure system variables
syslog.c - Print system log to stdout
tail.c - Print only the last N bytes of a file path $1
*time.c - Print the system time to stdout or configure
touch.c - 
tr.c - 
umount.c - 
who.c - 
xxd.c - 
===============================================================================