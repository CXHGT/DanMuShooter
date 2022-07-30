#ifndef PLAYER_STATE_MACHINE_H__
#define PLAYER_STATE_MACHINE_H__

#include "hfsm2/machine.hpp"
#include "entt/entt.hpp"

namespace danMuGame
{

    struct Context
    {
        entt::entity entity = (entt::entity)0;
        int32_t testCount = 0;
        int32_t runTestCount = 0;
        int32_t lostEnemyCount = 0;
        int32_t recoveryHealthCount = 0;
    };

    using GameMachine = hfsm2::MachineT<hfsm2::Config::ContextT<Context>>;

#define STRUCT_S(s) struct s

    // state machine structure
    using FSM = GameMachine::PeerRoot <
                GameMachine::Composite<
                    STRUCT_S(Player_Idle), // 空闲状态
                    STRUCT_S(Player_Stop), // 停止不动
                    STRUCT_S(Player_Walk), // 步行
                    STRUCT_S(Player_Run),  // 跑步
                    STRUCT_S(Player_Treat) // 治疗
                    >,
                GameMachine::Composite<
                    STRUCT_S(Player_Alert),        // 警戒状态
                    STRUCT_S(Player_DetectDanger), // 发现危险，听到枪声
                    STRUCT_S(Player_SearchEnemy),  // 搜索敌人
                    STRUCT_S(Player_BeAttacked),   // 被攻击
                    STRUCT_S(Player_FindTheEnemy)  // 找到敌人，并追踪
                    >,
                GameMachine::Composite<
                    STRUCT_S(Player_War),           // 交战状态
                    STRUCT_S(Player_OpenFire),      // 开火
                    STRUCT_S(Player_ReloadBullet),  // 换弹
                    STRUCT_S(Player_UrgentTreat),   // 紧急治疗
                    STRUCT_S(Player_CloseCombat),   // 近身交战
                    STRUCT_S(Player_ThrowGrenades), // 扔手雷
                    STRUCT_S(Player_Chase),         // 追击敌人
                    STRUCT_S(Player_Guard),         // 守卫
                    STRUCT_S(Player_SuckerPunch)    // 垂死挣扎
                    >,
                GameMachine::Composite<
                    STRUCT_S(Player_Retreat), // 撤退状态
                    STRUCT_S(Player_RunAway)  // 逃跑
                    >,
                GameMachine::Composite<
                    STRUCT_S(Player_End),  // 终结
                    STRUCT_S(Player_Death), // 死亡
                    STRUCT_S(Player_NO)
                    >>;

#undef STRUCT_S

    static_assert(FSM::regionId<Player_Idle>() == 1, "");
    static_assert(FSM::regionId<Player_Alert>() == 2, "");
    static_assert(FSM::regionId<Player_War>() == 3, "");
    static_assert(FSM::regionId<Player_Retreat>() == 4, "");

    // static_assert(FSM::stateId<Player_Idle>() == 1, "");
    // static_assert(FSM::stateId<Player_Stop>() == 2, "");
    // static_assert(FSM::stateId<Player_Walk>() == 3, "");
    // static_assert(FSM::stateId<Player_Run>() == 4, "");
    // static_assert(FSM::stateId<Player_Treat>() == 5, "");

    // static_assert(FSM::stateId<Player_Alert>() == 6, "");
    // static_assert(FSM::stateId<Player_DetectDanger>() == 7, "");
    // static_assert(FSM::stateId<Player_SearchEnemy>() == 8, "");
    // static_assert(FSM::stateId<Player_BeAttacked>() == 9, "");
    // static_assert(FSM::stateId<Player_FindTheEnemy>() == 10, "");

    // static_assert(FSM::stateId<Player_End>() == 4, "");
    // static_assert(FSM::stateId<Player_Stop>() == 5, "");

#define CREATE_MACHINE_STRUCT(X)                    \
    struct X : FSM::State                           \
    {                                               \
        void enter(Control &control) noexcept;      \
        void update(FullControl &control) noexcept; \
    }

    /****************************************************************************************************************/
    /****************************************************** Idle ****************************************************/
    /****************************************************************************************************************/
    CREATE_MACHINE_STRUCT(Player_Idle);
    CREATE_MACHINE_STRUCT(Player_Stop);
    CREATE_MACHINE_STRUCT(Player_Walk);
    CREATE_MACHINE_STRUCT(Player_Run);
    CREATE_MACHINE_STRUCT(Player_Treat);

    /****************************************************************************************************************/
    /***************************************************** Alert ****************************************************/
    /****************************************************************************************************************/
    CREATE_MACHINE_STRUCT(Player_Alert);
    CREATE_MACHINE_STRUCT(Player_DetectDanger);
    CREATE_MACHINE_STRUCT(Player_SearchEnemy);
    CREATE_MACHINE_STRUCT(Player_BeAttacked);
    CREATE_MACHINE_STRUCT(Player_FindTheEnemy);

    /****************************************************************************************************************/
    /*****************************************************  War  ****************************************************/
    /****************************************************************************************************************/
    CREATE_MACHINE_STRUCT(Player_War);
    CREATE_MACHINE_STRUCT(Player_OpenFire);
    CREATE_MACHINE_STRUCT(Player_ReloadBullet);
    CREATE_MACHINE_STRUCT(Player_UrgentTreat);
    CREATE_MACHINE_STRUCT(Player_CloseCombat);
    CREATE_MACHINE_STRUCT(Player_ThrowGrenades);
    CREATE_MACHINE_STRUCT(Player_Chase);
    CREATE_MACHINE_STRUCT(Player_Guard);
    CREATE_MACHINE_STRUCT(Player_SuckerPunch);

    /****************************************************************************************************************/
    /**************************************************  Retreat  ***************************************************/
    /****************************************************************************************************************/
    CREATE_MACHINE_STRUCT(Player_Retreat);
    CREATE_MACHINE_STRUCT(Player_RunAway);

    /****************************************************************************************************************/
    /****************************************************** End  ****************************************************/
    /****************************************************************************************************************/
    CREATE_MACHINE_STRUCT(Player_End);
    CREATE_MACHINE_STRUCT(Player_Death);
    CREATE_MACHINE_STRUCT(Player_NO);

}
#endif