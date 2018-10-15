pub mod gpio;
pub mod time;
pub mod uart;

/// Base memory mapped peripheral address (Section 1.2.2)
pub const P_BASE: usize = 0x3F00_0000;
