# TODO
- Move screen back when car is going faster
- Configure car performance
    - Max Speed
    - Accelerate
    - Turn

- Increase level size
- Add more levels

- Integrate into a FX image with multiple games

# Bugs
- Save dazta fails to save first time (or maybe when there is a new record)
    - FX Busy error
    - Save file at startup if it's not found
- Car is too fast at high frame rates    
- Work out why flashing text causes slow downs

# DONE
- Add an ingame map screen with current location
- Move level times into FX
- Rewrite Number Zoom to use FX
- Rewrite Stage Zoom to use FX
- Move levels into FX
    - Single level var to store current level layout
    - Max level size increase 30x30 ?
- Use FX Save
- Resolved performance issues
    - Change ArdBitmap to another effect 
        - scroll the number tiles across the screen
        - Grayscale race starter

    - Remove ArdBitmap
        - Number zoom, create tiles and stream
- Use fewer loads 
    - Smaller tiles for maps

# Skip
- Add missing tiles to minimize rendering (game logic changes)
- Make newline detection optional (minimal benefit)
- Use FX Arduboy Font (my impl is simpler)
- Update all graphics to grayscale (not fast enough)
- Add a win video (waste of space)
- Add new car models (not without grayscale)
- Use Grayscale (not rendering fast enough)
