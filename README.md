# GMBY
##### A work in progress Game Boy DMG emulator

Very early work in progress. Keyboard input and sound is not yet implemented, just the CPU and graphics. Currently, the only game known to boot correctly is Boxxle, however the majority of Blargg's CPU ROM tests successfully pass. The next step is to fix inaccuracies in timing, and get Tetris running.

Like I said, this is a work in progress, and the code is not documented very well. I will go through and document/organize it once a little more progress is made.

In order to run the code, build the project in Visual Studio 2017. SDL2 is required.

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
Failed: 08-misc instrs.gb  
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;"F0 E0 F2 E2 FA EA 08 01 11 21 31 F5 C5 D5 E5 F1 C1 D1 E1"  
Passed: 09-op r,r.gb  
Passed: 10-bit ops.gb  
Passed: 11-op a,(hl).gb  
