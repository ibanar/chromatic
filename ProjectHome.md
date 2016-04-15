![http://chromatic.googlecode.com/svn/trunk/chromatic.jpg](http://chromatic.googlecode.com/svn/trunk/chromatic.jpg)

<pre>

Integrated Development Environment for Asm/C/C++/Pascal and more.<br>
<br>
The tool is still under development, the integrated debugger, dialogs editor and LUA<br>
script engine aren't finished yet. It may be or may not be completed depending<br>
on the motivation / spare time / feedback.<br>
<br>
----<br>
<br>
Recent history.txt entries:<br>
<br>
V1.3.6<br>
<br>
- Improved hexadecimal & binary numbers highlighting,<br>
conversions and also selections.<br>
- Fixed a bug in binary to dec or hex numbers conversions<br>
(also those conversions are now automatically unsigned).<br>
<br>
V1.3.5<br>
<br>
- Fixed a small issue with comments in 68k language file.<br>
- Cleaned up the code for number conversions also several types<br>
can now be specified for HexType & BinType in language files.<br>
<br>
V1.3.4<br>
<br>
- Removed the ChromeSup.dll which is now statically linked to the program<br>
this fixed a few bugs due to obsolete functions begin used.<br>
(the new static lib project has been added into the svn repository).<br>
<br>
V1.3.3<br>
<br>
- Source code now compiles under Visual Studio 2010 (projects added).<br>
- Added Motorola 680x0 language file.<br>
- Added languages manager in properties<br>
(especially used to define priorities between languages sharing the same files extensions).<br>
<br>
V1.3.2<br>
<br>
- Fixed free pascal skin and example.<br>
- Added an option to display a requester before exiting.<br>
- Most icons should look better on Windows 7.<br>
- Added a popup menu with several options in Windows dock list.<br>
- Fixed a bug which trashed projects files and main.ini when one of the user's specified folder was empty.<br>
<br>
V1.3.1<br>
<br>
- Changed the behaviour of the filters menus, now each filters<br>
profile is automatically available for both windows and files output.<br>
- Added a filter to convert binaries to dc.b statements.<br>
- Fixed several compilation issues with the examples.<br>
- Added octal column in ASCII addin.<br>
</pre>