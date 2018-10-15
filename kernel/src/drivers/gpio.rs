use drivers::time;

const GPIO_BASE: usize = super::P_BASE + 0x0020_0000;

const fn fsel_reg(x: u8) -> *mut u32 {
    (GPIO_BASE + (x as usize) * 0x4) as *mut u32
}

#[inline]
fn valid_pin(pin: u8) -> bool {
    2 <= pin && pin <= 27
}

pub enum PinFunc {
    Input = 0,
    Output = 1,
    F5 = 2,
    F4 = 3,
    F0 = 4,
    F1 = 5,
    F2 = 6,
    F3 = 7,
}

pub fn set_func(pin: u8, f: PinFunc) {
    assert!(valid_pin(pin));

    let reg = fsel_reg(pin / 10);
    let shift = 3 * (pin % 10);

    unsafe {
        let mut cur = reg.read_volatile();
        cur &= !(0b111 << shift);
        cur |= (f as u32) << shift;
        reg.write_volatile(cur);
    }
}

#[derive(Copy, Clone)]
pub enum PinPull {
    None = 0,
    Down = 1,
    Up = 2,
}

const GPPUD: *mut u32 = (GPIO_BASE + 0x94) as *mut u32;
const GPPUDCLK0: *mut u32 = (GPIO_BASE + 0x98) as *mut u32;
const GPPUDCLK1: *mut u32 = (GPIO_BASE + 0x9C) as *mut u32;

pub fn set_pull(pin: u8, pull: PinPull) {
    assert!(valid_pin(pin));

    unsafe {
        // Set pull
        GPPUD.write_volatile(pull as u32);
        time::spin_sleep_ins(150);

        // Set Clock
        GPPUDCLK0.write_volatile(pull as u32);
        time::spin_sleep_ins(150);

        // Clear Clock
        GPPUD.write_volatile(0);
        GPPUDCLK0.write_volatile(0);
    }
}

const GPSET0: *mut u32 = (GPIO_BASE + 0x1C) as *mut u32;
const GPSET1: *mut u32 = (GPIO_BASE + 0x20) as *mut u32;
const GPCLR0: *mut u32 = (GPIO_BASE + 0x28) as *mut u32;
const GPCLR1: *mut u32 = (GPIO_BASE + 0x2C) as *mut u32;

pub fn set(pin: u8, val: bool) {
    assert!(valid_pin(pin));

    let reg = if val { GPSET0 } else { GPCLR0 };
    unsafe {
        reg.write_volatile(1 << pin);
    }
}
