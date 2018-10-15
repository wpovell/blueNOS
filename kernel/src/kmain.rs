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
    uart::print("hello world!");
    panic!()
}
