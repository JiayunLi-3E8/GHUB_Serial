function Component()
{
    // 初始化代码，例如连接信号和槽、设置界面等
}

Component.prototype.createOperations = function()
{
    component.createOperations();
    if (systemInfo.productType === "windows") {
        component.addOperation("CreateShortcut",
            "@TargetDir@/bin/GHUBSerial.exe",
            "@DesktopDir@/GHUBSerial.lnk",
            "workingDirectory=@TargetDir@/bin",
            "description=Launch GHUBSerial");

        component.addOperation("CreateShortcut",
            "@TargetDir@/bin/GHUBSerial.exe",
            "@StartMenuDir@/GHUBSerial.lnk",
            "workingDirectory=@TargetDir@/bin",
            "description=Launch GHUBSerial");
    }
}
