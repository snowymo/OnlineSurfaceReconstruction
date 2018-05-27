#cs ----------------------------------------------------------------------------

 AutoIt Version: 3.3.14.2
 Author:         myName

 Script Function:
	Template AutoIt script.

#ce ----------------------------------------------------------------------------

; Script Start - Add your code below here

#include <Color.au3>

Opt("WinTitleMatchMode", 2)

WinActivate("Online Surface Reconstruction")
WinWaitActive("Online Surface Reconstruction")

$OSRWindowPos = WinGetPos("[ACTIVE]")

;$pos = MouseGetPos()

$integrateX = $OSRWindowPos[0] + 217
$integrateY = $OSRWindowPos[1] + 436;

; check if the color turns white, means new scan is loaded
;$iColor = PixelGetColor($integrateX, $integrateY)
;$rgb = _ColorGetRGB("0x00"&Hex($iColor))

;Do
   ;$currentColor = PixelGetColor($integrateX, $integrateY)
   ;$rgb = _ColorGetRGB("0x00"&Hex($currentColor))
   ;Sleep(200)
;Until $rgb[0] > 70 And $rgb[1] > 70 And $rgb[2] > 70

;MsgBox(0, "Mouse Color", "The decimal color is: " & $rgb[0] & $rgb[1] & $rgb[2])

; Integrate
MouseMove($integrateX, $integrateY)
MouseClick("left", $integrateX, $integrateY)
;$integrateAllX = $OSRWindowPos[0] + 137
;$integrateAllY = $OSRWindowPos[1] + 725;

