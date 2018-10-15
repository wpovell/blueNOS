//! Implements language features required by Rust
use core::panic::PanicInfo;

/// Called on panic
#[panic_handler]
fn panic(_info: &PanicInfo) -> ! {
    loop {}
}
