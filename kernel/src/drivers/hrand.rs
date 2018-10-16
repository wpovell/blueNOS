//! Module to interface with hardware randomness
//!
//! Based on these sources [1], [2]
//!
//! [1]: https://github.com/thenaran/linux-rpi/blob/master/drivers/char/hw_random/bcm2708-rng.c
//! [2]: https://github.com/bztsrc/raspi3-tutorial/blob/master/06_random/rand.c

use drivers;
use rand_core;
use spin::Mutex;

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

lazy_static! {
    /// Global rand generator
    pub static ref RAND: Mutex<HRand> = Mutex::new(HRand::init(false));
}

pub struct HRand;

impl HRand {
    fn init(fast: bool) -> HRand {
        // @NOTE: Other srcs mask interrupts here?
        unsafe {
            if fast {
                STATUS.write_volatile(WARMUP_COUNT);
                CTRL.write_volatile(FAST_INIT);
            } else {
                // Faster, less random
                STATUS.write_volatile(WARMUP_COUNT);
                CTRL.write_volatile(NORMAL_INIT);
            }
        }
        HRand
    }
}

impl rand_core::RngCore for HRand {
    /// Returns a random u32.
    /// This call will spin
    /// @TODO: Check that hrand is enabled
    /// @QUESTION: What prevents two rapid reads from getting the same number here?
    fn next_u32(&mut self) -> u32 {
        unsafe {
            while (STATUS.read_volatile() >> 24) == 0 {}
            DATA.read_volatile()
        }
    }

    fn next_u64(&mut self) -> u64 {
        rand_core::impls::next_u64_via_fill(self)
    }

    fn fill_bytes(&mut self, dest: &mut [u8]) {
        rand_core::impls::fill_bytes_via_next(self, dest)
    }

    fn try_fill_bytes(&mut self, dest: &mut [u8]) -> Result<(), rand_core::Error> {
        self.fill_bytes(dest);
        Ok(())
    }
}
