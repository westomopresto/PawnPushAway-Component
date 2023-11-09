# PawnPushAway-Component
A component for Unreal that pushes pawns away

Disclaimer:: i used chatGPT to help format my code
I performance tested this with around 25 enemies chasing me. No noticable hitches on my end.
No Warranty. As is, MIT license. 

![Image](https://i.imgur.com/RvrfZRW.png)

![Image](https://i.imgur.com/rTF27yR.png)

Debug is for debug shapes in the editor/world.:

![Image](https://i.imgur.com/XBm4mCc.png)

Push Shape tag is the tag of the collider you want to use for Pushing. This colldier should ignore all except Overlap Pawn. And be a Pawn itself.
This can be the default CapsuleComponent, or a new component. Just add the Tag you defined here. Default "PushShape"

TickRate is the rate of the component tick, for performance this is set to 1/60 (60 tick) by default
Feel free to adjust

Interpolation Speed is the Interp value for how "fast" it pushes you away
This scales by distance of overlap as well. So 2.5 is how strong it is when both actors are overlapping almost entirely.
And then it scales down to 50% of that value at the edge. This creates a "soft" feel

You can watch the video on [Twitter](https://twitter.com/Weston_Mitchell/status/1721799355592761401).
