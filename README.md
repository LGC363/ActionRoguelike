#ActionMagicRPG       2023.09-2023.12

项目概述:基于虚幻引擎4制作的动作魔法射击游戏Demo，实现了人物控制、魔法技能和射击优化，敌人生成与行为树，简单的状态同步。


1.GAS框架



2.AI行为树



3.通用对象池系统
单例ObjectPoolSystem，定义在UWorld中，修改了world源码，生命周期和world相同，因此不必关心world切换带来的对象池切换问题，在world销毁时整个system也跟着销毁了。
World在BeginPlay时初始化对象池系统。World在BeginDestroy时销毁对象池系统。
修改了world中的SpawnActor和DestroyActor源码，SpawnActor原先直接调用NewObject<AActor>，现在会先去ObjectPoolSystem中找对象池：
    Actor = Cast<AActor>(PoolSystem->Spawn(Class, LevelToSpawnIn, FOnSpawnObjectFromPoolDelegate::CreateLambda(OnSpawnObjectFromPool)));
DestroyActor同理：
	  PoolSystem->Recycle(ThisActor)；
 
网络同步问题：
    使用对象池导致UWorld::DestroyActor没有真正Destroy，所以客户端也不会Destroy，解决方案是在Actor回收到对象池后，通知NetDriver销毁Actor：
    do
	{
		...
		if (PoolSystem->Recycle(ThisActor))
		{
			// Notify net drivers that this guy has been destroyed.
			if (FWorldContext* Context = GEngine->GetWorldContextFromWorld(this))
			{
				for (FNamedNetDriver& Driver : Context->ActiveNetDrivers)
				{
					if (Driver.NetDriver != nullptr && Driver.NetDriver->ShouldReplicateActor(ThisActor))
					{
						Driver.NetDriver->NotifyActorDestroyed(ThisActor);
					}
				}
			}
			
			return true;
		}

		// Simply return 'false' if the pooled-object has been destroyed more than once
		return false;
	}
	while (false);
 
    同理客户端无法走到SpawnActor：
    ......



4.工厂，单例



5.状态同步






