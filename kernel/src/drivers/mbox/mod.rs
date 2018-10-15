//! Functions related to RPi's mailbox interface
//! Okayish documentation can be found [here](https://github.com/raspberrypi/firmware/wiki)

use drivers;

const BASE: usize = drivers::P_BASE + 0xB880;

const READ: usize = BASE;
const STATUS: *const u32 = (BASE + 0x18) as *const u32;
const WRITE: usize = BASE + 0x20;

const READ_EMPTY: u32 = 1 << 30;
const WRITE_FULL: u32 = 1 << 31;