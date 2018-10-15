#![no_std]
#![no_main]
#![feature(compiler_builtins_lib, lang_items, asm)]
#![allow(dead_code)]

mod atags;
mod drivers;
mod lang;

use drivers::{gpio, time, uart};

#[no_mangle]
pub extern "C" fn kmain() -> ! {
    uart::init();
    uart::print("hello!\n");
    for tag in atags::ATags::new() {
        match tag.head.typ {
            atags::MEM => uart::putc('m'),
            atags::CORE => uart::putc('c'),
            _ => break,
        }
    }
    gpio::set_func(16, gpio::PinFunc::Output);
    loop {
        gpio::set(16, true);
        time::spin_sleep_ins(1000);
        gpio::set(16, false);
        time::spin_sleep_ins(1000);
    }
}
