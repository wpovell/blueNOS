//! Module to parse ATAGs
//!
//! See [here](http://www.simtec.co.uk/products/SWLINUX/files/booting_article.html) for where most of these magic numbers came from

/// Denotes end of ATAGs
pub const NONE: u32 = 0x0000_0000;

#[repr(packed)]
#[derive(Copy, Clone)]
pub struct Header {
    pub size: u32,
    pub typ: u32,
}

/// Holds information about available memory
#[repr(packed)]
#[derive(Copy, Clone)]
pub struct Mem {
    pub size: u32,
    pub start: u32,
}
pub const MEM: u32 = 0x5441_0002;

/// First ATAG
#[repr(packed)]
#[derive(Copy, Clone)]
pub struct Core {
    pub flags: u32,
    pub pagesize: u32,
    pub rootdev: u32,
}
pub const CORE: u32 = 0x5441_0001;

/// Holds data from commandline, usually stored in cmdline.txt on the boot filesystem
/// @TODO: I've never had this work
/// @NOTE: `cmd` is a null-terminated string
#[repr(packed)]
#[derive(Copy, Clone)]
pub struct Command {
    pub cmd: *const u8,
}
pub const CMD: u32 = 0x5441_0009;

#[repr(packed)]
#[derive(Copy, Clone)]
pub union Tag {
    pub core: Core,
    pub mem: Mem,
    pub cmd: Command,
}

/// Single ATAG
#[repr(packed)]
#[derive(Copy, Clone)]
pub struct ATag {
    pub head: Header,
    pub body: Tag,
    pub cmd: Tag,
}

const START: *const ATag = 0x100 as *const ATag;

/// Iterator that returns ATAGs present on system
pub struct ATags {
    cur: *const ATag,
}

impl ATags {
    pub fn new() -> ATags {
        ATags { cur: START }
    }
}

impl Iterator for ATags {
    type Item = ATag;

    fn next(&mut self) -> Option<ATag> {
        unsafe {
            let tag: ATag = *self.cur;
            self.cur = (self.cur as *const u32).offset(tag.head.size as isize) as *const ATag;
            match tag.head.typ {
                CORE | MEM | CMD => Some(tag),
                _ => None,
            }
        }
    }
}
