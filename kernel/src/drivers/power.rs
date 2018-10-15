//! Module for power management stuff
//!
//! Right now this just contains reboot stuff via the watchdog.
//! It's super poorly documented, so I've mostly been relying on [this].
//! @NOTE: This does not work in QEMU
//! @TODO: Test this actually works
//!
//! [this]: https://github.com/raspberrypi/linux/blob/3b01f059d2ef9e48aca5174fc7f3b5c40fe2488c/drivers/watchdog/bcm2835_wdt.c
use drivers;

const BASE: usize = drivers::P_BASE + 0x0010_0000;
const RSTC: *mut u32 = (BASE + 0x1c) as *mut u32;
const RSTS: *const u32 = (BASE + 0x20) as *const u32;
const WDOG: *mut u32 = (BASE + 0x24) as *mut u32;

const TIME_SET: u32 = 0x000f_ffff;
const RSTC_WRCFG_CLR: u32 = 0xffff_ffcf;
const RSTC_WRCFG_FULL_RESET: u32 = 0x0000_0020;
const RSTC_RESET: u32 = 0x0000_0102;
const PASSWORD: u32 = 0x5a00_0000;

/// Sets the watchdog to `timeout`
pub fn wdog_start(timeout: u32) {
    unsafe {
        let mut rstc = RSTC.read_volatile();
        let wdog = PASSWORD | (timeout & TIME_SET);
        rstc = PASSWORD | (rstc & RSTC_WRCFG_CLR) | RSTC_WRCFG_FULL_RESET;
        WDOG.write_volatile(wdog);
        RSTC.write_volatile(rstc);
    }
}

/// Gets the current watchdog timer value
pub fn wdog_get() -> u32 {
    unsafe { WDOG.read_volatile() & TIME_SET }
}

/// Stops the watchdog
pub fn wdog_stop() {
    unsafe {
        RSTC.write_volatile(PASSWORD | RSTC_RESET);
    }
}

/// Reboot the RPi
/// 
/// Sets the watchdog to a small value and lets it expire
pub fn reboot() {
    wdog_start(10);
}
