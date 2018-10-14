#![no_std]
#![no_main]
#![feature(compiler_builtins_lib, lang_items, asm)]
#![allow(dead_code)]

mod drivers;
mod lang;

use drivers::{gpio, time};

#[no_mangle]
pub extern "C" fn kmain() -> ! {
    drivers::uart::init();
    drivers::uart::print("hello!\n");

    gpio::set_func(16, gpio::PinFunc::Output);
    loop {
        gpio::set(16, true);
        time::spin_sleep_ins(1000);
        gpio::set(16, false);
        time::spin_sleep_ins(1000);
    }
}
