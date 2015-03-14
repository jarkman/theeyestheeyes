# Introduction #

There are many ways to make eyeballs. This is a quick introduction to the parts and techniques we used.

## Electronics ##

There's not much to it. The parts are:
  * One mbed processor (http://mbed.org, http://uk.farnell.com/nxp/om11043/kit-mbed-prototyping-board-lpc1768/dp/1761179)
  * One serial camera module (http://www.coolcomponents.co.uk/catalog/product_info.php?cPath=45&products_id=369)
  * Three servos (http://servoshop.co.uk/index.php?pid=STECENERG&area=Servo)
  * One switch for the power supply
  * One 4-AA-cell battery box, or a spare iPhone charger putting out 5v on a USB plug.

Servos wire directly to PWM pins on the mbed. The camera wires to UART pins on the mbed. The mbed also provides a 3.3v supply to run the camera.

Specific pin wiring is:
  * X servo - pin 23
  * Y servo - pin 22
  * Eyelid servo - pin 21
  * Camera tx - pin 13
  * Camera rx - pin 14

Servos are powered directly from the battery. The camera is powered from the 3.3v output on the mbed, pin 40.

## Software ##

See the source for this Google Code project.

To build it as an mbed project, you need to add the files in 'common' to a new project in the mbed compiler.

The current mbed project should also be available from this Notebook page:
http://mbed.org/users/jarkman/notebook/ucam-serial-camera/

To build the testframe for Visual Studio, you need to open `UCamDemo/UCamDemo.sln`.

To build the testframe for XCode, you need to open `MacTest/MacTest.xcodeproj` (but note that at the time of writing, that testframe didn't do much).

There are three main behaviours:
  * Motion tracking (actually just the center of gravity of the different-from-background pixels in each frame - see `MotionFinder::processFrame()` in `MotionFinder.cpp`)
  * Sleepiness (when less than 3 percent of the pixels have changed in each frame, the eye slowly centers and droops, and the eyelid doesn't open as wide on each blink)
  * Blinking (every 15 seconds)

## Mechanics ##

There are only two moving parts - the eyeball and the eyelid. Two servos move the eyeball left & right (X) and up & down (Y) , and one moves the eyelid.

Here's the servo assembly for X and Y, on its support arm:

<a href='http://picasaweb.google.co.uk/lh/photo/oATuF0muCme48akcJK76Zw?feat=embedwebsite'><img src='http://lh4.ggpht.com/_PBDnF38pZCU/Sz9yxGHgndI/AAAAAAAAH5E/J3jQUeoZkPI/s400/DSC04863.JPG' /></a>


The ball itself is made from a Christmas ornament, a hard white plastic ball. The back of the ball is cut away, and pivot holes are drilled across a diameter. The two servos live inside the ball:

<a href='http://picasaweb.google.co.uk/lh/photo/C2YdE9dLjF6LFrIpUXXkjA?feat=embedwebsite'><img src='http://lh4.ggpht.com/_PBDnF38pZCU/Sz9yx6D99gI/AAAAAAAAH5I/h-HoSnHikDU/s400/DSC04864.JPG' /></a>

With a smaller ball, or bigger servos, it would be easier to mount the ball on a simple gimbal and move it with wire pushrods from servos mounted behind the ball.

The upper eyelid (along with the non-moving lower eyelid) is hammered from thin copper sheet. The copper has to be annealed, then domed into a sandbag to stretch it into a curve, then hammered over a suitable former to make a spherical shell. It's easiest to make the shell oversize and then trim it back to the right shape.

The eyelids have a common pivot (actually, a small woodscrew) on each side of the case. It's important to get these pivots in line with the center of the eyeball, so the clearance between lid and ball stays the same as the lids open & close.

<a href='http://picasaweb.google.co.uk/lh/photo/aVfV_FhnNEWnAVhS8RLZQQ?feat=embedwebsite'><img src='http://lh6.ggpht.com/_PBDnF38pZCU/Sz9yzD7q4PI/AAAAAAAAH5M/nVW4KPZRMnw/s400/DSC04866.JPG' /></a>

The lower lid is fixed, and the upper one is moved by a wire pushrod from a servo. The ball is removed from this photo for clarity:

<a href='http://picasaweb.google.co.uk/lh/photo/KBca9dj671VbqKaT-NulGQ?feat=embedwebsite'><img src='http://lh6.ggpht.com/_PBDnF38pZCU/Sz9y0TsJTOI/AAAAAAAAH5Q/BSxQ5XagC3c/s400/DSC04867.JPG' /></a>


<a href='http://picasaweb.google.co.uk/lh/photo/82k8xVos1pmAZT8Gc2lcdQ?feat=embedwebsite'><img src='http://lh5.ggpht.com/_PBDnF38pZCU/Sz9y1ZOYYvI/AAAAAAAAH5U/Iox5427jApw/s400/DSC04868.JPG' /></a>

This is what it looks like with all the parts in place. The support arm is screwed to the underside of the MDF:

<a href='http://picasaweb.google.co.uk/lh/photo/8j9yKaCXuMgQ2YnxzQES9g?feat=embedwebsite'><img src='http://lh5.ggpht.com/_PBDnF38pZCU/Sz9y2iD3XNI/AAAAAAAAH5Y/RW3w3icoG4w/s400/DSC04869.JPG' /></a>

The eyelid takes up a suprising amount of space behind the eyeball when it opens all the way, so it's important to kepp the servo support arm low:

<a href='http://picasaweb.google.co.uk/lh/photo/maWAsH9TL0Op1XdZtGjd4g?feat=embedwebsite'><img src='http://lh4.ggpht.com/_PBDnF38pZCU/Sz9y3-qZAzI/AAAAAAAAH5g/idME6FFHUU8/s400/DSC04870.JPG' /></a>

The iris is just a bit of hammered copper, glued to the ball:

<a href='http://picasaweb.google.co.uk/lh/photo/jnG9t3oXk2HSB4EdRv8bKw?feat=embedwebsite'><img src='http://lh4.ggpht.com/_PBDnF38pZCU/Sz9y40bx8xI/AAAAAAAAH5k/cWm5EElo6zI/s400/DSC04871.JPG' /></a>


## Case ##

The eyeball and lids are attached to a simple U-shaped MDF frame inside a plywood box.

The camera is mounted in a brass flange - actually a plumbing part:

<a href='http://picasaweb.google.co.uk/lh/photo/CHOe7i4dAAvljJYNH-EuPA?feat=embedwebsite'><img src='http://lh3.ggpht.com/_PBDnF38pZCU/Sz9y6qoFdJI/AAAAAAAAH5o/ppYSQEPHeEQ/s400/DSC04872.JPG' /></a>

The case is fabric-covered, with patinated brass corners, for a simple steampunky kind of a look.