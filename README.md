# osuReplayEditorV3 Source Code Release

# Instructions on how to use or download this software found [HERE](https://github.com/thebetioplane/osuReplayEditorV3)
Instructions on how to use or download this software found [HERE](https://github.com/thebetioplane/osuReplayEditorV3)
* Instructions on how to use or download this software found [HERE](https://github.com/thebetioplane/osuReplayEditorV3)

Behold the shitstorm.

# How to build

Use Visual Studio 2019. Select CPU to be x64, do not use "Any CPU" or "x86". Once built you will need to place bass.dll and bass_fx.dll in the Debug or Release directory (wherever you are running the exe from).

# How to code

Explanations of the different Visual Studio projects in this solution:

- osuReplayEditor - (C#) - the WinForms user interface
- ReplayEditor - (C++) - a DLL loaded at runtime where nearly all of the actual replay editing code is
- Builder - (C#) - my automated building system in order to produce builds and produces the manifest files to upload 

# Licenses

## osuReplayEditorV3

Copyright (C) 2022

This program is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License along with this program. If not, see https://www.gnu.org/licenses/.

## Bass.dll

Free for non-commercial use. https://www.un4seen.com/

## LZMA

Public domain. https://www.7-zip.org

## Sqlite

Public domain. https://sqlite.org

## GLM

MIT License. https://github.com/g-truc/glm
```
================================================================================
The MIT License
--------------------------------------------------------------------------------
Copyright (c) 2005 - G-Truc Creation

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
```

