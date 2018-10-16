//! Time related functions
//!
//! Magic here is described in Section 12

/// Spin sleep for approximately `inst` instructions
pub fn delay(inst: usize) {
    unsafe {
        asm!("
        delay:
        subs $0, $0, #1
        bne delay
        " : : "r"(inst) : );
    }
}

/// @TODO: Test if this can be made into a single u64
const TIMER_LOW: *mut u32 = (super::P_BASE + 0x3004) as *mut u32;
const TIMER_HIGH: *mut u32 = (super::P_BASE + 0x3008) as *mut u32;

/// Gets systime from RPi hardware clock in ms
pub fn sys() -> u64 {
    unsafe {
        let mut hi = TIMER_HIGH.read_volatile();
        let mut low = TIMER_LOW.read_volatile();
        if hi != TIMER_HIGH.read_volatile() {
            hi = TIMER_HIGH.read_volatile();
            low = TIMER_LOW.read_volatile();
        }

        (u64::from(hi) << 32) | u64::from(low)
    }
}
