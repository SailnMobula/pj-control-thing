echo "Compiling sketch"
arduino-cli compile --fqbn arduino:mbed:nano33ble
echo "Uploading to target sketch"
arduino-cli upload -p /dev/cu.usbmodem143201 --fqbn arduino:mbed:nano33ble