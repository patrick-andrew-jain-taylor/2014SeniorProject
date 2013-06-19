RPi Configuration

For first-time install, perform the following:

1) Quick Start
--Insert SD Card
--Connect USB Keyboard/Mouse
--Connect HDMI
--Connect Power

2) Load Raspbian
--Select Raspbian
--Wait for Install

3) Raspi-config
--Change user password to "glasses"
--Disable boot to desktop
--Under advanced options
	--Set hostname to GlassPi#
	--Enable SSH
	--Run Update
--Finish

4) Shutdown/Reboot
sudo shutdown -h now -- Halt
sudo shutdown -r now -- Reboot

login: pi
password: glasses

5) Git
sudo apt-get install git -- likely already installed
git config --global user.name "Your Name Here"
# Sets the default name for git to use when you commit
git config --global user.email "your_email@example.com"
# Sets the default email for git to use when you commit -- should be github email
git clone https://github.com/trickyconverse/2014SeniorProject.git

6) Keyboard
sudo dpkg-reconfigure keyboard-configuration
--Generic 105-key (Intl) PC
--Other
--English (US)
--English (US)
--The default for the keyboard layout
--No compose key
--No




