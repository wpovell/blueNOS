//! Module to parse ATAGs
//!
//! See [here](http://www.simtec.co.uk/products/SWLINUX/files/booting_article.html) for where most of these magic numbers came from

// See comment below about pointer alignment
#![cfg_attr(feature = "cargo-clippy", allow(clippy::cast_ptr_alignment))]

const CORE: u32 = 0x5441_0001;
const MEM: u32 = 0x5441_0002;
const NONE: u32 = 0x0000_0000;
const CMD: u32 = 0x5441_0009;

#[repr(packed)]
#[derive(Copy, Clone)]
struct Header {
    pub size: u32,
    pub typ: u32,
}

#[repr(packed)]
#[derive(Copy, Clone)]
struct Mem {
    pub size: u32,
    pub start: u32,
}

#[repr(packed)]
#[derive(Copy, Clone)]
struct Core {
    pub flags: u32,
    pub pagesize: u32,
    pub rootdev: u32,
}

#[repr(packed)]
#[derive(Copy, Clone)]
struct Command {
    pub cmd: *const u8,
}

#[repr(packed)]
#[derive(Copy, Clone)]
union Tag {
    core: Core,
    mem: Mem,
    cmd: Command,
}

/// Raw C-style struct for parsing memory
#[repr(packed)]
#[derive(Copy, Clone)]
struct RawATag {
    head: Header,
    body: Tag,
    cmd: Tag,
}

const START: *const RawATag = 0x100 as *const RawATag;

/// Iterator that returns ATAGs present on system
pub struct ATags {
    cur: *const RawATag,
}

#[derive(Debug)]
pub enum ATag {
    /// Holds information about available memory
    Mem { size: u32, start: u32 },

    /// First ATAG
    Core {
        flags: u32,
        pagesize: u32,
        rootdev: u32,
    },

    /// Holds data from commandline, usually stored in cmdline.txt on the boot filesystem
    /// @TODO: I've never had this work
    /// @NOTE: `cmd` is a null-terminated string
    Cmd { cmd: *const u8 },
}

impl ATags {
    pub fn list() -> ATags {
        ATags { cur: START }
    }
}

impl Iterator for ATags {
    type Item = ATag;

    fn next(&mut self) -> Option<ATag> {
        unsafe {
            let tag: RawATag = *self.cur;

            // Clippy complains here about pointer alignment when casting to a u32. I believe this is fine since
            // a RawATag should have the same alignment, as it is a C-style struct and it's first element is a u32
            self.cur = (self.cur as *const u32).offset(tag.head.size as isize) as *const RawATag;

            let body: Tag = tag.body;
            match tag.head.typ {
                MEM => Some(ATag::Mem {
                    size: body.mem.size,
                    start: body.mem.start,
                }),
                CORE => Some(ATag::Core {
                    flags: body.core.flags,
                    pagesize: body.core.pagesize,
                    rootdev: body.core.rootdev,
                }),
                CMD => Some(ATag::Cmd { cmd: body.cmd.cmd }),
                _ => None,
            }
        }
    }
}
