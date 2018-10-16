#![feature(slice_patterns, type_ascription)]
use std::env;
use std::fs::File;
use std::io::{self, Read, Write};

const DEF_IMG: &str = "build/kernel8.img";
const DEF_DEV: &str = "/dev/ttyUSB0";

enum Tkns {
    SOH = 1,
    EOT = 4,
    ACK = 6,
    NAK = 21,
    CAN = 24,
}

fn srv(kern: &File, dev: &File) -> Result<(), Box<std::error::Error>> {
    println!("Starting server, waiting for NAK");
    Ok(())
}

fn sh() {}

fn main() -> Result<(), Box<std::error::Error>> {
    let args: Vec<String> = env::args().collect();
    let args: Vec<&str> = args.iter().map(|x| x.as_str()).collect();

    let (img_fn, dev_fn): (&str, &str) = match args.as_ref(): &[&str] {
        [img] => (img, DEF_DEV),
        [img, dev, ..] => (img, dev),
        [] => (DEF_IMG, DEF_DEV),
    };

    let img_fd = File::open(img_fn)?;
    let dev_fd = File::open(dev_fn)?;

    let mut buffer = String::new();
    loop {
        print!("> ");
        io::stdout().flush()?;
        io::stdin().read_line(&mut buffer)?;
        match buffer.as_ref() {
            "s" => srv(&img_fd, &dev_fd)?,
            "c" => sh(),
            _ => println!("Unknown command"),
        }
    }
}
