# Hazel
Hazel Engine

This is a game engine to practice some of my ideas about the game engine. I wrote most of the code myself, and I referred to [1] and [2].

这是我为了实践自己关于游戏引擎的想法制作的游戏引擎。大部分代码是我自己编写的，我参考了[1]和[2]。


its name, Hazel, is the same with [1]’s.

它的名字，Hazel，和[1]的是一样的。

You can click on the screenshot file to see what it looks like.

你可以点击屏幕截图文件看看实际渲染效果。

If you want to try this out, you can download GLM to the corresponding folder，then you need to download premake5.exe to your working directory, then open the cmd and run code: call premake5.exe vs2019(my vs version is vs2019, you can try other versions by yourself). Then you can double click the .sln file just generated to open it in the vs. The solution needs to be built multiple times so that the DLL files are automatically copied to the specified directory.Set the Sandbox as a startup item.

如果您想试试这个，你可以下载premake5.exe到工作目录，然后打开cmd，运行代码：call premake5.exe vs2019（我的VS版本是vs2019，你可以自己尝试其他版本）。然后你可以双击刚才生成的.sln，在vs里打开它。生成解决方案时需要生成多次，以便dll文件被自动复制到指定目录。将Sandbox设为启动项。

I haven't written the UI yet, so if you want to see a different Actor, you'll need to go to editor.cpp and get rid of the // of addDefaultActorXXX to display the ActorXXX.

我当前未编写UI，所以如果你想要看到不同的Actor, 你需要到Editor.cpp里修改代码, 将addDefaultActorXXX的//去除掉，就能显示XXX这个Actor了。

How to play?
F5: enter the scene.
Esc: leave the scene.
Mouse move: camera direction.
Mouse scroll: camera move speed.
WSAD: camera move.

怎么操作？
F5:进入场景。
Esc:离开场景。
鼠标移动:摄像机方向。
鼠标滚动:相机移动速度。
WSAD:相机移动。


reference:

 [1] https://github.com/TheCherno/Hazel
 
 
 [2] https://learnopengl-cn.github.io/
 
 
 [3] http://www.realtimerendering.com/
 
 
