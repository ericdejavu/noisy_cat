# 路由配置文件说明

### 简介

路由配置文件，主要用于充当http中的controller模块，串口中的功能函数选择器。

由major，stable，dev三个json文件构成

- **stable**中存放稳定的路径
- **dev**中表示正在开发的逻辑功能模块
- **major**配置路由运行模式

### 使用说明

major.json

```json
{
    "version": 1.0, //修改过的master生效时，version号需要变大
    "auto_switch": true //是否将dev中确定稳定的url，自动同步到master中，并自动增加版本号
}
```

dev.json

```json
{
    "auth": {
        "args": {
            "username":"string", "password":"array_string", "right_required":"int"
        }, // 访问时可能会携带的参数，以及参数类型
        "status":{"stroage":true, "stable": false, "required":[ "username", "password"]}
    },
     "other_url": {"args":{...}, "status":{...}}
}
```

stable.json

规则与dev.json一致