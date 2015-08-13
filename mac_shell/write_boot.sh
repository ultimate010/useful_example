hdiutil convert -format UDRW -o /path/to/generate/img/file /path/to/your/iso/file
diskutil list
diskutil unmountDisk /dev/disk1
sudo dd if=ubuntu.img.dmg of=/dev/disk1 bs=1m
diskutil eject /dev/disk1
