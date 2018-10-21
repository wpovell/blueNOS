#![no_std]
#![feature(compiler_builtins_lib, lang_items, asm)]
#![allow(dead_code)]

#[macro_use]
mod drivers;
mod atags;
mod graphics;
mod lang;

#[macro_use]
extern crate lazy_static;
extern crate rand_core;
extern crate spin;

use drivers::time::sys;

/// Entry point into Rust code. It all starts here!
#[no_mangle]
pub extern "C" fn kmain() -> core::fmt::Result {
    loop {
        println!("{}", sys());
    }
}
