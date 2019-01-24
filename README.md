# GMBY
##### A work in progress Game Boy DMG emulator

Very early work in progress. Keyboard input and sound is not yet implemented, just the CPU and graphics. The next step is to implement keyboard controls and sound.

Like I said, this is a work in progress, and the code is not documented very well. I will go through and document/organize it once a little more progress is made.

In order to run the code, build the project in Visual Studio 2017. SDL2 is required.

### Game Compatibility
Boxxle: Boots to title screen, demo runs correctly
Tetris: Boots to title screen, demo runs correctly

### Blargg's CPU Tests (Currently):
Passed: 01-special.gb  
Failed: 02-interrupts.gb  
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;"Timer doesn't work"  
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;"Failed #4"  
Passed: 03-op sp,hl.gb   
Passed: 04-op r,imm.gb  
Passed: 05-op rp.gb  
Passed: 06-ld r,r.gb  
Passed: 07-jr,jp,call,ret,rst.gb  
Passed: 08-misc instrs.gb  
Passed: 09-op r,r.gb  
Passed: 10-bit ops.gb  
Passed: 11-op a,(hl).gb  
