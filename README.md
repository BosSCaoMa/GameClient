# GameClient
游戏服务器

核心模块

- 物品
物品不涉及升级，所以基础部分全部保存在itemconfig中，背包系统保存{id, count}
- 装备
- 效果
- 技能
- 武将
- 玩家
- 战斗



# BattleManager 函数调用流程

## 一、整体流程图

```
┌─────────────────────────────────────────────────────────────────────────────┐
│                           BattleManager 生命周期                             │
└─────────────────────────────────────────────────────────────────────────────┘

 构造阶段                    战斗阶段                           结束阶段
    │                          │                                 │
    ▼                          ▼                                 ▼
┌────────┐               ┌──────────┐                      ┌──────────┐
│ 构造函数 │───────────▶│ runBattle │────────────────────▶│ 返回结果  │
└────────┘               └──────────┘                      └──────────┘
    │                          │
    │                          │ (循环执行直到战斗结束)
    │                          ▼
    │                    ┌─────────────┐
    │                    │executeRound │ ◄─────────┐
    │                    └─────────────┘           │
    │                          │                   │
    │                          ▼                   │
    │                   Result == ONGOING? ────────┘
    │                          │ No
    │                          ▼
    │                      战斗结束
    │
    ▼
┌─────────────────────────────────────────┐
│           初始化流程                     │
│                                         │
│  BattleManager()                        │
│       │                                 │
│       ├──▶ rng_.seed()                 │
│       │                                 │
│       └──▶ initBattle()                │
│               │                         │
│               ├──▶ createBattleCharacters() │    // 创建战斗角色
│               │                         │
│               └──▶ triggerSkills(BATTLE_START) │  // 执行开局技能
│                                         │
└─────────────────────────────────────────┘
```

---

## 二、详细调用顺序

### 1. 构造阶段

```
BattleManager::BattleManager(Player* user, Player* enemy)
│
├── 1. 成员变量初始化
│       ├── userPlayer_ = user
│       ├── enemyPlayer_ = enemy
│       ├── round_ = 0
│       ├── maxRounds_ = 50
│       └── result_ = ONGOING
│
├── 2. rng_.seed(std::random_device{}())
│
└── 3. initBattle()
        │
        ├── 3.1 清空队伍: userTeam_.clear(), enemyTeam_.clear()
        │
        ├── 3.2 createBattleCharacters()
        │       │
        │       ├── 遍历 userPlayer_ 创建己方 BattleCharacter
        │       │   └── BattleCharacter::fromCharacter()
        │       │
        │       └── 遍历 enemyPlayer_ 创建敌方 BattleCharacter
        │           └── BattleCharacter::fromCharacter()
        │
        ├── 3.3 log("=== 战斗开始 ===")
        │
        └── 3.4 triggerSkills(SkillTrigger::BATTLE_START)
                │
                ├── 收集所有存活角色
                ├── 按速度排序
                └── 依次触发开局技能
                    └── executeSkill() [如果有]
```

---

### 2. 战斗主循环

```
BattleManager::runBattle()
│
└── while (result_ == ONGOING)
    │
    └── executeRound()
        │
        ├── 返回 result_
        │
        └── 检查是否继续循环
```

---

### 3. 单回合执行流程 (核心)

```
BattleManager::executeRound()
│
├── 1. round_++   log("===== 第 N 回合 =====")
│
│
├── 3. onRoundStart() ─────────────────────────────────────┐
│       │                                                   │
│       ├── 重置所有角色 hasActed = false                    │
│       │                                                   │z
│       └── triggerSkills(SkillTrigger::ROUND_START)        │
│               │                                           │
│               └── 执行回合开始触发的技能                    │
│                                                           │
├── 4. calculateActionOrder() ─────────────────────────────┤
│       │                                                   │
│       ├── 收集所有存活角色到 actionOrder_                  │
│       │                                                   │
│       └── 按速度降序排序                                   │
│                                                           │
├── 5. for (actor : actionOrder_) ─────────────────────────┤
│       │                                                   │
│       ├── 跳过条件: !isAlive || hasActed                  │
│       │                                                   │
│       ├── executeAction(actor) ◄─────────────────────────┤
│       │       │                                           │
│       │       │ (详见下方)                                 │
│       │                                                   │
│       ├── checkBattleResult()                             │
│       │       │                                           │
│       │       └── 检查双方存活情况                         │
│       │                                                   │
│       └── if (result_ != ONGOING) return                  │
│                                                           │
├── 6. onRoundEnd() ───────────────────────────────────────┤
│       │                                                   │
│       ├── 所有存活角色 tickBuffs()                        │
│       │       └── 处理Buff持续时间、DOT伤害                │
│       │                                                   │
│       ├── 所有存活角色 tickCooldowns()                    │
│       │       └── 技能冷却减1                             │
│       │                                                   │
│       ├── triggerSkills(SkillTrigger::ROUND_END)          │
│       │                                                   │
│       └── checkDeaths()                                   │
│               └── 处理死亡、触发死亡技能                   │
│                                                           │
├── 7. checkBattleResult()                                  │
│                                                           │
├── 8. 检查回合上限 (round_ >= maxRounds_)                   │
│                                                           │
└── 9. return result_                                       │
```

---

### 4. 行动执行流程

```
BattleManager::executeAction(BattleCharacter* actor)
│
├── 1. actor->hasActed = true
│
├── 2. 控制检查
│       │
│       └── if (actor->isControlled())
│               └── log("被控制，无法行动") → return
│
├── 3. 选择技能
│       │
│       ├── skill = actor->getRageSkill()  // 优先怒气技能
│       │       │
│       │       └── 检查怒气是否足够、是否冷却完毕
│       │
│       └── if (!skill)
│               └── skill = actor->getNormalAttack()
│
├── 4. 扣除怒气 (如果是怒气技能)
│
├── 5. log(释放技能信息)
│
├── 6. executeSkill(actor, skill) ◄────────────────────────┐
│       │                                                   │
│       └── for (effect : skill->effects)                   │
│               │                                           │
│               └── executeEffect(actor, effect, skillId)   │
│                                                           │
├── 7. 普攻回怒                                              │
│       │                                                   │
│       └── if (trigger == NORMAL_ATTACK)                   │
│               └── actor->addRage(20)                      │
│                                                           │
├── 8. skill->use() // 设置冷却                              │
│                                                           │
└── 9. checkDeaths()                                        │
```

---

### 5. 技能效果执行流程

```
BattleManager::executeEffect(caster, effect, skillId)
│
├── 1. 概率判定
│       │
│       └── if (effect.chance < 100 && !rollChance())
│               └── return
│
├── 2. getTargets(caster, effect.target) ◄─────────────────┐
│       │                                                   │
│       ├── 构建 Context                                    │
│       │                                                   │
│       ├── 查找 selectors map                              │
│       │                                                   │
│       └── 返回目标列表                                     │
│                                                           │
└── 3. for (target : targets)                               │
        │                                                   │
        └── applyEffect(caster, target, effect, skillId)    │
                │                                           │
                └── (详见下方)                               │
```

---

### 6. 效果应用流程

```
BattleManager::applyEffect(caster, target, effect, skillId)
│
└── switch (effect.effect)
    │
    ├── DAMAGE ─────────────────────────────────────────────┐
    │       │                                               │
    │       ├── calculateDamage(caster, target, effect)     │
    │       │       │                                       │
    │       │       ├── calculateValue() // 基础伤害        │
    │       │       ├── 闪避判定 (rollChance)               │
    │       │       ├── 暴击判定 (rollChance)               │
    │       │       ├── 防御减伤计算                        │
    │       │       └── 伤害加成/减免                       │
    │       │                                               │
    │       ├── target->takeDamage(damage)                  │
    │       │                                               │
    │       ├── target->addRage(10) // 受击回怒             │
    │       │                                               │
    │       ├── triggerOnHit(target, caster)                │
    │       │       └── 触发受击技能                        │
    │       │                                               │
    │       └── triggerOnLowHp(target)                      │
    │               └── 血量<30%时触发濒死技能              │
    │                                                       │
    ├── HEAL ───────────────────────────────────────────────┤
    │       │                                               │
    │       ├── calculateHeal()                             │
    │       └── target->heal(amount)                        │
    │                                                       │
    ├── RAGE_ADD / RAGE_REDUCE ─────────────────────────────┤
    │       │                                               │
    │       └── target->addRage(±amount)                    │
    │                                                       │
    ├── SHIELD ─────────────────────────────────────────────┤
    │       │                                               │
    │       ├── calculateValue()                            │
    │       └── target->addShield(amount)                   │
    │                                                       │
    ├── BUFF_* (ATK/DEF/SPEED/CRIT) ────────────────────────┤
    │       │                                               │
    │       └── target->addBuff(type, value, duration)      │
    │                                                       │
    ├── STUN / SILENCE / FREEZE ────────────────────────────┤
    │       │                                               │
    │       └── target->addBuff(type, 0, duration)          │
    │                                                       │
    ├── POISON / BURN / BLEED ──────────────────────────────┤
    │       │                                               │
    │       ├── calculateValue() // DOT伤害                 │
    │       └── target->addBuff(type, dotDamage, duration)  │
    │                                                       │
    ├── TAUNT ──────────────────────────────────────────────┤
    │       │                                               │
    │       └── target->addBuff(TAUNT, caster.id, duration) │
    │                                                       │
    └── REVIVE ─────────────────────────────────────────────┘
            │
            ├── target->isAlive = true
            └── target->hp = maxHp * percent
```

---

## 三、完整时序图

```
时间轴 ──────────────────────────────────────────────────────────────────────▶

[构造]        [回合1]                    [回合2]                   [结束]
   │             │                          │                        │
   ▼             ▼                          ▼                        ▼
┌─────┐    ┌──────────┐              ┌──────────┐              ┌────────┐
│ctor │───▶│ Round 1  │─────────────▶│ Round 2  │─── ... ────▶│ Result │
└─────┘    └──────────┘              └──────────┘              └────────┘
   │             │                          │
   │             │                          │
   ▼             ▼                          ▼
initBattle   onRoundStart               onRoundStart
   │             │                          │
   ▼             ▼                          ▼
createChars  calcOrder                  calcOrder
   │             │                          │
   ▼             ▼                          ▼
trigger      executeAction x N          executeAction x N
(BATTLE_     (每个存活角色)              (每个存活角色)
 START)          │                          │
                 ▼                          ▼
             onRoundEnd                 onRoundEnd
                 │                          │
                 ▼                          ▼
             checkResult                checkResult
```

---

## 四、关键函数调用频率

| 函数                         | 调用时机    | 频率                  |
| -------------------------- | ------- | ------------------- |
| `BattleManager()`          | 战斗开始    | 1次                  |
| `initBattle()`             | 构造时     | 1次                  |
| `createBattleCharacters()` | 初始化时    | 1次                  |
| `runBattle()`              | 外部调用    | 1次                  |
| `executeRound()`           | 每回合     | N次 (1~50)           |
| `onRoundStart()`           | 回合开始    | N次                  |
| `calculateActionOrder()`   | 回合开始    | N次                  |
| `executeAction()`          | 每个角色行动  | N × 角色数             |
| `executeSkill()`           | 每次行动    | N × 角色数             |
| `executeEffect()`          | 每个技能效果  | N × 角色数 × 效果数       |
| `applyEffect()`            | 每个目标    | N × 角色数 × 效果数 × 目标数 |
| `getTargets()`             | 每个效果    | N × 角色数 × 效果数       |
| `calculateDamage()`        | 每次伤害    | 高频                  |
| `checkDeaths()`            | 行动后/回合末 | 高频                  |
| `onRoundEnd()`             | 回合结束    | N次                  |
| `checkBattleResult()`      | 多处检查    | 高频                  |
