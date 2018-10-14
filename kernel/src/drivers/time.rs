#[inline(never)]
pub fn spin_sleep_ins(ms: usize) {
    for _ in 0..(ms * 600) {
        unsafe {
            asm!("nop" :::: "volatile");
        }
    }
}

/*
#[inline(never)]
pub fn spin_sleep_ins(inst: usize) {
    unsafe {
        asm!("
        delay:
        subs $0, $0, #1
        bne delay
        " : : "r"(inst) : );
    }
}*/

const TIMER_LOW: *mut u32 = (super::P_BASE + 0x3004) as *mut u32;
const TIMER_HIGH: *mut u32 = (super::P_BASE + 0x3008) as *mut u32;

pub fn get_systime() -> u64 {
    unsafe {
        let mut hi = TIMER_HIGH.read_volatile();
        let mut low = TIMER_LOW.read_volatile();
        if hi != TIMER_HIGH.read_volatile() {
            hi = TIMER_HIGH.read_volatile();
            low = TIMER_LOW.read_volatile();
        }

        ((hi as u64) << 32) | (low as u64)
    }
}
