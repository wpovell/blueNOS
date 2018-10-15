#![no_std]
#![no_main]
#![feature(compiler_builtins_lib, lang_items, asm)]
#![allow(dead_code)]

mod atags;
mod drivers;
mod graphics;
mod lang;

use drivers::{gpio, time, uart};

/// Entry point into Rust code. It all starts here!
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
        time::delay(1000);
        gpio::set(16, false);
        time::delay(1000);
    }
}
