//! Functions related to RPi's mailbox interface
//! Okayish documentation can be found [here](https://github.com/raspberrypi/firmware/wiki)

use drivers;

const BASE: usize = drivers::P_BASE + 0xB880;

const READ: usize = BASE;
const STATUS: usize = BASE + 0x18;
const WRITE: usize = BASE + 0x20;

const READ_EMPTY: usize = 1 << 30;
const WRITE_FULL: usize = 1 << 31;
