# ofAtem

=========

ofAtem is a program to control the ATEM Production Switcher from Blackmagic for Mac OSX. It is an openframeworks project with sources.

To control the ATEM switch, you can send messages via OSC to ofAtem.app
So you can use ofAtem.app out of the box with any software or hardware controller, which can send OSC messages.
If you want to control it via Midi, it is possible to modify the openframeworks app or use an converter program like osculator. [osculator.net]

The basic functionalities of the ATEM switches are controllable with the ofAtem.app.  Switching input and output channels, perform auto transitions, set transition style and many more. (see command list below).
If you need more control you can tweek the openframeworks application by yourself or feel free to ask me to implement more features.

So there is no limit to control your ATEM switch.

There is also a demo application called "tele.ofAtem" made with max/msp, which shows a possible control setup.

A second example shows, how to control the ATEM switch with Ableton Live (using Max for Live). With this demo project you can control the ATEM switch with Midi sequences or just map the switch buttons to your keyboard or Midi device.

If you have an sugestions, new ideas or you want to hire me for expanding the project, feel free to contact me.
- email: [telematique@web.de] (telematique@web.de)
- web: [www.telematique.de] (www.telematique.de)

I also want to thank the following people for making the ofAtem project possible:
- [Johnny DeKam] (www.bejohnny.com)  contribution
- [Timm Ringewaldt] (www.autokolor.org) contribution, technical support
- [Till Wimmer] (www.bit1.pro) for programming support



# How ofAtem works:

1) Install the driver from Blackmagic.
"Blackmagic ATEM Switchers 3.5 for Macintosh" or higher.<br />I recomment to check, if the "ATEM Software Control.app" from Blackmagic is working with your hardware.

2) Put the IP address of your Atem device in the settings file
something like that: <atemIp>192.168.10.240</atemIp>
The settings file is located "data" folder near the ofAtem.app.

3) Start the ofAtem.app
You should see that.

![connected](https://raw.github.com/telematique/ofAtem/master/images/ofAtem_status1.jpg)

If you see something like that, something is wrong with the network setup or driver installation.
Go to step 1 :-)

![connection failed](https://raw.github.com/telematique/ofAtem/master/images/ofAtem_status0.jpg)

If the connection between the ofAtem.app and your hardware is working you should be able to control it via osc.

There are 2 examples, which demonstrates how to use ofAtem.

1) tele.ofAtem.app
This is made with max/msp, which shows a possible control setup.
Start the application and press "get data from atem"
Now you should be able to remote control your atem device.

![tele.ofAtem](https://raw.github.com/telematique/ofAtem/master/images/tele.ofAtem.jpg)

2) live.ofAtem - a Ableton Live project
To control the switch, simply start one of the clips.
for example "Program switch"
You can also switch all input channels and aux outputs with Midi notes, keys or mouse. 

![live.ofAtem](https://raw.github.com/telematique/ofAtem/master/images/live.ofAtem.jpg)

# Technical details:

ofAtem is based on the version 3.5 of the Blackmagic ATEM SDK.
To get it to work, it is necessary to install the "Blackmagic ATEM Switchers 3.5 for Macintosh" from  [blackmagicdesign] (http://www.blackmagicdesign.com/de/products/atem)

The software is sucessfully tested with the BLACK MAGIC DESIGN ATEM 1/ME.
THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND.

I used the following environment:
Mac OSX 10.6, 10.7, 10.8

openframeworks
- XCode 3.2.6
- [openframeworks](http://www.openframeworks.cc/) version 0073 for mac osx.

tele.ofAtem
- Max6  (version 6.0.8)

live.ofAtem
- Ableton Live 8.2.7, Max for Live 6.0.8


# ofAtem OSC command reference:

OSC commands to the Atem 

`/atem/cut`               perform the cut transition<br />
`/atem/auto`              perform the auto transition<br />
`/atem/fadeToblack`       perform fade To black<br />
`/atem/preview ID`  			set the source with the number ID to preview source (ID:integer)<br />
`/atem/program ID` 				set the source with the number ID to program source (ID:integer)<br />
`/atem/mix MIXAMOUNT`			perform mix transition (MIXAMOUNT:float from 0. to 1.)<br />
`/atem/FTBRate FRAMES`			set the "Fade to Black" time in frames (FRAMES:integer)<br />
`/atem/getInputNames`			returns the source IDs with names<br />
`/atem/getTransitionFrames`		returns the transition frames<br />
`/atem/getFTBFrames`			returns the "fade to black" frames<br />
`/atem/isInTransition`			returns the state of the transition<br />
`/atem/getAuxCount`				returns the number of auxiliary outputs<br />
`/atem/getAuxSource AUXNUM`		returns the source id of the selected auxiliary output (AUXNUM:integer)<br />
`/atem/auxSource AUXNUM ID`		set source of auxiliary output (AUXNUM: integer, ID: integer)<br />
`/atem/transitionStyle STYLE`	set the transitionstyle (STYLE:string mix, dip, wipe, sting, dve )<br />

OSC commands from Atem 

`atem/input/ ID INPUTNAME`		ID: source ID, INPUTNAME: name of the Input (ID:integer, INPUTNAME string)<br />
`/atem/programInputID ID`		ID: actual selected program source ID (ID:integer)<br />
`/atem/previewInputID ID`		ID: actual selected preview source ID (ID:integer)<br />
`/atem/mix MIXAMOUNT`			MIXAMOUNT actual mixamount (MIXAMOUNT:float from 0. to 1.)<br />
`/atem/tfr FRAMES`				FadeToBlackFramesRemaining (FRAMES:integer)<br />
`/atem/ftbfr FRAMES`			TransitionFramesRemaining (FRAMES:integer)<br />
`/atem/isInTransition YES/NO`	Transition active or not (YES/NO bool 0 or 1)<br />
`/atem/ftbrate FRAMES`			"Fade to Black" time in frames (FRAMES:integer)<br />
`/atem/auxSource AUXNUM ID`		AUXNUM: number of auxiliary output, ID: source ID of auxiliary output<br />


