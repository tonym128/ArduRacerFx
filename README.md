# Ardu Racer FX 🏎️
A time trail game, not for the faint of heart. Get your heart pumping, get your game face on and gun it down the striaghts and hammer it in the corners.

This is a port of ArduRacer which includes usage of the FX chip on the ArduboyFX!

## Original Game Features
- 30+ fps gameplay
- 10 stages
- 3 times to beat per stage, beat the 3rd best time to progress to the next stage
- Track view prior to starting race
- Speedometer to track you roaring engine
- Dynamic zoom into level to see your start

## New FX Features
- Locked 60 FPS gameplay
- 20 Stages
- Beat the Dev Times
  - 1st place is the best time possible with tuning and shortcuts
  - 2nd place is the best time possible with no tuning and shortcuts
  - 3rd place is a chosen time which should be possible with no tuning and no shortcuts and some leeway
  - All times were done on an ArduboyFX, not using an emulator. Wanted to keep the playing field fair
- Bigger maps for later levels
- Car Tuning for optimising the performance to your preference or the track
- 2x frames on car to track 16 directions
- Integrated Map to give you some information on upcoming turns
- Much smoother intro zoom animations

## Technical callouts
- 50kb of assets
- FX Bitmap streaming of tiles
- Locked 60 FPS
- Much smoother map zoom using mip mapped tiled animation
- ArduBitmap ported to FX for offloading assets
- Car Tuning feature
- FX Save feature, no more eeprom issues
- Integrated Map using dynamic map cache for high speed drawing
- Bigger maps
- Allowing timing up to 99.99 seconds
