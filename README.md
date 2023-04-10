Run `make` and follow the steps it outlines.

Make sure also to run the following once after the BBG's bootup:
`config-pin p8.11 pruout`
`config-pin p8.12 pruout`
`config-pin p8.15 pruin`

Then, for Zen Cape Green, run the following once after the BBG's bootup:
`config-pin p9.18 i2c`
`config-pin p9.17 i2c`
`i2cset -y 1 0x20 0x00 0x00`
`i2cset -y 1 0x20 0x01 0x00`
`i2cset -y 1 0x20 0x14 0x1e`
`i2cset -y 1 0x20 0x15 0x78`

To run the Linux app, on BBG, navigate to /mnt/remote/myApps. Then:
`sudo ./find-dot`