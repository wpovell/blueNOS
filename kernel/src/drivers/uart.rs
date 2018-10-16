//! Functions to setup and perform I/O with the UART
//!
//! Magic here is described in Section 2

use core::fmt;
use drivers::gpio::*;
use spin::Mutex;

const AUX_BASE: usize = (super::P_BASE + 0x0021_5000);

/// Enable
const AUX_ENB: *mut u32 = (AUX_BASE + 0x04) as *mut u32;
/// IO
const AUX_MU_IO_REG: *mut u32 = (AUX_BASE + 0x40) as *mut u32;
/// Interrupt
const AUX_MU_IER_REG: *mut u32 = (AUX_BASE + 0x44) as *mut u32;
/// FIFO
const AUX_MU_IIR_REG: *mut u32 = (AUX_BASE + 0x48) as *mut u32;
/// Data Format
const AUX_MU_LCR_REG: *mut u32 = (AUX_BASE + 0x4C) as *mut u32;
/// RTS
const AUX_MU_MCR_REG: *mut u32 = (AUX_BASE + 0x50) as *mut u32;
/// Data Status
const AUX_MU_LSR_REG: *mut u32 = (AUX_BASE + 0x54) as *mut u32;
/// Control trans/recv
const AUX_MU_CNTL_REG: *mut u32 = (AUX_BASE + 0x60) as *mut u32;
/// Baudrate
const AUX_MU_BAUD_REG: *mut u32 = (AUX_BASE + 0x68) as *mut u32;

/// print macro, for UART
macro_rules! print {
    ($($arg:tt)*) => ($crate::drivers::uart::print(format_args!($($arg)*)));
}

/// println macro, for UART
macro_rules! println {
    () => (print!("\n"));
    ($fmt:expr) => (print!(concat!($fmt, "\n")));
    ($fmt:expr, $($arg:tt)*) => (print!(concat!($fmt, "\n"), $($arg)*));
}

lazy_static! {
    /// Global Uart ref used by print!
    pub static ref WRITER: Mutex<Writer> = Mutex::new(Writer::new());
}

/// Underlying function used by print!
pub fn print(args: fmt::Arguments) {
    use core::fmt::Write;
    WRITER.lock().write_fmt(args).unwrap();
}

pub struct Writer;

impl Writer {
    /// Initializes UART.
    fn new() -> Writer {
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
        }
        Writer
    }

    /// Writes character to UART.
    /// May spin if previous write hasn't finished.
    pub fn putc(&self, c: char) {
        unsafe {
            // Idle while transmitting
            while (AUX_MU_LSR_REG.read_volatile() & (1 << 6)) == 0 {}

            AUX_MU_IO_REG.write_volatile(c as u32);
        }
    }

    /// Prints all characters in `s` to UART via [putc]
    pub fn print(&self, s: &str) {
        for c in s.chars() {
            self.putc(c);
        }
    }

    /// Returns character from UART.
    /// May spin if no character currently read
    pub fn getc(&self) -> char {
        unsafe {
            // Idle while waiting for data
            while (AUX_MU_IO_REG.read_volatile() & (1 << 6)) == 0 {}

            (AUX_MU_IO_REG.read_volatile() as u8) as char
        }
    }

    /// Like [getc], but times out after `timeout` loops
    pub fn getc_timeout(timeout: u64) -> Option<char> {
        let mut res = 0;
        let mut i = 0;

        unsafe {
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
}

impl fmt::Write for Writer {
    fn write_str(&mut self, s: &str) -> fmt::Result {
        self.print(s);
        Ok(())
    }
}
