#include "shell.h"
#include "vga.h"
#include "../drivers/keyboard.h"
#include "../drivers/timer.h"
#include "../libc/string.h"

#define MAX_INPUT 256

static char input_buffer[MAX_INPUT];
static int input_pos = 0;

void shell_init(void) {
    input_pos = 0;
    memset(input_buffer, 0, MAX_INPUT);
}

static void print_prompt(void) {
    vga_setcolor(VGA_COLOR_LIGHT_GREEN | (VGA_COLOR_BLACK << 4));
    vga_puts("kimi@os");
    vga_setcolor(VGA_COLOR_WHITE | (VGA_COLOR_BLACK << 4));
    vga_puts(":");
    vga_setcolor(VGA_COLOR_LIGHT_BLUE | (VGA_COLOR_BLACK << 4));
    vga_puts("~");
    vga_setcolor(VGA_COLOR_WHITE | (VGA_COLOR_BLACK << 4));
    vga_puts("$ ");
}

static void cmd_help(void) {
    vga_puts("Available commands:\n");
    vga_puts("  help     - Show this help message\n");
    vga_puts("  clear    - Clear the screen\n");
    vga_puts("  time     - Show system uptime\n");
    vga_puts("  reboot   - Reboot the system\n");
    vga_puts("  echo     - Echo text back\n");
    vga_puts("  color    - Change text color (0-15)\n");
    vga_puts("  about    - About Kimi-OS\n");
    vga_puts("  mem      - Show memory info\n");
}

static void cmd_clear(void) {
    vga_clear();
}

static void cmd_time(void) {
    uint32_t ticks = timer_get_ticks();
    uint32_t seconds = ticks / TIMER_FREQ;
    uint32_t minutes = seconds / 60;
    seconds %= 60;
    
    vga_printf("Uptime: %d minutes, %d seconds (ticks: %d)\n", minutes, seconds, ticks);
}

static void cmd_reboot(void) {
    vga_puts("Rebooting...\n");
    sleep(500);
    
    // Triple fault to reboot
    uint8_t good = 0x02;
    while (good & 0x02)
        good = inb(0x64);
    outb(0x64, 0xFE);
    
    __asm__ volatile("hlt");
}

static void cmd_echo(const char* text) {
    vga_puts(text);
    vga_putchar('\n');
}

static void cmd_color(const char* arg) {
    if (!arg || !*arg) {
        vga_puts("Usage: color <0-15>\n");
        vga_puts("Colors: 0=Black 1=Blue 2=Green 3=Cyan 4=Red 5=Magenta 6=Brown 7=Grey\n");
        vga_puts("        8=DGrey 9=LBue 10=LGreen 11=LCyan 12=LRed 13=LMagenta 14=Yellow 15=White\n");
        return;
    }
    
    int color = arg[0] - '0';
    if (color >= 0 && color <= 15) {
        vga_setcolor(color | (VGA_COLOR_BLACK << 4));
        vga_puts("Color changed!\n");
    } else {
        vga_puts("Invalid color. Use 0-15.\n");
    }
}

static void cmd_about(void) {
    vga_setcolor(VGA_COLOR_LIGHT_CYAN | (VGA_COLOR_BLACK << 4));
    vga_puts("\n");
    vga_puts("    _    ___ __  __ _ _     ___   ____   _____ \n");
    vga_puts("   | |  |_ _|  \\/  | | |   / _ \\ / ___| |___ / \n");
    vga_puts("   | |   | || |\\/| | | |  | | | | |      |_ \\ \n");
    vga_puts("   | |___| || |  | | | |__| |_| | |___  ___) |\n");
    vga_puts("   |_____|___|_|  |_|_|_____\\___/ \\____||____/ \n");
    vga_puts("\n");
    vga_setcolor(VGA_COLOR_WHITE | (VGA_COLOR_BLACK << 4));
    vga_puts("Version: 0.2.0\n");
    vga_puts("Built with: GCC Cross-Compiler (i686-elf)\n");
    vga_puts("Features: Keyboard, Timer, Shell, VGA Text Mode\n");
    vga_puts("Created: 2024\n\n");
}

static void cmd_mem(void) {
    extern char end;
    vga_printf("Kernel end address: 0x%x\n", (uint32_t)&end);
    vga_printf("Approximate memory used: %d KB\n", ((uint32_t)&end - 0x100000) / 1024);
}

void shell_execute(const char* command) {
    // Skip leading spaces
    while (*command == ' ') command++;
    
    if (strlen(command) == 0) return;
    
    // Parse command and arguments
    char cmd[32];
    char args[MAX_INPUT];
    int i = 0;
    
    while (command[i] && command[i] != ' ' && i < 31) {
        cmd[i] = command[i];
        i++;
    }
    cmd[i] = '\0';
    
    // Get arguments
    while (command[i] == ' ') i++;
    strcpy(args, &command[i]);
    
    if (strcmp(cmd, "help") == 0) cmd_help();
    else if (strcmp(cmd, "clear") == 0) cmd_clear();
    else if (strcmp(cmd, "time") == 0) cmd_time();
    else if (strcmp(cmd, "reboot") == 0) cmd_reboot();
    else if (strcmp(cmd, "echo") == 0) cmd_echo(args);
    else if (strcmp(cmd, "color") == 0) cmd_color(args);
    else if (strcmp(cmd, "about") == 0) cmd_about();
    else if (strcmp(cmd, "mem") == 0) cmd_mem();
    else {
        vga_puts("Unknown command: ");
        vga_puts(cmd);
        vga_puts("\nType 'help' for available commands.\n");
    }
}

void shell_run(void) {
    shell_init();
    
    vga_setcolor(VGA_COLOR_LIGHT_CYAN | (VGA_COLOR_BLACK << 4));
    vga_puts("Welcome to Kimi-OS Shell v0.2\n");
    vga_puts("Type 'help' for commands, 'about' for system info.\n\n");
    
    while (1) {
        vga_setcolor(VGA_COLOR_WHITE | (VGA_COLOR_BLACK << 4));
        print_prompt();
        
        input_pos = 0;
        memset(input_buffer, 0, MAX_INPUT);
        
        while (1) {
            char c = keyboard_get_char();
            
            if (c == '\n') {
                vga_putchar('\n');
                input_buffer[input_pos] = '\0';
                shell_execute(input_buffer);
                break;
            } else if (c == '\b') {
                if (input_pos > 0) {
                    input_pos--;
                    input_buffer[input_pos] = '\0';
                    vga_putchar('\b');
                    vga_putchar(' ');
                    vga_putchar('\b');
                }
            } else if (input_pos < MAX_INPUT - 1 && c >= 32 && c < 127) {
                input_buffer[input_pos++] = c;
                vga_putchar(c);
            }
        }
    }
}
