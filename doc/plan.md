# server
### 整体
- [ ] 目前使用`nodejs` + `TypeScript` 服务器本身就资源紧张
- [ ] 后续会使用 `.NET` 重写 (综合性能、与开发效率)
### 数据库
- [ ] 当前`MySQL`, 增加其他信息到表
- [ ] 改用索引等优化查询
### 功能
1. 增加`注册`支持
2. 信息 队列

# client
### 整体
- [ ] 逻辑复杂, 代码冗余
- [ ] 大量优化逻辑，增加注释
### 功能
1. 黑夜模式(主题)
2. 历史消息

# 已知的的问题
- [ ] 新登录挤下线对方后会使其无法进行消息转发
- [ ] 其他非恶性Bug

# ISSUE
- [ ] `#&hyw7ad5q` in `client/header/ui/MessageDialog.h(及其实现)` 
- 构造函数：`MessageDialog(Type type, QWidget* parent);`（整个类）  
当`MessageDialog`对象有父窗口时：`Window::nativeEvent`中：Windows不会分发`WM_NCHITTEST`等消息  
导致无法拖动窗口，两个窗口基本没有交互能力  
// 目前只能使用无父窗口、模态为ApplicationModal解决  
// 并非`MessageDialog::exec`中局部变量事件循环导致  