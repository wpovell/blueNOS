use drivers::gpio::*;

const AUX_BASE: usize = (super::P_BASE + 0x0021_5000);

// Enable
const AUX_ENB: *mut u32 = (AUX_BASE + 0x04) as *mut u32;
// IO
const AUX_MU_IO_REG: *mut u32 = (AUX_BASE + 0x40) as *mut u32;
// Interrupt
const AUX_MU_IER_REG: *mut u32 = (AUX_BASE + 0x44) as *mut u32;
// FIFO
const AUX_MU_IIR_REG: *mut u32 = (AUX_BASE + 0x48) as *mut u32;
// Data Format
const AUX_MU_LCR_REG: *mut u32 = (AUX_BASE + 0x4C) as *mut u32;
// RTS
const AUX_MU_MCR_REG: *mut u32 = (AUX_BASE + 0x50) as *mut u32;
// Data Status
const AUX_MU_LSR_REG: *mut u32 = (AUX_BASE + 0x54) as *mut u32;
// Control trans/recv
const AUX_MU_CNTL_REG: *mut u32 = (AUX_BASE + 0x60) as *mut u32;
// Baudrate
const AUX_MU_BAUD_REG: *mut u32 = (AUX_BASE + 0x68) as *mut u32;

static mut SETUP: bool = false;
pub fn init() {
    set_func(14, PinFunc::F5);
    set_func(15, PinFunc::F5);

    set_pull(14, PinPull::None);
    set_pull(15, PinPull::None);

    unsafe {
        // Enable UART
        AUX_ENB.write_volatile(1);
        // Disable flow control + transmit + recieve
        AUX_MU_CNTL_REG.write_volatile(0);
        // Disable interrupts
        AUX_MU_IER_REG.write_volatile(0);
        // Set data to be 8bit
        AUX_MU_LCR_REG.write_volatile(0b11);
        // Set RTS to be high (not relevant, used for FC)
        AUX_MU_MCR_REG.write_volatile(0);

        /*
        baudrate = 115200
        sys_clock_freq = 250MHz
        baudrate = (sys_clock_freq) / (8 * baudrate_reg + 1)
        baudrate_reg = (sys_clock_freq) / (8 * baudrate) - 1
        baudrate_reg = 270
        */
        AUX_MU_BAUD_REG.write_volatile(270);

        // Enable transmit + recieve
        AUX_MU_CNTL_REG.write_volatile(0b11);

        SETUP = true;
    }
}

pub fn putc(c: char) {
    unsafe {
        assert!(SETUP);

        // Idle while transmitting
        while (AUX_MU_LSR_REG.read_volatile() & (1 << 6)) == 0 {}

        AUX_MU_IO_REG.write_volatile(c as u32);
    }
}

pub fn print(s: &str) {
    for c in s.chars() {
        putc(c);
    }
}

pub fn getc() -> char {
    unsafe {
        assert!(SETUP);
        // Idle while waiting for data
        while (AUX_MU_IO_REG.read_volatile() & (1 << 6)) == 0 {}

        (AUX_MU_IO_REG.read_volatile() as u8) as char
    }
}

pub fn getc_timeout(timeout: u64) -> Option<char> {
    let mut res = 0;
    let mut i = 0;

    unsafe {
        assert!(SETUP);
        while i < timeout {
            res = AUX_MU_LSR_REG.read_volatile() & 1;
            i += 1;
        }

        if res == 0 {
            None
        } else {
            Some((AUX_MU_IO_REG.read_volatile() as u8) as char)
        }
    }
}
