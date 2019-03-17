# RR

RR是一个基于STM32标准外设库和FreeRTOS操作系统的外设驱动库，目前已支持发光二极管、按键、移位寄存器、数据选择器、以太网控制器及看门狗定时器，并在STM32F103C8T6上测试通过。

RR使用面向对象思想，并采用设计模式提高代码的可读性、降低代码的重复率。本文将介绍如何使用RR API。

## 发光二极管

发光二极管相关方法的声明位于头文件`led.h`中。发光二极管基于单一GPIO引脚操作，因此在创建发光二极管实例时需传入GPIOPin实例指定发光二极管对应的引脚，以及点亮发光二极管的引脚状态。

```C
// 创建一个发光二极管实例led1；发光二极管位于PC13引脚上，向该引脚输出低电平点亮发光二极管
LED led1 = newLED(newGPIOPin(GPIOC, GPIO_Pin_13), LOW);
```

使用`onLED`或`offLED`方法可以点亮或熄灭发光二极管。

```C
// 点亮发光二极管led1
onLED(&led1);

// 熄灭发光二极管led1
offLED(&led1);
```

vTestLEDTask任务可以使作为参数传入的发光二极管闪烁，请使用FreeRTOS的任务创建API创建该任务。

```C
// 创建vTestLEDTask任务，使led1实例的发光二极管闪烁
xTaskCreate(vTestLEDTask, NULL, configMINIMAL_STACK_SIZE, &led1, 1, NULL);
```

## 按键

按键相关方法的声明位于头文件`button.h`中。与发光二极管相同，按键也基于单一GPIO引脚操作，因此在创建按键实例时也需传入GPIOPin实例指定按键对应的引脚，以及按键被按下时的引脚状态。

```C
// 创建一个按键实例button1；按键位于PA3引脚上，被按下时该引脚为低电平
Button button1 = newButton(newGPIOPin(GPIOA, GPIO_Pin_3), LOW);
```

在使用按键时，还需指定按键被按下时调用的回调函数；假设有一回调函数`onButton1Click`，则语句`button1.onClick = onButton1Click;`将button1实例的回调函数设置为该函数。

若需使用外部中断响应按键被按下的事件，请参考如下代码块：

```C
// 启用外部中断响应按键button1被按下的事件
setButtonInterrupt(&button1, ENABLE);

// 创建按键中断处理任务，按键的回调函数将在中断退出后自动执行
xTaskCreate(vButtonInterruptHandler, NULL, configMINIMAL_STACK_SIZE, &button1, 2, NULL);

// 编写按键的中断服务函数；若使用BSP版本驱动，则可忽略
void EXTI3_IRQHandler(void)
{
	static portBASE_TYPE xResult = pdFALSE;
	
	if (EXTI_GetITStatus(EXTI_Line3) == SET) {
		EXTI_ClearITPendingBit(EXTI_Line3);
		xSemaphoreGiveFromISR(button1._semaphore, &xResult);
	}
}
```

## 移位寄存器

74HC595是8位并具有三态输出功能的移位寄存器，其相关方法的声明位于头文件`shiftregister.h`中。

使用`setShiftRegisterBit`或`resetShiftRegisterBit`方法可以将移位寄存器中的某位置位或清零。

```C
// 创建一个移位寄存器实例shiftRegister1
shiftRegister1 = newShiftRegister();

// 将移位寄存器shiftRegister1的第0位置位
setShiftRegisterBit(&shiftRegister1, 0);

// 将移位寄存器shiftRegister1的第0位清零
resetShiftRegisterBit(&shiftRegister1, 0);
```

移位寄存器一般支持级联操作，因此移位寄存器必须通过移位寄存器组进行移位输出；移位寄存器组相关方法的声明位于头文件`registergroup.h`中。在创建移位寄存器组实例时，需传入GPIOPin实例数组指定移位寄存器组的数据引脚、移位时钟引脚、锁存时钟引脚和输出使能引脚，可以使用指定初始化初始化该数组；若使用硬件使能，则可忽略输出使能引脚的初始化。

```C
// 创建一个移位寄存器组实例registerGroup1
RegisterGroup registerGroup1 = newRegisterGroup((GPIOPin []) {
	[REGGRP_SER_PIN]	= newGPIOPin(GPIOA, GPIO_Pin_7), 	// 移位寄存器组的数据引脚为PA7
	[REGGRP_SRCLK_PIN]	= newGPIOPin(GPIOA, GPIO_Pin_5), 	// 移位寄存器组的移位时钟引脚为PA5
	[REGGRP_RCLK_PIN]	= newGPIOPin(GPIOA, GPIO_Pin_6), 	// 移位寄存器组的锁存时钟引脚为PA6
	[REGGRP_E_PIN]		= newGPIOPin(GPIOA, GPIO_Pin_4)		// 移位寄存器组的使能引脚为PA4；若使用硬件使能，则可忽略
});

// 使能移位寄存器组registerGroup1输出；若使用硬件使能，则可忽略
setRegisterGroupOutput(&registerGroup1, ENABLE);
```

移位寄存器必须按自先向后的顺序添加到移位寄存器组中，即靠近微控制器的移位寄存器先添加，远离微控制器的移位寄存器后添加；将移位寄存器添加到移位寄存器组后，即可执行移位输出操作。

```C
// 将移位寄存器shiftRegister1添加到移位寄存器组registerGroup1
addRegisterToGroup(&registerGroup1, &shiftRegister1);

// 移位输出移位寄存器组registerGroup1
outputRegisterGroup(&registerGroup1);
```

也可以使用`setRegisterGroupBit`或`resetRegisterGroupBit`方法将移位寄存器组作为整体对某位进行置位或清零；例如`setRegisterGroupBit(&registerGroup1, 8);`语句对移位寄存器组中第2个移位寄存器的第0位置位。

vTestRegisterGroupTask任务可以在作为参数传入移位寄存器组的第1个移位寄存器上呈现流水灯效果，请使用FreeRTOS的任务创建API创建该任务。

```C
// 创建vTestRegisterGroupTask任务，在移位寄存器组的第1个移位寄存器上呈现流水灯效果
xTaskCreate(vTestRegisterGroupTask, NULL, configMINIMAL_STACK_SIZE, &registerGroup1, 1, NULL);
```

## 数据选择器

74HC151是8选1的数据选择器，其相关方法的声明位于头文件`dataselector.h`中。在创建数据选择器实例时，需传入数据选择器的数据选择端地址范围

```C
// 创建一个数据选择器实例dataSelector1，数据选择端地址范围从0到7
DataSelector dataSelector1 = newDataSelector(0, 7);
```

与移位寄存器类似，数据选择器一般支持通过使能端进行扩展，因此数据选择器必须通过数据选择器组执行扫描操作；数据选择器组相关方法的声明位于头文件`selectorgroup.h`中。在创建数据选择器组实例时，需传入最多允许添加到数据选择器组的观察者实例数量，并使用`setSelectorGroupScanPin`和`setSelectorGroupAddressPins`方法指定数据选择器组的输出引脚和数据选择端引脚。

```C
// 创建一个数据选择器组实例selectorGroup1，最多允许添加5个观察者实例
SelectorGroup selectorGroup1 = newSelectorGroup(5);

// 设置数据选择器组selectorGroup1的输出引脚为PB5
setSelectorGroupScanPin(&selectorGroup1, newGPIOPin(GPIOB, GPIO_Pin_5));

// 设置数据选择器组selectorGroup1的数据选择端引脚为PB6、PB7、PB8和PB9 4个引脚
setSelectorGroupAddressPin(&selectorGroup1, newGPIOPin(GPIOB, GPIO_Pin_6), 4);
```

可以使用FreeRTOS的任务创建API创建vScanSelectorGroupTask任务，并传入指向数据选择器组实例的指针扫描数据选择器。

```C
// 创建vScanSelectorGroupTask任务
xTaskCreate(vScanSelectorGroupTask, NULL, configMINIMAL_STACK_SIZE, &selectorGroup1, 1, NULL);
```

不再使用数据选择器组时，请通过`deleteSelectorGroup`方法释放数据选择器组占用的堆内存空间；不建议反复创建和删除数据选择器组。

```C
// 不再使用数据选择器组selectorGroup1，释放它的堆内存空间
deleteSelectorGroup(&selectorGroup1);
```

**获取数据选择器组的扫描结果**

数据选择器组的扫描结果保存在数据选择器组实例的队列中，可以通过向数据选择器组添加观察者实例获取每次扫描的结果；观察者接口的声明位于头文件`observer.h`中。可参考如下代码块创建自己的观察者实例。

```C
typedef struct MyObserver {
	struct IObserver observer; // 实现观察者接口，必须为结构体第一成员
} MyObserver;

// 实现观察者接口的update方法
void updateMyObserver(MyObserver * myObserver, struct ISubject * subject)
{
	SelectorMessage message; // 声明一个空的数据选择器消息实例
	SelectorGroup * group = (SelectorGroup *)subject; // 获得指向数据选择器组的指针
	
	// 获取数据选择器消息
	if (xQueuePeek(group->_xMessagesQueue, &message, 0) == pdTRUE) {
		// 通过访问数据选择器消息的id和data成员获取数据选择器输入端的引脚状态
		// 例如，message = {.id = 0, .data = HIGH}，表明数据选择器输入端0引脚上为高电平
	}
}

// 创建观察者实例myObserver1，并指定其update方法为updateMyObserver
MyObserver myObserver1 = {.observer.update = (update_fp)updateMyObserver};
```

使用`registerObserverToSelectorGroup`方法将观察者实例注册到数据选择器组后，每当数据选择器组扫描完毕观察者即会收到通知，并自动调用update方法。

```C
// 将myObserver1注册到数据选择器组selectorGroup1
registerObserverToSelectorGroup(&selectorGroup1, &myObserver1.base);
```

## 以太网控制器

W5500是一款集成全硬件TCP/IP协议栈的以太网控制器芯片，其相关方法的声明位于头文件`w5500.h`中。W5500芯片通过串行外设接口（SPI）与微控制器通信，因此在创建以太网控制器实例时需指定以太网控制器对应的串行外设接口。

```C
// 创建一个以太网控制器实例，以太网控制器位于SPI2上
W5500 w5500 = newW5500(SPI2);
```

在使用以太网控制器前，还需设置网络相关参数；网络相关参数继承自父结构体，因此在调用设置方法时第一个参数应为`&w5500.base`。

```C
// 设置HTTP客户端参数；假设本机IP地址为192.168.1.199，使用50000端口
setEthernetClientParams(&w5500.base,
	(uint8_t[]) {
	192, 168, 1, 199
}, 50000);

// 设置HTTP服务器端参数；假设服务器IP地址为192.168.1.107, 使用60000端口
setEthernetServerParams(&w5500.base, 
	(uint8_t []) {
	192, 168, 1, 107
}, 60000);

// 设置子网掩码及网关；假设子网掩码为255.255.255.0，网关IP地址为192.168.1.1
setEthernetNetworkParams(&w5500.base, 
	(uint8_t []) {
	255, 255, 255, 0
}, 
	(uint8_t []) {
	192, 168, 1, 1
});
```

同时还需设置用于发送和接收数据的缓冲区，假设缓冲区定义为

```C
#define BUFFER_SIZE 255					// 缓冲区大小

uint8_t txBuffer[BUFFER_SIZE] = {0};	// 发送缓冲区
uint8_t rxBuffer[BUFFER_SIZE] = {0};	// 接收缓冲区
```

则语句`setEthernetTxBuffer(&w5500.base, txBuffer, BUFFER_SIZE);`和`setEthernetRxBuffer(&w5500.base, rxBuffer, BUFFER_SIZE);`将以太网控制器实例的发送和接收缓冲区设置为txBuffer和rxBuffer。

W5500父结构体初始化完成后，还需设置用于W5500的回调函数，回调函数的实现请参照Ethernet目录下的porting.h和porting.c文件；如下代码块将用于W5500的回调函数设置为已实现的函数。

```C
w5500.cris_en   = SPI2_Cris_Enter;
w5500.cris_ex   = SPI2_Cris_Exit;
w5500.cs_sel    = SPI2_CS_Select;
w5500.cs_desel  = SPI2_CS_Deselect;
w5500.spi_rb    = SPI2_ReadByte;
w5500.spi_wb    = SPI2_WriteByte;
```

一切准备就绪后，即可初始化W5500芯片；

```C
// 初始化W5500实例
initW5500(&w5500);
```

还可通过重写W5500父结构体中的`send`和`onReceive`回调函数更改以太网控制器在发送和接收到数据时的行为，否则以太网控制器将默认实现一个回环。请使用FreeRTOS的任务创建API创建vSyncW5500任务，与W5500芯片进行同步。

```C
// 创建vSyncW5500Task任务，与W5500芯片进行同步
xTaskCreate(vTestW5500Task, NULL, configMINIMAL_STACK_SIZE, &w5500, 1, NULL);
```

### 看门狗定时器

看门狗定时器可以防止程序运行异常，其相关方法的声明位于头文件`watchdog.h`中。看门狗定时器集成在STM32 SoC中，因此可以通过`getWatchdogInstance()`获取获取指向看门狗定时器实例的指针，而无需手动创建看门狗定时器实例。

获取指向看门狗定时器实例的指针后，即可对看门狗定时器执行初始化操作，并在初始化时传入定时器的超时时间，单位为毫秒。

```C
// 获取指向看门狗定时器实例的指针
Watchdog * watchdog = getWatchdogInstance();

// 初始化看门狗定时器，并设置超时时间为1000ms
initWatchdog(watchdog, 1000);
```

看门狗定时器初始化完成后，须在定时器超时前进行重置，宏`RELOAD_WATCHDOG()`可以完成重置操作。