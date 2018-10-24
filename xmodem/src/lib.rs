// @TODO: Figure out how to use core_io here instead
use std::io;

#[cfg(test)] mod tests;
mod read_ext;

use read_ext::ReadExt;

const SOH: u8 = 0x01;
const EOT: u8 = 0x04;
const ACK: u8 = 0x06;
const NAK: u8 = 0x15;
const CAN: u8 = 0x18;

const RETRIES: u8 = 10;
const PACKET_SIZE: usize = 128;

pub struct Xmodem<R> {
    packet: u8,
    inner: R,
    started: bool,
}

impl Xmodem<()> {
    /// Transmits `data` to the reciever `to` using the XMODEM protocol.
    /// If `data` is not a multiple of PACKET_SIZE it will be padded with zeros.
    ///
    /// Returns the number of bytes written to `to`, excluding padding.
    pub fn transmit<R, W>(mut data: R, to: W) -> io::Result<usize>
    where
        W: io::Read + io::Write,
        R: io::Read,
    {
        let mut transmitter = Xmodem::new(to);
        let mut packet = [0u8; PACKET_SIZE];
        let mut written = 0;
        'next_packet: loop {
            let n = data.read_max(&mut packet)?;

            // Nothing left to read
            if n == 0 {
                transmitter.write_packet(&[])?;
                return Ok(written);
            }

            packet[n..].iter_mut().for_each(|b| *b = 0);

            for _ in 0..RETRIES {
                match transmitter.write_packet(&packet) {
                    Err(ref e) if e.kind() == io::ErrorKind::Interrupted => continue,
                    Err(e) => return Err(e),
                    Ok(_) => {
                        written += n;
                        continue 'next_packet;
                    }
                }
            }

            return Err(io::Error::new(io::ErrorKind::BrokenPipe, "bad transmit"));
        }
    }

    /// Recieves data from `from` using the XMODEM protocol and writes it into `into`.
    /// Returns the number of bytes read from `from`, a multiple of PACKET_SIZE.
    pub fn receive<R, W>(from: R, mut into: W) -> io::Result<usize>
    where
        R: io::Read + io::Write,
        W: io::Write,
    {
        let mut receiver = Xmodem::new(from);
        let mut packet = [0u8; PACKET_SIZE];
        let mut received = 0;
        'next_packet: loop {
            for _ in 0..RETRIES {
                match receiver.read_packet(&mut packet) {
                    Err(ref e) if e.kind() == io::ErrorKind::Interrupted => continue,
                    Err(e) => return Err(e),
                    Ok(0) => break 'next_packet,
                    Ok(n) => {
                        received += n;
                        into.write_all(&packet)?;
                        continue 'next_packet;
                    }
                }
            }

            return Err(io::Error::new(io::ErrorKind::BrokenPipe, "bad recieve"));
        }

        Ok(received)
    }
}

/// Calculates XMODEM checksum for `data`
fn checksum(data: &[u8]) -> u8 {
    let mut sum: u8 = 0;
    data.iter().for_each(|i| sum = sum.wrapping_add(*i));
    sum
}

impl<T: io::Read + io::Write> Xmodem<T> {
    /// Returns a new `Xmodem` instance with the internal reader/writer set to `inner`.
    pub fn new(inner: T) -> Self {
        Xmodem {
            packet: 1,
            started: false,
            inner,
        }
    }

    /// Reads a single byte from I/O stream.
    ///
    /// # Errors
    ///
    /// Returns an error if reading from `inner` fails or `abort_on_can` is `true` and the read byte is `CAN`.
    fn read_byte(&mut self, abort_on_can: bool) -> io::Result<u8> {
        let mut buf = [0u8; 1];
        self.inner.read_exact(&mut buf)?;

        let byte = buf[0];
        if abort_on_can && byte == CAN {
            return Err(io::Error::new(
                io::ErrorKind::ConnectionAborted,
                "received CAN",
            ));
        }

        Ok(byte)
    }

    /// Reads a single byte from `inner` checking that it equals `expected`.
    ///
    /// # Errors
    ///
    /// * `ConnectionAborted`: Recieved `CAN`
    /// * `InvalidData`: Read byte does not match `expected`
    fn expect_byte(&mut self, expected: u8, msg: &'static str) -> io::Result<u8> {
        let byte = self.read_byte(false)?;
        if expected != byte {
            if byte == CAN {
                return Err(io::Error::new(
                    io::ErrorKind::ConnectionAborted,
                    "received CAN",
                ));
            } else {
                return Err(io::Error::new(io::ErrorKind::InvalidData, msg));
            }
        }

        Ok(byte)
    }

    /// Same as `expect_byte` except writes `CAN` before erroring out.
    fn expect_byte_or_cancel(&mut self, expected: u8, msg: &'static str) -> io::Result<u8> {
        match self.expect_byte(expected, msg) {
            Err(e) => {
                self.write_byte(CAN)?;
                Err(e)
            }
            Ok(b) => Ok(b),
        }
    }

    /// Writes a single byte to the inner I/O stream.
    ///
    /// # Errors
    ///
    /// Returns an error if writing to `inner` fails.
    fn write_byte(&mut self, byte: u8) -> io::Result<()> {
        self.inner.write_all(&[byte])
    }

    /// Reads a single packet from `inner` using XMODEM protocol.
    /// Returns size of packet read. On success this will always be `PACKET_SIZE`.
    pub fn read_packet(&mut self, buf: &mut [u8]) -> io::Result<usize> {
        // Start transmission
        if !self.started {
            self.write_byte(NAK)?;
            self.expect_byte(SOH, "Expected SOH")?;
            self.started = true;
            self.packet = 0;
        }

        let byte = self.read_byte(true)?;
        let expected_packet = self.packet;

        if byte == EOT {
            // End transmission
            self.write_byte(NAK)?;
            self.expect_byte_or_cancel(EOT, "Expected EOT")?;
            self.write_byte(ACK)?;
            self.started = false;
            return Ok(0);
        } else if byte == expected_packet {
            // Packet
            self.expect_byte_or_cancel(!expected_packet, "Expected packet complement")?;
            self.inner.read_exact(buf)?;
            let check = self.read_byte(true)?;
            if checksum(buf) != check {
                self.write_byte(NAK)?;
            } else {
                self.write_byte(ACK)?;
            }
        } else {
            return Err(io::Error::new(
                io::ErrorKind::InvalidData,
                "Wrong packet num",
            ));
        }

        Ok(PACKET_SIZE)
    }

    /// Write packet to `inner` using the XMODEM protocol.
    ///
    /// # Errors
    ///
    /// * `UnexpectedEof`: `buf` is not 0 or PACKET_SIZE long
    /// * `ConnectionAborted`: Recieved `CAN` from reciever
    /// * `Interrupted`: Bad checksum
    /// * `InvalidData`: Unexpected response
    pub fn write_packet(&mut self, buf: &[u8]) -> io::Result<usize> {
        if buf.len() < PACKET_SIZE && !buf.is_empty() {
            return Err(io::Error::new(
                io::ErrorKind::UnexpectedEof,
                "Buffer must be len 0 or PACKET_SIZE",
            ));
        }

        // Start transmission
        if !self.started {
            self.expect_byte(NAK, "Expected NAK")?;
            self.write_byte(SOH)?;
            self.started = true;
            self.packet = 0;
        }

        // End transmission
        if buf.is_empty() {
            self.write_byte(EOT)?;
            self.expect_byte(NAK, "Expected NAK")?;
            self.write_byte(EOT)?;
            self.expect_byte(ACK, "Expected ACK")?;
            self.started = false;
            return Ok(0);
        }

        // Packet
        let current_packet = self.packet;
        self.write_byte(current_packet)?;
        self.write_byte(!current_packet)?;

        self.inner.write_all(buf)?;

        self.write_byte(checksum(buf))?;

        self.packet += 1;

        // Response
        match self.read_byte(true)? {
            NAK => return Err(io::Error::new(io::ErrorKind::Interrupted, "Bad checksum")),
            ACK => (),
            _ => {
                return Err(io::Error::new(
                    io::ErrorKind::InvalidData,
                    "Unexpected response",
                ))
            }
        }

        Ok(PACKET_SIZE)
    }

    /// Flush the output stream.
    ///
    /// # Errors
    ///
    /// Returns error if not all bytes written due to I/O error or EOF.
    pub fn flush(&mut self) -> io::Result<()> {
        self.inner.flush()
    }
}
