#ifndef __GETCONTENTS_H__
#define __GETCONTENTS_H__

/// Reads keyboard and doesn't wait for ENTER
/// @param[in]    fd            file descriptor of open spidev
/// @param[in]    address       address of register that needs to be read
void getContents(int, uint8_t);

#endif
