//! Module to interface with hardware randomness
//!
//! Based on these sources [1], [2]
//!
//! [1]: https://github.com/thenaran/linux-rpi/blob/master/drivers/char/hw_random/bcm2708-rng.c
//! [2]: https://github.com/bztsrc/raspi3-tutorial/blob/master/06_random/rand.c

use drivers;

const BASE: usize = drivers::P_BASE + 0x0010_4000;
const CTRL: *mut u32 = BASE as *mut u32;
const STATUS: *mut u32 = (BASE + 0x4) as *mut u32;
const DATA: *const u32 = (BASE + 0x8) as *const u32;

// Used when enabling rand
const NORMAL_INIT: u32 = 0x1;
// This option is faster but less random
const FAST_INIT: u32 = 0x2;

// Initial numbers are less random
const WARMUP_COUNT: u32 = 0x4_0000;

/// Initializes hrand
/// @NOTE: Other srcs mask interrupts here?
pub fn init() {
    unsafe {
        STATUS.write_volatile(WARMUP_COUNT);
        CTRL.write_volatile(NORMAL_INIT);
    }
}

/// Initializes faster, less random version of hrand
pub fn fast_init() {
    unsafe {
        STATUS.write_volatile(WARMUP_COUNT);
        CTRL.write_volatile(FAST_INIT);
    }
}

/// Returns a large random number.
/// Unclear what RAND_MAX is.
/// @TODO: Check that hrand is enabled
/// @QUESTION: What prevents two rapid reads from getting the same number here?
fn hrand() -> u32 {
    unsafe {
        while (STATUS.read_volatile() >> 24) == 0 {}
        DATA.read_volatile()
    }
}
