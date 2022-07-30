#ifndef __GAME_STATUE_H__
#define __GAME_STATUE_H__

enum GameStatue
{
    Test,        // 测试专用
    Prepare,     // 准备阶段，玩家可以加入游戏
    Distribute,  // 分配玩家位置
    Running,     // 游戏进行时，此时新玩家不可加入游戏，已加入游戏的玩家可以通过命令来控制
    Pause,       // 暂停
    FrontEnd,    // 结算前
    Ending,      // 结尾，赢家已经出现，庆祝之后进入准备阶段
};

#endif