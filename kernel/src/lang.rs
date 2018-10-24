//! Implements language features required by Rust
use core::panic::PanicInfo;

/// Called on panic
#[panic_handler]
fn panic(_info: &PanicInfo) -> ! {
    loop {}
}

/// Used for stack unwinding.
/// Since our panic behavior is abort this will never happen, but quiets some warnings
#[lang = "eh_personality"]
pub extern "C" fn eh_personality() {}
