<a href='http://surveillance-camera.googlecode.com/hg/images/Head%20Image/Camera_640.png'><img src='http://habrastorage.org/storage/7c315e8f/5c3512ae/4935d288/f95feed0.png' align='left' /></a>

# Introduction #
There are a lot of surveillance cameras out there on the market. Some provide old-school AV output, some plug into local network with Ethernet cable, some need Wi-Fi to work. However, there are almost no fully autonomous cameras that do not require either external power or local network connection.

The Autonomous Surveillance Camera is, as name implies, a fully autonomous device. It is able to operate up to a month on single battery charge. It uses cellular network for communication. The device checks pre-defined email mailbox once an hour. If a new email is received the camera takes a picture, saves it on a SD-card and emails the image to its owner.

I have designed this device for the case when I am our for a long time, but I still want to make sure how are things back home. You know, when you have one of those paranoia moments about an iron or a leaking tap you could just send an email and in about 30 minutes get a picture confirming that everything is OK.

Making of the camera was also an interesting journey I would like to share.
<br />

# Components Choice #
The Autonomous Surveillance Camera consists of four primary components: GSM modem, camera, SD-card, micro-controller and power supply system. Here are parts that I have used:

[MultiTech MTSMC-G2-IP](http://www.multitech.com/en_US/documents/collateral/data_sheets/86002108.pdf)
I have chosen this  GSM modem mainly because of its built-in POP3 and SMTP support. Although these protocols are not very complicated to implement having them implemented saved some time.

[LinkSpite JPEG Camera](http://www.linksprite.com/download/showdownload.php?lang=en&id=6)
This is a great little device. Built-in JPEG compression support saved me a lot of development effort. The camera produces a 640x480 image. The image is around 50K; it is small enough to rapidly transmit over a cellular network.

[Sparkfun's microSD Shield](http://www.sparkfun.com/products/9802)
Although there are a few SD card circuits available I decided to go with this one because it is very easy to work with both in software (Sparkfun provides great APIs) and in hardware (again, thanks Sparkfun for built-in voltage conversion).

[ATmega 328P](http://www.atmel.com/dyn/products/product_card.asp?part_id=4198)
This micro-controller was a natural choice. Arduino Uno uses this chip, so there are plenty of libraries and examples out there. Unfortunately, Arduino Uno itself consumes too much power when it is in a sleep mode, so I had to build a board with more efficient power supply system myself.

[TPS2020](http://www.datasheetcatalog.org/datasheet/texasinstruments/tps2023.pdf) and [LM2936](http://www.national.com/ds/LM/LM2936.pdf)
[TPS2020](http://www.datasheetcatalog.org/datasheet/texasinstruments/tps2023.pdf) is a great power distribution IC. It allowed me turning circuit components on and off as I needed it. [LM2936](http://www.national.com/ds/LM/LM2936.pdf) is an awesome regulator with really low quiescent current. Combination of the two parts allowed my circuit to consume as little as 0.14mA in sleep mode. It stretches battery life for up to a month on a single charge; the camera is powered by 5V from two CR123A batteries.

Here are all parts laid out:<br />
<a href='http://surveillance-camera.googlecode.com/hg/images/1024/Parts.png'><img src='http://habrastorage.org/storage/1a1876f6/79980841/71835bb7/e3b42301.png' /></a>
<br />

# Breadboard Layout #
Once parts have arrived I laid everything out on a breadboard. Proto-prototype helped me debug the circuit. It also transformed my table into a small cyberpunk jungle. Here is how it looked:

<a href='http://surveillance-camera.googlecode.com/hg/images/1024/Breadboard.png'><img src='http://habrastorage.org/storage/ed0dfd2f/4a261c59/9af1b45a/526d8cb1.png' align='center' /></a>
<br />

# Coding #
It took me about two months to fully code and debug camera's firmware. Following resources were very helpful when developing micro controller program:
  1. [AVR Eclipse Plugin](http://avr-eclipse.sourceforge.net/) made it very easy to set up AVR project and flush it to the chip (I used [AVRISP mkII](http://www.atmel.com/dyn/products/tools_card.asp?tool_id=3808), which worked great with my OS X).
  1. [This implementation](http://code.google.com/p/arms22/source/browse/trunk/JPEGCamera/?r=88) of LinkSpite Camera provided easy way of capturing an image.
  1. [SdFat Library](http://code.google.com/p/sdfatlib/) built by William Greiman was small and efficient (although I had to remove some un-used methods to make it even smaller).
  1. Martin Nawrath's [Nightingale example](http://interface.khm.de/index.php/lab/experiments/sleep_watchdog_battery/) gave a good idea about how sleep mode works on ATmegas.

I had to write a wrapper serial communication with MultiTech GSM modem (it certainly works with MTSMC-G2-IP and it should also work with an Universal Socket device). Check out [SocketModem.cpp](https://code.google.com/p/surveillance-camera/source/browse/src/#src%2Fmodem).

All code is released under MIT license and could be found on Google Code.
<br />

# Body Design #
Unfortunately, not all DIY projects have nice looking enclosures. I decided to use 3D printing service to achieve a look I want.

After struggling quite a bit with 3D CAD tools, I have discovered a great program [OpenSCAD](http://www.openscad.org/). It is a great script-based CAD editing software. Instead of using mouse to draw 3D shapes one just types in commands. For example, a sphere with radius of 5 would be `sphere(r=5)`. Awesome, right?

Caliper, couple weeks of modeling and an order placed at [Ponoko](http://www.ponoko.com/) brought me to this:
<table>

<tr>
<td><img src='http://habrastorage.org/storage/7a9b130e/7235e6ba/5cdfc979/b8eea91b.png' align='center' /></td>
<td>
<a href='http://surveillance-camera.googlecode.com/hg/images/1024/IC%20In%20Bottom%20Half.png'>
<img src='http://habrastorage.org/storage/4b44e37d/c45ab07b/b6180703/d479fe92.png' align='center' />
</a>
</td>
</tr>

<tr>
<td><img src='http://habrastorage.org/storage/02b9a41b/f1889d32/b575c9d1/619ce145.png' align='center' /></td>
<td>
<a href='http://surveillance-camera.googlecode.com/hg/images/1024/IC%20In%20Top%20Half.png'>
<img src='http://habrastorage.org/storage/92e9cb74/0c3e3f6a/5ef2b553/3df12e44.png' align='center' />
</a>
</td>
</tr>
<tr>
<td><img src='http://habrastorage.org/storage/af2a64f9/1ea92f93/28106821/f1402cd7.png' align='center' /></td>
<td>
<a href='http://surveillance-camera.googlecode.com/hg/images/1024/IC%20In%20Holder.png'>
<img src='http://habrastorage.org/storage/ecd53076/4422d13c/6e45953d/1b38844a.png' align='center' />
</a>
</td>
</tr>
</table>

Source for the CAD files could be found under [enclosure folder](https://code.google.com/p/surveillance-camera/source/browse/#hg%2Fenclosure) in the project source.
<br />

# Result #
Everything fit together nicely. The camera is currently working and carefully surveilling my house. I am using AT&T GoPhone as a network plan. It costs about 8 cents to check an email and about 70 cents to send an email with a picture.

Check out [the video](http://www.youtube.com/watch?v=X85ASoRxHRU) and some pictures of the contraption.

<img src='http://habrastorage.org/storage/43b70cbc/199d7883/82dedff5/a7670dce.png' />

<img src='http://habrastorage.org/storage/391133b0/627b899f/f54ccc0f/be664f59.png' />