//! Various memory peripherals on the RPi
//!
//! Sections referenced in this module usually refer [BCM2837 ARM Peripherals Manual](https://web.stanford.edu/class/cs140e/docs/BCM2837-ARM-Peripherals.pdf)
pub mod gpio;
pub mod hrand;
pub mod mbox;
pub mod power;
pub mod time;
pub mod uart;

/// Base memory mapped peripheral address Section 1.2.2
pub const P_BASE: usize = 0x3F00_0000;
