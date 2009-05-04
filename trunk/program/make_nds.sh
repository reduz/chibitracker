/usr/local/devkitPro/devkitARM/bin/arm-eabi-objcopy -O binary chibitracker_ds.elf chibitracker_ds.bin
/usr/local/devkitPro/devkitARM/bin/arm-eabi-objcopy -O binary chibitracker_ds_arm7.elf chibitracker_ds_arm7.bin
/usr/local/devkitPro/devkitARM/bin/ndstool -c chibitracker_ds.nds -9 chibitracker_ds.bin -7 chibitracker_ds_arm7.bin
/usr/local/devkitPro/devkitARM/bin/dsbuild chibitracker_ds.nds 
